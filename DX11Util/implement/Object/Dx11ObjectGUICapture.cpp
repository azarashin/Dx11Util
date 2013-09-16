#include <DX11Util/Object/Dx11ObjectGUICapture.h>

#define SAFE_RELEASE(x)  { if(x) { (x)->Release(); (x)=NULL; } }



IPin* Dx11ObjectGUICapture::GetPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir)
{
    BOOL       bFound = FALSE;
    IEnumPins  *pEnum;
    IPin       *pPin;

    pFilter->EnumPins(&pEnum);
    while(pEnum->Next(1, &pPin, 0) == S_OK)
    {
        PIN_DIRECTION PinDirThis;
        pPin->QueryDirection(&PinDirThis);
        if (bFound = (PinDir == PinDirThis))
            break;
        pPin->Release();
    }
    pEnum->Release();
    return (bFound ? pPin : 0);  
}

IPin* Dx11ObjectGUICapture::GetInPin( IBaseFilter * pFilter)
{
    return GetPin(pFilter, PINDIR_INPUT);
}

IPin* Dx11ObjectGUICapture::GetOutPin( IBaseFilter * pFilter)
{
    return GetPin(pFilter, PINDIR_OUTPUT);
}


Dx11ObjectGUICapture::Dx11ObjectGUICapture()
{
}


Dx11ObjectGUICapture::~Dx11ObjectGUICapture(void)
{
}



void Dx11ObjectGUICapture::SetupCanvas(Dx11Context* _context, int width, int height)
{
	canvas = new Dx11ObjectGUICanvas(width, height); 
	canvas->Setup(_context); 
}


void Dx11ObjectGUICapture::TermCanvas()
{
	canvas->Term(); 
	delete canvas; 
}

void Dx11ObjectGUICapture::Update(Dx11Context* context)
{
	D3D11_MAPPED_SUBRESOURCE resource; 
	canvas->Map(context, &resource); 
	GetPicture((char*)resource.pData); 
	canvas->Unmap(context); 

}

BOOL Dx11ObjectGUICapture::GetBufNum(unsigned int* num)
{
	return canvas->GetBufNum(num); 
}

BOOL Dx11ObjectGUICapture::GetVertexBuf(unsigned int num, ID3D11Buffer** buf)
{
	return canvas->GetVertexBuf(num, buf); 
}

BOOL Dx11ObjectGUICapture::GetWeightBuf(unsigned int num, ID3D11Buffer** buf)
{
	return canvas->GetWeightBuf(num, buf); 
}

BOOL Dx11ObjectGUICapture::GetIndexBuf(unsigned int num, ID3D11Buffer** buf)
{
	return canvas->GetIndexBuf(num, buf); 
}

BOOL Dx11ObjectGUICapture::GetTexture(unsigned int num, ID3D11ShaderResourceView** pTexture)
{
	return canvas->GetTexture(num ,pTexture); 
}

BOOL Dx11ObjectGUICapture::GetVertexNum(unsigned int idx, unsigned int* num)
{
	return canvas->GetVertexNum(idx, num); 
}

BOOL Dx11ObjectGUICapture::GetIndexNum(unsigned int idx, unsigned int* num)
{
	return canvas->GetIndexNum(idx, num); 
}

BOOL Dx11ObjectGUICapture::GetTextureNum(unsigned int* num)
{
	return canvas->GetTextureNum(num); 
}


