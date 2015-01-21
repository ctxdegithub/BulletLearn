#include "MainLayer.h"
#include "HelloWorldScene.h"
#include "ComplexTest.h"

Scene* MainLayer::createScene()
{
	auto scene = Scene::create();
	auto mainLayer = MainLayer::create();
	scene->addChild(mainLayer);

	return scene;
}

bool MainLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	auto size = Director::getInstance()->getVisibleSize();

	// hello world
	auto lblHello = Label::createWithSystemFont("HelloWorld", "Arial", 24);
	auto menuHello = MenuItemLabel::create(lblHello, CC_CALLBACK_1(MainLayer::menuHelloWorld, this));
	menuHello->setPosition(Vec2(size.width * 0.5f, size.height * 0.9f));

	// force impulse
	auto lblFI = Label::createWithSystemFont("ComplexTest", "Arial", 24);
	auto menuFI = MenuItemLabel::create(lblFI, CC_CALLBACK_1(MainLayer::menuComplexTest, this));
	menuFI->setPosition(Vec2(size.width * 0.5f, size.height * 0.8f));

	// close
	auto lalClose = Label::createWithSystemFont("Close", "Arial", 24);
	auto menuClose = MenuItemLabel::create(lalClose, CC_CALLBACK_1(MainLayer::menuClose, this));
	menuClose->setPosition(Vec2(size.width * 0.95f, size.height * 0.95f));

	// menu
	auto menu = Menu::create(menuHello, menuFI, menuClose, nullptr);
	this->addChild(menu);
	menu->setPosition(Vec2::ZERO);

	return true;
}

void MainLayer::menuHelloWorld(Ref* sender)
{
	auto layer = HelloWorld::create();
	auto helloScene = HelloWorld::createScene(layer);
	Director::getInstance()->replaceScene(helloScene);
}

void MainLayer::menuComplexTest(Ref* sender)
{
	auto layer = ComplexTest::create();
	auto complexTest = ComplexTest::createScene(layer);
	Director::getInstance()->replaceScene(complexTest);
}

void MainLayer::menuClose(Ref* sender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
	return;
#endif

	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}