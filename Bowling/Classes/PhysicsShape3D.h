#ifndef __PHYSICS_SHAPE_3D_H__
#define __PHYSICS_SHAPE_3D_H__
#include <vector>
#include "Bullet/btBulletCollisionCommon.h"

class PhysicsShape3D
{
public:
	bool getShapes(std::vector<int> shapesInfo, std::vector<btCollisionShape*>& shapes);

private:
	btCollisionShape* getShapeByInfo();
};

#endif