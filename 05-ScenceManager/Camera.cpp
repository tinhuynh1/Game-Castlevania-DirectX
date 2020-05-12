#include "Camera.h"
Camera::Camera()
{

}
Camera::~Camera()
{

}
void Camera::Update(DWORD dt, int scene, bool isMoveScene, int state)
{
	if (scene == 1)
	{ 
		if (cam_x <= 0) //giới hạn cam không vượt qua tọa độ âm
		{
			cam_x = 0;
		}
	}
}