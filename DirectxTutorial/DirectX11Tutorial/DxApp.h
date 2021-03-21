#pragma once
#include <windows.h>
#include <string>
#include "DXUtil.h"

class DxApp
{
public:
	DxApp(HINSTANCE hInstance);
	virtual ~DxApp();

	//MAIN APPLICATION LOOP
	int Run();

	//FRAMEWORK METHODS
	virtual bool Init();
	virtual bool UnInit();
	virtual void Update(float dt) = 0;
	virtual void Render(float dt) = 0;
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	 
protected:
	//WIN32 ATTRIBUTES
	HWND		m_hAppWnd;
	HINSTANCE	m_hAppIntance;
	UINT		m_ClientWidth;
	UINT		m_ClientHeight;
	std::string	m_AppTilte;
	DWORD		m_WndStyle;

	//DIRECTX ATTRIBUTES
	ID3D11Device*			m_pDevice;
	ID3D11DeviceContext*	m_pImmediateContext;
	IDXGISwapChain*			m_pSwapChain;
	ID3D11RenderTargetView* m_pRenderTargetView;
	D3D_DRIVER_TYPE			m_DriverType;
	D3D_FEATURE_LEVEL		m_FeatureLevel;
	D3D11_VIEWPORT			m_ViewPort;

protected:
	bool	InitWindow();

	bool	InitDirect3D();
};

