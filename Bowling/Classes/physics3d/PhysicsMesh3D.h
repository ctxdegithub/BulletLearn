#ifndef __PHYSICS_MESH_3D_H__
#define __PHYSICS_MESH_3D_H__
#include "Bullet/btBulletDynamicsCommon.h"
#include "cocos2d.h"
USING_NS_CC;

class PhysicsMesh3D
{
public:
	static PhysicsMesh3D* constuct(const char* fileName);
	
	void destroy();
	bool initWithFile(const char* fileName);

private:
	std::vector<float> _verts;
	std::vector<int> _verIndices;
	btTriangleIndexVertexArray* _indexVertexArrays;
	CC_SYNTHESIZE_READONLY(btBvhTriangleMeshShape*, _meshShape, MeshShape);
};

#endif