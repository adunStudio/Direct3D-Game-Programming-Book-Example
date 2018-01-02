#pragma once

class CVertex
{
	D3DXVECTOR3						m_d3dxvPosition;
public:
	CVertex() { m_d3dxvPosition = D3DXVECTOR3(0, 0, 0); }
	CVertex(D3DXVECTOR3 d3dxvPosition) { m_d3dxvPosition = d3dxvPosition; }
	~CVertex() { }
};


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

	// �����Ͷ����� ���� ��ü�� ���� �������̽� ������
	ID3D11RasterizerState* m_pd3dRasterizerState;

public:
	CMesh();
	virtual ~CMesh();

	void addRef();
	void release();

	// ���� �����͸� ������ �ϴ� ��� �Լ�
	virtual void render(ID3D11DeviceContext* pd3dDeviceContext);

	virtual void setRasterizerState(ID3D11Device* pd3dDevice);
};

class CTriangleMesh : public CMesh
{
public:
	CTriangleMesh(ID3D11Device* pd3dDevice);
	virtual ~CTriangleMesh();

	virtual void render(ID3D11DeviceContext* pd3dDeviceContext);

	virtual void setRasterizerState(ID3D11Device* pd3dDevice);
};
