#ifndef __PHYSICS_WORLD_3D_H__
#define __PHYSICS_WORLD_3D_H__

#include "bullet/btBulletDynamicsCommon.h"
#include "cocos2d.h"
USING_NS_CC;

class PhysicsDraw3D;

struct PhysicsMaterial3D
{
	btScalar friction;
	btScalar rollingFriction;
	btScalar restitution;
	btScalar mass;
		
	PhysicsMaterial3D() :
		friction(0.0f),
		rollingFriction(0.f),
		restitution(0.f),
		mass(0.f)
	{}

	PhysicsMaterial3D(btScalar aMass, btScalar aFriction, btScalar aRestitution, btScalar aRollingFriction) :
		friction(aFriction),
		rollingFriction(aRollingFriction),
		restitution(aRestitution),
		mass(aMass)
	{}
};

const PhysicsMaterial3D PHYSICS_MATERIAL3D_DEFAULT(1.f, 0.5f, 0.5f, 0.0f);
const PhysicsMaterial3D PHYSICS_MATERIAL3D_PLANE(0.f, 0.5f, 0.5f, 0.0f);

class PhysicsWorld3D
{
public:
	~PhysicsWorld3D();

	static PhysicsWorld3D* create(const btVector3& gravity = btVector3(0, -10, 0));
	static PhysicsWorld3D* createWithDebug(Node* layer, const btVector3& gravity = btVector3(0, -10, 0));

	void destroy();
	void debugDraw();

	void setDebugDrawMode(int mode);

	btRigidBody* addPlane(const btVector3& normal, const btVector3& position, const PhysicsMaterial3D& material = PHYSICS_MATERIAL3D_PLANE);
	btRigidBody* addSphere(btScalar radius, const btVector3& position, const PhysicsMaterial3D& material = PHYSICS_MATERIAL3D_DEFAULT);
	btRigidBody* addBox(const btVector3& size, const btVector3& position, const PhysicsMaterial3D& material = PHYSICS_MATERIAL3D_DEFAULT);
	btRigidBody* addCylinder(const btVector3& size, const btVector3& position, const PhysicsMaterial3D& material = PHYSICS_MATERIAL3D_DEFAULT, char xyz = 'y');
	btRigidBody* addCapsule(btScalar radius, btScalar height, const btVector3& position, const PhysicsMaterial3D& material = PHYSICS_MATERIAL3D_DEFAULT, char xyz = 'y');
	btRigidBody* addCone(btScalar radius, btScalar height, const btVector3& position, const PhysicsMaterial3D& material = PHYSICS_MATERIAL3D_DEFAULT, char xyz = 'y');
	btRigidBody* addMultiSphere(btScalar *radi, const btVector3* positions, int numSpheres, const btVector3& position, const PhysicsMaterial3D& material = PHYSICS_MATERIAL3D_DEFAULT);

	void update(float dt);

private:
	bool initWorld(const btVector3& gravity);
	bool initWorldWithDebug(Node* layer, const btVector3& gravity);
	void clear();

private:
	btRigidBody* getBody(btCollisionShape* colShape, const btVector3& position, const PhysicsMaterial3D& material);

private:
	btDiscreteDynamicsWorld* _world;
	btDefaultCollisionConfiguration* _collisionConfiguration;
	btCollisionDispatcher* _dispatcher;
	btBroadphaseInterface* _overlappingPairCache;
	btSequentialImpulseConstraintSolver* _solver;

	PhysicsDraw3D* _debugDraw;
};


#endif