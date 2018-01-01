#include "stdafx.h"
#include "Object.h"


CGameObject::CGameObject()
{
	D3DXMatrixIdentity(&m_d3dxmtxWorld);
	m_mesh = nullptr;
	m_references = 1;
}

CGameObject::~CGameObject()
{
	if (m_mesh)
		m_mesh->release();
}

void CGameObject::addRef()
{
	m_references++;
}

void CGameObject::release()
{
	if (m_references > 0)
		m_references--;

	if (m_references <= 0)
		delete this;
}

// ��ü�� �޽ø� �����ϱ� ���� �Լ��̴�.
// �̹� ����� �޽ð� ������ ����� �޽��� ���� ī���͸� ���ҽ�Ų��.
// ���ο� �޽ø� �����ϰ� ���ο� �޽��� ���� ī���͸� ������Ų��.
void CGameObject::setMesh(CMesh* mesh)
{
	if (m_mesh)
		m_mesh->release();

	m_mesh = mesh;

	if (m_mesh)
		m_mesh->addRef();
}

void CGameObject::animate(float fTimeElapsed)
{

}

// ��ü�� �������ϴ� ���� ��ü�� ����� �޽ø� �������ϴ� ���̴�.
void CGameObject::render(ID3D11DeviceContext* pd3dDeviceContext)
{
	if (m_mesh)
	{		
		m_mesh->render(pd3dDeviceContext);
	}
}

/* CTriangleObject */

CTriangleObject::CTriangleObject()
{

}

CTriangleObject::~CTriangleObject()
{

}

void CTriangleObject::animate(float fTimeElapsed)
{
	CGameObject::animate(fTimeElapsed);
}

void CTriangleObject::render(ID3D11DeviceContext* pd3dDeviceContext)
{
	CGameObject::render(pd3dDeviceContext);
}