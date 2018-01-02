#include "stdafx.h"
#include "Mesh.h"


CMesh::CMesh()
{
	m_stride = sizeof(CVertex);
	m_offset = 0;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_references = 1;
	m_pd3dRasterizerState = nullptr;
}

CMesh::~CMesh()
{
	if (m_pd3dVertexBuffer)
		m_pd3dVertexBuffer->Release();

	if (m_pd3dRasterizerState)
		m_pd3dRasterizerState->Release();
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

	pd3dDeviceContext->IASetPrimitiveTopology(m_d3dPrimitiveTopology);

	// �����Ͷ����� ���¸� ����̽� ���ؽ�Ʈ�� �����Ѵ�.
	if (m_pd3dRasterizerState)
		pd3dDeviceContext->RSSetState(m_pd3dRasterizerState);


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
	m_stride = sizeof(CVertex);
	m_offset = 0;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	// 1. �ﰢ���� �� �������� ��ġ ���͸� �����Ѵ�. (�ð� ����)
	CVertex pVertices[3];
	pVertices[0] = CVertex(D3DXVECTOR3( 0.0f,  0.5f, 0.0f));
	pVertices[1] = CVertex(D3DXVECTOR3( 0.5f, -0.5f, 0.0f));
	pVertices[2] = CVertex(D3DXVECTOR3(-0.5f, -0.5f, 0.0f));

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
