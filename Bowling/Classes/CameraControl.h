#ifndef __CAMERA_CONTROL_H__
#define __CAMERA_CONTROL_H__

#include "cocos2d.h"

class Camera3D;

class CameraControl : public cocos2d::Layer
{
public:
	CREATE_FUNC(CameraControl);
	bool init();
	void onExit();

	void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event);
	void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event);
	void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
#endif

	void update(float delta);
	void changeCameraType();
private:
	

private:
	struct TouchInfo
	{
		cocos2d::Vec2 lastPos, curPos;
		int id;
		bool bChange;
		TouchInfo():
			lastPos(cocos2d::Vec2::ZERO),
			curPos(cocos2d::Vec2::ZERO),
			id(-1),
			bChange(false)
		{}
	};
	CC_SYNTHESIZE(Camera3D*, _camera, Camera);
	TouchInfo _moveTouch;
	TouchInfo _rotTouch;

	cocos2d::EventListenerTouchAllAtOnce* _touchListener;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	cocos2d::EventListenerKeyboard* _keyListener;
	cocos2d::Vec2 _moveDir;
	cocos2d::Vec2 _rotDir;

#endif
	
};

#endif