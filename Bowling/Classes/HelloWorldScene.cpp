#include "HelloWorldScene.h"
#include "physics3d/PhysicsMesh3D.h"

bool HelloWorld::init()
{
    do
	{
		CC_BREAK_IF(!BaseTest::init());
		
		CC_BREAK_IF(!initCamera());
		CC_BREAK_IF(!initPhysics3D());
		CC_BREAK_IF(!initListener());
		
		CC_BREAK_IF(!initUI());

		this->scheduleUpdate();

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
	_phyMesh3D = PhysicsMesh3D::constuct("map.raw");
	_world->addTriangleMeshShape(_phyMesh3D, btVector3(0, 0, 0));

	// 载入plane模型
	auto spPlane = Sprite3D::create("model/map.c3b"); 
	this->addChild(spPlane);
	spPlane->setPosition3D(Vec3(0, 0, 0));
	//spPlane->setRotation3D(Vec3(0, 180, 0));

	//// add a plane 方向向上,位置(0,0,0), 0.5的摩擦,0.5的弹性
	//_world->addPlane(btVector3(0, 1, 0), btVector3(0, 0, 0), PHYSICS_MATERIAL3D_PLANE);
	
	btRigidBody* body = nullptr;

	// read raw
	//_heightMapData = FileUtils::getInstance()->getDataFromFile("map.raw");
	
	// add height field
	
	//extern char heightMap[];
	////float *mapData = (float*)heightMap;
	//short *mapData = (short*)heightMap;
	//unsigned char uData = 0;
	//for (int i=0; i<128; ++i)
	//{
	//	for (int j=0; j<128; ++j)
	//	{
	//		mapData[i*128+j] = j % 2;
	//		if (uData < _heightMapData.getBytes()[i*128+j])
	//		{
	//			uData = _heightMapData.getBytes()[i*128+j];
	//		}
	//	}
	//}
	//HeightfieldInfo info(128, 128, _heightMapData.getBytes(), PHY_UCHAR, 1.6f / uData, -1.f, 1.f, btVector3(25.f / uData, 1.f, 25.f / uData));
	////HeightfieldInfo info(128, 128, mapData, PHY_SHORT, 1.f, -2.f, 1.f, btVector3(1.f, 1.f, 1.f));
	//_world->addHeightfieldTerrain(info, btVector3(-0.25f, 0.f, 0.42f));

	Sprite3D* ojbect;
	// 载入盒子模型
	ojbect = Sprite3D::create("model/box.c3b");
	this->addChild(ojbect);
	// add a box
	body = _world->addBox(btVector3(1.f, 1.f, 1.f), btVector3(6, 15.f, 0));
	body->setUserPointer(ojbect);
	_rigidBodies.push_back(body);

	// load ball model
	ojbect = Sprite3D::create("model/ball.c3b");
	this->addChild(ojbect);
	// add Sphere
	body = _world->addSphere(1.f, btVector3(4.f, 15.f, 0.f));
	body->setUserPointer(ojbect);
	_rigidBodies.push_back(body);
	
	// add Capsule
	//body = _world->addCapsule(1.f, 1.f, btVector3(-10.f, 15.f, 0.f));
	//_rigidBodies.push_back(body);

	// load cylinder model
	ojbect = Sprite3D::create("model/cylinder.c3b");
	this->addChild(ojbect);
	// add Cylinder
	body = _world->addCylinder(btVector3(2.f, 2.0f, 2.f), btVector3(-6.f, 15.f, 0.f));
	body->setUserPointer(ojbect);
	_rigidBodies.push_back(body);

	// add Cone
	//body = _world->addCone(1.f, 2.f, btVector3(-2.f, 15.f, 0.f));
	//_rigidBodies.push_back(body);
	// add multi spheres
	btVector3 positions[] = {
		btVector3(-1.f, 1.f, 0.f),
		btVector3(1.f, 1.f, 0.f),
		btVector3(0.f, -1.f, 0.f)
	};

	btScalar radi[] = {
		1.f, 0.5f, 1.f
	};

	//body = _world->addMultiSphere(radi, positions, 3, btVector3(4.f, 15.f, 0.f));
	//_rigidBodies.push_back(body);
}

void HelloWorld::onExit()
{
	_world->destroy();		// 销毁物理世界
	_world = nullptr;

	_rigidBodies.clear();
	_phyMesh3D->destroy();

	Director::getInstance()->getEventDispatcher()->removeEventListener(_touchListener);

	Layer::onExit();
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
	Layer::draw(renderer, transform, flags);
	//_world->debugDraw();
}

void HelloWorld::update(float delta)
{
	this->updateWorld(delta);
	this->updateCamera(delta);
}

void HelloWorld::updateWorld(float delta)
{
	static float m[16];
	_world->update(delta);

	// update model position
	std::vector<btRigidBody*> needRemove;
	for (int i=0; i<_rigidBodies.size(); ++i)
	{
		auto trans = _rigidBodies[i]->getWorldTransform();		// 获取box的变换矩阵
		if (trans.getOrigin().getY() < -20.f)
		{
			needRemove.push_back(_rigidBodies[i]);
		}
		else
		{
			trans.getOpenGLMatrix(m);
			((Sprite3D*)_rigidBodies[i]->getUserPointer())->setNodeToParentTransform(m);
		}
	}

	for (auto body : needRemove)
	{
		((Sprite3D*)body->getUserPointer())->removeFromParent();
		for (auto bodyTmp = _rigidBodies.begin(); bodyTmp != _rigidBodies.end(); ++bodyTmp)
		{
			if (body == *bodyTmp)
			{
				_rigidBodies.erase(bodyTmp);
				_world->removeCollisionObject(body);
				break;
			}
		}
	}
}

void HelloWorld::updateCamera(float delta)
{
	auto camPos = _camera->getPosition3D();
	auto camInfo = __String::createWithFormat("%.2f,%.2f,%.2f", camPos.x, camPos.y, camPos.z);
	_labelInfo->setString(camInfo->getCString());
}