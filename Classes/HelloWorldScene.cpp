#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "LoadingScene.hpp"
#include "AlertView.hpp"


Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
//    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);

    screenSize = Director::getInstance()->getVisibleSize();
    isStartGame = false;
    isGameOver = false;
    isPause = false;
    this->setTouchEnabled(true);
    
    
    initUI();
    initWorld();
    addBird();
    addground();
    
    return true;
}

#pragma mark 初始化游戏界面固定元素
void HelloWorld::initUI(){
    
    nTotalScore = UserDefault::getInstance()->getIntegerForKey("totalScore");
    
    birdUpForce = 5.0;
    addBarInterval = 1.2f;
    barSpeed = b2Vec2(-3, 0);
    
    nPresentScore = 0;
    
    barContainer = Sprite::create();
    addChild(barContainer,1);
    
    pScoreContainer = Sprite::create();
    addChild(pScoreContainer,2);
    
    alertView = AlertView::getInstance();
    alertView->gameTutorial();
    addChild(alertView,3);
    
    auto background = Sprite::create("background.png");
    background->setPosition(screenSize.width/2,screenSize.height/2);
    addChild(background);
    
    auto normal = Sprite::create("button_pause.png");
    pause = MenuItemSprite::create(normal,
                                   normal,
                                   CC_CALLBACK_0(HelloWorld::gamePause, this));
    pause->setPosition(pause->getContentSize().width,screenSize.height-pause->getContentSize().height);
    auto *menu = Menu::create(pause,NULL);
    menu->setPosition(Vec2::ZERO);
    addChild(menu,2);
    
    //const char str = (char)nPresentScore;
    
    pPresentScoreGe = Sprite::create(__String::createWithFormat("font_%d.png",0)->getCString());
    pPresentScoreGe->setPosition(screenSize.width/2,screenSize.height/2+150);
    pScoreContainer->addChild(pPresentScoreGe);
    
    pPresentScoreShi = Sprite::create();
    pScoreContainer->addChild(pPresentScoreShi);
    
    pPresentScoreBai = Sprite::create();
    pScoreContainer->addChild(pPresentScoreBai);
    
    
    pTotalScoreGe = Sprite::create(__String::createWithFormat("font_%d.png",0)->getCString());
    addChild(pTotalScoreGe,2);
    
    pTotalScoreShi = Sprite::create();
    addChild(pTotalScoreShi,2);
    
    pTotalScoreBai = Sprite::create();
    addChild(pTotalScoreBai,2);
    
    pMedal = Sprite::create("medals_1.png");
    addChild(pMedal,2);
    
    
    updateTotalScore();
}

#pragma mark 初始化物理世界
void HelloWorld::initWorld(){

    //设置重力为10；
    world = new b2World(b2Vec2(0, -10.0f));
    world->SetContactListener(this);
    
}

#pragma mark 初始化游戏主角——小鸟
void HelloWorld::addBird(){

    auto birdAni = Animation::create();
    birdAni->addSpriteFrameWithFile("bird_blue_0.png");
    birdAni->addSpriteFrameWithFile("bird_blue_1.png");
    birdAni->addSpriteFrameWithFile("bird_blue_2.png");
    birdAni->setDelayPerUnit(0.1f);
    birdAni->setRestoreOriginalFrame(true);
    
    pBird = Sprite::create("bird_blue_1.png");
    pBird->runAction(RepeatForever::create(Animate::create(birdAni)));
    
    pBird->setPosition(Vec2(screenSize.width/2-50, screenSize.height/2));
    pBird->setScale(0.8);
    addChild(pBird);
    Size birdSize = pBird->getContentSize();
    
    b2BodyDef bodyDef;               //设置对象的物理定义
    bodyDef.type = b2_dynamicBody;   //设置对象为动态物体
    bodyDef.position.Set(pBird->getPosition().x/RATIO,pBird->getPosition().y/RATIO);
    bodyDef.userData = pBird;         //把精灵附给在刚体上
    
    b2PolygonShape birdShape;        //设置对象的多边形形状
    birdShape.SetAsBox(birdSize.width*pBird->getScale()/2/RATIO, birdSize.height*pBird->getScale()/2/RATIO);
    
    b2FixtureDef birdFixtureDef;     //定义夹具
    birdFixtureDef.density=0;      //设置密度
    birdFixtureDef.friction=0.0;   //设置摩擦力
    birdFixtureDef.restitution=0;  //反弹力？
    birdFixtureDef.shape = &birdShape;
    
    birdBody = world->CreateBody(&bodyDef);
    birdBody->CreateFixture(&birdFixtureDef);
//    birdBody->SetAwake(false);    //设置刚体睡眠，但当与其他刚体碰撞的时候将被唤醒，如果附加在物体上的连接器和接触被销毁的时候它们也会醒来
//    birdBody->SetGravityScale(10);
    
}

#pragma mark 初始化陆地
void HelloWorld::addground(){

    auto ground = Sprite::create("land.png");
    Size landSize = ground->getContentSize();
    ground->setPosition(Vec2(screenSize.width/2, landSize.height/2));
    addChild(ground,2);
    
    b2BodyDef groundBodyDef;
    groundBodyDef.type = b2_staticBody;
    groundBodyDef.position.Set(ground->getPositionX()/RATIO, ground->getPositionY()/RATIO);
    groundBodyDef.userData = ground;
    
    b2PolygonShape groundShape;
    groundShape.SetAsBox(landSize.width, landSize.height/2/RATIO);
    
    b2FixtureDef groundFixtureDef;
    groundFixtureDef.shape = &groundShape;
    
    b2Body *groundBody = world->CreateBody(&groundBodyDef);
    groundBody->CreateFixture(&groundFixtureDef);
    
}

#pragma mark 初始化障碍物
void HelloWorld::addBar(float dt){
    
    
    float spriteScale = 0.75;    //精灵的缩放
    float offset = rand()%5;
    
    // 加载下方柱子
    
    auto downBar = Sprite::create("bound_down.png");
    Size downBarSize = downBar->getContentSize();
    downBar->setScale(spriteScale);
    downBar->setPosition(screenSize.width,downBarSize.height*spriteScale+offset*30-300);
    barContainer->addChild(downBar);
    
    b2BodyDef downBarBodyDef;
    downBarBodyDef.type = b2_kinematicBody;
    downBarBodyDef.position.Set(downBar->getPositionX()/RATIO, downBar->getPositionY()/RATIO);
    downBarBodyDef.userData = downBar;
    downBarBodyDef.linearVelocity = barSpeed;
    
    
    b2PolygonShape downBarShape;
    downBarShape.SetAsBox(downBarSize.width*spriteScale/2/RATIO, downBarSize.height*spriteScale/2/RATIO);
    
    b2FixtureDef downBarFixtureDef;
    downBarFixtureDef.shape = &downBarShape;
    
    b2Body *downBarBody = world->CreateBody(&downBarBodyDef);
    downBarBody->CreateFixture(&downBarFixtureDef);
    
    // 加载上方柱子
    
    auto upBar = Sprite::create("bound_up.png");
    Size upBarSize = upBar->getContentSize();
    upBar->setScale(spriteScale);
    upBar->setPosition(downBar->getPositionX(),downBar->getPositionY()+upBarSize.height*spriteScale+120);
    barContainer->addChild(upBar);
    
    b2BodyDef upBarBodyDef;
    upBarBodyDef.type = b2_kinematicBody;
    upBarBodyDef.position.Set(upBar->getPositionX()/RATIO, upBar->getPositionY()/RATIO);
    upBarBodyDef.userData = upBar;
    upBarBodyDef.linearVelocity = barSpeed;
    
    b2PolygonShape upBarShape;
    upBarShape.SetAsBox(upBarSize.width*spriteScale/2/RATIO, upBarSize.height*spriteScale/2/RATIO);
    
    b2FixtureDef upBarFixtureDef;
    upBarFixtureDef.shape = &upBarShape;
    
    
    b2Body *upBarBody = world->CreateBody(&upBarBodyDef);
    upBarBody->CreateFixture(&upBarFixtureDef);
    upBar->setPosition(upBarBody->GetPosition().x * RATIO,upBarBody->GetPosition().y * RATIO);
    
    // 加载柱子间的金币
    
    auto coin = Sprite::create("medals_1.png");
    coin->setTag(11);
    coin->setPosition(upBar->getPosition().x,upBar->getPositionY()-upBarSize.height*spriteScale/2-60);
    Size coinSize = coin->getContentSize();
    barContainer->addChild(coin,1);
    
    b2BodyDef coinBodyDef;
    coinBodyDef.type = b2_kinematicBody;
    coinBodyDef.position.Set(coin->getPositionX()/RATIO, coin->getPositionY()/RATIO);
    coinBodyDef.userData = coin;
    coinBodyDef.linearVelocity = barSpeed;
    
    b2PolygonShape coinShape;
    coinShape.SetAsBox(coinSize.width*coin->getScale()/2.5/RATIO,coinSize.height*coin->getScale()/2.5/RATIO);
    
    b2FixtureDef coinFixtureDef;
    coinFixtureDef.shape = &coinShape;
    coinFixtureDef.density=0;      //设置密度
    coinFixtureDef.friction=0.0;   //设置摩擦力
    coinFixtureDef.restitution=0;  //反弹力？
    
    b2Body *coinBody = world->CreateBody(&coinBodyDef);
    coinBody->CreateFixture(&coinFixtureDef);
    
}

#pragma mark 游戏准备开始
void HelloWorld::readyToStart(){
    
    alertView->removeAllChildren();
    
    auto readyAni = Animation::create();
    readyAni->addSpriteFrameWithFile("font_3.png");
    readyAni->addSpriteFrameWithFile("font_2.png");
    readyAni->addSpriteFrameWithFile("font_1.png");
    readyAni->setDelayPerUnit(1);
    readyAni->setRestoreOriginalFrame(true);
    
    auto ready = Sprite::create("font_3.png");
    ready->setPosition(screenSize.width/2,screenSize.height/2+50);
    ready->setScale(2);
    addChild(ready,2,111);
    
    Action *readyAct = Animate::create(readyAni);
    
    Vector<FiniteTimeAction *> actVector; //创建一个盛放动画的容器
    actVector.pushBack((FiniteTimeAction *)readyAct);
    actVector.pushBack(CallFuncN::create(CC_CALLBACK_0(HelloWorld::gameStart,this)));
    
    auto sequence = Sequence::create(actVector);//动画序列的传入参数为一个容器（容器内对象类型为：FiniteTimeAction）->参考官方Api－>http://api.cocos.com/cn/d7/d30/classcocos2d_1_1_sequence.html#details
    ready->runAction(sequence);
}

#pragma mark 游戏开始
void HelloWorld::gameStart(){

    isStartGame = true;
    isGameOver = false;
    this->removeChildByTag(111);
    scheduleUpdate();
    schedule(schedule_selector(HelloWorld::addBar), addBarInterval);
}

#pragma mark 更新游戏总得分
void HelloWorld::updateTotalScore(){
    
    if (nTotalScore/100 > 0) {
        
        int ge = nTotalScore%10;
        int shi = nTotalScore%100/10;
        int bai = nTotalScore/100;
        
        pTotalScoreGe->setTexture(__String::createWithFormat("font_%d.png",ge)->getCString());
        pTotalScoreShi->setTexture(__String::createWithFormat("font_%d.png",shi)->getCString());
        pTotalScoreBai->setTexture(__String::createWithFormat("font_%d.png",bai)->getCString());
        
        pTotalScoreGe->setPosition(screenSize.width-pTotalScoreGe->getContentSize().width/2-5,screenSize.height-pTotalScoreGe->getContentSize().height/2-5);
        pTotalScoreShi->setPosition(pTotalScoreGe->getPositionX()-(pTotalScoreGe->getContentSize().width+5),pTotalScoreGe->getPositionY());
        pTotalScoreBai->setPosition(pTotalScoreGe->getPositionX()-(pTotalScoreGe->getContentSize().width+5)*2,pTotalScoreGe->getPositionY());
        pMedal->setPosition(pTotalScoreGe->getPositionX()-pMedal->getContentSize().width/2-(pTotalScoreGe->getContentSize().width+5)*2-15,pTotalScoreGe->getPositionY());
        
    }
    else if (nTotalScore/10 > 0) {
        
        int ge = nTotalScore%10;
        int shi = nTotalScore/10;
        pTotalScoreGe->setTexture(__String::createWithFormat("font_%d.png",ge)->getCString());
        pTotalScoreShi->setTexture(__String::createWithFormat("font_%d.png",shi)->getCString());
        
        pTotalScoreGe->setPosition(screenSize.width-pTotalScoreGe->getContentSize().width/2-5,screenSize.height-pTotalScoreGe->getContentSize().height/2-5);
        pTotalScoreShi->setPosition(pTotalScoreGe->getPositionX()-pTotalScoreGe->getContentSize().width-5,pTotalScoreGe->getPositionY());
        pMedal->setPosition(pTotalScoreGe->getPositionX()-pMedal->getContentSize().width/2-(pTotalScoreGe->getContentSize().width+5)-15,pTotalScoreGe->getPositionY());
    }
    else{
        
        pTotalScoreGe->setTexture(__String::createWithFormat("font_%d.png",nTotalScore)->getCString());
        
        pTotalScoreGe->setPosition(screenSize.width-pTotalScoreGe->getContentSize().width/2-5,screenSize.height-pTotalScoreGe->getContentSize().height/2-5);
        pMedal->setPosition(pTotalScoreGe->getPositionX()-pMedal->getContentSize().width/2-pTotalScoreGe->getContentSize().width/2-15,pTotalScoreGe->getPositionY());
    }
}

#pragma mark 游戏暂停
void HelloWorld::gamePause(){
    
//    if (isPause == false) {
//        
//        isPause = true;
//        auto normal = Sprite::create("button_resume.png");
//        pause->setNormalImage(normal);
//        unscheduleUpdate();
//        unschedule(schedule_selector(HelloWorld::addBar));
//
//    }
//    else{
//        
//        isPause = false;
//        auto normal = Sprite::create("button_pause.png");
//        pause->setNormalImage(normal);
//        scheduleUpdate();
//        schedule(schedule_selector(HelloWorld::addBar),addBarInterval);
//    }
    
    //调试总得分显示
    UserDefault::getInstance()->setIntegerForKey("totalScore", 0);
}

#pragma mark 游戏结束（失败）
void HelloWorld::gameOver(){

    isStartGame = false;
    isGameOver = true;
    
    //CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_hit.ogg",false,0.5,0.5,0.5);
    UserDefault::getInstance()->setIntegerForKey("totalScore", nTotalScore);

    alertView->gameOver();
    
    unscheduleUpdate();
    unschedule(schedule_selector(HelloWorld::addBar));
    
}

#pragma mark 重新开始游戏时清理场景
void HelloWorld::doCleanUp(){

    barContainer->removeAllChildrenWithCleanup(true);
    barContainer->removeFromParent();
    
    pScoreContainer->removeAllChildrenWithCleanup(true);
    pScoreContainer->removeFromParent();
    
    world->ClearForces();
    world->Dump();
}

#pragma mark 更新当前游戏获得的分数
void HelloWorld::updateScore(){

    nPresentScore++;
    nTotalScore++;
    //CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_point.ogg");
    updateTotalScore();
    if (nPresentScore/100 > 0) {
        
        int ge = nPresentScore%10;
        int shi = nPresentScore%100/10;
        int bai = nPresentScore/100;
        
        log("ge : %d , shi : %d , bai : %d",ge ,shi  ,bai );
        
        pPresentScoreGe->setTexture(__String::createWithFormat("font_%d.png",ge)->getCString());
        pPresentScoreShi->setTexture(__String::createWithFormat("font_%d.png",shi)->getCString());
        pPresentScoreBai->setTexture(__String::createWithFormat("font_%d.png",bai)->getCString());
        
        pPresentScoreGe->setPositionX(screenSize.width/2+pPresentScoreShi->getContentSize().width+5*2);
        pPresentScoreShi->setPosition(screenSize.width/2,pPresentScoreGe->getPositionY());
        pPresentScoreBai->setPosition(pPresentScoreShi->getPositionX()-pPresentScoreShi->getContentSize().width-5*2,pPresentScoreShi->getPositionY());

    }
    else if (nPresentScore/10 > 0) {
        
        int ge = nPresentScore%10;
        int shi = nPresentScore/10;
        pPresentScoreGe->setTexture(__String::createWithFormat("font_%d.png",ge)->getCString());
        pPresentScoreShi->setTexture(__String::createWithFormat("font_%d.png",shi)->getCString());
        
        pPresentScoreGe->setPositionX(screenSize.width/2+pPresentScoreShi->getContentSize().width/2+5);
        pPresentScoreShi->setPosition(screenSize.width/2-pPresentScoreShi->getContentSize().width/2-5,pPresentScoreGe->getPositionY());
    }
    else{
        
        pPresentScoreGe->setTexture(__String::createWithFormat("font_%d.png",nPresentScore)->getCString());
    }
}

#pragma mark 碰撞检测
void HelloWorld::BeginContact(b2Contact* contact){

    b2Body *body_A = contact->GetFixtureA()->GetBody();
    b2Body *body_B = contact->GetFixtureB()->GetBody();
    Sprite *sprite_A = (Sprite *)body_A->GetUserData();
    Sprite *sprite_B = (Sprite *)body_B->GetUserData();
    
    if (sprite_A == pBird && sprite_B->getTag() == 11 ) {
        
        updateScore();
//        sprite_B->removeFromParent();
//        world->DestroyBody(body_B);
        sprite_B->setPosition(-screenSize.width,0);
        body_B->SetAwake(false);
    }
    else if (sprite_A->getTag() == 11 && sprite_B == pBird) {
        
        updateScore();
//        sprite_A->removeFromParent();
//        world->DestroyBody(body_A);
        sprite_A->setPosition(-screenSize.width,0);
        body_A->SetAwake(false);
    }
    else{
    
        //gameOver();
    }
}

#pragma mark 实时刷新游戏对象数据（坐标）
void HelloWorld::update(float dt){

    int positionIterations = 10;
    int velocityIterations = 3;
    
    world->Step(dt, velocityIterations, positionIterations);
    Sprite *sprite;
    
    //sprite->setRotation(-1 * CC_RADIANS_TO_DEGREES(body->GetAngle()));
    
    for (b2Body *body = world->GetBodyList(); body != NULL; body = body->GetNext()){
        
        sprite = (Sprite *) body->GetUserData();

        if (sprite->getPositionX() < -100) {
            
            if (sprite != NULL) {
                
                sprite->removeFromParent();
            }
            world->DestroyBody(body);
        }
        else {
            if (sprite != NULL) {
                
                sprite->setPosition(body->GetPosition().x * RATIO,body->GetPosition().y * RATIO );
                if (sprite == pBird) {
                    if (sprite->getPositionX()<0) {
                        gameOver();
                    }
                }
            }
        }
    }
    
    world->ClearForces();
    world->DrawDebugData();
}

#pragma mark 点击屏幕
void HelloWorld::onTouchesBegan(const std::vector<Touch *> &touches, cocos2d::Event *unused_event){
    
    if (isPause == false) {
        
        if (isStartGame == true && isGameOver == false) {
            
            birdBody->SetLinearVelocity(b2Vec2(0, 5.0));
            //CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("7386.wav",false);
        }
        else if (isStartGame == false && isGameOver == true){
            
        }
        else if(isStartGame == false && isGameOver == false){
            
            isGameOver = true; //傻瓜处理，防止readyToStart()多次调用
            readyToStart();
        }
    }
}
