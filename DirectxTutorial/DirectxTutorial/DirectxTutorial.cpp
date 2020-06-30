// DirectxTutorial.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "DirectxTutorial.h"
#include "MainClass.h"

#define MAX_LOADSTRING 100
// define the screen resolution
#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600

// 全局变量:
HINSTANCE                   g_hInst;                                // 当前实例
HWND                        g_hMainWindowWnd;
WCHAR                       g_szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR                       g_szWindowClass[MAX_LOADSTRING];            // 主窗口类名

IDXGISwapChain*             g_SwapChain;
ID3D11Device*               g_Device;
ID3D11DeviceContext*        g_DeviceContext;
ID3D11RenderTargetView*     g_RenderTargetView;
ID3D11VertexShader*         g_pVertexShader;
ID3D11PixelShader*          g_pPixelShader;

ID3D11Buffer*               g_pVertexBuffer;
ID3D11InputLayout*			g_pLayout;

struct VERTEX
{
    FLOAT x, y, z;
    D3DXCOLOR Color;
};

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
BOOL                InitD3D(HWND hWnd);
BOOL                CleanD3D(void);
BOOL                RenderFrame();
BOOL                InitPipeline();
BOOL                InitGraphics();

//int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
//                     _In_opt_ HINSTANCE hPrevInstance,
//                     _In_ LPWSTR    lpCmdLine,
//                     _In_ int       nCmdShow)
//{
//    UNREFERENCED_PARAMETER(hPrevInstance);
//    UNREFERENCED_PARAMETER(lpCmdLine);
//
//    // TODO: 在此处放置代码。
//
//    // 初始化全局字符串
//    LoadStringW(hInstance, IDS_APP_TITLE, g_szTitle, MAX_LOADSTRING);
//    LoadStringW(hInstance, IDC_DIRECTXTUTORIAL, g_szWindowClass, MAX_LOADSTRING);
//    MyRegisterClass(hInstance);
//
//    // 执行应用程序初始化:
//    if (!InitInstance (hInstance, nCmdShow))
//    {
//        return FALSE;
//    }
//
//    if (!InitD3D(g_hMainWindowWnd))
//    {
//        return FALSE;
//    }
//	//MessageBox(NULL,
//	//	L"Hello World!",
//	//	L"Just another Hello World program!",
//	//	MB_ICONEXCLAMATION | MB_OK);
//
//    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DIRECTXTUTORIAL));
//
//    MSG msg = {0};
//
//    // 主消息循环:
//    while (TRUE)
//    {
//        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
//        {
//			// translate keystroke messages into the right format
//			TranslateMessage(&msg);
//
//			// send the message to the WindowProc function
//			DispatchMessage(&msg);
//
//			// check to see if it's time to quit
//			if (msg.message == WM_QUIT)
//				break;
//        }
//        RenderFrame();
//    }
//
//    CleanD3D();
//    return (int) msg.wParam;
//}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 在此处放置代码。
	int nRetCode = 0;
	HRESULT hrResult = E_FAIL;
	MainClass mainClass;
	hrResult = mainClass.Init(hInstance, nCmdShow);
	if (FAILED(hrResult))
	{
		TCHAR szMessage[260];
			_sntprintf_s(szMessage, 260, 260, _T("MainClass Fail %u"), hrResult);
			MessageBox(0, szMessage, 0, 0);
			goto Exit0;
	}
	nRetCode = mainClass.Run();

Exit0:
	mainClass.UnInit();
	return nRetCode;
}

//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DIRECTXTUTORIAL));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DIRECTXTUTORIAL);
    wcex.lpszClassName  = g_szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassEx(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	BOOL bResult = FALSE;
   g_hInst = hInstance; // 将实例句柄存储在全局变量中

   RECT wr = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };    // set the size, but not the position
   AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);    // adjust the size
   
   INT nPosX = 0;
   INT nPosY = 0;
   INT nWindowWidth = wr.right - wr.left;
   INT nWindowHeight = wr.bottom - wr.top;

   HWND hWnd = CreateWindowEx(NULL, g_szWindowClass, g_szTitle, WS_OVERLAPPEDWINDOW, nPosX, nPosY, nWindowWidth, nWindowHeight, NULL, NULL, hInstance, NULL);
   if (!hWnd)
       goto Exit0;

   g_hMainWindowWnd = hWnd;
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   bResult = TRUE;
Exit0:
   return bResult;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
                DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
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
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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

BOOL InitD3D(HWND hWnd)
{
	BOOL bResult = FALSE;

	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;
    ID3D11Texture2D* pBackBuffer = NULL;
	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
    scd.BufferDesc.Width = SCREEN_WIDTH;
    scd.BufferDesc.Height = SCREEN_HEIGHT;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.OutputWindow = hWnd;                                // the window to be used
	scd.SampleDesc.Count = 4;                               // how many multisamples
	scd.Windowed = TRUE;                                    // windowed/full-screen mode
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;     // allow full-screen switching
	// create a g_Device, g_Device context and swap chain using the information in the scd struct
     HRESULT hrResult = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&g_SwapChain,
		&g_Device,
		NULL,
		&g_DeviceContext);

     if (FAILED(hrResult))
     {
		 TCHAR szMessage[260];
		 _sntprintf_s(szMessage, 260, 260, _T("D3D11CreateDeviceAndSwapChain Fail %u"), hrResult);
		 MessageBox(0, szMessage, 0, 0);
         goto Exit0;
     }

	 // get the address of the back buffer
	 g_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	 // use the back buffer address to create the render target
     hrResult = g_Device->CreateRenderTargetView(pBackBuffer, NULL, &g_RenderTargetView);
	 if (FAILED(hrResult))
	 {
		 TCHAR szMessage[260];
		 _sntprintf_s(szMessage, 260, 260, _T("CreateRenderTargetView Fail %u"), hrResult);
		 MessageBox(0, szMessage, 0, 0);
		 goto Exit0;
	 }

	 // set the render target as the back buffer
	 g_DeviceContext->OMSetRenderTargets(1, &g_RenderTargetView, NULL);

	 // Set the viewport
	 D3D11_VIEWPORT viewport;
	 ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	 viewport.TopLeftX = 0;
	 viewport.TopLeftY = 0;
	 viewport.Width = SCREEN_WIDTH;
	 viewport.Height = SCREEN_HEIGHT;

	 g_DeviceContext->RSSetViewports(1, &viewport);

	 bResult = InitPipeline();
	 bResult = InitGraphics();

	bResult = TRUE;
Exit0:
    if (pBackBuffer)
    {
        pBackBuffer->Release();
        pBackBuffer = NULL;
    }
	return bResult;
}

BOOL CleanD3D(void)
{
	BOOL bResult = FALSE;
    
	// close and release all existing COM objects
    if (g_pVertexShader)
        g_pVertexShader->Release();

    if (g_pPixelShader)
        g_pPixelShader->Release();

    if (g_SwapChain)
    {
		g_SwapChain->SetFullscreenState(FALSE, NULL);    // switch to windowed mode
		g_SwapChain->Release();
    }

	if (g_RenderTargetView)
		g_RenderTargetView->Release();

    if (g_Device)
        g_Device->Release();

    if (g_DeviceContext)
        g_DeviceContext->Release();

	bResult = TRUE;

	return bResult;
}

BOOL RenderFrame()
{
	BOOL bResult = FALSE;

	// clear the back buffer to a deep blue
    if (g_DeviceContext)
	    g_DeviceContext->ClearRenderTargetView(g_RenderTargetView, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));

	// do 3D rendering on the back buffer here
	// select which vertex buffer to display
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	g_DeviceContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	// select which primtive type we are using
	g_DeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// draw the vertex buffer to the back buffer
	g_DeviceContext->Draw(3, 0);

	// switch the back buffer and the front buffer
    if (g_SwapChain)
	    g_SwapChain->Present(0, 0);

	bResult = TRUE;
//Exit0:
	return bResult;
}

BOOL InitPipeline()
{
	BOOL bResult = FALSE;
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
    hrResult = g_Device->CreateVertexShader(pVertexBlob->GetBufferPointer(), pVertexBlob->GetBufferSize(), NULL, &g_pVertexShader);
	if (FAILED(hrResult))
	{
		TCHAR szMessage[260];
		_sntprintf_s(szMessage, 260, 260, _T("CreateVertexShader Fail %u"), hrResult);
		MessageBox(0, szMessage, 0, 0);
		goto Exit0;
	}

    hrResult = g_Device->CreatePixelShader(pPixelBlob->GetBufferPointer(), pPixelBlob->GetBufferSize(), NULL, &g_pPixelShader);
	if (FAILED(hrResult))
	{
		TCHAR szMessage[260];
		_sntprintf_s(szMessage, 260, 260, _T("CreatePixelShader Fail %u"), hrResult);
		MessageBox(0, szMessage, 0, 0);
		goto Exit0;
	}

    g_DeviceContext->VSSetShader(g_pVertexShader, 0, 0);
    g_DeviceContext->PSSetShader(g_pPixelShader, 0, 0);

	hrResult = g_Device->CreateInputLayout(inputElementDesc, 2, pVertexBlob->GetBufferPointer(), pVertexBlob->GetBufferSize(), &g_pLayout);
	if (FAILED(hrResult))
	{
		TCHAR szMessage[260];
		_sntprintf_s(szMessage, 260, 260, _T("CreateInputLayout Fail %u"), hrResult);
		MessageBox(0, szMessage, 0, 0);
		goto Exit0;
	}
	g_DeviceContext->IASetInputLayout(g_pLayout);

	bResult = TRUE;
Exit0:
	return bResult;
}

BOOL InitGraphics()
{
	BOOL bResult = FALSE;
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

    hrResult = g_Device->CreateBuffer(&bd, NULL, &g_pVertexBuffer);       // create the buffer
	if (FAILED(hrResult))
	{
		TCHAR szMessage[260];
		_sntprintf_s(szMessage, 260, 260, _T("CreateBuffer Fail %u"), hrResult);
		MessageBox(0, szMessage, 0, 0);
		goto Exit0;
	}

	D3D11_MAPPED_SUBRESOURCE ms;
	g_DeviceContext->Map(g_pVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);   // map the buffer
	memcpy(ms.pData, OurVertices, sizeof(OurVertices));                // copy the data
	g_DeviceContext->Unmap(g_pVertexBuffer, NULL);

	bResult = TRUE;
Exit0:
	return bResult;
}