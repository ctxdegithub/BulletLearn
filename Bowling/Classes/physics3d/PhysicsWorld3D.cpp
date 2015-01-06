#include "PhysicsWorld3D.h"
#include "PhysicsDraw3D.h"
#include "cocos2d.h"


PhysicsWorld3D::~PhysicsWorld3D()
{

}

void PhysicsWorld3D::clear()
{
	int i;
	//remove the bodies from the dynamics world and delete them
	for (i = _world->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = _world->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
	
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
			delete body->getCollisionShape();
		}
		_world->removeCollisionObject(body);  
		delete obj;
	}
}

void PhysicsWorld3D::destroy()
{
	this->clear();

	_debugDraw->destroy();
	_debugDraw = nullptr;

	delete _collisionConfiguration;
	delete _dispatcher;
	delete _solver;
	delete _overlappingPairCache;
	delete _world;
	delete this;
}

PhysicsWorld3D* PhysicsWorld3D::create(const btVector3& gravity)
{
	auto world = new PhysicsWorld3D;
	if (world && world->initWorld(gravity))
	{
		return world;
	}

	delete world;
	return nullptr;
}

bool PhysicsWorld3D::initWorld(const btVector3& gravity)
{
	_collisionConfiguration = new btDefaultCollisionConfiguration();

	_dispatcher = new btCollisionDispatcher(_collisionConfiguration);

	_overlappingPairCache = new btDbvtBroadphase();

	_solver = new btSequentialImpulseConstraintSolver;
	_world = new btDiscreteDynamicsWorld(_dispatcher, _overlappingPairCache, _solver, _collisionConfiguration);
	if (_world == nullptr)
	{
		return false;
	}
	
	_world->setGravity(gravity);

	_debugDraw = PhysicsDraw3D::create();
	_world->setDebugDrawer(_debugDraw);

	return true;
}

void PhysicsWorld3D::setDebugDrawMode(int mode)
{
	_debugDraw->setDebugMode(mode);
}

btRigidBody* PhysicsWorld3D::addPlane(const btVector3& normal, const btVector3& position, const PhysicsMaterial3D& material)
{
	CCAssert(material.mass == 0.f, "plane's mass must be 0.");
	btCollisionShape* groundShape = new btStaticPlaneShape(normal, 0.f);
	auto body = getBody(groundShape, position, material);

	_world->addRigidBody(body);
	return body;
}

btRigidBody* PhysicsWorld3D::addSphere(btScalar radius, const btVector3& position, const PhysicsMaterial3D& material)
{
	btCollisionShape* colShape = new btSphereShape(radius);
	auto body = getBody(colShape, position, material);

	_world->addRigidBody(body);

	return body;
}

btRigidBody* PhysicsWorld3D::addBox(const btVector3& size, const btVector3& position, const PhysicsMaterial3D& material)
{
	btCollisionShape* colShape = new btBoxShape(size * 0.5f); // halfSize
	auto body = getBody(colShape, position, material);

	_world->addRigidBody(body);

	return body;
}

btRigidBody* PhysicsWorld3D::getBody(btCollisionShape* colShape, const btVector3& position, const PhysicsMaterial3D& material)
{
	/// Create Dynamic Objects
	btTransform startTransform;
	startTransform.setIdentity();

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (material.mass != 0.f);

	btVector3 localInertia(0,0,0);
	if (isDynamic)
		colShape->calculateLocalInertia(material.mass, localInertia);

	startTransform.setOrigin(position);

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(material.mass, myMotionState,colShape,localInertia);
	rbInfo.m_restitution = material.restitution;
	rbInfo.m_friction = material.friction;
	rbInfo.m_rollingFriction = material.rollingFriction;
	btRigidBody* body = new btRigidBody(rbInfo);

	return body;
}

void PhysicsWorld3D::debugDraw()
{
	_world->debugDrawWorld();
}

void PhysicsWorld3D::update(float dt)
{
	_world->stepSimulation(dt);
}
