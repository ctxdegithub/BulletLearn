#include "PhysicsDraw3D.h"

PhysicsDraw3D::PhysicsDraw3D()
{

}

PhysicsDraw3D::~PhysicsDraw3D()
{
	
}

PhysicsDraw3D* PhysicsDraw3D::createWithLayer(Node* layer)
{
	auto draw = new PhysicsDraw3D;
	if (draw && draw->initWithLayer(layer))
	{
		return draw;
	}

	return nullptr;
}

bool PhysicsDraw3D::initWithLayer(Node* layer)
{	
	_drawNode = DrawNode3D::create();
	layer->addChild(_drawNode);
	_debugDrawMode = btIDebugDraw::DBG_MAX_DEBUG_DRAW_MODE;
	return true;
}

void PhysicsDraw3D::destroy()
{
	_drawNode->removeFromParent();
	delete this;
}

void PhysicsDraw3D::clearDraw()
{
	_drawNode->clear();
}

void PhysicsDraw3D::drawLine(const btVector3& from,const btVector3& to,const btVector3& color)
{
	Vec3 vertices[2] = {
		Vec3(from.x(), from.y(), from.z()),
		Vec3(to.x(), to.y(), to.z())
	};

	_color.r = color.x();
	_color.g = color.y();
	_color.b = color.z();
	_color.a = 1.f;

	_drawNode->drawLine(vertices[0], vertices[1], _color);
}

void PhysicsDraw3D::drawContactPoint(const btVector3& pointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color)
{
	btVector3 tov3 = pointOnB + normalOnB * distance;

	Vec3 from = Vec3(pointOnB.x(), pointOnB.y(), pointOnB.z());
	Vec3 to = Vec3(tov3.x(), tov3.y(), tov3.z());
	_color.r = color.x();
	_color.g = color.y();
	_color.b = color.z();
	_color.a = 1.f;

	_drawNode->drawLine(from, to, _color);
	_drawNode->drawPoint(from, 2, _color);
}

void PhysicsDraw3D::reportErrorWarning(const char* warningString)
{

}

void PhysicsDraw3D::draw3dText(const btVector3& location,const char* textString)
{

}

void PhysicsDraw3D::setDebugMode(int debugMode)
{
	_debugDrawMode = debugMode;
}

int PhysicsDraw3D::getDebugMode() const
{
	return _debugDrawMode;
}


