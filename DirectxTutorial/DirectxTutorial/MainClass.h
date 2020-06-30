#pragma once
#include <cstring>

#define MAX_LOADSTRING_LEN 128

class MainClass
{
protected:
	static HINSTANCE            m_hInstance;                                // 当前实例
	static HWND                 m_hWnd;

	static WCHAR                m_szTitle[MAX_LOADSTRING_LEN];                  // 标题栏文本
	static WCHAR                m_szWindowClass[MAX_LOADSTRING_LEN];            // 主窗口类名
	HWND						m_hMenu;

	static IDXGISwapChain*		m_piSwapChain;
	static ID3D11Device*		m_piDevice;
	static D3D_FEATURE_LEVEL	m_FeatureLevel;
	static ID3D11DeviceContext* m_piDeviceContext;
	static ID3D11RenderTargetView* m_piRenderTargetView;
	static ID3D11VertexShader* m_piVertexShader;
	static ID3D11PixelShader* m_piPixelShader;
	static 	ID3D11DepthStencilView* m_piDepthStencilView;

	static ID3D11Buffer* m_piVertexBuffer;
	static ID3D11InputLayout* m_piLayout;

	struct VERTEX
	{
		FLOAT x, y, z;
		D3DXCOLOR Color;
	};


public:
	MainClass();
	~MainClass();

	HRESULT Init(HINSTANCE hInstance, int nCmdShow);
	HRESULT UnInit();
	int Run();

protected:
	static HRESULT RegisterWndClass();
	static HRESULT UnRegisterWndClass();

	static HRESULT CreateMainWindow(HINSTANCE hInstance, int nCmdShow);
	static LRESULT CALLBACK StaticWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	static HRESULT CreateD3D(HWND hWnd);
	static HRESULT CleanD3D();
	static HRESULT RenderFrame();
	static HRESULT InitPipeline();
	static HRESULT InitGraphics();
};
