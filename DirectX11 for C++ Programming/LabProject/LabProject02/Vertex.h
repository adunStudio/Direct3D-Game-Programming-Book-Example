#pragma once
class CVertex
{
private:
	// 정점의 위치(3차원 벡터)를 저장하기 위한 멤버 변수
	D3DXVECTOR3 m_d3dxvPosition;

public:
	CVertex();
	CVertex(D3DXVECTOR3 d3dxvPosition);
	~CVertex();
};

