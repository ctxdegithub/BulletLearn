#include "Camera3D.h"

Camera3D::Camera3D() :
	_cameraType(LAND),
	_moveSpeed(1.f)
{

}

Camera3D* Camera3D::create(float fieldOfView, float aspectRatio, float nearPlane, float farPlane)
{
	auto camera = new (std::nothrow)Camera3D;
	if (camera && camera->initPerspective(fieldOfView, aspectRatio, nearPlane, farPlane))
	{
		camera->autorelease();
		return camera;
	}
	
	CC_SAFE_DELETE(camera);
	return nullptr;
}

void Camera3D::updatePosition(const Vec2& dir, float dt)
{
	if (dir.equals(Vec2::ZERO))
	{
		return;
	}
	
	switch (_cameraType)
	{
	case Camera3D::LAND:
		updateLand(dir, dt);
		break;
	case Camera3D::AIRCRAFT:
		updateAircraft(dir, dt);
		break;
	default:
		break;
	}
}

void Camera3D::updateLand(const Vec2& dir, float dt)
{
	auto cameraPos = this->getPosition3D();
	auto cameraRotY = this->getRotation3D().y;
	
	auto dis = dir.getNormalized() * _moveSpeed * dt;
	dis = dis.rotateByAngle(Vec2::ZERO, CC_DEGREES_TO_RADIANS(cameraRotY));
	cameraPos.x += dis.x;
	cameraPos.z -= dis.y;
	
	this->setPosition3D(cameraPos);
}

void Camera3D::updateAircraft(const Vec2& dir, float dt)
{
	Vec3 forward, up;
	this->getNodeToWorldTransform().getForwardVector(&forward);
	this->getNodeToWorldTransform().getRightVector(&up);
	auto cameraPos = this->getPosition3D();
	
	auto dis = dir.getNormalized() * _moveSpeed * dt;
	up *= dis.x;
	forward *= dis.y;
	cameraPos += up;
	cameraPos += forward;

	this->setPosition3D(cameraPos);
}