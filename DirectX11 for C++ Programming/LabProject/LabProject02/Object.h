#pragma once

#include "Mesh.h"

// ������ ��ü�� ǥ���ϱ� ���� Ŭ����
class CGameObject
{
private:
	int m_references;

public:
	D3DXMATRIX m_d3dxmtxWorld; // ���� ��ȯ ���
	CMesh* m_mesh;             // ���� ������ ��ü���� �ϳ��� �޽ø� ������ �� �ֵ��� �޽�(Mesh)�� ���� �����ͷ� ����

public:
	CGameObject();
	virtual ~CGameObject();

	void addRef();
	void release();

	virtual void setMesh(CMesh* mesh);
	virtual void animate(float fTimeElapsed);
	virtual void render(ID3D11DeviceContext* pd3dDeviceContext);
};

class CTriangleObject : public CGameObject
{
public:
	CTriangleObject();
	virtual ~CTriangleObject();

	virtual void animate(float fTimeElapsed);
	virtual void render(ID3D11DeviceContext* pd3dDeviceContext);
};