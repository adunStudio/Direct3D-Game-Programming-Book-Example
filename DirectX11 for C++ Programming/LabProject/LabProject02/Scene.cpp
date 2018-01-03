#include "stdafx.h"
#include "Scene.h"


CScene::CScene()
{
	m_ppShaders = nullptr;
	m_nShaders = 0;

	m_ppObjects = nullptr;
	m_nObjects = 0;
}


CScene::~CScene()
{
}

bool CScene::onProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return false;
}

bool CScene::onProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return false;
}

void CScene::buildObjects(ID3D11Device* pd3dDevice)
{
	
	// 셰이더 객체에 대한 포인터들의 배열을 정의한다.
	m_nShaders = 1;
	m_nObjects = 1;

	m_ppShaders = new CObjectShader*[m_nShaders];

	// CObjectShader 클래스 객체를 생성한다.
	m_ppShaders[0] = new CObjectShader();
	m_ppShaders[0]->createShader(pd3dDevice, m_nObjects);

	// 게임 객체에 대한 포인터들의 배열을 정의한다.
	m_ppObjects = new CGameObject*[m_nObjects];

	// 삼각형 메시(TriangleMesh)를 생성한다.
	//CTriangleMesh* pMesh = new CTriangleMesh(pd3dDevice);

	// 정육면체 메시(CubeMesh)를 생성한다.
	CCubeMesh* pMesh = new CCubeMesh(pd3dDevice, 1.0f, 1.0f, 1.0f);

	// 삼각형 객체(CTriangleObject)를 생성하고 삼각형 메시를 연결한다.
	CTriangleObject* pObject = new CTriangleObject();
	pObject->setMesh(pMesh);
	

	// 삼각형 객체를 셰이더 객체에 연결한다.
	m_ppShaders[0]->addObject(pObject);
	m_ppObjects[0] = pObject;

	pMesh->release();

	
}

void CScene::releaseObjects()
{
	if (m_ppShaders)
	{
		for (int i = 0; i < m_nShaders; ++i)
		{
			if (m_ppShaders[i])
				delete m_ppShaders[i];

			delete[] m_ppShaders;
		}
	}

	if (m_ppObjects)
	{
		for (int i = 0; i < m_nObjects; ++i)
			m_ppObjects[i]->release();

		delete[] m_ppObjects;
	}
}

bool CScene::processInput()
{
	return false;
}

void CScene::animateObjects(float timeElapsed)
{
	for (int i = 0; i < m_nObjects; ++i)
		m_ppObjects[i]->animate(timeElapsed);
}

void CScene::render(ID3D11DeviceContext* pd3dDeviceContext, CCamera* camera)
{
	for (int i = 0; i < m_nShaders; ++i)
	{
		m_ppShaders[i]->render(pd3dDeviceContext, camera);
	}
}
