#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "physics3d/PhysicsWorld3D.h"
USING_NS_CC;

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    void menuCloseCallback(cocos2d::Ref* pSender);
    CREATE_FUNC(HelloWorld);

	bool init();
	void onExit();				
	void update(float delta);
	void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags);

	bool onTouchBegan(Touch *touch, Event *unused_event); 
	void onTouchMoved(Touch *touch, Event *unused_event); 
	void onTouchEnded(Touch *touch, Event *unused_event); 

private:
	bool initPhysics3D();		// ��ʼ����������
	bool initCamera();			// ��ʼ�������
	bool initListener();		

private:
	PhysicsWorld3D* _world;		// 3d ��������
	btRigidBody* _box;			// ����
	cocos2d::Sprite3D* _spBox;	// ����ģ��
	cocos2d::Camera* _camera;	// �����
	ui::Text* _labelInfo;
	EventListenerTouchOneByOne* _touchListener;
};

#endif // __HELLOWORLD_SCENE_H__
