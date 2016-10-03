//
//  AlertView.hpp
//  FlappyBird
//
//  Created by 白菜哥 on 16/9/12.
//
//

#ifndef AlertView_hpp
#define AlertView_hpp

#include <stdio.h>

USING_NS_CC;

class AlertView : public Layer
{
public:
    
    static AlertView * getInstance();
    static void destroyInstance();
    
    void gameTutorial();
    void gameOver();
    void gameAgain();
    
    static AlertView *pAlertView;
    
    Size screenSize;
    
    CREATE_FUNC(AlertView);

protected:
    
    AlertView();
    virtual bool init();
};

#endif /* AlertView_hpp */
