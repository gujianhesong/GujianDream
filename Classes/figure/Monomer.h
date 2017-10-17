//
//  Monomer.h
//  guyue
//
//  Created by dsh on 15/1/8.
//
//
//人物和怪物父类
#ifndef __guyue__Monomer__
#define __guyue__Monomer__

#include "Figure.h"
#include "AttackSkillSystem.h"
#include "FlutteringFairy.h"
#include "MapPoint.h"
#include "cocos-ext.h"

USING_NS_CC_EXT;
USING_NS_CC;


#define TAG_MOVET 0xfffffff1
#define TAG_FOLLOWATTACK 0xfffffff2
#define TAG_COOLINGTIMEATTACK 0xfffffff3
#define TAG_MOVETWAIT 0xfffffff4

typedef struct MoveInfo
{
    float time;
    
    MapPoint point;
    
    bool isCanNotFineTheWay;
    
}MoveInfo;

class Monomer
: public Sprite
, public FigureDelegate
{
    
protected:

	string m_nickName;
    
    TexturePathType m_texturePathType;
    
    unsigned int m_currHairNumber;
    
    unsigned int m_currWeaponNumber;
    
    unsigned int m_currEffectID;
    
    bool m_isCooling;
    
    AttackSkillInfo* m_attackSkillInfo;
    
    float m_fReactionInterval;
    
    std::set<Monomer*>* m_againstMeSet;
    
    std::map<unsigned int, AttackSkillInfo*>* m_attackSkillMap;
    
    std::deque<FlutteringFairy*>* m_flutteringFairyDeque;

    
    CC_SYNTHESIZE_READONLY(Figure*, m_nMonomer, Figure);
    
    CC_SYNTHESIZE_READONLY(FigureStateType, m_stateType, StateType);
    
    CC_SYNTHESIZE_READONLY(FigureDirectionType, m_directionType, DirectionType);

	CC_SYNTHESIZE(int, m_currRoleNumber, RoleNumber);
    
    CC_SYNTHESIZE(float, m_isCanMoved, CanMoved);
    
    CC_SYNTHESIZE(M_INT, m_runSpeed, RunSpeed);
    
    CC_SYNTHESIZE(M_INT, m_visualRange, VisualRange);
    
    CC_SYNTHESIZE(int, m_bloodCap, BloodCap);
    
    CC_SYNTHESIZE(int, m_magicCap, MagicCap);
    
    CC_SYNTHESIZE(int, m_blood, Blood);
    
    CC_SYNTHESIZE(int, m_magic, Magic);
    
    CC_SYNTHESIZE(int, m_pTheAttack, TheAttack);
    
    CC_SYNTHESIZE(float, m_attackTime, AttackTime);

    //我攻击的
    CC_SYNTHESIZE(Monomer*, m_attackMonomerMajor, AttackMonomerMajor);

    
public:
    
    Monomer();
    
    virtual ~Monomer();
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExitTransitionDidStart();
    
    bool init();

	string getNickName();

	void setNickName(string nickName);
    
    void setFigure(TexturePathType type, unsigned int roleNumber);
    
    void setHair(unsigned int hairNumber);
    
    void setWeapon(unsigned int weaponNumber);

    void addAttackSkill(unsigned int skillNumber);
    
    void subAttackSkill(unsigned int skillNumber);
    
    void removeAgainstMe(Monomer* var);
    
    void removeAttackMonomerMajor(Monomer* var);
    
    void updateVertexZ(float delay = 0);
    
    void updateBloodProgress();
    
    void followAttackAndSetAttackMethods(Monomer* otherEnemy, int attackSkillNumber = 0);
    
    MapPoint ownAttackPoint(Monomer* var, M_INT attackRange);
    
    void standAndWatch(MapPoint point);
    
    bool isMoveRunning();
    
    Sprite* m_bloodBg;
    
    Sprite* bloodIn;

	Label* label_nickname;

	Scale9Sprite* m_bg;//背景
    
public:
    
    virtual void stand();
    
    virtual void hurt();
    
    virtual void death();
    
    virtual void addAgainstMe(Monomer* var, float blood);
    
    virtual void bloodReturn(float addBlood);
    
    virtual MoveInfo runBy(MapPoint point);
    
    virtual MoveInfo walkBy(MapPoint point);
    
    virtual MoveInfo goTo(MapPoint point);
    
public:
    
    void walkLeft();
    
    void walkRight();
    
    void walkUp();
    
    void walkDown();
    
    void walkLeftAndUp();
    
    void walkUpAndRight();
    
    void walkRightAndDown();
    
    void walkDownAndLeft();
    
    void runLeft();
    
    void runRight();
    
    void runUp();
    
    void runDown();
    
    void runLeftAndUp();
    
    void runUpAndRight();
    
    void runRightAndDown();
    
    void runDownAndLeft();
    
public:
    
    Point getHandPointRelativeFootOffset();
    
    Point getHandPoint();
    
    Point getHurtPointRelativeFootOffset();
    
    Point getHurtPoint();
    
    Monomer* getAgainstMeOfFirst();
    
    Rect getRect();
    
    int getAlphaWithPoint(Point point);
    
    virtual void showFigure();
    
    virtual void hideFigure();
    
protected:
    
    void playEffect(Node* node, const char* path);
    
    virtual void detectionWhetherCounter();
    
    virtual void detectionOfEachOther();
    
    virtual void setFigureState(FigureStateType stateType, FigureDirectionType directionType);
    
    void pushFlutteringFairy(FlutteringFairy* fairy);
    
    void flutteringFairyFinish();
    
    void scheduleUpdateVertexZ();
    
    void unscheduleUpdateVertexZ();
    
    CCArray* actionsWithPoint(MapPoint& pStart, MapPoint& pFinish);
    
    CCArray* actionsWithMoveTo(std::deque<MapPoint>& pArray);
    
    bool isInTheAttackRange(Monomer* var);
    
    bool isInTheFieldOfView(Monomer* var);
    
    virtual void moveByBegin();
    
    virtual void moveByEnd();
    
    virtual void goBegin();
    
    virtual void goEnd();
    
    virtual bool attackEnemy();
    
    virtual void followAttack();
    
    virtual void followTheTracks();
    
    virtual void beyondVisualRange();
    
    virtual void clearData();
    
    Action* delayCallBack(float time, SEL_CallFunc callFunc);
    
    FigureDirectionType getDirectionType(float angle);
    
    void setCoolingFalse();
    
protected:
    
    virtual void attacking();
    
    virtual void attackCompleted();
    
    virtual void underAttack();
    
    virtual void deathActionFinish();

	virtual void onContentSizeChanged();
};


#endif /* defined(__guyue__Monomer__) */
