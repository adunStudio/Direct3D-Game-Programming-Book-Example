#include "stdafx.h"
#include "Vertex.h"


CVertex::CVertex()
{
	m_d3dxvPosition = D3DXVECTOR3(0, 0, 0);
}

CVertex::CVertex(D3DXVECTOR3 d3dxvPosition)
{
	m_d3dxvPosition = d3dxvPosition;
}

CVertex::~CVertex()
{
}
