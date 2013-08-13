#include <DX11Util/Object/Dx11ObjectGUIText.h>

#define SAFE_RELEASE(x)  { if(x) { (x)->Release(); (x)=NULL; } }


Dx11ObjectGUIText::Dx11ObjectGUIText(unsigned int _size)
{
	size = _size; 
}

Dx11ObjectGUIText::~Dx11ObjectGUIText(void)
{
}



void Dx11ObjectGUIText::Setup(Dx11Context* _context)
{
	ID3D11Device* pd3dDevice = _context->GetDXDevice(); 
	width = 0; 
	height = 0; 
	hFont = CreateFontA(size,            // 文字セルまたは文字の高さ
		size/2,            // 平均文字幅
		0,       // 文字送りの方向とX軸との角度
		0,      // ベースラインとX軸との角度
		FW_HEAVY,           // フォントの太さ
		0,         // イタリック体指定
		0,      // 下線付き指定
		0,      // 打ち消し線付き指定
		0,        // キャラクタセット
		0,   // 出力精度
		0,  // クリッピングの精度
		0,        // 出力品質
		0, // ピッチとファミリ
		0       // フォント名
	);
	bitmap = 0; 
	canvas = 0; 

	HDC		dc		= GetDC(NULL);
	hdc = CreateCompatibleDC(dc); 
	ReleaseDC(NULL, dc); 

}

float Dx11ObjectGUIText::SetText(Dx11Context* _context, WCHAR* mes)
{
	RECT rtmp;
	ID3D11DeviceContext* context = _context->GetDXDC(); 

	HFONT	oldFont	= (HFONT)SelectObject( hdc, hFont );
	HBITMAP oldbitmap;
	unsigned int oldwidth = width; 
	unsigned int oldheight = height; 
	D3D11_MAPPED_SUBRESOURCE map_info; 
	unsigned int x, y; 
	int offset; 


	rtmp.left = 0; 
	rtmp.top = 0; 
	DrawText(hdc, mes, wcslen(mes), &rtmp, DT_CALCRECT); 
	if(bitmap == 0) {
		width = rtmp.right; 
		height = rtmp.bottom; 
		BITMAPINFO biBMP; 
		ZeroMemory(&biBMP, sizeof(biBMP));
		biBMP.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		biBMP.bmiHeader.biBitCount = 32;
		biBMP.bmiHeader.biPlanes = 1;
		biBMP.bmiHeader.biWidth = width;
		biBMP.bmiHeader.biHeight = height;

		bitmap = CreateDIBSection(hdc, &biBMP, DIB_RGB_COLORS, (void **)(&buf_bitmap), NULL, 0);
	} else if((width != rtmp.right) || (height != rtmp.bottom)) { //if((width < rtmp.right) || (height < rtmp.bottom)) {
		if(bitmap) {
			DeleteObject(bitmap); 
		}

		width = rtmp.right; 
		height = rtmp.bottom; 

		BITMAPINFO biBMP; 
		ZeroMemory(&biBMP, sizeof(biBMP));
		biBMP.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		biBMP.bmiHeader.biBitCount = 32;
		biBMP.bmiHeader.biPlanes = 1;
		biBMP.bmiHeader.biWidth = width;
		biBMP.bmiHeader.biHeight = height;

		bitmap = CreateDIBSection(hdc, &biBMP, DIB_RGB_COLORS, (void **)(&buf_bitmap), NULL, 0);
	}

	oldbitmap = (HBITMAP)SelectObject(hdc, bitmap); 

	TextOut(hdc, 0, 0, mes, wcslen(mes)); 


	SelectObject(hdc, oldbitmap); 
	SelectObject(hdc, oldFont); 


	if(canvas == 0) {
		canvas = new Dx11ObjectGUICanvas(width, height); 
		canvas->Setup(_context); 
	} else if((oldwidth != width) || (oldheight != height)) { //if((oldwidth < width) || (oldheight < height)) { 
		canvas->Term(); 
		delete canvas ;

		canvas = new Dx11ObjectGUICanvas(width, height); 
		canvas->Setup(_context); 
	}


	canvas->Map(_context, &map_info); 

	offset = 0; 
	unsigned int aw, ah; 
	aw = (oldwidth < width)?width:oldwidth; 
	ah = (oldheight < height)?height:oldheight; 
	for(y=0;y<ah;y++) {
		for(x=0;x<aw;x++) {
			unsigned char* out = &(((unsigned char*)map_info.pData)[offset + x * 4]); 
			if((x < width) && (y < height)) {
				unsigned char* in = (unsigned char*)&(buf_bitmap[((height - y - 1) * width + x) * 4]); 
				if(in[1] > 128) {
					out[0] = 255; 
					out[1] = 255; 
					out[2] = 255; 
					out[3] = 0; 
				} else {
					out[0] = 0; 
					out[1] = 0; 
					out[2] = 0; 
					out[3] = 255; 
				}
			} else {
				out[0] = 255; 
				out[1] = 255; 
				out[2] = 255; 
				out[3] = 0; 
			}
		}

		offset += map_info.RowPitch; 
	}

	canvas->Unmap(_context); 

	return (float)width / (float)height; 

}



void Dx11ObjectGUIText::Term()
{
	if(canvas) {
		canvas->Term(); 
		delete canvas ;
	}

	if(bitmap) {
		DeleteObject(bitmap); 
	}
	DeleteObject(hFont); 

	DeleteDC(hdc); 
}

BOOL Dx11ObjectGUIText::GetBufNum(unsigned int* num)
{
	if(canvas) {
		return canvas->GetBufNum(num); 
	} else {
		*num = 0; 
	}
	return true; 
}

BOOL Dx11ObjectGUIText::GetVertexBuf(unsigned int num, ID3D11Buffer** buf)
{
	if(canvas) {
		return canvas->GetVertexBuf(num, buf); 
	} else {
		*buf = 0; 
	}
	return false; 
}

BOOL Dx11ObjectGUIText::GetWeightBuf(unsigned int num, ID3D11Buffer** buf)
{
	if(canvas) {
		return canvas->GetWeightBuf(num, buf); 
	} else {
		*buf = 0; 
	}
	return false; 
}

BOOL Dx11ObjectGUIText::GetIndexBuf(unsigned int num, ID3D11Buffer** buf)
{
	if(canvas) {
		return canvas->GetIndexBuf(num, buf); 
	} else {
		*buf = 0; 
	}
	return false; 
}

BOOL Dx11ObjectGUIText::GetTexture(unsigned int num, ID3D11ShaderResourceView** pTexture)
{
	if(canvas) {
		return canvas->GetTexture(num, pTexture); 
	} else {
		*pTexture = 0; 
	}
	return false; 
}

BOOL Dx11ObjectGUIText::GetVertexNum(unsigned int idx, unsigned int* num)
{
	if(canvas) {
		return canvas->GetVertexNum(idx, num); 
	} else {
		*num = 0; 
	}
	return true; 
}

BOOL Dx11ObjectGUIText::GetIndexNum(unsigned int idx, unsigned int* num)
{
	if(canvas) {
		return canvas->GetIndexNum(idx, num); 
	} else {
		*num = 0; 
	}
	return true; 
}

BOOL Dx11ObjectGUIText::GetTextureNum(unsigned int* num)
{
	*num = 1; 
	return true; 
}
