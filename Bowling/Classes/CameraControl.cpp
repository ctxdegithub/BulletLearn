#include "CameraControl.h"
#include "Camera3D.h"
USING_NS_CC;

bool CameraControl::init()
{
	if (!Layer::init())
	{
		return false;
	}

	_touchListener = EventListenerTouchAllAtOnce::create();
	_touchListener->onTouchesBegan = CC_CALLBACK_2(CameraControl::onTouchesBegan, this);
	_touchListener->onTouchesMoved = CC_CALLBACK_2(CameraControl::onTouchesMoved, this);
	_touchListener->onTouchesEnded = CC_CALLBACK_2(CameraControl::onTouchesEnded, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_touchListener, this);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	_keyListener = EventListenerKeyboard::create();
	_keyListener->onKeyPressed = CC_CALLBACK_2(CameraControl::onKeyPressed, this);
	_keyListener->onKeyReleased = CC_CALLBACK_2(CameraControl::onKeyReleased, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_keyListener, this);
	_moveDir.x =_moveDir.y = 0;
	_rotDir.x = _rotDir.y = 0;
#endif

	this->scheduleUpdate();

	return true;
}

void CameraControl::onExit()
{
	Layer::onExit();
	Director::getInstance()->getEventDispatcher()->removeEventListener(_touchListener);
}

void CameraControl::onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event)
{
	auto size = Director::getInstance()->getVisibleSize();
	auto pos = touches[0]->getLocation();

	if (touches.size() == 1)
	{
		if (pos.x < size.width * 0.5f)
		{
			_moveTouch.lastPos = pos;
			_moveTouch.id = touches[0]->getID();
		}
		else
		{
			_rotTouch.id = touches[0]->getID();
		}
	}

	if (touches.size() == 2)
	{
		for (auto touch : touches)
		{
			pos = touch->getLocation();
			if (pos.x < size.width * 0.5f)
			{
				if (_rotTouch.id != touch->getID() && _moveTouch.id == -1)
				{
					_moveTouch.lastPos = pos;
					_moveTouch.id = touch->getID();
				}
			}
			else if (_moveTouch.id != touch->getID() && _rotTouch.id == -1)
			{
				_rotTouch.id = touch->getID();
			}
		}

	}
}

void CameraControl::onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event)
{
	auto size = Director::getInstance()->getVisibleSize();	

	// touch two
	if (touches.size() <= 2)
	{
		for (auto touch : touches)
		{
			auto pos = touch->getLocation();
			if (touch->getID() == _moveTouch.id)
			{
				_moveTouch.curPos = pos;
				_moveTouch.bChange = true;
			}
			else if (touch->getID() == _rotTouch.id)
			{
				auto rot = _camera->getRotation3D();
				rot.y += (pos.x - touch->getPreviousLocation().x) * 2.f / size.width * 45.f;
				rot.x += (pos.y - touch->getPreviousLocation().y) * 2.f / size.height * 45.f;
				_camera->setRotation3D(rot);
			}
		}
		
	}
}

void CameraControl::onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event)
{

	for (auto touch : touches)
	{
		if (touch->getID() == _moveTouch.id)
		{
			_moveTouch.id = -1;
			_moveTouch.bChange = false;
		}
		else if (touch->getID() == _rotTouch.id)
		{
			_rotTouch.id = -1;
		}
	}
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
void CameraControl::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_W:
		_moveDir.y = 1;
		break;
	case EventKeyboard::KeyCode::KEY_S:
		_moveDir.y = -1;
		break;
	case EventKeyboard::KeyCode::KEY_D:
		_moveDir.x = 1;
		break;
	case EventKeyboard::KeyCode::KEY_A:
		_moveDir.x = -1;
		break;
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		_rotDir.x = 1;
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		_rotDir.x = -1;
		break;
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		_rotDir.y = 1;
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		_rotDir.y = -1;
		break;
	case EventKeyboard::KeyCode::KEY_I:
		_camera->setMoveSpeed(_camera->getMoveSpeed() + 0.1f);
		break;
	case EventKeyboard::KeyCode::KEY_O:
		_camera->setMoveSpeed(_camera->getMoveSpeed() - 0.1f);
		break;
	case EventKeyboard::KeyCode::KEY_T:
		this->changeCameraType();
		break;
	default:
		break;
	}
}

void CameraControl::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_W:
	case EventKeyboard::KeyCode::KEY_S:
		_moveDir.y = 0;
		break;		
	case EventKeyboard::KeyCode::KEY_D:
	case EventKeyboard::KeyCode::KEY_A:
		_moveDir.x = 0;
		break;	
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		_rotDir.x = 0;
		break;
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		_rotDir.y = 0;
		break;
	default:
		break;
	}
}
#endif

void CameraControl::changeCameraType()
{
	if (_camera->getCameraType() == Camera3D::LAND)
	{
		_camera->setCameraType(Camera3D::AIRCRAFT);
	}
	else
	{
		_camera->setCameraType(Camera3D::LAND);
	}
}

void CameraControl::update(float delta)
{
	auto size = Director::getInstance()->getVisibleSize();
	if (_moveTouch.bChange)
	{
		_camera->updatePosition(_moveTouch.curPos - _moveTouch.lastPos, delta);
	}


#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	_camera->updatePosition(_moveDir, delta);
	auto rot = _camera->getRotation3D();
	rot.x += _rotDir.x * 2.f;
	rot.y += _rotDir.y * 2.f;
	_camera->setRotation3D(rot);
	
#endif
}