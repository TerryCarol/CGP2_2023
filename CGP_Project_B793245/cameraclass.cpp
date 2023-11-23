////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "cameraclass.h"

XMVECTOR up, position, lookAt;
float yaw, pitch, roll;
XMMATRIX rotationMatrix;

XMVECTOR camPosition;
XMVECTOR camTarget;
XMVECTOR camUp;
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


XMFLOAT3 CameraClass::GetPosition()
{
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
	speed = syncSpeed;
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
	camPosition = XMVectorSet(0.0f, 0.0f, -10.0f, 0.0f);
}

// This uses the position and rotation of the camera to build and to update the view matrix.
void CameraClass::Render()
{
	camRotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, 0);

	camTarget = XMVector3TransformCoord(DefaultForward, camRotationMatrix);
	camTarget = XMVector3Normalize(camTarget);

	// Free-Look Camera
	camRight = XMVector3TransformCoord(DefaultRight, camRotationMatrix);
	camForward = XMVector3TransformCoord(DefaultForward, camRotationMatrix);
	camUp = XMVector3Cross(camForward, camRight);
	///////////////**************new**************////////////////////

	camPosition += moveLeftRight * camRight;
	camPosition += moveBackForward * camForward;

	moveLeftRight = 0.0f;
	moveBackForward = 0.0f;

	camTarget = camPosition + camTarget;

	m_viewMatrix = XMMatrixLookAtLH(camPosition, camTarget, camUp);

	return;
}

void CameraClass::GetViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
}