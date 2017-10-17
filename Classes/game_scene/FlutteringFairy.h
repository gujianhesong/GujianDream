//
//  FlutteringFairy.h
//  guyue
//
//  Created by dsh on 15/1/8.
//
//

#ifndef __guyue__FlutteringFairy__
#define __guyue__FlutteringFairy__

#include "cocos2d.h"
USING_NS_CC;

typedef enum {
    TypeAddExp,   //加经验
    TypeAddBlood, //加红
    TypeSubBlood, //减红
    TypeSubMagic  //减蓝
}FairyType;

class FlutteringFairy: public Node {
    
    CC_SYNTHESIZE_READONLY(LabelTTF*, m_fairy, Fairy);
    
    void initWithFairy(FairyType type, int value, Ref* target, SEL_CallFunc callfunc);
    
public:
    
    FlutteringFairy();
    
    virtual ~FlutteringFairy();
    
    static FlutteringFairy* addFairy(Node* parent, Point point, FairyType type, int value, Ref* target, SEL_CallFunc callfunc);
    
    virtual const Size& getContentSize() const;
    
};

#endif /* defined(__guyue__FlutteringFairy__) */
