#include "cocos2d.h"
#include "Bullet/LinearMath/btIDebugDraw.h"
USING_NS_CC;


class PhysicsDraw3D : public btIDebugDraw
{
public:
	void drawLine(const btVector3& from,const btVector3& to,const btVector3& color);
	void drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color);
	void reportErrorWarning(const char* warningString);
	void draw3dText(const btVector3& location,const char* textString);
	void setDebugMode(int debugMode);
	int getDebugMode() const;

private:
	int _debugDrawMode;
};