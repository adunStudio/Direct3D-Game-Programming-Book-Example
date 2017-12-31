#pragma once

#include "Vertex.h"

// �ﰢ���� �ϳ��� �޽÷� ǥ���ϰ� �������ϱ� ���� Ŭ����
class CMesh
{
private:
	// Cmesh Ŭ������ ��ü�� ����(Reference)�� ���õ� ��� ������ �Լ��� �����Ѵ�.
	int m_references;

protected:
	// ���� �����͸� �����ϱ� ���� ���� ���� �������̽� �����͸� �����Ѵ�.
	ID3D11Buffer* m_pd3dVertexBuffer;

	UINT m_vertices; // ���� ������ ���� ����
	UINT m_stride;   // ������ ����Ʈ ��
	UINT m_offset;   // ���� ������ ��𿡼����� �����ϴ����� ��Ÿ���� ����

	// ���� �����Ͱ� � ������Ƽ�긦 ǥ���ϰ� �ִ����� ��Ÿ���� ����
	D3D11_PRIMITIVE_TOPOLOGY m_d3dPrimitiveTopology;

public:
	CMesh(ID3D11Device* pd3dDevice);
	virtual ~CMesh();

	void addRef();
	void release();

	// ���� �����͸� ������ �ϴ� ��� �Լ�
	virtual void render(ID3D11DeviceContext* pd3dDeviceContext);
};

class CTriangleMesh : public CMesh
{
public:
	CTriangleMesh(ID3D11Device* pd3dDevice);
	virtual ~CTriangleMesh();

	virtual void render(ID3D11DeviceContext* pd3dDeviceContext);
};
