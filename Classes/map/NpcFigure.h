

#ifndef __mir9__NpcFigure__
#define __mir9__NpcFigure__

#include <iostream>
#include "cocos2d.h"
#include "NpcInfoSystem.h"
#include "TextureController.h"

USING_NS_CC;

typedef enum
{
	NPCDirDown            = 0,
	NPCDirLeft            = 1,
	NPCDirRight           = 2,
	NPCDirUp              = 3,
	NPCDirDownAndLeft     = 4,
	NPCDirRightAndDown    = 5,
    NPCDirLeftAndUp       = 6,
	NPCDirUpAndRight      = 7,
    NPCDirNone            = 8,
}NPCDirectionType;

//, public CCTargetedTouchDelegate
class NpcFigure : public Sprite
{
    
    //CCArray* m_pArray;
    Vector<SpriteFrame*>m_pArray;

    NPCDirectionType m_eTpye;
    
    bool m_nIsDisplay;
    
    Sprite* m_sprite;
    
    Sprite* m_spriteHigh;

	Sprite* m_shadow;//阴影

	Label* label_nickname;//名字
    
    Rect m_fSpriteRect;

	int m_roleNum;
    
    CC_SYNTHESIZE_READONLY(NpcInfo*, m_npcInfo, NpcInfo);
    
public:
    
    NpcFigure(unsigned int roleNumber, NPCDirectionType type);
    
    virtual ~NpcFigure();
    
    void onEnter();
    
    void onExit();
    
    static NpcFigure* create(unsigned int roleNumber, NPCDirectionType type);
    
    void IntelligentDisplay(Rect& showRect, Rect& hideRect);
    void buttonCallback();

	void setNickName(string& nickname);

	Rect getRect();
    
protected:
    
    void displayImageView();
    
    void hideImageView();
    
    void initWithImageView();
    
    Action* getActions();
    
    void setHighlight();
    
    void setNormal();
    
    virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
    
    virtual void onTouchMoved(Touch *pTouch, Event *pEvent);
    
    virtual void onTouchEnded(Touch *pTouch, Event *pEvent);
    
    virtual void onTouchCancelled(Touch *pTouch, Event *pEvent);
    
    void alertCallBack(const int number);

	

private:

};

#endif /* defined(__mir9__NpcFigure__) */
