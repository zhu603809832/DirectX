#include "GameApplication.h"

GameApplication::GameApplication(HINSTANCE hInstance) :
	DxApp(hInstance)
{
	m_AppTilte = "Game App Title"; 
}

GameApplication::~GameApplication()
{

}

bool GameApplication::Init()
{
	if (!DxApp::Init())
		return false;

	m_SpriteBatch.reset(new DirectX::SpriteBatch(m_pImmediateContext));
	m_SpriteFont.reset(new DirectX::SpriteFont(m_pDevice, L"./Resource/Font/myfone.sprintfont"));

	m_pSprite = new Sprite(DirectX::SimpleMath::Vector2(100, 100));
	m_pSprite->Load(m_pDevice, L"./Resource/Image/test.dds");

	return true;
}

bool GameApplication::UnInit()
{
	Memory::SafeDelete(m_pSprite);
	return DxApp::UnInit();
}

void GameApplication::Update(float dt)
{
	
}

void GameApplication::Render(float dt)
{
	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, DirectX::Colors::CornflowerBlue);
	m_SpriteBatch->Begin();
	//DRAW SPRINT, FONT ETC.
	m_pSprite->Draw(m_SpriteBatch.get());

	m_SpriteFont->DrawString(m_SpriteBatch.get(), L"Hello world", DirectX::SimpleMath::Vector2(300, 300));
	m_SpriteBatch->End();

	HR(m_pSwapChain->Present(0, 0));
}


