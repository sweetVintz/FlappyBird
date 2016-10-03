//
//  LoadingScene.hpp
//  FlappyBird
//
//  Created by 白菜哥 on 16/9/10.
//
//

#ifndef LoadingScene_hpp
#define LoadingScene_hpp

#include <stdio.h>

USING_NS_CC;

class LoadingScene : public Scene
{
public:
    
    LoadingScene();
    ~LoadingScene();
    
    virtual bool init() override;
    CREATE_FUNC(LoadingScene);
    
    void onEnter() override;
    
private:
    void loadingCallBack(Texture2D *tex);

};

#endif /* LoadingScene_hpp */
