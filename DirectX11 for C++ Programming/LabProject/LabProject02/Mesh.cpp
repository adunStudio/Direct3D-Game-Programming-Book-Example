#include "stdafx.h"
#include "Mesh.h"


CMesh::CMesh()
{
	m_stride = sizeof(CVertex);
	m_offset = 0;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_references = 1;
	m_pd3dRasterizerState = nullptr;

	m_pd3dIndexBuffer = nullptr;
	m_indices = 0;
	m_startIndex = 0;
	m_baseVertex = 0;
}

CMesh::~CMesh()
{
	if (m_pd3dVertexBuffer)
		m_pd3dVertexBuffer->Release();

	if (m_pd3dRasterizerState)
		m_pd3dRasterizerState->Release();

	if (m_pd3dIndexBuffer)
		m_pd3dIndexBuffer->Release();
}

void CMesh::addRef()
{
	m_references++;
}

void CMesh::release()
{
	m_references--;

	if (m_references == 0)
		delete this;
}

void CMesh::render(ID3D11DeviceContext* pd3dDeviceContext)
{
	if (m_pd3dVertexBuffer)
	{
		pd3dDeviceContext->IASetVertexBuffers(0, 1, &m_pd3dVertexBuffer, &m_stride, &m_offset);
	}

	if (m_pd3dIndexBuffer)
	{
		// 인덱스 버퍼가 있으면 인덱스 버퍼를 디바이스 컨텍스트에 연결한다.
		pd3dDeviceContext->IASetIndexBuffer(m_pd3dIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	}

	pd3dDeviceContext->IASetPrimitiveTopology(m_d3dPrimitiveTopology);

	// 래스터라이저 상태를 디바이스 컨텍스트에 설정한다.
	if (m_pd3dRasterizerState)
		pd3dDeviceContext->RSSetState(m_pd3dRasterizerState);


	// 인덱스 버퍼가 있으면 인덱스 버퍼를 사용하여 메시를 렌더링하고 없으면 정점 버퍼만을 사용하여 메시를 렌더링한다.
	if (m_pd3dIndexBuffer)
		pd3dDeviceContext->DrawIndexed(m_indices, m_startIndex, m_baseVertex);
	else
		pd3dDeviceContext->Draw(m_vertices, m_offset);
}

void CMesh::setRasterizerState(ID3D11Device* pd3dDevice)
{

}


/* CTriangleMesh */

CTriangleMesh::CTriangleMesh(ID3D11Device* pd3dDevice)

{

	// 3. 정점 버퍼의 원소 개수, 정점의 바이트 크기, 프리미티브 유형을 지정한다.
	m_vertices = 3;
	m_stride = sizeof(CDiffusedVertex);
	m_offset = 0;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	// 1. 삼각형의 세 꼭짓점의 위치 벡터를 정의한다. (시계 방향)
	CDiffusedVertex pVertices[3];
	pVertices[0] = CDiffusedVertex(D3DXVECTOR3( 0.0f,  0.5f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	pVertices[1] = CDiffusedVertex(D3DXVECTOR3( 0.5f, -0.5f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	pVertices[2] = CDiffusedVertex(D3DXVECTOR3(-0.5f, -0.5f, 0.0f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));

	// 2. 정점 버퍼를 생성한다.
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = m_stride * m_vertices;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = pVertices;

	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dVertexBuffer);

	setRasterizerState(pd3dDevice);
}

CTriangleMesh::~CTriangleMesh()
{
}

void CTriangleMesh::render(ID3D11DeviceContext* pd3dDeviceContext)
{
	CMesh::render(pd3dDeviceContext);
}

void CTriangleMesh::setRasterizerState(ID3D11Device* pd3dDevice)
{
	D3D11_RASTERIZER_DESC d3dRasterizerDesc;
	ZeroMemory(&d3dRasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

	// 래스터라이저 단계에서 컬링(은면 제거)을 하지 않도록 래스터라이저 상태를 생성한다.
	d3dRasterizerDesc.CullMode = D3D11_CULL_NONE;
	d3dRasterizerDesc.FillMode = D3D11_FILL_SOLID;

	pd3dDevice->CreateRasterizerState(&d3dRasterizerDesc, &m_pd3dRasterizerState);
}

/* CCubeMesh */

CCubeMesh::CCubeMesh(ID3D11Device* pd3dDevice, float fWidth, float fHeight, float fDepth)
{
	// 3. 정점 버퍼의 원소 개수, 정점의 바이트 크기, 프리미티브 유형을 지정한다.
	m_vertices = 8;
	m_stride = sizeof(CDiffusedVertex);
	m_offset = 0;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

	// 1. 삼각형의 세 꼭짓점의 위치 벡터를 정의한다. (시계 방향)

	float fx = fWidth  * 0.5f;
	float fy = fHeight * 0.5f;
	float fz = fDepth  * 0.5f;

	// 정점 버퍼 데이터는 직육면체의 꼭짓점 8개에 대한 정점 데이터를 가진다.

	CDiffusedVertex pVertices[8];
	pVertices[0] = CDiffusedVertex(D3DXVECTOR3(-fx, -fy, -fz), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	pVertices[1] = CDiffusedVertex(D3DXVECTOR3(-fx, -fy, +fz), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	pVertices[2] = CDiffusedVertex(D3DXVECTOR3(+fx, -fy, +fz), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	pVertices[3] = CDiffusedVertex(D3DXVECTOR3(+fx, -fy, -fz), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	pVertices[4] = CDiffusedVertex(D3DXVECTOR3(-fx, +fy, -fz), D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
	pVertices[5] = CDiffusedVertex(D3DXVECTOR3(-fx, +fy, +fz), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
	pVertices[6] = CDiffusedVertex(D3DXVECTOR3(+fx, +fy, +fz), D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	pVertices[7] = CDiffusedVertex(D3DXVECTOR3(+fx, +fy, -fz), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	// 2. 정점 버퍼를 생성한다.
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = m_stride * m_vertices;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = pVertices;

	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dVertexBuffer);


	// 인덱스 버퍼는 전체 36(= 6 * 2 * 3)개의 인덱스를 가진다.
	m_indices = 18;

	WORD pIndices[18];
	pIndices[0] = 5; //5,6,4 - cw
	pIndices[1] = 6; //6,4,7 - ccw
	pIndices[2] = 4; //4,7,0 - cw
	pIndices[3] = 7; //7,0,3 - ccw
	pIndices[4] = 0; //0,3,1 - cw
	pIndices[5] = 3; //3,1,2 - ccw
	pIndices[6] = 1; //1,2,2 - cw 
	pIndices[7] = 2; //2,2,3 - ccw
	pIndices[8] = 2; //2,3,3 - cw  - Degenerated Index
	pIndices[9] = 3; //3,3,7 - ccw - Degenerated Index
	pIndices[10] = 3;//3,7,2 - cw  - Degenerated Index
	pIndices[11] = 7;//7,2,6 - ccw
	pIndices[12] = 2;//2,6,1 - cw
	pIndices[13] = 6;//6,1,5 - ccw
	pIndices[14] = 1;//1,5,0 - cw
	pIndices[15] = 5;//5,0,4 - ccw
	pIndices[16] = 0;
	pIndices[17] = 4;

	// 2. 인덱스 버퍼를 생성한다.
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = sizeof(WORD) * m_indices;
	d3dBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;

	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = pIndices;

	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dIndexBuffer);



	setRasterizerState(pd3dDevice);
}

CCubeMesh:: ~CCubeMesh()
{

}

void CCubeMesh::render(ID3D11DeviceContext* pd3dDeviceContext)
{
	CMesh::render(pd3dDeviceContext);
}

void CCubeMesh::setRasterizerState(ID3D11Device* pd3dDevice)
{
	D3D11_RASTERIZER_DESC d3dRasterizerDesc;
	ZeroMemory(&d3dRasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

	// 래스터라이저 단계에서 컬링(은면 제거)을 하지 않도록 래스터라이저 상태를 생성한다.
	d3dRasterizerDesc.CullMode = D3D11_CULL_NONE;
	d3dRasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;

	pd3dDevice->CreateRasterizerState(&d3dRasterizerDesc, &m_pd3dRasterizerState);
}


/* CAirplaneMesh */
CAirplaneMesh::CAirplaneMesh(ID3D11Device* pd3dDevice, float fWidth, float fHeight, float fDepth, D3DXCOLOR d3dxColor)
{
	m_vertices = 24 * 3;
	m_stride = sizeof(CDiffusedVertex);
	m_offset = 0;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	CDiffusedVertex pVertices[24 * 3];

	float fx = fWidth * 0.5f, fy = fHeight * 0.5f, fz = fDepth * 0.5f;
	float x1 = fx * 0.2f, y1 = fy * 0.2f, x2 = fx * 0.1f, y3 = fy * 0.3f, y2 = ((y1 - (fy - y3)) / x1)*x2 + (fy - y3);
	int i = 0;
	//Upper Plane
	pVertices[i++] = CDiffusedVertex(0.0f, +(fy + y3), -fz, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	pVertices[i++] = CDiffusedVertex(+x1, -y1, -fz, d3dxColor + RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(0.0f, 0.0f, -fz, d3dxColor + RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(0.0f, +(fy + y3), -fz, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	pVertices[i++] = CDiffusedVertex(0.0f, 0.0f, -fz, d3dxColor + RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-x1, -y1, -fz, d3dxColor + RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(+x2, +y2, -fz, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	pVertices[i++] = CDiffusedVertex(+fx, -y3, -fz, d3dxColor + RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+x1, -y1, -fz, d3dxColor + RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(-x2, +y2, -fz, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	pVertices[i++] = CDiffusedVertex(-x1, -y1, -fz, d3dxColor + RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-fx, -y3, -fz, d3dxColor + RANDOM_COLOR);
	//Lower Plane
	pVertices[i++] = CDiffusedVertex(0.0f, +(fy + y3), +fz, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	pVertices[i++] = CDiffusedVertex(0.0f, 0.0f, +fz, d3dxColor + RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+x1, -y1, +fz, d3dxColor + RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(0.0f, +(fy + y3), +fz, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	pVertices[i++] = CDiffusedVertex(-x1, -y1, +fz, d3dxColor + RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(0.0f, 0.0f, +fz, d3dxColor + RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(+x2, +y2, +fz, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	pVertices[i++] = CDiffusedVertex(+x1, -y1, +fz, d3dxColor + RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+fx, -y3, +fz, d3dxColor + RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(-x2, +y2, +fz, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	pVertices[i++] = CDiffusedVertex(-fx, -y3, +fz, d3dxColor + RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-x1, -y1, +fz, d3dxColor + RANDOM_COLOR);
	//Right Plane
	pVertices[i++] = CDiffusedVertex(0.0f, +(fy + y3), -fz, d3dxColor + RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(0.0f, +(fy + y3), +fz, d3dxColor + RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+x2, +y2, -fz, d3dxColor + RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(+x2, +y2, -fz, d3dxColor + RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(0.0f, +(fy + y3), +fz, d3dxColor + RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+x2, +y2, +fz, d3dxColor + RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(+x2, +y2, -fz, d3dxColor + RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+x2, +y2, +fz, d3dxColor + RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+fx, -y3, -fz, d3dxColor + RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(+fx, -y3, -fz, d3dxColor + RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+x2, +y2, +fz, d3dxColor + RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+fx, -y3, +fz, d3dxColor + RANDOM_COLOR);
	//Back/Right Plane
	pVertices[i++] = CDiffusedVertex(+x1, -y1, -fz, d3dxColor + RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+fx, -y3, -fz, d3dxColor + RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+fx, -y3, +fz, d3dxColor + RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(+x1, -y1, -fz, d3dxColor + RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+fx, -y3, +fz, d3dxColor + RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+x1, -y1, +fz, d3dxColor + RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(0.0f, 0.0f, -fz, d3dxColor + RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+x1, -y1, -fz, d3dxColor + RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+x1, -y1, +fz, d3dxColor + RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(0.0f, 0.0f, -fz, d3dxColor + RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(+x1, -y1, +fz, d3dxColor + RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(0.0f, 0.0f, +fz, d3dxColor + RANDOM_COLOR);
	//Left Plane
	pVertices[i++] = CDiffusedVertex(0.0f, +(fy + y3), +fz, d3dxColor + RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(0.0f, +(fy + y3), -fz, d3dxColor + RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-x2, +y2, -fz, d3dxColor + RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(0.0f, +(fy + y3), +fz, d3dxColor + RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-x2, +y2, -fz, d3dxColor + RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-x2, +y2, +fz, d3dxColor + RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(-x2, +y2, +fz, d3dxColor + RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-x2, +y2, -fz, d3dxColor + RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-fx, -y3, -fz, d3dxColor + RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(-x2, +y2, +fz, d3dxColor + RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-fx, -y3, -fz, d3dxColor + RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-fx, -y3, +fz, d3dxColor + RANDOM_COLOR);
	//Back/Left Plane
	pVertices[i++] = CDiffusedVertex(0.0f, 0.0f, -fz, d3dxColor + RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(0.0f, 0.0f, +fz, d3dxColor + RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-x1, -y1, +fz, d3dxColor + RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(0.0f, 0.0f, -fz, d3dxColor + RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-x1, -y1, +fz, d3dxColor + RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-x1, -y1, -fz, d3dxColor + RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(-x1, -y1, -fz, d3dxColor + RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-x1, -y1, +fz, d3dxColor + RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-fx, -y3, +fz, d3dxColor + RANDOM_COLOR);

	pVertices[i++] = CDiffusedVertex(-x1, -y1, -fz, d3dxColor + RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-fx, -y3, +fz, d3dxColor + RANDOM_COLOR);
	pVertices[i++] = CDiffusedVertex(-fx, -y3, -fz, d3dxColor + RANDOM_COLOR);

	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = sizeof(CDiffusedVertex) * m_vertices;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = pVertices;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dVertexBuffer);

	setRasterizerState(pd3dDevice);
}

CAirplaneMesh::~CAirplaneMesh()
{

}

void CAirplaneMesh::render(ID3D11DeviceContext* pd3dDeviceContext)
{
	CMesh::render(pd3dDeviceContext);
}

void CAirplaneMesh::setRasterizerState(ID3D11Device* pd3dDevice)
{
	D3D11_RASTERIZER_DESC d3dRasterizerDesc;
	ZeroMemory(&d3dRasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

	d3dRasterizerDesc.CullMode = D3D11_CULL_BACK;
	d3dRasterizerDesc.FillMode = D3D11_FILL_SOLID;
	d3dRasterizerDesc.DepthClipEnable = true;

	pd3dDevice->CreateRasterizerState(&d3dRasterizerDesc, &m_pd3dRasterizerState);
}