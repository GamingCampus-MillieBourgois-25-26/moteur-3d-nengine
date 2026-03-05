/*#include "Engine/OBJ/WICTextureLoader.h"
#include <wincodec.h>

using Microsoft::WRL::ComPtr;

namespace DirectX
{
    HRESULT CreateWICTextureFromFile(
        ID3D11Device* device,
        ID3D11DeviceContext* context,
        const wchar_t* fileName,
        ID3D11Resource** texture,
        ID3D11ShaderResourceView** textureView,
        size_t maxsize
    )
    {
        // Charge l'image via WIC
        ComPtr<IWICImagingFactory> factory;
        CoCreateInstance(
            CLSID_WICImagingFactory,
            nullptr,
            CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(&factory)
        );

        ComPtr<IWICBitmapDecoder> decoder;
        factory->CreateDecoderFromFilename(
            fileName,
            nullptr,
            GENERIC_READ,
            WICDecodeMetadataCacheOnDemand,
            &decoder
        );

        ComPtr<IWICBitmapFrameDecode> frame;
        decoder->GetFrame(0, &frame);

        ComPtr<IWICFormatConverter> converter;
        factory->CreateFormatConverter(&converter);

        converter->Initialize(
            frame.Get(),
            GUID_WICPixelFormat32bppRGBA,
            WICBitmapDitherTypeNone,
            nullptr,
            0.0,
            WICBitmapPaletteTypeCustom
        );

        UINT width, height;
        converter->GetSize(&width, &height);

        std::vector<uint8_t> pixels(width * height * 4);
        converter->CopyPixels(nullptr, width * 4, pixels.size(), pixels.data());

        // Création de la texture DirectX
        D3D11_TEXTURE2D_DESC desc = {};
        desc.Width = width;
        desc.Height = height;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.SampleDesc.Count = 1;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

        D3D11_SUBRESOURCE_DATA initData = {};
        initData.pSysMem = pixels.data();
        initData.SysMemPitch = width * 4;

        ID3D11Texture2D* tex = nullptr;
        HRESULT hr = device->CreateTexture2D(&desc, &initData, &tex);
        if (FAILED(hr)) {
            std::cout << "CreateTexture2D FAILED: 0x" << std::hex << hr << std::endl;
        }

        if (texture) *texture = tex;

        return device->CreateShaderResourceView(tex, nullptr, textureView);
    }
}
*/