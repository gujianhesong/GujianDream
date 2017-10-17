//
//  Figure.h
//  guyue
//
//  Created by dsh on 15/1/8.
//
//
/*
 * 段生辉 20150108
 */
#ifndef __guyue__Figure__
#define __guyue__Figure__

#include "cocos2d.h"
#include "TextureController.h"
#include "cocos-ext.h"
#include <map>

USING_NS_CC_EXT;
USING_NS_CC;

#define TAG_ANIMATE 0xfffffff0
#define TAG_ANIMATE_WEAPON 0xfffffff1


//方向
typedef enum
{
	FDirRightAndDown    = 0,
    FDirDownAndLeft     = 1,
	FDirLeftAndUp       = 2,
    FDirUpAndRight      = 3,
	FDirDown			= 4,
	FDirLeft            = 5,
	FDirUp              = 6,
	FDirRight           = 7,
    FDirNone            = 8,
}FigureDirectionType;
//状态
typedef enum
{
    FStateDeath     = 6,     //死亡
    FStateHurt      = 5,     //受伤
    FStateCaster    = 4,     //施法
    FStateAttack    = 3,     //攻击
    FStateRun       = 2,     //跑
    FStateWalk      = 1,     //走
    FStateStand     = 0,     //站着
    FStateNone      = 9
}FigureStateType;

//武器类型
typedef enum
{
	WeaponNone		= 0,	//无
	WeaponJian      = 101,  //剑
	WeaponDao,				//刀
	WeaponShan,				//扇
	WeaponQiang,			//枪
	WeaponFutou,			//斧
	WeaponShuangJian,       //双剑
	WeaponPiaodai,			//飘带
	WeaponZhuaci,			//爪刺
	WeaponChui,				//锤
	WeaponBian,				//鞭
	WeaponHuanquan,			//环圈
	WeaponBang,				//棒
	
}WeaponType;

class Figure;
//接口
class FigureDelegate
{
public:
    //攻击中
    virtual void attacking() = 0;
    //攻击完成
    virtual void attackCompleted() = 0;
    //受到攻击
    virtual void underAttack() = 0;
    //死亡
    virtual void deathActionFinish() = 0;
	//size发生变化
	virtual void onContentSizeChanged() = 0;

    //删除Delegate
    void removeDelegate(Figure* var);
};
//角色
class Figure: public Sprite
{
    
    float m_frameRate;//等级
    
    unsigned int m_figureNumber;//编号
    
    unsigned int m_hairNumber;//头像
    
    unsigned int  m_weaponNumber;//武器
    
    TexturePathType m_pathType; //纹理路径
    
    FigureDelegate* m_pDelegate;//代理

	cocos2d::Map<FigureStateType, Animate*> animateMap;

	Scale9Sprite* m_bg;//背景

	Sprite* m_weaponSprite;//武器

	Sprite* m_bodySprite;//身体

	Sprite* m_shadow;//阴影

	Size m_contentSize;

	std::map<int, Point> role_texture_pos_map;
	std::map<int, Point> weapon_texture_pos_map;
    
public:
    
    Figure(TexturePathType type, unsigned int figureNumber);
    
    virtual ~Figure();
    
    virtual void onEnter();
    
    virtual void onExit();
    
    static Figure* create(TexturePathType type, unsigned int figureNumber);
    
    bool init();
    
    void setWeapon(unsigned int weaponNumber);
    
    void setFigureState(FigureStateType stateType, FigureDirectionType directionType);
    
    void setDelegate(FigureDelegate* delegate);
    //状态
    CC_SYNTHESIZE_READONLY(FigureStateType, m_stateType, StateType);
    //方向
    CC_SYNTHESIZE_READONLY(FigureDirectionType, m_directionType, DirectionType);
    //武器
    CC_SYNTHESIZE_READONLY(Sprite*, m_weaponSprite1, WeaponSprite);
    //脚下－－点
    Point getAnchorPointWithFoot();
    //头－－点
    Point getAnchorPointWithHand();
    //中间－－点
    Point getAnchorPointCenter();
    //透明度
    void setOpacity(GLubyte opacity);
    
private:

	void updateFigure();
    
    void runActions();

	void runWeaponActions();
    
    ActionInterval* getRunActionsFromSprite(std::string& path);

	ActionInterval* getWeaponActions(std::string& path);

    //------------接口方法------
    void attackCompleted();
    
    void hurtCompleted();
    
    void deathCompleted();
    
    void attacking();

	void initBodyContentSize(std::string& path);

	void initWeaponContentSize(std::string& path);

	inline int get4Direction(){
		
		int direction = 0;
		switch (m_directionType)
		{
		case FDirRight:
			direction = FDirRightAndDown;
			break;
		case FDirDown:
			direction = FDirDownAndLeft;
			break;
		case FDirLeft:
			direction = FDirLeftAndUp;
			break;
		case FDirUp:
			direction = FDirUpAndRight;
			break;
		default:
			break;
		}

		return direction;
	}

	inline bool is4Direction(string& path){

		std::string fullPath = StringUtils::format("%s_%d_%d_%d.png", path.c_str(), m_stateType, 7, 1);
		
		SpriteFrame* spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(fullPath);

		if(spriteFrame){
			return false;
		}

		return true;
	}

	inline bool is4DirectionWeapon(string& path){

		std::string fullPath = StringUtils::format("%s_%d_%d_%d.png", path.c_str(), m_stateType, 7, 1);

		log("--------------------ffff--%s", fullPath.c_str());

		SpriteFrame* spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(fullPath);

		if(spriteFrame){
			return false;
		}

		return true;
	}

	inline Size getFrameSize(string& path){

		std::string fullPath = StringUtils::format("%s_%d_%d_%d.png", path.c_str(), m_stateType, 0, 1);

		SpriteFrame* spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(fullPath);

		if(spriteFrame){
			return spriteFrame->getOriginalSize();
		}

		return Size::ZERO;
	}

	inline Size getFrameSizeWeapon(string& path){
		std::string fullPath = StringUtils::format("%s_%d_%d_%d.png", path.c_str(), m_stateType, 0, 1);

		SpriteFrame* spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(fullPath);

		if(spriteFrame){
			return spriteFrame->getOriginalSize();
		}

		return Size::ZERO;
	}
    
};



#endif /* defined(__guyue__Figure__) */
