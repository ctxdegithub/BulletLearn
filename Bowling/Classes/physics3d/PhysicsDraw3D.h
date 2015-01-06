#ifndef __PHYSICS_DRAW_3D_H__
#define __PHYSICS_DRAW_3D_H__

#include "cocos2d.h"
#include "Bullet/LinearMath/btIDebugDraw.h"
#include "DrawNode3D.h"
USING_NS_CC;

class PhysicsDraw3D : public btIDebugDraw
{
public:
	PhysicsDraw3D();
	~PhysicsDraw3D();
	static PhysicsDraw3D* createWithLayer(Node* layer);
	void destroy();
	void clearDraw();

	void drawLine(const btVector3& from,const btVector3& to,const btVector3& color);
	void drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color);
	void draw3dText(const btVector3& location,const char* textString);
	void reportErrorWarning(const char* warningString);
	
	void setDebugMode(int debugMode);
	int getDebugMode() const;

private:
	bool initWithLayer(Node* layer);

private:
	int _debugDrawMode;

private:
	Color4F _color;
	DrawNode3D* _drawNode;
};

#endif 