#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "CameraControl.h"
#include "Camera3D.h"
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
static DrawNode* drawNode;
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
		
		_labelInfo = static_cast<ui::Text*>(rootNode->getChildByName("lbl_body_info"));
		
		this->scheduleUpdate();
		drawNode = DrawNode::create();
		this->addChild(drawNode);


		return true;
	} while (0);

    return false;
}

bool HelloWorld::initPhysics3D()
{
	_world = PhysicsWorld3D::createWithDebug(this);	// 创建3d物理世界
	if (_world == nullptr)
	{
		return false;
	}
	
	this->addSomeBodies();	// to test bodies

	// 设置2摄像机可见
	this->setCameraMask(2);
	return true;
}

bool HelloWorld::initCamera()
{
	auto size = Director::getInstance()->getVisibleSize();
	_camera = Camera3D::create(60, size.width / size.height, 0.1f, 1000);
	if (_camera == nullptr)
	{
		return false;
	}
	_camera->setCameraFlag(CameraFlag::USER1); // 设置为2
	this->addChild(_camera);
	_camera->setPosition3D(Vec3(0, 2, 0));	// 摄像机位置
	_camera->lookAt(Vec3::ZERO, Vec3(0, 1, 0)); // 摄像机target
	_camera->setMoveSpeed(5.f);

	auto cameraCtrl = CameraControl::create();
	cameraCtrl->setCamera(_camera);
	
	this->addChild(cameraCtrl);

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

void HelloWorld::addSomeBodies()
{
	// 载入plane模型
	auto spPlane = Sprite3D::create("map.c3b"); 
	this->addChild(spPlane);
	spPlane->setPosition3D(Vec3(0, 0, 0));

	//// add a plane 方向向上,位置(0,0,0), 0.5的摩擦,0.5的弹性
	//_world->addPlane(btVector3(0, 1, 0), btVector3(0, 0, 0), PHYSICS_MATERIAL3D_PLANE);
	
	btRigidBody* body = nullptr;

	// read raw
	_heightMapData = FileUtils::getInstance()->getDataFromFile("map.raw");
	
	// add height field
	extern char heightMap[];
	float *mapData = (float*)heightMap;
	unsigned char uData = 0;
	for (int i=0; i<128; ++i)
	{
		for (int j=0; j<128; ++j)
		{
			mapData[i*50+j] = CCRANDOM_0_1();
			//mapData[i*50+j] *= 2;
			//log("%d",  _heightMapData.getBytes()[i*50+j]);
			if (uData < _heightMapData.getBytes()[i*50+j])
			{
				uData = _heightMapData.getBytes()[i*50+j];
			}
		}
	}
	HeightfieldInfo info(128, 128, _heightMapData.getBytes(), PHY_UCHAR, 1.6f / uData, -1.f, 1.f, btVector3(24.f / uData, 1.f, 24.f / uData));
	_world->addHeightfieldTerrain(info, btVector3(0.f, 0.f, 0.f));

	// 载入盒子模型
	_spBox = Sprite3D::create("model/box.c3b");
	this->addChild(_spBox);
	_spBox->setPosition3D(Vec3(0, 5, 0));

	// add a box
	_box = _world->addBox(btVector3(1, 1, 1), btVector3(6, 5.f, 0));
	_box->setUserPointer(_spBox);

	// add Sphere
	body = _world->addSphere(1.f, btVector3(4.f, 10.f, 0.f));
	_rigidBodies.push_back(body);
	// add Capsule
	body = _world->addCapsule(1.f, 1.f, btVector3(-10.f, 10.f, 0.f));
	_rigidBodies.push_back(body);
	// add Cylinder
	body = _world->addCylinder(btVector3(2.f, 2.0f, 2.f), btVector3(-6.f, 10.f, 0.f));
	_rigidBodies.push_back(body);
	// add Cone
	body = _world->addCone(1.f, 2.f, btVector3(-2.f, 10.f, 0.f));
	_rigidBodies.push_back(body);
	// add multi spheres
	btVector3 positions[] = {
		btVector3(-1.f, 1.f, 0.f),
		btVector3(1.f, 1.f, 0.f),
		btVector3(0.f, -1.f, 0.f)
	};

	btScalar radi[] = {
		1.f, 0.5f, 1.f
	};

	body = _world->addMultiSphere(radi, positions, 3, btVector3(4.f, 3.f, 0.f));
	_rigidBodies.push_back(body);
}

void HelloWorld::onExit()
{
	Layer::onExit();

	_world->destroy();		// 销毁物理世界
	_world = nullptr;

	_rigidBodies.clear();

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
	auto cameraPos = _camera->getPosition3D();
	for (auto body : _rigidBodies)
	{
		body->setActivationState(ACTIVE_TAG);
		body->applyCentralImpulse(btVector3(0, 5.f, 0));
	}
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

	auto camPos = _camera->getPosition3D();
	auto camInfo = __String::createWithFormat("%.2f,%.2f,%.2f", camPos.x, camPos.y, camPos.z);
	_labelInfo->setString(camInfo->getCString());
}