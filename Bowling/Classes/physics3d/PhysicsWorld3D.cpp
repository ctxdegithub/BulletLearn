#include "PhysicsWorld3D.h"
#include "PhysicsDraw3D.h"

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
		_world->removeCollisionObject(obj);  
		delete obj;
	}
}

void PhysicsWorld3D::destroy()
{
	this->clear();
	if (_debugDraw)
	{
		_debugDraw->destroy();
		_debugDraw = nullptr;
	}

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

PhysicsWorld3D* PhysicsWorld3D::createWithDebug(Node* layer, const btVector3& gravity)
{
	auto world = new PhysicsWorld3D;
	if (world && world->initWorldWithDebug(layer, gravity))
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
	_debugDraw = nullptr;

	return true;
}

bool PhysicsWorld3D::initWorldWithDebug(Node* layer, const btVector3& gravity)
{
	if (!this->initWorld(gravity))
	{
		return false;
	}

	_debugDraw = PhysicsDraw3D::createWithLayer(layer);
	_world->setDebugDrawer(_debugDraw);
	
	return true;
}

void PhysicsWorld3D::setDebugDrawMode(int mode)
{
	if (_debugDraw)
	{
		_debugDraw->setDebugMode(mode);
	}
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

btRigidBody* PhysicsWorld3D::addCylinder(const btVector3& size, const btVector3& position, const PhysicsMaterial3D& material, char xyz)
{
	btCollisionShape* colShape = nullptr;
	switch (xyz)
	{
	case 'x':
		colShape = new btCylinderShapeX(size * 0.5f); // halfSize
		break;
	case 'y':
		colShape = new btCylinderShape(size * 0.5f); // halfSize
		break;
	case 'z':
		colShape = new btCylinderShapeZ(size * 0.5f); // halfSize
		break;
	}
	
	auto body = getBody(colShape, position, material);

	_world->addRigidBody(body);

	return body;
}

btRigidBody* PhysicsWorld3D::addCapsule(btScalar radius, btScalar height, const btVector3& position, const PhysicsMaterial3D& material, char xyz)
{
	btCollisionShape* colShape = nullptr;
	switch (xyz)
	{
	case 'x':
		colShape = new btCapsuleShapeX(radius, height); // halfSize
		break;
	case 'y':
		colShape = new btCapsuleShape(radius, height); // halfSize
		break;
	case 'z':
		colShape = new btCapsuleShapeZ(radius, height); // halfSize
		break;
	}

	auto body = getBody(colShape, position, material);

	_world->addRigidBody(body);

	return body;
}

btRigidBody* PhysicsWorld3D::addCone(btScalar radius, btScalar height, const btVector3& position, const PhysicsMaterial3D& material, char xyz)
{
	btCollisionShape* colShape = nullptr;
	switch (xyz)
	{
	case 'x':
		colShape = new btConeShapeX(radius, height); // halfSize
		break;
	case 'y':
		colShape = new btConeShape(radius, height); // halfSize
		break;
	case 'z':
		colShape = new btConeShapeZ(radius, height); // halfSize
		break;
	}

	auto body = getBody(colShape, position, material);

	_world->addRigidBody(body);

	return body;
}

btRigidBody* PhysicsWorld3D::addMultiSphere(btScalar *radi, const btVector3* positions, int numSpheres, const btVector3& position, const PhysicsMaterial3D& material)
{
	btCollisionShape* colShape = new btMultiSphereShape(positions, radi, numSpheres);
	auto body = getBody(colShape, position, material);
	_world->addRigidBody(body);

	return body;
}

btRigidBody* PhysicsWorld3D::getBody(btCollisionShape* colShape, const btVector3& position, const PhysicsMaterial3D& material)
{
	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(position);

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (material.mass != 0.f);

	btVector3 localInertia(0,0,0);
	if (isDynamic)
		colShape->calculateLocalInertia(material.mass, localInertia);

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
	if (_debugDraw)
	{
		_debugDraw->clearDraw();
		_world->debugDrawWorld();
	}
}

void PhysicsWorld3D::update(float dt)
{
	_world->stepSimulation(dt);
}
