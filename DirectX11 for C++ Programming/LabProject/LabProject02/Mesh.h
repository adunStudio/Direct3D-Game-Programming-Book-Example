#pragma once

class CVertex
{
	D3DXVECTOR3						m_d3dxvPosition;
public:
	CVertex() { m_d3dxvPosition = D3DXVECTOR3(0, 0, 0); }
	CVertex(D3DXVECTOR3 d3dxvPosition) { m_d3dxvPosition = d3dxvPosition; }
	~CVertex() { }
};

#define RANDOM_COLOR D3DXCOLOR((rand() * 0xFFFFFF) / RAND_MAX)

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

class CDiffusedVertex
{
private:
	D3DXVECTOR3 m_d3dxvPosition;

	// ������ ����� Ÿ������ ��� ����(D3DXCOLOR ����ü) ����
	D3DXCOLOR m_d3dxcDiffuse;

public:
	CDiffusedVertex(float x, float y, float z, D3DXCOLOR d3dxcDiffuse)
	{
		m_d3dxvPosition = D3DXVECTOR3(x, y, z);
		m_d3dxcDiffuse = d3dxcDiffuse;
	}

	CDiffusedVertex(D3DXVECTOR3 d3dxvPosition, D3DXCOLOR d3dxcDiffuse)
	{
		m_d3dxvPosition = d3dxvPosition;
		m_d3dxcDiffuse = d3dxcDiffuse;
	}

	CDiffusedVertex()
	{
		m_d3dxvPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_d3dxcDiffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	}

	~CDiffusedVertex()
	{

	}
};

class CCubeMesh : public CMesh
{
public:
	CCubeMesh(ID3D11Device* pd3dDevice, float fWidth = 2.0f, float fHeight = 2.0f, float fDepth = 2.0f);
	virtual ~CCubeMesh();

	virtual void render(ID3D11DeviceContext* pd3dDeviceContext);

	virtual void setRasterizerState(ID3D11Device* pd3dDevice);
};