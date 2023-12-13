////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"

float speedGraphics = 0;

GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;

	m_Terrain = 0;
	m_Skybox = 0;

	m_Model = 0;
	m_Model2 = 0;
	m_PlayerModel = 0;
	m_PlayerTurretModel = 0;

	m_LightShader = 0;
	m_Light = 0;

	m_TextureShader = 0;
	m_Bitmap = 0;

	m_Text = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	XMMATRIX baseViewMatrix;

	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->InitializeCameraPosition();

	//스카이맵 생성
	m_Skybox = new SkymapClass;
	if (!m_Skybox)
	{
		return false;
	}
	
	//스카이맵 초기화
	result = m_Skybox->Initialize(m_D3D->GetDevice(), (WCHAR*)L"./data/mountain_skymap.dds", 10, 10);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize skymap object.", L"Error", MB_OK);
	}

	////Reset sphereWorld
	//sphereWorld = XMMatrixIdentity();

	////Define sphereWorld's world space matrix
	//Scale = XMMatrixScaling(5.0f, 5.0f, 5.0f);
	////Make sure the sphere is always centered around camera
	//Translation = XMMatrixTranslation(cameraPos.x, cameraPos.y, cameraPos.z);

	////Set sphereWorld's world space using the transformations
	//sphereWorld = Scale * Translation;

	////the loaded models world space
	//meshWorld = XMMatrixIdentity();

	//Rotation = XMMatrixRotationY(3.14f);
	//Scale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	//Translation = XMMatrixTranslation(0.0f, 0.0f, 0.0f);

	////meshWorld 변수구조체가 왜 따로 있어?
	//meshWorld = Rotation * Scale * Translation;


	//지형(바닥)을 생성
	m_Terrain = new ModelClass;
	if (!m_Terrain)
	{
		return false;
	}

	//지형 초기화
	result = m_Terrain->Initialize(m_D3D->GetDevice(), L"./data/ground.obj", L"./data/grass.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize terrain object.", L"Error", MB_OK);
	}

	// 플레이어 차체 모델 오브젝트 생성
	m_PlayerModel = new ModelClass;
	if (!m_PlayerModel)
	{
		return false;
	}

	// 플레이어 차체 모델 초기화
	result = m_PlayerModel->Initialize(m_D3D->GetDevice(), L"./data/tank_hull.obj", L"./data/color-palette.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// 플레이어 포탑 모델 오브젝트 생성
	m_PlayerTurretModel = new ModelClass;
	if (!m_PlayerTurretModel)
	{
		return false;
	}

	// 플레이어 포탑 모델 초기화
	result = m_PlayerTurretModel->Initialize(m_D3D->GetDevice(), L"./data/tank_turret.obj", L"./data/color-palette.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// 모델 오브젝트 생성
	m_Model = new ModelClass;
	if(!m_Model)
	{
		return false;
	}

	// 모델 초기화
	result = m_Model->Initialize(m_D3D->GetDevice(), L"./data/tank_edited_2.obj", L"./data/color-palette.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// 모델 2 오브젝트 생성
	m_Model2 = new ModelClass;
	if (!m_Model2)
	{
		return false;
	}

	// 모델 2 초기화
	result = m_Model2->Initialize(m_D3D->GetDevice(), L"./data/tank_edited_2.obj", L"./data/color-palette.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize second model object.", L"Error", MB_OK);
		return false;
	}

	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	// Initialize the light object.
//	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetAmbientColor(0.5f, 0.5f, 0.5f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
//	m_Light->SetDiffuseColor(0.0f, 0.0f, 0.0f, 1.0f);
//	m_Light->SetDirection(0.0f, 0.0f, 1.0f);
//	m_Light->SetDirection(1.0f, 0.0f, 0.0f);
	m_Light->SetDirection(1.0f, 0.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);

	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}


	// Create the bitmap object.
	//m_Bitmap = new BitmapClass;
	//if (!m_Bitmap)
	//{
	//	return false;
	//}

	//// Initialize the bitmap object.
	//result = m_Bitmap->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/seafloor.dds", 256, 256);
	//if (!result)
	//{
	//	MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
	//	return false;
	//}

	// Initialize a base view matrix with the camera for 2D user interface rendering.
	m_Camera->SetPosition(0.0f, 0.0f, -1.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	// Create the text object.
	m_Text = new TextClass;
	if (!m_Text)
	{
		return false;
	}

	// Initialize the text object.
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void GraphicsClass::SetSyncSpeed(float syncSpeed)
{
	speedGraphics = syncSpeed;
	m_Camera->SetSyncSpeed(syncSpeed);
}

void GraphicsClass::CameraRotation(float a, float b)
{
	m_Camera->SetYaw(a);
	m_Camera->SetPitch(b);
}

void GraphicsClass::CameraMovement(char input)
{
	if (input == 'W') {
		m_Camera->MoveCameraForward();
		return;
	}
	if (input == 'S') {
		m_Camera->MoveCameraBack();
		return;
	}
	if (input == 'A') {
		m_Camera->MoveCameraLeft();
		return;
	}
	if (input == 'D') {
		m_Camera->MoveCameraRight();
		return;
	}

	return;
}

void GraphicsClass::Shutdown()
{
	// 플레이어 모델
	if (m_PlayerModel)
	{
		m_PlayerModel->Shutdown();
		delete m_PlayerModel;
		m_PlayerModel = 0;
	}

	// 플레이어 터렛 모델
	if (m_PlayerTurretModel)
	{
		m_PlayerTurretModel->Shutdown();
		delete m_PlayerTurretModel;
		m_PlayerTurretModel = 0;
	}

	// 모델1
	if(m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	// 모델2
	if (m_Model2)
	{
		m_Model2->Shutdown();
		delete m_Model2;
		m_Model2 = 0;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}
	
	// Release the bitmap object.
	if (m_Terrain)
	{
		m_Terrain->Shutdown();
		delete m_Terrain;
		m_Terrain = 0;
	}

	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	return;
}

bool GraphicsClass::Frame()
{
	bool result;

	static float rotation = 0.0f;


	// Update the rotation variable each frame.
	rotation += XM_PI * 0.005f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	// Render the graphics scene.
	result = Render(rotation);
	if(!result)
	{
		return false;
	}

	return true;
}

bool GraphicsClass::Frame(int fps, float cpu, int mouseX, int mouseY)
{
	bool result;
	static float rotation = 0.0f;

	// Update the rotation variable each frame.
	rotation += (float)XM_PI * 0.005f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	// Set the frames per second.
	result = m_Text->SetResolutionSize(m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	// Set the frames per second.
	result = m_Text->SetFPS(fps, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	// Set the cpu usage.
	result = m_Text->SetCPU(cpu, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	// Set the location of the mouse.
	result = m_Text->SetMousePosition(mouseX, mouseY, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	// Render the graphics scene.
	result = Render(rotation);
	if (!result)
	{
		return false;
	}

	// Set the position of the camera.
//	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);

	return true;
}

bool GraphicsClass::Render(float rotation)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix, tempMatrix, translateMatrix;
	XMFLOAT3 tempFloat;
	bool result;
	
	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	// 구의 위치로 변환합니다.
	translateMatrix = XMMatrixTranslation(-5.0f, 1.0f, 5.0f);
	worldMatrix = XMMatrixMultiply(worldMatrix, translateMatrix);

	m_D3D->TurnOffCulling();
	m_D3D->TurnDSLessEqualOn();

	m_D3D->GetWorldMatrix(worldMatrix_skymap);
	XMFLOAT3 cameraPos = m_Camera->GetPosition();

	scaleMatrix_skymap = XMMatrixScaling(100.0f, 100.0f, 100.0f);
	transMatrix_skymap = XMMatrixTranslation(cameraPos.x, cameraPos.y - 10.0f, cameraPos.z);
	rotateMatrix_skymap = XMMatrixRotationY(256 * 0.0174532925f);

	worldMatrix_skymap = scaleMatrix_skymap * rotateMatrix_skymap * transMatrix_skymap;

	//스카이맵 랜더링
	m_Skybox->Render(m_D3D->GetDeviceContext(), worldMatrix_skymap, viewMatrix, projectionMatrix);

	m_D3D->TurnDSLessEqualOff();
	m_D3D->TurnOnCulling();

	// 플레이어 모델 렌더링
	tempFloat = m_Camera->GetPosition();

	tempMatrix = XMMatrixTranslation(2.0f * (tempFloat.x + (0.0f)), 2.0f * (tempFloat.y + (-2.0f)), 2.0f * (tempFloat.z + (-0.0f)));
	tempMatrix *= XMMatrixScaling(0.5f, 0.5f, 0.5f);

	// 플레이어 모델 버퍼 할당
	m_PlayerModel->Render(m_D3D->GetDeviceContext());

	// 플레이어 모델 셰이더 호출
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_PlayerModel->getVertexCount(), m_PlayerModel->GetIndexCount(), m_PlayerModel->GetInstanceCount(),
		tempMatrix, viewMatrix, projectionMatrix,
		m_PlayerModel->GetTexture(),
		m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

	// 플레이어 터렛 모델 렌더링
	tempFloat = m_Camera->GetPosition();

	tempMatrix = XMMatrixTranslation(2.0f * (tempFloat.x + (0.0f)), 2.0f * (tempFloat.y + (-2.0f)), 2.0f * (tempFloat.z + (-0.0f)));
	tempMatrix *= XMMatrixScaling(0.5f, 0.5f, 0.5f);
	

	// 플레이어 터렛 모델 버퍼 할당
	m_PlayerTurretModel->Render(m_D3D->GetDeviceContext());

	// 플레이어 터렛 모델 셰이더 호출
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_PlayerTurretModel->getVertexCount(), m_PlayerTurretModel->GetIndexCount(), m_PlayerTurretModel->GetInstanceCount(),
		tempMatrix, viewMatrix, projectionMatrix,
		m_PlayerTurretModel->GetTexture(),
		m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

	// 모델 1 렌더링
	tempMatrix = worldMatrix;
	tempMatrix *= XMMatrixTranslation(-2.0f, -1.0f, 1.0f);
	tempMatrix *= XMMatrixScaling(0.5f, 0.5f, 0.5f);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model->Render(m_D3D->GetDeviceContext());

	// Render the model using the light shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->getVertexCount(), m_Model->GetIndexCount(), m_Model->GetInstanceCount(),
		tempMatrix, viewMatrix, projectionMatrix,
		m_Model->GetTexture(), 
		m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

	// 모델 2 렌더링
	tempMatrix = worldMatrix;
	tempMatrix *= XMMatrixTranslation(6.0f, -1.0f, 5.0f);
	tempMatrix *= XMMatrixRotationY(100.0f);
	tempMatrix *= XMMatrixScaling(0.5f, 0.5f, 0.5f);
	//월드매트릭스는 말 그대로 월드야 모델별 상대좌표계를 쓰던가 모델 함수로 직접 이동시키는 것이 안전할 것임.

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model2->Render(m_D3D->GetDeviceContext());

	// Render the model using the light shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model2->getVertexCount(), m_Model2->GetIndexCount(), m_Model2->GetInstanceCount(),
		tempMatrix, viewMatrix, projectionMatrix,
		m_Model2->GetTexture(),
		m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

	//지형모델 랜더링
	worldMatrix = XMMatrixTranslation(0.0f, 0.0f, 0.0f);

	m_Terrain->Render(m_D3D->GetDeviceContext());

	// Render the terrain using the light shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), 0, m_Terrain->GetIndexCount(), 0,
		worldMatrix, viewMatrix, projectionMatrix,
		m_Terrain->GetTexture(),
		m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());

	if(!result)
	{
		return false;
	}

	// 월드 행렬을 재설정합니다.
	m_D3D->GetWorldMatrix(worldMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();

	// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
	//result = m_Terrain->Render(m_D3D->GetDeviceContext(), 200, 200);
	//if (!result)
	//{
	//	return false;
	//}

	// Render the bitmap with the texture shader.
	//result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Terrain->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_Terrain->GetTexture());
	//if (!result)
	//{
	//	return false;
	//}

	// Turn on the alpha blending before rendering the text.
	m_D3D->TurnOnAlphaBlending();

	// Render the text strings.
	result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	// Turn off alpha blending after rendering the text.
	m_D3D->TurnOffAlphaBlending();

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}