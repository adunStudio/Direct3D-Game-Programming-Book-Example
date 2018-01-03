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
	
	// ���̴� ��ü�� ���� �����͵��� �迭�� �����Ѵ�.
	m_nShaders = 1;
	m_nObjects = 1;

	m_ppShaders = new CObjectShader*[m_nShaders];

	// CObjectShader Ŭ���� ��ü�� �����Ѵ�.
	m_ppShaders[0] = new CObjectShader();
	m_ppShaders[0]->createShader(pd3dDevice, m_nObjects);

	// ���� ��ü�� ���� �����͵��� �迭�� �����Ѵ�.
	m_ppObjects = new CGameObject*[m_nObjects];

	// �ﰢ�� �޽�(TriangleMesh)�� �����Ѵ�.
	//CTriangleMesh* pMesh = new CTriangleMesh(pd3dDevice);

	// ������ü �޽�(CubeMesh)�� �����Ѵ�.
	CCubeMesh* pMesh = new CCubeMesh(pd3dDevice, 1.0f, 1.0f, 1.0f);

	// �ﰢ�� ��ü(CTriangleObject)�� �����ϰ� �ﰢ�� �޽ø� �����Ѵ�.
	CTriangleObject* pObject = new CTriangleObject();
	pObject->setMesh(pMesh);
	

	// �ﰢ�� ��ü�� ���̴� ��ü�� �����Ѵ�.
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
