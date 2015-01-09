#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "physics3d/PhysicsWorld3D.h"
USING_NS_CC;

class Camera3D;

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    void menuCloseCallback(cocos2d::Ref* pSender);
    CREATE_FUNC(HelloWorld);

	bool init();
	void onExit();				
	void update(float delta);
	void updateWorld(float delta);
	void updateCamera(float delta);

	void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags);

	bool onTouchBegan(Touch *touch, Event *unused_event); 
	void onTouchMoved(Touch *touch, Event *unused_event); 
	void onTouchEnded(Touch *touch, Event *unused_event); 

private:
	bool initPhysics3D();		// 初始化物理世界
	bool initCamera();			// 初始化摄像机
	bool initListener();		

	void addSomeBodies();

private:
	PhysicsWorld3D* _world;		// 3d 物理世界
	std::vector<btRigidBody*> _rigidBodies;
	PhysicsMesh3D* _phyMesh3D;	// mesh shape
	ui::Text* _labelInfo;

	Camera3D* _camera;	// 摄像机
	
	EventListenerTouchOneByOne* _touchListener;
	Data _heightMapData;
};

#endif // __HELLOWORLD_SCENE_H__
