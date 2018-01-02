#pragma once

class CVertex
{
	D3DXVECTOR3						m_d3dxvPosition;
public:
	CVertex() { m_d3dxvPosition = D3DXVECTOR3(0, 0, 0); }
	CVertex(D3DXVECTOR3 d3dxvPosition) { m_d3dxvPosition = d3dxvPosition; }
	~CVertex() { }
};


// 삼각형을 하나의 메시로 표현하고 렌더링하기 위한 클래스
class CMesh
{
private:
	// Cmesh 클래스의 객체의 참조(Reference)와 관련된 멤버 변수와 함수를 선언한다.
	int m_references;

protected:
	// 정점 데이터를 저장하기 위한 정점 버퍼 인터페이스 포인터를 선언한다.
	ID3D11Buffer* m_pd3dVertexBuffer;

	UINT m_vertices; // 정점 버퍼의 정점 개수
	UINT m_stride;   // 정점의 바이트 수
	UINT m_offset;   // 정점 버퍼의 어디에서부터 시작하는지를 나타내는 변수

	// 정점 데이터가 어떤 프리미티브를 표현하고 있는지를 나타내는 변수
	D3D11_PRIMITIVE_TOPOLOGY m_d3dPrimitiveTopology;

	// 래스터라이저 상태 객체에 대한 인터페이스 포인터
	ID3D11RasterizerState* m_pd3dRasterizerState;

public:
	CMesh();
	virtual ~CMesh();

	void addRef();
	void release();

	// 정점 데이터를 렌더링 하는 멤버 함수
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
