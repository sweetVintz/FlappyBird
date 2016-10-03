#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <Box2D/Box2D.h>
#include "AlertView.hpp"

USING_NS_CC;

#define RATIO 48.0f
class HelloWorld : public Layer ,public b2ContactListener
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    
    virtual void update(float dt);
    virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event);
    
    virtual void BeginContact(b2Contact* contact);
    
    void doCleanUp();
    
    AlertView *alertView;
    
    float birdUpForce;      //小鸟向上的力
    float offset;           //竖条上下的偏移量
    float addBarInterval;   //创建竖条的时间
    
    b2Vec2 barSpeed;        //竖条横向速度
    bool isStartGame;
    bool isGameOver;
    bool isPause;
    
    int nPresentScore;       //当前的得分
    int nTotalScore;         //全部得分
    
    Sprite *pPresentScoreGe;
    Sprite *pPresentScoreShi;
    Sprite *pPresentScoreBai;
    Sprite *pScoreContainer;
    
    
    
    Sprite *pTotalScoreGe;
    Sprite *pTotalScoreShi;
    Sprite *pTotalScoreBai;

    Sprite *pMedal;
    
    MenuItemSprite *pause;
    b2World *world;
    Size screenSize;
    b2Body *birdBody;
    
    Sprite *pBird;
    
    Sprite *barContainer;
    
private:
    
    void initUI();
    void initWorld();
    
    void addBird();
    void addground();
    void addBar(float dt);
    
    void readyToStart();
    
    void gameStart();
    void gamePause();
    void gameOver();
    
    void updateScore();
    void updateTotalScore();
};

#endif // __HELLOWORLD_SCENE_H__
