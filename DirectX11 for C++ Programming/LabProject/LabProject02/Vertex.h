#pragma once
class CVertex
{
private:
	// ������ ��ġ(3���� ����)�� �����ϱ� ���� ��� ����
	D3DXVECTOR3 m_d3dxvPosition;

public:
	CVertex();
	CVertex(D3DXVECTOR3 d3dxvPosition);
	~CVertex();
};

