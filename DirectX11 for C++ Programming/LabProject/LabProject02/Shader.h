#pragma once

#include "Object.h"

class CShader
{
public:
	// ���� ���̴� �������̽� ������
	ID3D11VertexShader* m_pd3dVertexShader;
	
	// �Է� ���̾ƿ� �������̽� ������
	ID3D11InputLayout*  m_pd3dVertexLayout;
	
	// �ȼ� ���̴� �������̽� ������
	ID3D11PixelShader*  m_pd3dPixelShader;

public:
	CShader();
	virtual ~CShader();

	// ���Ͽ��� ���� ���̴��� �����ϴ� �Լ�
	void createVertexShaderFromFile
	(
		ID3D11Device* pd3dDevice,
		WCHAR* pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel,
		ID3D11VertexShader** ppd3dVertexShader,
		D3D11_INPUT_ELEMENT_DESC* pd3dInputLayout, UINT nElements,
		ID3D11InputLayout** ppd3dVertexLayout
	);

	// ���Ͽ��� �ȼ� ���̴��� �����ϴ� �Լ�
	void createPixelShaderFromFile
	(
		ID3D11Device* pd3dDevice,
		WCHAR* pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel,
		ID3D11PixelShader** ppd3dPixelShader
	);

	// CShader �Ļ� Ŭ�������� ���̴��� �����ϱ� ���� ȣ���ϴ� ���� �Լ�
	virtual void createShader(ID3D11Device* pd3dDevice);

	// CShader �Ļ� Ŭ�������� �������ϱ� ���� ȣ���ϴ� ���� �Լ�
	virtual void render(ID3D11DeviceContext* pd3dDeviceContext);
};

class CObjectShader : public CShader
{
private:
	// �� ���̴����� �������� ���� ��ü�� ���� �����͵��� ����Ʈ
	CGameObject** m_ppObjects;
	int m_nObjects;

	// �� ���̴����� �������� ���� ��ü�� �߰��� ��ġ�� ��Ÿ���� �ε���
	int m_nIndexToAdd;

public:
	CObjectShader();
	~CObjectShader();

	// ���̴� ���� �Լ� ����, nObjects�� �� ���̴��� ���� �������Ǵ� ��ü�� ����
	void createShader(ID3D11Device* pd3dDevice, int nObjects);

	// ������ �Լ� ����
	virtual void render(ID3D11DeviceContext* pd3dDeviceContext);

	// ���̴����� �������� ���� ��ü�� �߰��ϴ� �Լ�
	void addObject(CGameObject* object);
};