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
		// �ε��� ���۰� ������ �ε��� ���۸� ����̽� ���ؽ�Ʈ�� �����Ѵ�.
		pd3dDeviceContext->IASetIndexBuffer(m_pd3dIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	}

	pd3dDeviceContext->IASetPrimitiveTopology(m_d3dPrimitiveTopology);

	// �����Ͷ����� ���¸� ����̽� ���ؽ�Ʈ�� �����Ѵ�.
	if (m_pd3dRasterizerState)
		pd3dDeviceContext->RSSetState(m_pd3dRasterizerState);


	// �ε��� ���۰� ������ �ε��� ���۸� ����Ͽ� �޽ø� �������ϰ� ������ ���� ���۸��� ����Ͽ� �޽ø� �������Ѵ�.
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

	// 3. ���� ������ ���� ����, ������ ����Ʈ ũ��, ������Ƽ�� ������ �����Ѵ�.
	m_vertices = 3;
	m_stride = sizeof(CDiffusedVertex);
	m_offset = 0;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	// 1. �ﰢ���� �� �������� ��ġ ���͸� �����Ѵ�. (�ð� ����)
	CDiffusedVertex pVertices[3];
	pVertices[0] = CDiffusedVertex(D3DXVECTOR3( 0.0f,  0.5f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	pVertices[1] = CDiffusedVertex(D3DXVECTOR3( 0.5f, -0.5f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	pVertices[2] = CDiffusedVertex(D3DXVECTOR3(-0.5f, -0.5f, 0.0f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));

	// 2. ���� ���۸� �����Ѵ�.
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

	// �����Ͷ����� �ܰ迡�� �ø�(���� ����)�� ���� �ʵ��� �����Ͷ����� ���¸� �����Ѵ�.
	d3dRasterizerDesc.CullMode = D3D11_CULL_NONE;
	d3dRasterizerDesc.FillMode = D3D11_FILL_SOLID;

	pd3dDevice->CreateRasterizerState(&d3dRasterizerDesc, &m_pd3dRasterizerState);
}

/* CCubeMesh */

CCubeMesh::CCubeMesh(ID3D11Device* pd3dDevice, float fWidth, float fHeight, float fDepth)
{
	// 3. ���� ������ ���� ����, ������ ����Ʈ ũ��, ������Ƽ�� ������ �����Ѵ�.
	m_vertices = 8;
	m_stride = sizeof(CDiffusedVertex);
	m_offset = 0;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	// 1. �ﰢ���� �� �������� ��ġ ���͸� �����Ѵ�. (�ð� ����)

	float fx = fWidth  * 0.5f;
	float fy = fHeight * 0.5f;
	float fz = fDepth  * 0.5f;

	// ���� ���� �����ʹ� ������ü�� ������ 8���� ���� ���� �����͸� ������.

	CDiffusedVertex pVertices[8];
	pVertices[0] = CDiffusedVertex(D3DXVECTOR3(-fx, +fy, -fz), RANDOM_COLOR);
	pVertices[1] = CDiffusedVertex(D3DXVECTOR3(+fx, +fy, -fz), RANDOM_COLOR);
	pVertices[2] = CDiffusedVertex(D3DXVECTOR3(+fx, +fy, +fz), RANDOM_COLOR);
	pVertices[3] = CDiffusedVertex(D3DXVECTOR3(-fx, +fy, +fz), RANDOM_COLOR);
	pVertices[4] = CDiffusedVertex(D3DXVECTOR3(-fx, -fy, -fz), RANDOM_COLOR);
	pVertices[5] = CDiffusedVertex(D3DXVECTOR3(+fx, -fy, -fz), RANDOM_COLOR);
	pVertices[6] = CDiffusedVertex(D3DXVECTOR3(+fx, -fy, +fz), RANDOM_COLOR);
	pVertices[7] = CDiffusedVertex(D3DXVECTOR3(-fx, -fy, +fz), RANDOM_COLOR);

	// 2. ���� ���۸� �����Ѵ�.
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


	// �ε��� ���۴� ��ü 36(= 6 * 2 * 3)���� �ε����� ������.
	m_indices = 36;
	WORD pIndices[36];

	pIndices[0]  = 3; pIndices[1]  = 1; pIndices[2]  = 0;
	pIndices[3]  = 2; pIndices[4]  = 1; pIndices[5]  = 3;
	pIndices[6]  = 0; pIndices[7]  = 5; pIndices[8]  = 4;
	pIndices[9]  = 1; pIndices[10] = 5; pIndices[11] = 0;
	pIndices[12] = 3; pIndices[13] = 4; pIndices[14] = 7;
	pIndices[15] = 0; pIndices[16] = 4; pIndices[17] = 3;
	pIndices[18] = 1; pIndices[19] = 6; pIndices[20] = 5;
	pIndices[21] = 2; pIndices[22] = 6; pIndices[23] = 1;
	pIndices[24] = 2; pIndices[25] = 7; pIndices[26] = 6;
	pIndices[27] = 3; pIndices[28] = 7; pIndices[29] = 2;
	pIndices[30] = 6; pIndices[31] = 4; pIndices[32] = 5;
	pIndices[33] = 7; pIndices[34] = 4; pIndices[35] = 6;

	// 2. �ε��� ���۸� �����Ѵ�.
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

	// �����Ͷ����� �ܰ迡�� �ø�(���� ����)�� ���� �ʵ��� �����Ͷ����� ���¸� �����Ѵ�.
	//d3dRasterizerDesc.CullMode = D3D11_CULL_NONE;
	d3dRasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;

	pd3dDevice->CreateRasterizerState(&d3dRasterizerDesc, &m_pd3dRasterizerState);
}
