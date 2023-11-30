////////////////////////////////////////////////////////////////////////////////
// Filename: terrainclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "skymapclass.h"
#include "DDSTextureLoader.h"
#include <vector>

SkymapClass::SkymapClass()
{
	sphereVertBuffer = 0;
	sphereIndexBuffer = 0;
	NumSphereVertices = 0;
	NumSphereFaces = 0;
	m_Texture = 0;
	m_skybox = 0;
	cbPerObject cbPerObj;
}

SkymapClass::SkymapClass(const SkymapClass& other)
{
}

SkymapClass::~SkymapClass()
{
}

D3D11_INPUT_ELEMENT_DESC layout[] =
{
	///////////////**************new**************////////////////////
	{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0}
	///////////////**************new**************////////////////////
};
UINT numElements = ARRAYSIZE(layout);

bool SkymapClass::Initialize(ID3D11Device* device, const WCHAR* textureFilename, XMFLOAT3 position)
{
	bool result;

	//±¸ »ý¼º
	result = CreateSphere(device, 10, 10, position);
	if (!result)
	{
		return false;
	}

	// Initialize the vertex and index buffers.
	//result = InitializeBuffers(device);
	//if (!result)
	//{
	//	return false;
	//}

	// Load the texture for this bitmap.
	result = LoadTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}
}

void SkymapClass::Shutdown()
{
	// Release the model texture.
	ReleaseTexture();
	ShutdownBuffers();
}

bool SkymapClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return true;
}

//bool SkymapClass::Render(ID3D11DeviceContext* deviceContext, XMVECTOR position)
//{
//	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
//	RenderBuffers(deviceContext);
//
//	return true;
//}

bool SkymapClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	// Create the vertex array.
	vertices = new VertexType[NumSphereVertices];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[NumSphereFaces];
	if (!indices)
	{
		return false;
	}

	// Load the vertex array and index array with data.
	for (i = 0; i < NumSphereVertices; i++)
	{
		vertices[i].position = XMFLOAT3(m_skybox[i].x, m_skybox[i].y, m_skybox[i].z);
		vertices[i].texture = XMFLOAT2(m_skybox[i].tu, m_skybox[i].tv);
		vertices[i].normal = XMFLOAT3(m_skybox[i].nx, m_skybox[i].ny, m_skybox[i].nz);

		indices[i] = i;
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * NumSphereVertices;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &sphereVertBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * NumSphereVertices;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &sphereIndexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

bool SkymapClass::CreateSphere(ID3D11Device* device, int LatLines, int LongLines, XMFLOAT3 skymapPosition)
{
	NumSphereVertices = ((LatLines - 2) * LongLines) + 2;
	NumSphereFaces = ((LatLines - 3) * (LongLines) * 2) + (LongLines * 2);

	float sphereYaw = 0.0f;
	float spherePitch = 0.0f;

	std::vector<VertexType> vertices(NumSphereVertices);

	XMVECTOR currVertPos;
	currVertPos = XMVectorSet(0.0f, 0.0f, 1.0f, 0);
	//currVertPos = XMVectorSet(skymapPosition.x, skymapPosition.y, skymapPosition.z, 0);

	vertices[0].position.x = 0.0f;
	vertices[0].position.y = 0.0f;
	vertices[0].position.z = 1.0f;

	for (DWORD i = 0; i < (DWORD)LatLines - 2; ++i)
	{
		spherePitch = (i + 1) * (3.14f / (LatLines - 1));
		Rotationx = XMMatrixRotationX(spherePitch);
		for (DWORD j = 0; j < (DWORD)LongLines; ++j)
		{
			sphereYaw = j * (6.28f / (LongLines));
			Rotationy = XMMatrixRotationZ(sphereYaw);
			currVertPos = XMVector3TransformNormal(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), (Rotationx * Rotationy));
			currVertPos = XMVector3Normalize(currVertPos);
			vertices[i * LongLines + j + 1].position.x = XMVectorGetX(currVertPos);
			vertices[i * LongLines + j + 1].position.y = XMVectorGetY(currVertPos);
			vertices[i * LongLines + j + 1].position.z = XMVectorGetZ(currVertPos);
		}
	}

	vertices[NumSphereVertices - 1].position.x = 0.0f;
	vertices[NumSphereVertices - 1].position.y = 0.0f;
	vertices[NumSphereVertices - 1].position.z = -1.0f;


	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * NumSphereVertices;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;

	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = &vertices[0];

	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &sphereVertBuffer)))
	{
		return false;
	}

	std::vector<DWORD> indices(NumSphereFaces * 3);

	int k = 0;
	for (DWORD l = 0; l < (DWORD)LongLines - 1; ++l)
	{
		indices[k] = 0;
		indices[k + 1] = l + 1;
		indices[k + 2] = l + 2;
		k += 3;
	}

	indices[k] = 0;
	indices[k + 1] = LongLines;
	indices[k + 2] = 1;
	k += 3;

	for (DWORD i = 0; i < (DWORD)LatLines - 3; ++i)
	{
		for (DWORD j = 0; j < (DWORD)LongLines - 1; ++j)
		{
			indices[k] = i * LongLines + j + 1;
			indices[k + 1] = i * LongLines + j + 2;
			indices[k + 2] = (i + 1) * LongLines + j + 1;

			indices[k + 3] = (i + 1) * LongLines + j + 1;
			indices[k + 4] = i * LongLines + j + 2;
			indices[k + 5] = (i + 1) * LongLines + j + 2;

			k += 6; // next quad
		}

		indices[k] = (i * LongLines) + LongLines;
		indices[k + 1] = (i * LongLines) + 1;
		indices[k + 2] = ((i + 1) * LongLines) + LongLines;

		indices[k + 3] = ((i + 1) * LongLines) + LongLines;
		indices[k + 4] = (i * LongLines) + 1;
		indices[k + 5] = ((i + 1) * LongLines) + 1;

		k += 6;
	}

	for (DWORD l = 0; l < (DWORD)LongLines - 1; ++l)
	{
		indices[k] = NumSphereVertices - 1;
		indices[k + 1] = (NumSphereVertices - 1) - (l + 1);
		indices[k + 2] = (NumSphereVertices - 1) - (l + 2);
		k += 3;
	}

	indices[k] = NumSphereVertices - 1;
	indices[k + 1] = (NumSphereVertices - 1) - LongLines;
	indices[k + 2] = NumSphereVertices - 2;

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * NumSphereFaces * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;

	iinitData.pSysMem = &indices[0];	

	if (FAILED(device->CreateBuffer(&indexBufferDesc, &iinitData, &sphereIndexBuffer)))
	{
		return false;
	}

	return true;
}

ID3D11ShaderResourceView* SkymapClass::GetTexture()
{
	return m_Texture->GetTexture();
}

void SkymapClass::ShutdownBuffers()
{
	if (SKYMAP_VS)
	{
		SKYMAP_VS_Buffer->Release();
		SKYMAP_VS_Buffer = 0;
	}

	if (SKYMAP_PS)
	{
		SKYMAP_PS_Buffer->Release();
		SKYMAP_PS_Buffer = 0;
	}
}

void SkymapClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &sphereVertBuffer, &stride, &offset);

	deviceContext->IASetIndexBuffer(sphereIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}

int SkymapClass::GetIndexCount()
{
	return NumSphereVertices;
}

bool SkymapClass::LoadTexture(ID3D11Device* device, const WCHAR* filename)
{
	bool result;


	// Create the texture object.
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture->Initialize(device, filename);
	if (!result)
	{
		return false;
	}

	//ID3D11Texture2D* SMTexture = 0;
	//result = CreateDDSTextureFromFileEx(device, L"skymap.dds", 0, D3D11_USAGE_IMMUTABLE, D3D11_BIND_SHADER_RESOURCE,
	//	0, D3D11_RESOURCE_MISC_TEXTURECUBE, false, (ID3D11Resource**)&SMTexture, nullptr);
	//
	//D3D11_TEXTURE2D_DESC SMTextureDesc;
	//SMTexture->GetDesc(&SMTextureDesc);

	//D3D11_SHADER_RESOURCE_VIEW_DESC SMViewDesc;
	//SMViewDesc.Format = SMTextureDesc.Format;
	//SMViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	//SMViewDesc.TextureCube.MipLevels = SMTextureDesc.MipLevels;
	//SMViewDesc.TextureCube.MostDetailedMip = 0;

	//result = device->CreateShaderResourceView(SMTexture, &SMViewDesc, &smrv);

	//D3D11_SAMPLER_DESC sampDesc;
	//ZeroMemory(&sampDesc, sizeof(sampDesc));
	//sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	//sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	//sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	//sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	//sampDesc.MinLOD = 0;
	//sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	//// Create the Sample State
	//result = device->CreateSamplerState(&sampDesc, &CubesTexSamplerState);

	//result->CreateBlendState(&blendDesc, &Transparency);

	//D3D11_RASTERIZER_DESC cmdesc;

	//ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));
	//cmdesc.FillMode = D3D11_FILL_SOLID;
	//cmdesc.CullMode = D3D11_CULL_BACK;
	//cmdesc.FrontCounterClockwise = true;
	//result = device->CreateRasterizerState(&cmdesc, &CCWcullMode);

	//cmdesc.FrontCounterClockwise = false;

	//result = device->CreateRasterizerState(&cmdesc, &CWcullMode);

	return true;
}

void SkymapClass::ReleaseTexture()
{
	// Release the texture object.
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}