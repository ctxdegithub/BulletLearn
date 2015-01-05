#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "physics3d/PhysicsWorld3D.h"

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    void menuCloseCallback(cocos2d::Ref* pSender);
    CREATE_FUNC(HelloWorld);

	virtual bool init();
	void onExit();				
	void update(float delta);

private:
	bool initPhysics3D();		// ��ʼ����������
	bool initCamera();			// ��ʼ�������

private:
	PhysicsWorld3D* _world;		// 3d ��������
	btRigidBody* _box;			// ����
	cocos2d::Sprite3D* _spBox;	// ����ģ��
	cocos2d::Camera* _camera;	// �����
};

#endif // __HELLOWORLD_SCENE_H__
