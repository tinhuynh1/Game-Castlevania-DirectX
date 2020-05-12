#pragma once
#include <d3dx9.h>
class Camera
{
	float cam_x;
	float cam_y;
public:
	Camera();
	~Camera();

	void Update(DWORD dt, int scene, bool isMoveScene, int state);
	void SetCamera(float x, float y);
	D3DXVECTOR2 GetPositon();
	D3DXVECTOR2 transform(float x_world, float y_world);

};

