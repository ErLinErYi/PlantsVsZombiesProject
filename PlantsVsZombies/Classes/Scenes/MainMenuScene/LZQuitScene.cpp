﻿/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.7.17
 *Modify: 2020.2.3
 *Email: 2117610943@qq.com
 */

#include "LZQuitScene.h"
#include "../LoadingScene/LZLoadingScene.h"
#include "Based/LZGlobalVariable.h"
#include "Based/LZPlayMusic.h"
#include "Based/LZAppDelegate.h"

#include <powrprof.h>
#pragma comment(lib,"powrprof.lib")

bool QuitMenu::init()
{
	if (!LayerColor::initWithColor(Color4B(0, 0, 0, 180)))return false;
	
	createShieldLayer(this);

	this->createDiglog();

	return true;
}

void QuitMenu::deleteDialog()
{
	setMouseListenerEnable();
	this->removeFromParent();
}

void QuitMenu::createDiglog()
{
	_quitDialog = Sprite::createWithSpriteFrameName("QuitDialog.png");
	_quitDialog->setPosition(Director::getInstance()->getWinSize() / 2);
	_quitDialog->setScale(2.0f);
	this->addChild(_quitDialog);

	this->createButtons(_global->userInformation->getGameText().find("退出游戏")->second->text, Vec2(90, 65), 1);
	this->createButtons(_global->userInformation->getGameText().find("取消")->second->text, Vec2(225, 65), 2);

	/* 创建触摸监听 */
	this->createTouchtListener(_quitDialog);
}

void QuitMenu::createButtons(const std::string &Label, const Vec2 &vec2,const int& ID)
{
	auto button = Button::create("button.png", "button_down.png", "",cocos2d::ui::Widget::TextureResType::PLIST);
	button->setTitleLabel(label(Label, 20));
	button->setPosition(vec2);
	_quitDialog->addChild(button);

	_quitDialog->addChild(label(_global->userInformation->getGameText().find("退出")->second->text, 25, Vec2(155, 160), 0, Color3B::YELLOW));
	_quitDialog->addChild(label(_global->userInformation->getGameText().find("确定要退出游戏吗？")->second->text, 
		_global->userInformation->getGameText().find("确定要退出游戏吗？")->second->fontsize, Vec2(160, 130), 0, Color3B::YELLOW));

	button->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type){
		switch (type)
		{
		case Widget::TouchEventType::BEGAN:
			PlayMusic::playMusic("gravebutton");
			break;
		case Widget::TouchEventType::ENDED:
			switch (ID)
			{
			case 1:quitGame();    break;
			case 2:deleteDialog();break;
			}
		}
	});
}

void QuitMenu::caveTime(const int time)
{
	UserDefault::getInstance()->setIntegerForKey("SUMRUNTIME",time);
}

void QuitMenu::quitGame()
{
	caveTime(getSumRunTime());
	LoadingScene::caveUserFileData();

	PowerSetActiveScheme(nullptr, &scheme_default);
	Director::getInstance()->end();
}

int QuitMenu::getSumRunTime()
{
	time_t tt;
	struct tm* nowtime;
	time(&tt);
	nowtime = localtime(&tt);

	const auto beginday = UserDefault::getInstance()->getIntegerForKey("BEGINDAY");
	const auto beginhour = UserDefault::getInstance()->getIntegerForKey("BEGINHOUR");
	const auto beginmin = UserDefault::getInstance()->getIntegerForKey("BEGINMIN");
	const auto beginsec = UserDefault::getInstance()->getIntegerForKey("BEGINSEC");

	int sumTime = 0;

	sumTime = UserDefault::getInstance()->getIntegerForKey("SUMRUNTIME") +
		(nowtime->tm_mday * 24 * 3600 + nowtime->tm_hour * 3600 + nowtime->tm_min * 60 + nowtime->tm_sec) -
		(beginday * 24 * 3600 + beginhour * 3600 + beginmin * 60 + beginsec);

	return sumTime;
}
