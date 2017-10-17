

#ifndef __mir_mobile__BgMapFloorTile__
#define __mir_mobile__BgMapFloorTile__

#include <iostream>
#include "cocos2d.h"
#include "BgMap.h"

#define OFF_SIZE Point(128, 128)

USING_NS_CC;
class BgMap;
class BgMapFloorTile: public cocos2d::Node {
    
protected:
    
    bool m_nIsDisplay;

    Sprite* m_sprite;

	Vector<Node*> m_nodeVector;
    
public:
    
    BgMapFloorTile();
    
    virtual ~BgMapFloorTile();
    
    CREATE_FUNC(BgMapFloorTile);

    CC_SYNTHESIZE_RETAIN(cocos2d::CCString*, m_fileName, FloorTileFileName);

	CC_SYNTHESIZE(BgMap*, m_bgMap, CurrBgMap);
    
    void IntelligentDisplay(Rect& showRect, Rect& hideRect);

protected:
    
    void displayImageView();
    
    void hideImageView();
    
    void initWithImageView(Texture2D* texture);

	void checkValuesEnable();

};


#endif /* defined(__mir_mobile__BgMapFloorTile__) */
