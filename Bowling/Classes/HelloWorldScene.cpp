#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    do
	{
		CC_BREAK_IF(!Layer::init());

		CC_BREAK_IF(!initCamera());
		CC_BREAK_IF(!initPhysics3D());
		CC_BREAK_IF(!initListener());

		auto rootNode = CSLoader::createNode("MainScene.csb");
		addChild(rootNode);
		auto closeItem = static_cast<ui::Button*>(rootNode->getChildByName("Button_1"));
		closeItem->addClickEventListener(CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

		this->scheduleUpdate();

		return true;
	} while (0);

    return false;
}

bool HelloWorld::initPhysics3D()
{
	_world = PhysicsWorld3D::create();	// 创建3d物理世界
	if (_world == nullptr)
	{
		return false;
	}
	
	// 载入plane模型
	auto spPlane = Sprite3D::create("model/ground.c3b"); 
	this->addChild(spPlane);
	spPlane->setPosition3D(Vec3::ZERO);
	
	// add a plane 方向向上,位置(0,0,0), 0.5的摩擦,0.5的弹性
	_world->addPlane(btVector3(0, 1, 0), btVector3(0, 0, 0), PHYSICS_MATERIAL3D_PLANE);
	
	// 载入盒子模型
	_spBox = Sprite3D::create("model/box.c3b");
	this->addChild(_spBox);
	_spBox->setPosition3D(Vec3(0, 20, 0));

	// add a box
	_box = _world->addBox(btVector3(1, 1, 1), btVector3(0, 20, 0));
	_box->setUserPointer(_spBox);

	// 设置2摄像机可见
	this->setCameraMask(2);
	return true;
}

bool HelloWorld::initCamera()
{
	auto size = Director::getInstance()->getVisibleSize();
	_camera = Camera::createPerspective(60, size.width / size.height, 0.1f, 1000);
	if (_camera == nullptr)
	{
		return false;
	}
	_camera->setCameraFlag(CameraFlag::USER1); // 设置为2
	this->addChild(_camera);
	_camera->setPosition3D(Vec3(0, 10, 20));	// 摄像机位置
	_camera->lookAt(Vec3::ZERO, Vec3(0, 1, 0)); // 摄像机target

	return true;
}

bool HelloWorld::initListener()
{
	_touchListener = EventListenerTouchOneByOne::create();
	if (_touchListener == nullptr)
	{
		return false;
	}

	_touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	_touchListener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
	_touchListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_touchListener, this);
	return true;
}

void HelloWorld::onExit()
{
	Layer::onExit();

	_world->destroy();		// 销毁物理世界
	_world = nullptr;

	Director::getInstance()->getEventDispatcher()->removeEventListener(_touchListener);
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

bool HelloWorld::onTouchBegan(Touch *touch, Event *unused_event)
{
	_box->setActivationState(ACTIVE_TAG);
	_box->applyCentralImpulse(btVector3(0, 0, -5));

	auto pos = btVector3(0, 40, 0);
	pos.setX(CCRANDOM_MINUS1_1() * 10);
	pos.setZ(CCRANDOM_MINUS1_1() * 10);
	_world->addSphere(0.5f, pos, PhysicsMaterial3D(CCRANDOM_0_1() + 0.1f, 0.5f, 0.5f, 0.7f));
	return true;
}

void HelloWorld::onTouchMoved(Touch *touch, Event *unused_event)
{

}

void HelloWorld::onTouchEnded(Touch *touch, Event *unused_event)
{

}

void HelloWorld::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	_world->debugDraw();
	Layer::draw(renderer, transform, flags);
}

void HelloWorld::update(float delta)
{
	static float m[16];
	_world->update(delta);
	auto trans = _box->getWorldTransform();		// 获取box的变换矩阵
	trans.getOpenGLMatrix(m);
	_spBox->setNodeToParentTransform(Mat4(m));	// 设置box模型的变换矩阵，但是getPosition3D不会得到正确位置，这个以后讨论
}