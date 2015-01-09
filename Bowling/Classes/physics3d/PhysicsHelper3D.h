#ifndef __PHYSICS_HELPER_3D_H__
#define __PHYSICS_HELPER_3D_H__
#include <cocos2d.h>
USING_NS_CC;

class PhysicsHelper3D
{
public:
	static std::vector<float> loadRaw(const char* fileName);
	static bool loadRaw(const char* fileName, std::vector<float>& verts);
};

#endif // !__PHYSICS_HELPER_3D_H__

