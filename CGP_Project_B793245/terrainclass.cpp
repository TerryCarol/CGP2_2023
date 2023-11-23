////////////////////////////////////////////////////////////////////////////////
// Filename: terrainclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "terrainclass.h"

TerrainClass::TerrainClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Texture = 0;
}

TerrainClass::TerrainClass(const TerrainClass& other)
{
}

TerrainClass::~TerrainClass()
{
}

bool TerrainClass::Initialize(ID3D11Device* device, const WCHAR* textureFilename, int terrainWidth, int terrainHeight)
{
	bool result;

	m_terrainWidth = terrainWidth;
	m_terrainHeight = terrainHeight;

	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	// Load the texture for this bitmap.
	result = LoadTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}
}

void TerrainClass::Shutdown()
{
	ShutdownBuffers();
}

bool TerrainClass::Render(ID3D11DeviceContext* deviceContext, int positionX, int positionY)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return true;
}

int TerrainClass::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView* TerrainClass::GetTexture()
{
	return m_Texture->GetTexture();
}

bool TerrainClass::InitializeBuffers(ID3D11Device* device)
{
//	m_vertexCount = (m_terrainWidth - 1) * (m_terrainHeight - 1) * 8;
	m_vertexCount = 4;
	m_indexCount = m_vertexCount;

	VertexType* vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	unsigned long* indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	int index = 0;
	float width((float)m_terrainWidth / 2.0f);
	float height((float)m_terrainHeight / 2.0f);

	vertices[index].position = XMFLOAT3(width, 0.0f, height);
	vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	indices[index] = index;
	index++;

	vertices[index].position = XMFLOAT3((-1) * width, 0.0f, height);
	vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	indices[index] = index;
	index++;

	vertices[index].position = XMFLOAT3(width, 0.0f, (-1) * height);
	vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	indices[index] = index;
	index++;

	vertices[index].position = XMFLOAT3((-1) * width, 0.0f, (-1) * height);
	vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	indices[index] = index;
	index++;

	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer)))
	{
		return false;
	}

	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer)))
	{
		return false;
	}

	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void TerrainClass::ShutdownBuffers()
{
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
}

void TerrainClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}

bool TerrainClass::LoadTexture(ID3D11Device* device, const WCHAR* filename)
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

	return true;
}