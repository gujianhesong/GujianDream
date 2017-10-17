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
	//����������ʼ������   
	static AnimateGroup* actionWithAnimation(cocos2d::CCAnimation *animation, Vector<AnimationMember*> &members); 
	//�ö�������������ʼ��
	bool initWithAnimation(cocos2d::CCAnimation *animation, Vector<AnimationMember*> &members);
	//�ó�Ա������ʼ��  
	static AnimateGroup* actionWithAnimation(cocos2d::CCAnimation *animation); 
	//�ö���������������ʼ��  
	bool initWithAnimation(cocos2d::CCAnimation *animation);

	void startWithTarget(cocos2d::CCNode *pTarget);  
	void stop();  //���ж���ֹͣ  

	void update(float dt);  

	Vector<AnimationMember*> _members;  //������Ա  
protected:  

};  

#endif  