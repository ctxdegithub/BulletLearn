#ifndef __CAMERA_3D_H__
#define __CAMERA_3D_H__

#include "cocos2d.h"
USING_NS_CC;

class Camera3D : public Camera
{
public:
	enum Camera3DType
	{
		LAND,
		AIRCRAFT
	};
	
	static Camera3D* create(float fieldOfView, float aspectRatio, float nearPlane, float farPlane);

	Camera3D();
	void updatePosition(const Vec2& dir, float dt);

private:
	
	void updateLand(const Vec2& dir, float dt);
	void updateAircraft(const Vec2& dir, float dt);

private:
	CC_SYNTHESIZE(Camera3DType, _cameraType, CameraType);
	CC_SYNTHESIZE(float, _moveSpeed, MoveSpeed);
};

#endif // !__CAMERA_3D_H__