#include "PhysicsShape3D.h"

bool PhysicsShape3D::getShapes(std::vector<int> shapesInfo, std::vector<btCollisionShape*>& shapes)
{
	btCollisionShape* shape = getShapeByInfo();
}