//
//  AlertView.cpp
//  FlappyBird
//
//  Created by 白菜哥 on 16/9/12.
//
//

#include "AlertView.hpp"
#include "HelloWorldScene.h"

AlertView * AlertView::pAlertView = nullptr;
AlertView * AlertView::getInstance(){

    if (pAlertView == NULL) {
        pAlertView = new AlertView();
        
        if (!pAlertView->init()) {
            
            delete pAlertView;
            pAlertView = NULL;
        }
    }
    return pAlertView;
}

void AlertView::destroyInstance(){

    CC_SAFE_DELETE(pAlertView);
}

AlertView::AlertView(){}


bool AlertView::init(){

    if (!Layer::init()) {
        return false;
    }
    screenSize = Director::getInstance()->getVisibleSize();
    
    return true;
}

void AlertView::gameTutorial(){
    
    
    auto getReady = Sprite::create("text_ready.png");
    Size readySize = getReady->getContentSize();
    getReady->setPosition(screenSize.width/2,screenSize.height/2+readySize.height);
    addChild(getReady);
    
    auto tutorial = Sprite::create("tutorial.png");
    Size tutorialSize = tutorial->getContentSize();
    tutorial->setPosition(getReady->getPositionX(),screenSize.height/2-tutorialSize.height/2+20);
    addChild(tutorial,3);

}

void AlertView::gameOver(){
    
    auto gameOver = Sprite::create("text_game_over.png");
    gameOver->setPosition(screenSize.width/2,screenSize.height/2+50);
    addChild(gameOver);
    
    auto startAgain = MenuItemImage::create("button_play.png",
                                            "button_play.png",
                                            CC_CALLBACK_0(AlertView::gameAgain, this));
    startAgain->setPosition(gameOver->getPositionX(),gameOver->getPositionY()-100);
    auto menu = Menu::create(startAgain, NULL);
    menu->setPosition(Vec2::ZERO);
    addChild(menu);
}

void AlertView::gameAgain(){

    HelloWorld *helloWord = (HelloWorld *)this->getParent();
    helloWord->doCleanUp();
    
    this->removeAllChildrenWithCleanup(true);
    this->removeFromParent();
    
    auto scene = HelloWorld::createScene();
    Director::getInstance()->replaceScene(scene);

}