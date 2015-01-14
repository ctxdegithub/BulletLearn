#ifndef __BASE_TEST_H__
#define __BASE_TEST_H__
#include "cocos2d.h"
#include "Camera3D.h"
#include "CameraControl.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class BaseTest : public Layer
{
public:
	static Scene* createScene(Layer* layer);

	bool init();
	void menuClose(Ref* sender);
	void update(float delta);

protected:
	virtual bool initUI();
	virtual bool initCamera();			// 初始化摄像机

protected:
	ui::Text* _labelInfo;
	Camera3D* _camera;	// 摄像机
	CameraControl* _cameraCtrl;
};

#endif