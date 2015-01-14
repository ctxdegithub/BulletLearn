#ifndef __MAIN_LAYER_H__
#define __MAIN_LAYER_H__

#include "cocos2d.h"
USING_NS_CC;

class MainLayer : public Layer
{
public:
	static Scene* createScene();

	CREATE_FUNC(MainLayer);
	bool init();

	void menuHelloWorld(Ref* sender);
	void menuComplexTest(Ref* sender);

	void menuClose(Ref* sender);

private:


};

#endif