////////////////////////////////////////////////////////////////////////////////
// Filename: controlclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _CONTROLCLASS_H_
#define _CONTROLCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <directxmath.h>

#include "AlignedAllocationPolicy.h"

using namespace DirectX;


////////////////////////////////////////////////////////////////////////////////
// Class name: CameraClass
////////////////////////////////////////////////////////////////////////////////
class ControlClass : public AlignedAllocationPolicy<16>
{
public:
	ControlClass();
	ControlClass(const ControlClass&);
	~ControlClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);
	void InitializePosition(XMMATRIX);
	void SetSyncSpeed(float);
	void TurnLeft();
	void TurnRight();
	void MoveForward();
	void MoveBack();

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	void UpdatePlayer();

private:
	XMFLOAT3 m_position;
	XMFLOAT3 m_rotation;
	XMMATRIX m_localMatrix;
};

#endif