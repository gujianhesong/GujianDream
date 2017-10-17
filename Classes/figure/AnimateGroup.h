#ifndef _AnimateGroup_  
#define _AnimateGroup_  

#include "cocos2d.h"  
#include "AnimationMember.h"

USING_NS_CC;

class AnimateGroup : public cocos2d::Animate  
{  
public:  
	AnimateGroup();  
	~AnimateGroup();  
	//用数组来初始化函数   
	static AnimateGroup* actionWithAnimation(cocos2d::CCAnimation *animation, Vector<AnimationMember*> &members); 
	//用动画和数组来初始化
	bool initWithAnimation(cocos2d::CCAnimation *animation, Vector<AnimationMember*> &members);
	//用成员数来初始化  
	static AnimateGroup* actionWithAnimation(cocos2d::CCAnimation *animation); 
	//用动画和数组数来初始化  
	bool initWithAnimation(cocos2d::CCAnimation *animation);

	void startWithTarget(cocos2d::CCNode *pTarget);  
	void stop();  //所有动画停止  

	void update(float dt);  

	Vector<AnimationMember*> _members;  //动画成员  
protected:  

};  

#endif  