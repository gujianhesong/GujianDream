#include "AnimateGroup.h"

AnimateGroup* AnimateGroup::actionWithAnimation(cocos2d::CCAnimation *animation)  
{  
	AnimateGroup* pRet = new AnimateGroup();  
	if (pRet && pRet->initWithAnimation(animation))  
	{  
		return pRet;  
	}  
	else  
	{  
		delete pRet;  
		pRet = NULL;  
		return pRet;  
	}  
}  

AnimateGroup::AnimateGroup()  
{  
}  

AnimateGroup::~AnimateGroup()  
{  
	_members.clear();  
}  

AnimateGroup* AnimateGroup::actionWithAnimation(cocos2d::CCAnimation *animation, Vector<AnimationMember*> &members)  
{  
    AnimateGroup* pRet = new AnimateGroup();  
    if (pRet && pRet->initWithAnimation(animation, members))  
    {  
        return pRet;  
    }  
    else  
    {  
        delete pRet;  
        pRet = NULL;  
        return pRet;  
    }  
}  
  
bool AnimateGroup::initWithAnimation(cocos2d::CCAnimation *animation, Vector<AnimationMember*> &members)  
{  
    bool bRet = false;  
    do   
    {  
        CC_BREAK_IF(!CCAnimate::initWithAnimation(animation));  
		this->_members = members;  

        bRet = true;  
    } while (0);  
  
    return bRet;  
}  

bool AnimateGroup::initWithAnimation(cocos2d::CCAnimation *animation)  
{  
	bool bRet = false;  

	do   
	{  
		CC_BREAK_IF(!CCAnimate::initWithAnimation(animation));  

		bRet = true;  
	} while (0);  

	return bRet;  
}  

void AnimateGroup::startWithTarget(CCNode *pTarget)  
{  
	CCAnimate::startWithTarget(pTarget);  

	AnimationMember* aniMember = NULL;  
	for(AnimationMember* member : _members)  
	{  
		aniMember = (AnimationMember*)member;  
		aniMember->start();  
	}  
}  

void AnimateGroup::stop()  
{  
	CCAnimate::stop();  

	AnimationMember *aniMember = NULL;  
	for(AnimationMember* member : _members)  
	{  
		aniMember = (AnimationMember *)member;  
		aniMember->stop();  
	}  
}  

void AnimateGroup::update(float dt)  
{  
	CCAnimate::update(dt);  

	int frameIndex = MAX(0, _nextFrame - 1);  

	AnimationMember *aniMember = NULL;  
	for(AnimationMember* member : _members)
	{  
		aniMember = (AnimationMember *)member;  
		aniMember->setFrame(frameIndex);  
	}  
}  