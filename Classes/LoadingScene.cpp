//
//  LoadingScene.cpp
//  FlappyBird
//
//  Created by 白菜哥 on 16/9/10.
//
//

#include "LoadingScene.hpp"

LoadingScene::LoadingScene(){}

LoadingScene::~LoadingScene(){}


bool LoadingScene::init(){
    
    if (Scene::init()) {
        return true;
    }
    else{
        return false;
    }
}

void LoadingScene::onEnter(){
    
    auto background = Sprite::create("splash.png");
    Size screenSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
    background->setPosition(origin.x+screenSize.width/2, screenSize.height/2+origin.y);
    this->addChild(background,1);
    
    Director::getInstance()->getTextureCache()->addImageAsync("", CC_CALLBACK_1(LoadingScene::loadingCallBack, this));
}
void LoadingScene::loadingCallBack(Texture2D *tex){
    
    //AtlasNode::getInstance()->loadAtlas("atlas.txt", tex);
}