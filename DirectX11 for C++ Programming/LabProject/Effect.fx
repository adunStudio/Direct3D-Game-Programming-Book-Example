float4 VS(float4 Pos : POSITION) : SV_POSITION
{
	return Pos;
}

vertices[0] = CVertex(D3DXVECTOR3(0.0f, 0.5f, 0.0f));
vertices[1] = CVertex(D3DXVECTOR3(0.5f, -0.5f, 0.0f));
vertices[2] = CVertex(D3DXVECTOR3(-0.5f, -0.5f, 0.0f));

// �ȼ� ���̴��� �������� ��Ÿ���� �Լ�
float4 PS(float4 Pos : SV_POSITION) : SV_Target
{
	return float4(1.0f, 1.0f, 0.0f, 1.0f);
}