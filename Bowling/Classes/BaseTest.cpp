#include "BaseTest.h"
#include "MainLayer.h"
#include "cocostudio/CocoStudio.h"
using namespace cocostudio::timeline;

Scene* BaseTest::createScene(Layer* layer)
{
	auto scene = Scene::create();
	scene->addChild(layer);

	return scene;
}

bool BaseTest::init()
{
	if (!Layer::init())
	{
		return false;
	}

	this->scheduleUpdate();

	return true;
}

bool BaseTest::initUI()
{
	auto rootNode = CSLoader::createNode("MainScene.csb");
	addChild(rootNode);
	auto closeItem = static_cast<ui::Button*>(rootNode->getChildByName("Button_1"));
	closeItem->addClickEventListener(CC_CALLBACK_1(BaseTest::menuClose, this));

	_labelInfo = static_cast<ui::Text*>(rootNode->getChildByName("lbl_body_info"));

	return true;
}

bool BaseTest::initCamera()
{
	auto size = Director::getInstance()->getVisibleSize();
	_camera = Camera3D::create(60, size.width / size.height, 0.1f, 1000);
	if (_camera == nullptr)
	{
		return false;
	}
	_camera->setCameraFlag(CameraFlag::USER1); // 设置为2
	this->addChild(_camera);
	_camera->setPosition3D(Vec3(0, 2, 0));	// 摄像机位置
	_camera->lookAt(Vec3::ZERO, Vec3(0, 1, 0)); // 摄像机target
	_camera->setMoveSpeed(5.f);

	_cameraCtrl = CameraControl::create();
	_cameraCtrl->setCamera(_camera);

	this->addChild(_cameraCtrl);

	return true;
}

void BaseTest::menuClose(Ref* sender)
{
	auto mainScene = MainLayer::createScene();
	Director::getInstance()->replaceScene(mainScene);
}

void BaseTest::update(float delta)
{

}