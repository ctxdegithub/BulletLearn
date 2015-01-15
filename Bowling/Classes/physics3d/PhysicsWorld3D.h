#ifndef __PHYSICS_WORLD_3D_H__
#define __PHYSICS_WORLD_3D_H__

#include "bullet/btBulletDynamicsCommon.h"
#include "cocos2d.h"
USING_NS_CC;
class PhysicsMesh3D;
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

struct HeightfieldInfo
{
	int heightStickWidth;
	int heightStickLength;
	void* heightfieldData;
	PHY_ScalarType hdt;
	btScalar heightScale;
	btScalar minHeight;
	btScalar maxHeight;
	int upAxis;

	bool useFloatData;
	bool flipQuadEdges;
	btVector3 localScaling;

	HeightfieldInfo(int width, int length, void* data, PHY_ScalarType type = PHY_SHORT, 
					btScalar heiScale = 1.f, btScalar minHei = 0.f, btScalar maxHei = 1.f, 
					const btVector3& scale = btVector3(1, 1, 1), int up = 1, 
					bool useFloat = false, bool filpQuad = false) :
		heightStickWidth(width), heightStickLength(length), heightfieldData(data), 
			heightScale(heiScale), minHeight(minHei), maxHeight(maxHei), 
			localScaling(scale), upAxis(up), 
			hdt(type), useFloatData(useFloat), flipQuadEdges(filpQuad)
	{}
};

struct PhysicsShapeInfo3D
{
// 	enum class ShapeType3D
// 	{
// 		BOX,
// 		SPHERE,
// 		CYLINDER,
// 		CAPSULE,
// 		CONE,
// 	};
// 
// 	ShapeType3D shapeType;
	btCollisionShape* colShape;
	btTransform transform;

	PhysicsShapeInfo3D() {};

	PhysicsShapeInfo3D(/*ShapeType3D type,*/ btCollisionShape* shape, btTransform& trans)
		: /*shapeType(type), */ colShape(shape), transform(trans)
	{

	}
};

class PhysicsWorld3D
{
public:
	~PhysicsWorld3D();

	static PhysicsWorld3D* create(const btVector3& gravity = btVector3(0, -10, 0));
	static PhysicsWorld3D* createWithDebug(Node* layer, const btVector3& gravity = btVector3(0, -10, 0));

	void destroy();
	void debugDraw();

	void setDebugDrawMode(int mode);

	btRigidBody* addCompound(std::vector<PhysicsShapeInfo3D>& shapeList, const btVector3& position, const PhysicsMaterial3D& material = PHYSICS_MATERIAL3D_DEFAULT);

	btRigidBody* addConvexHull(std::vector<btVector3>& points, const btVector3& position, bool bPoly = true, const PhysicsMaterial3D& material = PHYSICS_MATERIAL3D_DEFAULT);
	btRigidBody* addConvexHull(const float* floatData, int numPoints, const btVector3& position, bool bPoly = true, const PhysicsMaterial3D& material = PHYSICS_MATERIAL3D_DEFAULT);

	btRigidBody* addTriangleMesh(PhysicsMesh3D* mesh3D, const btVector3& position, const PhysicsMaterial3D& material = PHYSICS_MATERIAL3D_PLANE);

	btRigidBody* addPlane(const btVector3& normal, const btVector3& position, const PhysicsMaterial3D& material = PHYSICS_MATERIAL3D_PLANE);
	btRigidBody* addHeightfieldTerrain(const HeightfieldInfo& fieldInfo, const btVector3& position, const PhysicsMaterial3D& material = PHYSICS_MATERIAL3D_PLANE);

	btRigidBody* addSphere(btScalar radius, const btVector3& position, const PhysicsMaterial3D& material = PHYSICS_MATERIAL3D_DEFAULT);
	btRigidBody* addBox(const btVector3& size, const btVector3& position, const PhysicsMaterial3D& material = PHYSICS_MATERIAL3D_DEFAULT);
	btRigidBody* addCylinder(const btVector3& size, const btVector3& position, const PhysicsMaterial3D& material = PHYSICS_MATERIAL3D_DEFAULT, char xyz = 'y');
	btRigidBody* addCapsule(btScalar radius, btScalar height, const btVector3& position, const PhysicsMaterial3D& material = PHYSICS_MATERIAL3D_DEFAULT, char xyz = 'y');
	btRigidBody* addCone(btScalar radius, btScalar height, const btVector3& position, const PhysicsMaterial3D& material = PHYSICS_MATERIAL3D_DEFAULT, char xyz = 'y');
	btRigidBody* addMultiSphere(btScalar *radi, const btVector3* positions, int numSpheres, const btVector3& position, const PhysicsMaterial3D& material = PHYSICS_MATERIAL3D_DEFAULT);

	void removeCollisionObject(btCollisionObject* collisionObject);

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