#include "bullet/btBulletDynamicsCommon.h"

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
const PhysicsMaterial3D PHYSICS_MATERIAL3D_PLANE(0.f, 0.5f, 0.5f, 0.f);

class PhysicsWorld3D
{
public:
	~PhysicsWorld3D();

	static PhysicsWorld3D* create(const btVector3& gravity = btVector3(0, -10, 0));
	bool initWorld(const btVector3& gravity);
	void destroy();
	void debugDraw();

	btRigidBody* addPlane(const btVector3& normal, const btVector3& position, const PhysicsMaterial3D& material = PHYSICS_MATERIAL3D_PLANE);
	btRigidBody* addSphere(btScalar radius, const btVector3& position, const PhysicsMaterial3D& material = PHYSICS_MATERIAL3D_DEFAULT);
	btRigidBody* addBox(const btVector3& size, const btVector3& position, const PhysicsMaterial3D& material = PHYSICS_MATERIAL3D_DEFAULT);

	void update(float dt);

private:
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
