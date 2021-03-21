#include "DXUtil.h"

void Utility::GetTextureDimension(ID3D11Resource* resource, UINT* width, UINT* height)
{
	D3D11_RESOURCE_DIMENSION dimention;
	resource->GetType(&dimention);
	switch (dimention)
	{
	case D3D11_RESOURCE_DIMENSION_UNKNOWN:
		break;
	case D3D11_RESOURCE_DIMENSION_BUFFER:
		break;
	case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
		break;
	case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
		{
			auto texture = reinterpret_cast<ID3D11Texture2D*>(resource);
			D3D11_TEXTURE2D_DESC desc;
			texture->GetDesc(&desc);
			if (width) *width = desc.Width;
			if (height) *height = desc.Height;
		}
		break;
	case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
		break;
	default:
		{
			if (width) *width = 0;
			if (height) *height = 0;
		}
		break;
	}
}
