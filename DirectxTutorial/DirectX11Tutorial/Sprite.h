#pragma once
#include "IElement2D.h"

class Sprite : public IElement2D
{
public:
	Sprite(void);
	Sprite(const Vector2& position);
	virtual ~Sprite(void);

	virtual const Vector2& GetPosition() const override;
	virtual const Vector2& GetOrigin() const override;
	virtual const Vector2& GetScale() const override;
	virtual const Color& GetTint() const override;
	virtual const float& GetAlpha() const override;
	virtual const float& GetRotation() const override;

	virtual void SetPosition(const Vector2& position) override;
	virtual void SetOrigin(const Vector2& origin) override;
	virtual void SetScale(const Vector2& scale) override;
	virtual void SetTint(const Color& color) override;
	virtual void SetAlpha(const float& alpha) override;
	virtual void SetRotation(const float& rotation) override;
	 
	virtual void Draw(DirectX::SpriteBatch* spriteBatch) override;
	virtual void Load(ID3D11Device* device, const wchar_t* file) override;

protected:
	ID3D11Resource*				m_pResource;
	ID3D11ShaderResourceView*	m_pShaderResourceView;
	
	UINT						m_Width;
	UINT						m_Height;
	RECT						m_SourceRect;
	Vector2						m_Position;
	Vector2						m_Origin;
	Vector2						m_Scale;
	Color						m_Tint;
	float						m_Alpha;
	float						m_Rotation;
};

