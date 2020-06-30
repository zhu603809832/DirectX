#include "framework.h"
#include "resource.h"
#include "MainClass.h"

#define MAIN_SCREEN_WIDTH  800
#define MAIN_SCREEN_HEIGHT 600

HINSTANCE MainClass::m_hInstance = NULL;

HWND MainClass::m_hWnd = NULL;

WCHAR MainClass::m_szWindowClass[MAX_LOADSTRING_LEN];

IDXGISwapChain* MainClass::m_piSwapChain = NULL;

ID3D11Device* MainClass::m_piDevice = NULL;

D3D_FEATURE_LEVEL MainClass::m_FeatureLevel = D3D_FEATURE_LEVEL_9_1;

ID3D11DeviceContext* MainClass::m_piDeviceContext = NULL;

ID3D11RenderTargetView* MainClass::m_piRenderTargetView = NULL;

ID3D11VertexShader* MainClass::m_piVertexShader = NULL;

ID3D11PixelShader* MainClass::m_piPixelShader = NULL;

ID3D11DepthStencilView* MainClass::m_piDepthStencilView = NULL;

ID3D11Buffer* MainClass::m_piVertexBuffer = NULL;

ID3D11InputLayout* MainClass::m_piLayout = NULL;

WCHAR MainClass::m_szTitle[MAX_LOADSTRING_LEN];

MainClass::MainClass()
{

}

MainClass::~MainClass()
{

}

HRESULT MainClass::Init(HINSTANCE hInstance, int nCmdShow)
{
	HRESULT hrResult = E_FAIL;
	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DIRECTXTUTORIAL));

	LoadStringW(hInstance, IDS_APP_TITLE, m_szTitle, MAX_LOADSTRING_LEN);
	LoadStringW(hInstance, IDC_DIRECTXTUTORIAL, m_szWindowClass, MAX_LOADSTRING_LEN);

	hrResult = RegisterWndClass();
	if (FAILED(hrResult))
		goto Exit0;

	hrResult = CreateMainWindow(hInstance, nCmdShow);
	if (FAILED(hrResult))
		goto Exit0;
	hrResult = CreateD3D(m_hWnd);
	if (FAILED(hrResult))
		goto Exit0;

	hrResult = S_OK;
Exit0:
	return hrResult;
}

HRESULT MainClass::UnInit()
{
	HRESULT hrResult = E_FAIL;
	hrResult = CleanD3D();
	if (FAILED(hrResult))
		goto Exit0;

	hrResult = S_OK;
Exit0:
	return hrResult;
}

int MainClass::Run()
{
	int nRetCode = 0;

	MSG msg = { 0 };

	// 主消息循环:
	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// translate keystroke messages into the right format
			TranslateMessage(&msg);

			// send the message to the WindowProc function
			DispatchMessage(&msg);

			// check to see if it's time to quit
			if (msg.message == WM_QUIT)
				break;
		}
		else
		{
			RenderFrame();
		}
	}

	nRetCode = (int)msg.wParam;
	return nRetCode;
}

HRESULT MainClass::RegisterWndClass()
{
	HRESULT hrResult = E_FAIL;

	if (m_hInstance == NULL)
		m_hInstance = (HINSTANCE)GetModuleHandle(NULL);

	HICON hIcon = NULL;
	WCHAR szExePath[MAX_PATH];
	GetModuleFileName(NULL, szExePath, MAX_PATH);

	// If the icon is NULL, then use the first one found in the exe
	if (hIcon == NULL)
		hIcon = ExtractIcon(m_hInstance, szExePath, 0);

	// Register the windows class
	WNDCLASS wndClass;
	wndClass.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = MainClass::StaticWindowProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = m_hInstance;
	wndClass.hIcon = hIcon;//LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DIRECTXTUTORIAL));
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);//HBRUSH)(COLOR_WINDOW);
	wndClass.lpszMenuName = MAKEINTRESOURCEW(IDC_DIRECTXTUTORIAL);
	wndClass.lpszClassName = m_szWindowClass;

	if (!RegisterClass(&wndClass))
	{
		DWORD dwError = GetLastError();
		if (dwError != ERROR_CLASS_ALREADY_EXISTS)
			return HRESULT_FROM_WIN32(dwError);
	}

	hrResult = S_OK;

	return hrResult;
}

HRESULT MainClass::UnRegisterWndClass()
{
	HRESULT hrResult = E_FAIL;

	UnregisterClass(m_szWindowClass, m_hInstance);

	hrResult = S_OK;

	return hrResult;
}

HRESULT MainClass::CreateMainWindow(HINSTANCE hInstance, int nCmdShow)
{
	HRESULT hrResult = E_FAIL;


	m_hInstance = hInstance; // 将实例句柄存储在全局变量中

	RECT wr = { 0, 0, MAIN_SCREEN_WIDTH, MAIN_SCREEN_HEIGHT };    // set the size, but not the position
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);    // adjust the size

	INT nPosX = 0;
	INT nPosY = 0;
	INT nWindowWidth = wr.right - wr.left;
	INT nWindowHeight = wr.bottom - wr.top;

	HWND hWnd = CreateWindowEx(NULL, m_szWindowClass, m_szTitle, WS_OVERLAPPEDWINDOW, nPosX, nPosY, nWindowWidth, nWindowHeight, NULL, NULL, hInstance, NULL);
	if (!hWnd)
		goto Exit0;

	m_hWnd = hWnd;
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	hrResult = S_OK;
Exit0:
	return hrResult;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK AboutWindow(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

LRESULT CALLBACK MainClass::StaticWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(m_hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, AboutWindow);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此处添加使用 hdc 的任何绘图代码...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_CLOSE:
	{
		HMENU hMenu;
		hMenu = GetMenu(hWnd);
		if (hMenu != NULL)
			DestroyMenu(hMenu);

		DestroyWindow(hWnd);
		UnRegisterWndClass();
		
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

HRESULT MainClass::CreateD3D(HWND hWnd)
{
	HRESULT hrResult = E_FAIL;

	D3D_FEATURE_LEVEL levels[] = {
	D3D_FEATURE_LEVEL_11_0,
	D3D_FEATURE_LEVEL_10_1,
	D3D_FEATURE_LEVEL_9_3,
	D3D_FEATURE_LEVEL_9_2,
	D3D_FEATURE_LEVEL_9_1,
	};

	UINT deviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(DEBUG) || defined(_DEBUG)
	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ID3D11Texture2D* pBackBuffer = NULL;
	D3D11_TEXTURE2D_DESC texture2DDesc;
	
	// clear out the struct for use
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	swapChainDesc.BufferCount = 2;                                    // one back buffer
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	swapChainDesc.BufferDesc.Width = MAIN_SCREEN_WIDTH;
	swapChainDesc.BufferDesc.Height = MAIN_SCREEN_HEIGHT;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	swapChainDesc.OutputWindow = hWnd;                                // the window to be used
	swapChainDesc.SampleDesc.Count = 1;                               // how many multisamples
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;                                    // windowed/full-screen mode
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;     // allow full-screen switching
	
	// create a g_Device, g_Device context and swap chain using the information in the scd struct
	hrResult = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		deviceFlags,
		levels,
		ARRAYSIZE(levels),
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&m_piSwapChain,
		&m_piDevice,
		&m_FeatureLevel,
		&m_piDeviceContext);

	if (FAILED(hrResult))
	{
		TCHAR szMessage[260];
		_sntprintf_s(szMessage, 260, 260, _T("D3D11CreateDeviceAndSwapChain Fail %u"), hrResult);
		MessageBox(0, szMessage, 0, 0);
		goto Exit0;
	}

	if (D3D_FEATURE_LEVEL_11_0 != m_FeatureLevel)
	{
		TCHAR szMessage[260];
		_sntprintf_s(szMessage, 260, 260, _T("System does not support DX11 Fail %u"), hrResult);
		MessageBox(0, szMessage, 0, 0);
		goto Exit0;
	}
	// get the address of the back buffer
	m_piSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	// use the back buffer address to create the render target
	hrResult = m_piDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_piRenderTargetView);
	if (FAILED(hrResult))
	{
		TCHAR szMessage[260];
		_sntprintf_s(szMessage, 260, 260, _T("CreateRenderTargetView Fail %u"), hrResult);
		MessageBox(0, szMessage, 0, 0);
		goto Exit0;
	}

	// set the render target as the back buffer
	m_piDeviceContext->OMSetRenderTargets(1, &m_piRenderTargetView, NULL);

	pBackBuffer->GetDesc(&texture2DDesc);
	//StencilBuffer
	//{
	//	ID3D11Texture2D* pDepthStencilBuffer = NULL;
	//	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
	//	CD3D11_TEXTURE2D_DESC depthStencilDesc(DXGI_FORMAT_D24_UNORM_S8_UINT, static_cast<UINT> (texture2DDesc.Width), static_cast<UINT> (texture2DDesc.Height),
	//		1, // This depth stencil view has only one texture.
	//		1, // Use a single mipmap level.
	//		D3D11_BIND_DEPTH_STENCIL
	//	);

	//	hrResult = m_piDevice->CreateTexture2D(&depthStencilDesc, NULL, &pDepthStencilBuffer);
	//	if (FAILED(hrResult))
	//	{
	//		TCHAR szMessage[260];
	//		_sntprintf_s(szMessage, 260, 260, _T("CreateTexture2D DepthStencilBuffer Fail %u"), hrResult);
	//		MessageBox(0, szMessage, 0, 0);
	//		goto Exit0;
	//	}

	//	hrResult = m_piDevice->CreateDepthStencilView(pDepthStencilBuffer, &depthStencilViewDesc, &m_piDepthStencilView);
	//	if (FAILED(hrResult))
	//	{
	//		TCHAR szMessage[260];
	//		_sntprintf_s(szMessage, 260, 260, _T("CreateDepthStencilView Fail %u"), hrResult);
	//		MessageBox(0, szMessage, 0, 0);
	//		goto Exit0;
	//	}
	//}

	// Set the viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = texture2DDesc.Width;
	viewport.Height = texture2DDesc.Height;

	m_piDeviceContext->RSSetViewports(1, &viewport);

	hrResult = InitPipeline();
	if (FAILED(hrResult))
	{
		TCHAR szMessage[260];
		_sntprintf_s(szMessage, 260, 260, _T("InitPipeline Fail %u"), hrResult);
		MessageBox(0, szMessage, 0, 0);
		goto Exit0;
	}
	hrResult = InitGraphics();
	if (FAILED(hrResult))
	{
		TCHAR szMessage[260];
		_sntprintf_s(szMessage, 260, 260, _T("InitGraphics Fail %u"), hrResult);
		MessageBox(0, szMessage, 0, 0);
		goto Exit0;
	}

	hrResult = S_OK;
Exit0:
	if (pBackBuffer)
	{
		pBackBuffer->Release();
		pBackBuffer = NULL;
	}
	return hrResult;
}

HRESULT MainClass::CleanD3D()
{
	BOOL bResult = FALSE;

	// close and release all existing COM objects
	if (m_piVertexShader)
		m_piVertexShader->Release();

	if (m_piPixelShader)
		m_piPixelShader->Release();

	if (m_piSwapChain)
	{
		m_piSwapChain->SetFullscreenState(FALSE, NULL);    // switch to windowed mode
		m_piSwapChain->Release();
	}

	if (m_piRenderTargetView)
		m_piRenderTargetView->Release();

	if (m_piDevice)
		m_piDevice->Release();

	if (m_piDeviceContext)
		m_piDeviceContext->Release();

	bResult = TRUE;

	return bResult;
}

HRESULT MainClass::RenderFrame()
{
	HRESULT hrResult = E_FAIL;

	// clear the back buffer to a deep blue
	if (m_piDeviceContext)
		m_piDeviceContext->ClearRenderTargetView(m_piRenderTargetView, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));

	// do 3D rendering on the back buffer here
	// select which vertex buffer to display
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	m_piDeviceContext->IASetVertexBuffers(0, 1, &m_piVertexBuffer, &stride, &offset);

	// select which primtive type we are using
	m_piDeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// draw the vertex buffer to the back buffer
	m_piDeviceContext->Draw(3, 0);

	// switch the back buffer and the front buffer
	if (m_piSwapChain)
		m_piSwapChain->Present(0, 0);

	hrResult = S_OK;
	return hrResult;
}

HRESULT MainClass::InitPipeline()
{
	HRESULT hrResult = E_FAIL;

	ID3D10Blob* pVertexBlob = NULL;
	ID3D10Blob* pPixelBlob = NULL;
	// create the input layout object
	D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	hrResult = D3DX11CompileFromFile(L"shaders.shader", 0, 0, "VShader", "vs_4_0", 0, 0, 0, &pVertexBlob, 0, 0);
	if (FAILED(hrResult))
	{
		TCHAR szMessage[260];
		_sntprintf_s(szMessage, 260, 260, _T("D3DX10CompileFromFile VShader Fail %u"), hrResult);
		MessageBox(0, szMessage, 0, 0);
		goto Exit0;
	}
	hrResult = D3DX11CompileFromFile(L"shaders.shader", 0, 0, "PShader", "ps_4_0", 0, 0, 0, &pPixelBlob, 0, 0);
	if (FAILED(hrResult))
	{
		TCHAR szMessage[260];
		_sntprintf_s(szMessage, 260, 260, _T("D3DX10CompileFromFile PShader Fail %u"), hrResult);
		MessageBox(0, szMessage, 0, 0);
		goto Exit0;
	}
	// encapsulate both shaders into shader objects
	hrResult = m_piDevice->CreateVertexShader(pVertexBlob->GetBufferPointer(), pVertexBlob->GetBufferSize(), NULL, &m_piVertexShader);
	if (FAILED(hrResult))
	{
		TCHAR szMessage[260];
		_sntprintf_s(szMessage, 260, 260, _T("CreateVertexShader Fail %u"), hrResult);
		MessageBox(0, szMessage, 0, 0);
		goto Exit0;
	}

	hrResult = m_piDevice->CreatePixelShader(pPixelBlob->GetBufferPointer(), pPixelBlob->GetBufferSize(), NULL, &m_piPixelShader);
	if (FAILED(hrResult))
	{
		TCHAR szMessage[260];
		_sntprintf_s(szMessage, 260, 260, _T("CreatePixelShader Fail %u"), hrResult);
		MessageBox(0, szMessage, 0, 0);
		goto Exit0;
	}

	m_piDeviceContext->VSSetShader(m_piVertexShader, 0, 0);
	m_piDeviceContext->PSSetShader(m_piPixelShader, 0, 0);

	hrResult = m_piDevice->CreateInputLayout(inputElementDesc, 2, pVertexBlob->GetBufferPointer(), pVertexBlob->GetBufferSize(), &m_piLayout);
	if (FAILED(hrResult))
	{
		TCHAR szMessage[260];
		_sntprintf_s(szMessage, 260, 260, _T("CreateInputLayout Fail %u"), hrResult);
		MessageBox(0, szMessage, 0, 0);
		goto Exit0;
	}
	m_piDeviceContext->IASetInputLayout(m_piLayout);

	hrResult = S_OK;
Exit0:
	return hrResult;
}

HRESULT MainClass::InitGraphics()
{
	HRESULT hrResult = E_FAIL;

	VERTEX OurVertices[] =
	{
		{0.0f, 0.5f, 0.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)},
		{0.45f, -0.5, 0.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f)},
		{-0.45f, -0.5f, 0.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f)}
	};

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	bd.ByteWidth = sizeof(VERTEX) * 3;             // size is the VERTEX struct * 3
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

	hrResult = m_piDevice->CreateBuffer(&bd, NULL, &m_piVertexBuffer);       // create the buffer
	if (FAILED(hrResult))
	{
		TCHAR szMessage[260];
		_sntprintf_s(szMessage, 260, 260, _T("CreateBuffer Fail %u"), hrResult);
		MessageBox(0, szMessage, 0, 0);
		goto Exit0;
	}

	D3D11_MAPPED_SUBRESOURCE ms;
	m_piDeviceContext->Map(m_piVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);   // map the buffer
	memcpy(ms.pData, OurVertices, sizeof(OurVertices));                // copy the data
	m_piDeviceContext->Unmap(m_piVertexBuffer, NULL);

	hrResult = S_OK;
Exit0:
	return hrResult;
}
