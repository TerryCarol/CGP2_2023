////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <directxmath.h>

using namespace DirectX;

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "textureclass.h"

#include <fstream>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////
class ModelClass
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
	    XMFLOAT2 texture;
		XMFLOAT3 normal;
	};

	struct FaceType
	{
		int vIndex1, vIndex2, vIndex3;
		int tIndex1, tIndex2, tIndex3;
		int nIndex1, nIndex2, nIndex3;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	struct InstanceType
	{
		XMFLOAT3 position;
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*, const WCHAR*, const WCHAR*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	int getVertexCount();
	int GetInstanceCount();
	ID3D11ShaderResourceView* GetTexture();

	bool LoadModel(const WCHAR*);
	void ReleaseModel();

	//const XMVECTOR& GetPositionVector() const;
	//const XMFLOAT3& GetPositionFloat3() const;
	//const XMVECTOR& GetRotationVector() const;
	//const XMFLOAT3& GetRotationFloat3() const;

	//void SetPosition(const XMVECTOR& pos);
	//void SetPosition(const XMFLOAT3& pos);
	//void SetPosition(float x, float y, float z);
	//void AdjustPosition(const XMVECTOR& pos);
	//void AdjustPosition(const XMFLOAT3& pos);
	//void AdjustPosition(float x, float y, float z);
	//void SetRotation(const XMVECTOR& rot);
	//void SetRotation(const XMFLOAT3& rot);
	//void SetRotation(float x, float y, float z);
	//void AdjustRotation(const XMVECTOR& rot);
	//void AdjustRotation(const XMFLOAT3& rot);
	//void AdjustRotation(float x, float y, float z);
	//void SetLookAtPos(XMFLOAT3 lookAtPos);
	//const XMVECTOR& GetForwardVector();
	//const XMVECTOR& GetRightVector();
	//const XMVECTOR& GetBackwardVector();
	//const XMVECTOR& GetLeftVector();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, const WCHAR*);
	void ReleaseTexture();

	bool ReadFileCounts(const WCHAR*);
	bool LoadDataStructures(const WCHAR*, int, int, int, int);
	//void UpdateWorldMatrix();

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer, * m_instanceBuffer;
	int m_vertexCount, m_indexCount, m_instanceCount, m_textureCount, m_normalCount, m_faceCount;
	TextureClass* m_Texture;

	ModelType* m_model;

	XMMATRIX worldMatrix = XMMatrixIdentity();

	XMVECTOR posVector;
	XMVECTOR rotVector;
	XMFLOAT3 pos;
	XMFLOAT3 rot;

	const XMVECTOR DEFAULT_FORWARD_VECTOR = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const XMVECTOR DEFAULT_UP_VECTOR = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_BACKWARD_VECTOR = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	const XMVECTOR DEFAULT_LEFT_VECTOR = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_RIGHT_VECTOR = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	XMVECTOR vec_forward;
	XMVECTOR vec_left;
	XMVECTOR vec_right;
	XMVECTOR vec_backward;
}
;
#endif