#include <DX11Util/Motion/Dx11XMotionManager.h>

Dx11XMotionManager::Dx11XMotionManager(int _bone_max)
{
	next = 0; 
}

Dx11XMotionManager::~Dx11XMotionManager()
{
}

HRESULT Dx11XMotionManager::Setup()
{
	std::map<std::string, Dx11XMotion*>::iterator it; 
	for(it=motion_map.begin();it!=motion_map.end();it++) {
		delete it->second; 
	}
	motion_map.clear();  
	next = 0;
	current = 0; 
	loop_flag = false; 
	return S_OK; 
}

HRESULT Dx11XMotionManager::Update()
{
	HRESULT ret = current->Update(); 
	if(ret != S_OK) {
		return ret; 
	}

	if(current->IsFinished()) {
		if(next != 0) {
			current = next; 
			current->Start(); 
		} else if(loop_flag) {
			// keep current
			current->Start(); 
		} else {
			// do nothing. 
		}
	}
	return S_OK; 
}

HRESULT Dx11XMotionManager::GetMatrix(XMMATRIX* mat)
{
	if(current == 0) {
		return E_FAIL; 
	}
	return current->GetMatrix(mat); 
}

HRESULT Dx11XMotionManager::GetNumberOfMatrix(int* num)
{
	if(current == 0) {
		return E_FAIL; 
	}
	return current->GetNumberOfMatrix(num); 
}

HRESULT Dx11XMotionManager::Term()
{
	std::map<std::string, Dx11XMotion*>::iterator it; 
	for(it=motion_map.begin();it!=motion_map.end();it++) {
		delete it->second; 
	}
	motion_map.clear();  
	next = 0;
	current = 0; 
	return S_OK; 
}

void Dx11XMotionManager::SelectAnimation(std::string name, bool loop)
{
	if(motion_map.find(name) == motion_map.end()) {
		current = motion_map[name]; 
		next = 0; 
		loop_flag = loop; 
	}
}

void Dx11XMotionManager::ReserveNextAnimation(std::string name, bool loop)
{
	if(motion_map.find(name) == motion_map.end()) {
		next = motion_map[name]; 
		loop_flag = loop; 
	}
}

void Dx11XMotionManager::AddAnimation(std::string name, Dx11XMotion* _motion)
{
	if(_motion == 0) {
		return; 
	}

	if(motion_map.find(name) == motion_map.end()) {
		motion_map[name] = _motion; 
		current = _motion; 
	}
}


