#include "PhysicsWorld3D.h"
#include "PhysicsDraw3D.h"
#include "PhysicsMesh3D.h"
#include "Bullet/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"

PhysicsWorld3D::~PhysicsWorld3D()
{

}

void PhysicsWorld3D::clear()
{
	//remove the bodies from the dynamics world and delete them
	for (int i = _world->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* colObj = _world->getCollisionObjectArray()[i];
		this->removeCollisionObject(colObj);
	}
}

void PhysicsWorld3D::removeCollisionObject(btCollisionObject* collisionObject)
{
	btRigidBody* rigidBody = btRigidBody::upcast(collisionObject);
	if (rigidBody != nullptr)
	{
		if (rigidBody->getMotionState() != nullptr)
		{
			delete rigidBody->getMotionState();
		}
		
		delete rigidBody->getCollisionShape();
	}
	_world->removeCollisionObject(collisionObject);
	delete collisionObject;
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

btRigidBody* PhysicsWorld3D::addCompound(std::vector<PhysicsShapeInfo3D>& shapeList, const btVector3& position, const PhysicsMaterial3D& material)
{
	btCompoundShape* shape = new btCompoundShape;
	
	for (auto& shapeInfo : shapeList)
	{
		shape->addChildShape(shapeInfo.transform, shapeInfo.colShape);
	}
	
	auto body = getBody(shape, position, material);
	_world->addRigidBody(body);
	return body;
}

btRigidBody* PhysicsWorld3D::addConvexHull(const float* floatData, int numPoints, const btVector3& position, bool bPoly, const PhysicsMaterial3D& material)
{
	btConvexHullShape* colShape = new btConvexHullShape(floatData, numPoints, sizeof(btVector3));
	if (bPoly)
	{
		colShape->initializePolyhedralFeatures();
	}

	auto body = getBody(colShape, position, material);
	_world->addRigidBody(body);

	return body;
}

btRigidBody* PhysicsWorld3D::addConvexHull(std::vector<btVector3>& points, const btVector3& position, bool bPoly, const PhysicsMaterial3D& material)
{
	auto body = addConvexHull(points[0].m_floats, points.size(), position, bPoly, material);

	return body;
}

btRigidBody* PhysicsWorld3D::addTriangleMesh(PhysicsMesh3D* mesh3D, const btVector3& position, const PhysicsMaterial3D& material)
{
	CCAssert(material.mass == 0.f, "body's mass must be 0.");

	auto body = getBody(mesh3D->getMeshShape(), position, material);
	_world->addRigidBody(body);

	return body;
}

btRigidBody* PhysicsWorld3D::addPlane(const btVector3& normal, const btVector3& position, const PhysicsMaterial3D& material)
{
	CCAssert(material.mass == 0.f, "plane's mass must be 0.");
	btCollisionShape* planeShape = new btStaticPlaneShape(normal, 0.f);
	auto body = getBody(planeShape, position, material);

	_world->addRigidBody(body);
	return body;
}

btRigidBody* PhysicsWorld3D::addHeightfieldTerrain(const HeightfieldInfo& fieldInfo, const btVector3& position, const PhysicsMaterial3D& material)
{
	CCAssert(material.mass == 0.f, "height field's mass must be 0.");

	btHeightfieldTerrainShape* heightfieldShape = new btHeightfieldTerrainShape(
		fieldInfo.heightStickWidth, fieldInfo.heightStickLength, fieldInfo.heightfieldData, fieldInfo.heightScale,
		fieldInfo.minHeight, fieldInfo.maxHeight, fieldInfo.upAxis, fieldInfo.hdt, fieldInfo.flipQuadEdges);

	heightfieldShape->setUseDiamondSubdivision(true);
	heightfieldShape->setLocalScaling(fieldInfo.localScaling);

	auto body = getBody(heightfieldShape, position, material);

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
