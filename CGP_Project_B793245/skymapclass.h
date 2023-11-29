////////////////////////////////////////////////////////////////////////////////
// Filename: terrainclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TERRAINCLASS_H_
#define _TERRAINCLASS_H_

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <DirectXMath.h>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "textureclass.h"
#include "modelclass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: SkymapClass
////////////////////////////////////////////////////////////////////////////////
class SkymapClass
{
private:
	struct VertexType
	{
		VertexType() {}
		VertexType(float x, float y, float z,
			float u, float v,
			float nx, float ny, float nz,
			float tx, float ty, float tz)
			: position(x, y, z), texture(u, v), normal(nx, ny, nz),
			tangent(tx, ty, tz) {}

		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
		///////////////**************new**************////////////////////
		XMFLOAT3 tangent;
		XMFLOAT3 biTangent;
		///////////////**************new**************////////////////////
	};

	struct SkyboxType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	struct cbPerObject
	{
		XMMATRIX  WVP;
		XMMATRIX World;

		BOOL hasTexture;
		//Because of HLSL structure packing, we will use windows BOOL
		//instead of bool because HLSL packs things into 4 bytes, and
		//bool is only one byte, where BOOL is 4 bytes
		BOOL hasNormMap;
		///////////////**************new**************////////////////////
	};

	//Create material structure
	struct SurfaceMaterial
	{
		int normMapTexArrayIndex;
		bool hasNormMap;
	};

public:
	SkymapClass();
	SkymapClass(const SkymapClass&);
	~SkymapClass();

	bool Initialize(ID3D11Device*, const WCHAR*, XMFLOAT3);
	void Shutdown();
	int GetIndexCount();
	bool Render(ID3D11DeviceContext*);
	
	ID3D11ShaderResourceView* GetTexture();

private:
	void ShutdownBuffers();
	bool InitializeBuffers(ID3D11Device*);
	bool CreateSphere(ID3D11Device*, int, int, XMFLOAT3);
	void RenderBuffers(ID3D11DeviceContext*);
	bool LoadTexture(ID3D11Device*, const WCHAR*);
	void ReleaseTexture();

private:
	int NumSphereVertices;
	int NumSphereFaces;

	XMMATRIX sphereWorld;

	XMMATRIX Rotationx;
	XMMATRIX Rotationz;
	XMMATRIX Rotationy;

	ID3D11Buffer* sphereIndexBuffer = nullptr;
	ID3D11Buffer* sphereVertBuffer = nullptr;
	ID3D11VertexShader* SKYMAP_VS;
	ID3D11PixelShader* SKYMAP_PS;
	ID3D10Blob* SKYMAP_VS_Buffer;
	ID3D10Blob* SKYMAP_PS_Buffer;

	TextureClass* m_Texture;
	SkyboxType* m_skybox;
};

#endif