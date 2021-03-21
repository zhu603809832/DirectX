#include "Sprite.h"
#include "DXUtil.h"
#include "DDSTextureLoader.h"

Sprite::Sprite(void)
{
	m_pResource = nullptr;
	m_pShaderResourceView = nullptr;

	m_Width = 0;
	m_Height = 0;
	ZeroMemory(&m_SourceRect, sizeof(m_SourceRect));
	m_Position = Vector2(0, 0);
	m_Origin = Vector2(0, 0);
	m_Origin = Vector2(1, 1);
	m_Tint = DirectX::Colors::White.v;
	m_Alpha = 1.0f;
	m_Rotation = 0.0f;
}

Sprite::Sprite(const Vector2& position)
{
	m_Position = position;
	m_Alpha = 1.0F;
	m_Rotation = 0.0F;
	m_Scale = Vector2(1, 1);
	m_Tint = DirectX::Colors::White.v;
}

Sprite::~Sprite(void)
{
	Memory::SafeRelease(m_pShaderResourceView);
	Memory::SafeRelease(m_pResource);
}

const DirectX::SimpleMath::Vector2& Sprite::GetPosition() const
{
	return m_Position;
}

const DirectX::SimpleMath::Vector2& Sprite::GetOrigin() const
{
	return m_Origin;
}

const DirectX::SimpleMath::Vector2& Sprite::GetScale() const
{
	return m_Scale;
}

const DirectX::SimpleMath::Color& Sprite::GetTint() const
{
	return m_Tint;
}

const float& Sprite::GetAlpha() const
{
	return m_Alpha;
}

const float& Sprite::GetRotation() const
{
	return m_Rotation;
}

void Sprite::SetPosition(const Vector2& position)
{
	m_Position = position;
}

void Sprite::SetOrigin(const Vector2& origin)
{
	m_Origin = origin;
}

void Sprite::SetScale(const Vector2& scale)
{
	m_Scale = scale;
}

void Sprite::SetTint(const Color& color)
{
	m_Tint = color;
}

void Sprite::SetAlpha(const float& alpha)
{
	m_Alpha = alpha;
}

void Sprite::SetRotation(const float& rotation)
{
	m_Rotation = rotation;
}

void Sprite::Draw(DirectX::SpriteBatch* spriteBatch)
{
	spriteBatch->Draw(m_pShaderResourceView, m_Position, &m_SourceRect, m_Tint, m_Rotation, m_Origin, m_Scale, DirectX::SpriteEffects::SpriteEffects_None, 0.0f);
}

void Sprite::Load(ID3D11Device* device, const wchar_t* file)
{
	HR(DirectX::CreateDDSTextureFromFile(device, file, &m_pResource, &m_pShaderResourceView));
	Utility::GetTextureDimension(m_pResource, &m_Width, &m_Height);
	m_Origin = Vector2(m_Width / 2, m_Height / 2);
	m_SourceRect.left = 0;
	m_SourceRect.top = 0;
	m_SourceRect.right = m_Width;
	m_SourceRect.bottom = m_Height;
}
