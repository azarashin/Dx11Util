#include <DX11Util/Dx11ObjectDae.h>



Dx11ObjectDae::Dx11ObjectDae(void)
{
}


Dx11ObjectDae::~Dx11ObjectDae(void)
{
}

HRESULT Dx11ObjectDae::LoadDAE(const char strFilename[], ID3D11Device* pd3dDevice)
{
	HRESULT hr = S_OK;
	DAE*	dae;

	dae = new DAE();

	int iRet = dae->load(strFilename);

	if(iRet!=DAE_OK) {
		return E_FAIL;
	}

	hr = LoadModel(dae, pd3dDevice); 
	dae->cleanup(); 
	delete dae; 

	return hr;
}
