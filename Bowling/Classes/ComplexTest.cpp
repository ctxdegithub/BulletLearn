#include "ComplexTest.h"
#include "Bullet/btBulletCollisionCommon.h"
#include "physics3d/PhysicsHelper3D.h"

bool ComplexTest::init()
{
	do 
	{
		CC_BREAK_IF(!BaseTest::init());

		CC_BREAK_IF(!initCamera());
		_camera->setPosition3D(Vec3(0, 10, 20.f));
		CC_BREAK_IF(!initPhysics3D());

		CC_BREAK_IF(!initListener());
		CC_BREAK_IF(!initUI());

		return true;
	} while (0);
	
	return false;
}

bool ComplexTest::initPhysics3D()
{
	_world = PhysicsWorld3D::createWithDebug(this);

	_world->addPlane(btVector3(0, 1, 0), btVector3(0, 0, 0));
	for (int i=-10; i<10; ++i)
	{
		for (int j=0; j<10; ++j)
		{
			_world->addBox(btVector3(1.f, 1.f, 1.f), btVector3(i, j, 0));
		}
	}

// 	std::vector<float> points;
// 	PhysicsHelper3D::loadRaw("test.raw", points);
// 
// 	_convexBody = _world->addConvexHullShape(&points[0], points.size() / 3, btVector3(0, 0, 5), true, PhysicsMaterial3D(5.f, 0.5f, 0.5f, 0.f));


	PhysicsShapeInfo3D shapeInfo;
	btTransform trans;
	trans.setIdentity();
	btCollisionShape* shape;
	
	// 1
	shape = new btBoxShape(btVector3(1.f, 1.f, 1.f));
	shapeInfo.colShape = shape;
	
	trans.setOrigin(btVector3(0, 1, 0));
	shapeInfo.transform = trans;
	_shapes.push_back(shapeInfo);

	// 2
	shape = new btSphereShape(1.f);
	shapeInfo.colShape = shape;
	trans.setOrigin(btVector3(0, 3, 0));
	shapeInfo.transform = trans;
	_shapes.push_back(shapeInfo);

	_world->addCompound(_shapes, btVector3(0, 0, 5.f));
	
	_boxBody = _world->addBox(btVector3(1, 1, 1), btVector3(0, 10, 10));

	_bForce = false;

	this->setCameraMask(2);
	return true;
}

bool ComplexTest::initListener()
{
	_touchListener = EventListenerTouchOneByOne::create();
	if (_touchListener == nullptr)
	{
		return false;
	}

	_touchListener->onTouchBegan = CC_CALLBACK_2(ComplexTest::onTouchBegan, this);
	_touchListener->onTouchMoved = CC_CALLBACK_2(ComplexTest::onTouchMoved, this);
	_touchListener->onTouchEnded = CC_CALLBACK_2(ComplexTest::onTouchEnded, this);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_touchListener, this);
	return true;
}

void ComplexTest::onExit()
{
	_world->destroy();
	for (auto& shapeInfo : _shapes)
	{
		delete shapeInfo.colShape;
	}
	_shapes.clear();
	Layer::onExit();
}

void ComplexTest::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	Layer::draw(renderer, transform, flags);
	_world->debugDraw();
}


bool ComplexTest::onTouchBegan(Touch *touch, Event *unused_event)
{
	_bForce = true;
	_boxBody->setActivationState(ACTIVE_TAG);
//	_boxBody->applyCentralImpulse(btVector3(0, 0, -10));
	return true;
}

void ComplexTest::onTouchMoved(Touch *touch, Event *unused_event)
{

}

void ComplexTest::onTouchEnded(Touch *touch, Event *unused_event)
{
	_bForce = false;
}

void ComplexTest::update(float delta)
{
	this->updateWorld(delta);
}

void ComplexTest::updateWorld(float delta)
{
	if (_bForce)
	{
		_boxBody->setActivationState(ACTIVE_TAG);
		_boxBody->applyCentralForce(btVector3(0, 0, -10.f));
	}
	_world->update(delta);
}