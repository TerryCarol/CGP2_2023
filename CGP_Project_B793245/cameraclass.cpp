////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "cameraclass.h"

XMVECTOR up, position, lookAt;
float yaw, pitch, roll;
XMMATRIX rotationMatrix;

XMVECTOR camPosition;
XMVECTOR camTarget;
XMVECTOR camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
XMVECTOR DefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
XMVECTOR DefaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
XMVECTOR camForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
XMVECTOR camRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

XMMATRIX camRotationMatrix;
float moveLeftRight = 0.0f;
float moveBackForward = 0.0f;

float speed = 0.0f;

CameraClass::CameraClass()
{
	m_position.x = 0.0f;
	m_position.y = 0.0f;
	m_position.z = 0.0f;

	m_rotation.x = 0.0f;
	m_rotation.y = 0.0f;
	m_rotation.z = 0.0f;
}


CameraClass::CameraClass(const CameraClass& other)
{
}


CameraClass::~CameraClass()
{
}


void CameraClass::SetPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}


void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotation.x = x;
	m_rotation.y = y;
	m_rotation.z = z;
}

XMFLOAT3 CameraClass::GetCamTargetPoint() {
	XMStoreFloat3(&m_targetPoint, camTarget);
	return m_targetPoint;
}


XMFLOAT3 CameraClass::GetPosition()
{
	XMStoreFloat3(&m_position, camPosition);
	return m_position;
}


XMFLOAT3 CameraClass::GetRotation()
{
	return m_rotation;
}

void CameraClass::SetYaw(float newYaw)
{
	yaw = newYaw;
}

void CameraClass::SetPitch(float newPitch)
{
	pitch = newPitch;
}

void CameraClass::SetSyncSpeed(float syncSpeed)
{
	speed = syncSpeed / 2;
}

void CameraClass::MoveCameraLeft()
{
	moveLeftRight -= speed;
}
void CameraClass::MoveCameraRight()
{
	moveLeftRight += speed;
}

void CameraClass::MoveCameraBack()
{
	moveBackForward -= speed;
}
void CameraClass::MoveCameraForward()
{
	moveBackForward += speed;
}

void CameraClass::InitializeCameraPosition()
{
	SetPosition(0.0f, 2.0f, -10.0f);
	camPosition = XMVectorSet(m_position.x, m_position.y, m_position.z, 0.0f);
}

// This uses the position and rotation of the camera to build and to update the view matrix.
void CameraClass::Render()
{
	camRotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, 0);

	camTarget = XMVector3TransformCoord(DefaultForward, camRotationMatrix);
	camTarget = XMVector3Normalize(camTarget);

	XMMATRIX RotateYTempMatrix;
	RotateYTempMatrix = XMMatrixRotationY(yaw);

	// Free-Look 에서 고도 조정 제한한 카메라
	camRight = XMVector3TransformCoord(DefaultRight, RotateYTempMatrix);
	camForward = XMVector3TransformCoord(DefaultForward, RotateYTempMatrix);
	camUp = XMVector3TransformCoord(camUp, RotateYTempMatrix);
	//camUp = XMVector3Cross(camForward, camRight);
	///////////////**************new**************////////////////////
	
	camPosition += moveLeftRight * camRight;
	camPosition += moveBackForward * camForward;

	moveLeftRight = 0.0f;
	moveBackForward = 0.0f;

	camPosition -= 4.0f * camForward;

	camTarget = camPosition + camTarget;
	m_viewMatrix = XMMatrixLookAtLH(camPosition, camTarget, camUp);

	camPosition += 4.0f * camForward;

	return;
}

void CameraClass::GetViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
}