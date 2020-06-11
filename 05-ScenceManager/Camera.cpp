#include "Camera.h"
#include "Camera.h"


Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::Update(DWORD dt, int scene, bool isMoveScene, int stage)
{
	if (scene == 1)
	{
		if (cam_x <= 0)
		{
			cam_x = 0;
		}
		else
			if (cam_x >= 770 - 320)
			{
				cam_x = 770 - 320;
			}
	}
	else if (scene == 2 && isMoveScene == false)
	{
		if (cam_x <= 0)
		{
			cam_x = 0;
		}
		else
			if (stage == 1)
			{
				if (cam_x <= 1522)
					cam_x = 1522;
				else
					if (cam_x >= 2058 - 320)
						cam_x = 2058 - 320;
			}
			else if (stage == 0)
			{
				if (cam_x >= 1545 - 320 && cam_x <= 1519)
				{
					cam_x = 1545 - 320;
				}
			}
			else if (stage == 2)
			{
				if (cam_x <= 1576)
				{
					cam_x = 1576;
				}
				else if (cam_x >= 2103 - 320)
				{
					cam_x = 2103 - 320;
				}
			}
			else if (stage == 3)
			{
				if (cam_x <= 2042)
				{
					cam_x = 2042;
				}
				else if (cam_x >= 2825 - 320)
				{
					cam_x = 2825 - 320;
				}
			}
			else if (stage == 4)
			{
				if (cam_x <= 2505)
				{
					cam_x = 2505;
				}
				else if (cam_x >= 2825 - 320)
				{
					cam_x = 2825 - 320;
				}
			}
	}

}


void Camera::SetCamera(float x, float y)
{
	cam_x = x;
	cam_y = y;
}

D3DXVECTOR2 Camera::GetPosition()
{
	return D3DXVECTOR2(cam_x, cam_y);
}

D3DXVECTOR2 Camera::transform(float x_world, float y_world)
{
	return D3DXVECTOR2(x_world - cam_x, y_world - cam_y);
}
