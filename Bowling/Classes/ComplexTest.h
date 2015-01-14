#ifndef __FORCE_IMPULSE_H__
#define __FORCE_IMPULSE_H__
#include "BaseTest.h"
#include "physics3d/PhysicsWorld3D.h"

class ComplexTest : public BaseTest
{
public:

	CREATE_FUNC(ComplexTest);
	bool init();

	void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags);
	void onExit();

	void update(float delta);
	void updateWorld(float delta);

	bool onTouchBegan(Touch *touch, Event *unused_event); 
	void onTouchMoved(Touch *touch, Event *unused_event); 
	void onTouchEnded(Touch *touch, Event *unused_event); 

private:
	bool initPhysics3D();		// 初始化物理世界
	bool initListener();	

private:
	PhysicsWorld3D* _world;
	btRigidBody* _boxBody;
	bool _bForce;

	EventListenerTouchOneByOne* _touchListener;
};

#endif