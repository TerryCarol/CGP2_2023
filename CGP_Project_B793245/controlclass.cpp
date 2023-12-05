////////////////////////////////////////////////////////////////////////////////
// Filename: controlclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "controlclass.h"

XMVECTOR playerPosition;

XMVECTOR DefaultUp2 = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
XMVECTOR DefaultForward2 = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
XMVECTOR DefaultRight2 = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

float turnLeftRight = 0.0f;
float movementBackForward = 0.0f;

float playerSpeed = 0.0f;

ControlClass::ControlClass()
{
	m_position.x = 0.0f;
	m_position.y = 0.0f;
	m_position.z = 0.0f;

	m_rotation.x = 0.0f;
	m_rotation.y = 0.0f;
	m_rotation.z = 0.0f;
}

ControlClass::ControlClass(const ControlClass& other)
{
}


ControlClass::~ControlClass()
{
}

void ControlClass::SetPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}


void ControlClass::SetRotation(float x, float y, float z)
{
	m_rotation.x = x;
	m_rotation.y = y;
	m_rotation.z = z;
}

XMFLOAT3 ControlClass::GetPosition()
{
	return m_position;
}

XMFLOAT3 ControlClass::GetRotation()
{
	return m_rotation;
}

void ControlClass::SetSyncSpeed(float syncSpeed)
{
	playerSpeed = syncSpeed;
}

void ControlClass::TurnLeft()
{
	turnLeftRight -= playerSpeed;
}
void ControlClass::TurnRight()
{
	turnLeftRight += playerSpeed;
}

void ControlClass::MoveBack()
{
	movementBackForward -= playerSpeed;
}
void ControlClass::MoveForward()
{
	movementBackForward += playerSpeed;
}

void ControlClass::InitializePosition(XMMATRIX worldMatrix)
{
	SetPosition(0.0f, 1.0f, 0.0f);
	playerPosition = XMVectorSet(m_position.x, m_position.y, m_position.z, 0.0f);
	m_localMatrix = worldMatrix;
}

void ControlClass::UpdatePlayer()
{
	//DefaultRight = XMVector3TransformCoord(DefaultRight, m_localMatrix);
	//DefaultForward = XMVector3TransformCoord(DefaultForward, m_localMatrix);
	//DefaultUp = XMVector3TransformCoord(DefaultUp, m_localMatrix);

	//camPosition += turnLeftRight * camRight;
	//camPosition += moveBackForward * camForward;

	//moveLeftRight = 0.0f;sssssa
	//moveBackForward = 0.0f;

	//XMVectorSetY(camPosition, 1.5f);

	//camPosition -= 4.0f * camForward;
	//camTarget = camPosition + camTarget;

	//m_viewMatrix = XMMatrixLookAtLH(camPosition, camTarget, camUp);

	//camPosition += 4.0f * camForward;

	return;
}