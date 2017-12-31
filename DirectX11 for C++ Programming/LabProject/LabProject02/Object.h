#pragma once

#include "Mesh.h"

// 게임의 객체를 표현하기 위한 클래스
class CGameObject
{
private:
	int m_references;

public:
	D3DXMATRIX m_d3dxmtxWorld; // 월드 변환 행렬
	CMesh* m_mesh;             // 같은 종류의 객체들이 하나의 메시를 공유할 수 있도록 메시(Mesh)에 대한 포인터로 선언

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