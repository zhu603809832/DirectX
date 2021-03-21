#pragma once
#include <memory.h>
#include "DxApp.h"
#include "DDSTextureLoader.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "SimpleMath.h"
#include "Sprite.h"

class GameApplication : public DxApp
{
public:
	GameApplication(HINSTANCE hInstance);
	~GameApplication();

	bool Init() override;
	bool UnInit() override;
	void Update(float dt) override;
	void Render(float dt) override;

protected:
	std::unique_ptr<DirectX::SpriteBatch>	m_SpriteBatch;
	std::unique_ptr<DirectX::SpriteFont>	m_SpriteFont;
	Sprite* m_pSprite;
};
