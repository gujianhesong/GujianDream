//
//  Figure.cpp
//  guyue
//
//  Created by dsh on 15/1/8.
//
//

#include "Figure.h"
#include "PublicDefine.h"


const string TAG_STR_ANIM_BODY = "tag_anim_body";
const string TAG_STR_ANIM_WEAPON = "tag_anim_weapon";

static float getFrameRate(FigureStateType stateType, TexturePathType type)
{
    float frameRate = 1/8.f;
    
    if (type == TexturePathFigure)
    {
		switch (type)
		{
		case FStateStand:
			frameRate = 1/4.f;
			break;
		case FStateWalk:
			frameRate = 0.8f/8.f;
			break;
		case FStateRun:
			frameRate = 0.8f/8.f;
			break;
		case FStateAttack:
			frameRate = 1/4.f;
			break;
		case FStateCaster:
			frameRate = 0.6f/8.f;
			break;
		case FStateHurt:
			frameRate = 1.f/8.f;
			break;
		case FStateDeath:
			frameRate = 1.f/8.f;
			break;
			default:
				break;
		}
		
    }
    
    if (type == TexturePathMonster)
    {
		switch (type)
		{
		case FStateStand:
			frameRate = 1/5.f;
			break;
		case FStateWalk:
			frameRate = 0.6f/8.f;
			break;
		case FStateRun:
			frameRate = 0.6f/8.f;
			break;
		case FStateAttack:
			frameRate = 1/8.f;
			break;
		case FStateCaster:
			frameRate = 0.6f/8.f;
			break;
		case FStateHurt:
			frameRate = 1.f/8.f;
			break;
		case FStateDeath:
			frameRate = 0.6f/8.f;
			break;
		default:
			break;
		}

    }

    return frameRate;
}

void FigureDelegate::removeDelegate(Figure* var)
{
    var->setDelegate(NULL);
}

Figure::Figure(TexturePathType type, unsigned int figureNumber)
:m_stateType(FStateStand)
,m_directionType(FDirDown)
,m_frameRate(0.5f)
,m_pathType(type)
,m_figureNumber(figureNumber)
,m_hairNumber(0)
,m_weaponNumber(0)
,m_bodySprite(NULL)
,m_weaponSprite(NULL)
,m_shadow(NULL)
,m_bg(NULL)
,m_pDelegate(NULL)
{

}

Figure::~Figure()
{
    TextureController::subSpriteFrames(m_pathType, m_figureNumber);
    TextureController::subSpriteFrames(TexturePathWeapon, m_weaponNumber);
}

void Figure::onEnter()
{
    Sprite::onEnter();
}

void Figure::onExit()
{
    Sprite::onExit();
}

Figure* Figure::create(TexturePathType type, unsigned int figureNumber)
{
    Figure* figure = new Figure(type, figureNumber);
    if (figure && figure->init())
    {
        figure->autorelease();
        return figure;
    }
    CC_SAFE_DELETE(figure);
    return NULL;
}

bool Figure::init()
{
    if (!Sprite::init())
        return false;
    
	this->setContentSize(Size(256, 256)* 0.4f);

	//初始化纹理中心点信息
	int row = CsvUtil::getInstance()->findValueInWithLine(StringUtils::format("%d", m_figureNumber), en_role_texture_id, CSVFILE_ROLE_TEXTURE_POS);
	log("position----%d,%d", row, m_figureNumber);
	if(row >= 0){
		
		for(int i=0; i<6; i++){
			int index = row + i;
			int centerPosX = CsvUtil::getInstance()->getInt(index, en_role_texture_center_x, CSVFILE_ROLE_TEXTURE_POS);
			int centerPosY = CsvUtil::getInstance()->getInt(index, en_role_texture_center_y, CSVFILE_ROLE_TEXTURE_POS);
			int stateType = CsvUtil::getInstance()->getInt(index, en_role_texture_state, CSVFILE_ROLE_TEXTURE_POS);
			log("position-=====---%d,%d,%d", stateType, centerPosX, centerPosY);
			role_texture_pos_map.insert(map<int, Point>::value_type(stateType, Point(centerPosX, centerPosY)));
		}
		
	}

	//背景
	m_bg = Scale9Sprite::create("button_1.png");
	m_bg->setOpacity(128);
	m_bg->setAnchorPoint(Vec2(0, 0));
	m_bg->setContentSize(Size(this->getContentSize()));
	//this->addChild(m_bg);

	Point bodyPoint = role_texture_pos_map[m_stateType];
	
	//身体
	m_bodySprite = Sprite::create();
	m_bodySprite->setAnchorPoint(Vec2((m_bodySprite->getContentSize().width - bodyPoint.x)/m_bodySprite->getContentSize().width, 
		(m_bodySprite->getContentSize().height - bodyPoint.y)/m_bodySprite->getContentSize().height));
	m_bodySprite->setPosition(Vec2(this->getContentSize().width/2, 0));
	this->addChild(m_bodySprite, 3);

	//武器
	m_weaponSprite = Sprite::create();
	m_weaponSprite->setPosition(Vec2(this->getContentSize().width/2, 0));
	this->addChild(m_weaponSprite, 8);

	//阴影
	m_shadow = Sprite::create("shadow.png");
	m_shadow->setAnchorPoint(Vec2(0.5, 0.5));
	m_shadow->setPosition(Vec2(this->getContentSize().width/2, 0));
	this->addChild(m_shadow, 0);
	m_shadow->setScale(1.0);

	TextureController::addSpriteFrames(m_pathType, m_figureNumber, this, callfunc_selector(Figure::updateFigure));

    return true;
}

void Figure::setWeapon(unsigned int weaponNumber)
{
	m_weaponNumber = weaponNumber;
	string value = StringUtils::format("%d", m_weaponNumber);

	int row = CsvUtil::getInstance()->findValueInWithLine(value, en_role_texture_id, CSVFILE_WEAPON_TEXTURE_POS);
	if(row >= 0){
		for(int i=0; i<6; i++){
			int index = row + i;
			int centerPosX = CsvUtil::getInstance()->getInt(index, en_role_texture_center_x, CSVFILE_WEAPON_TEXTURE_POS);
			int centerPosY = CsvUtil::getInstance()->getInt(index, en_role_texture_center_y, CSVFILE_WEAPON_TEXTURE_POS);
			int stateType = CsvUtil::getInstance()->getInt(index, en_role_texture_state, CSVFILE_WEAPON_TEXTURE_POS);

			weapon_texture_pos_map.insert(map<int, Point>::value_type(stateType, Point(centerPosX, centerPosY)));
		}

	}
	
	Point weaponPoint = weapon_texture_pos_map[m_stateType];
	m_weaponSprite->setAnchorPoint(Vec2((m_weaponSprite->getContentSize().width - weaponPoint.x)/m_weaponSprite->getContentSize().width, 
		(m_weaponSprite->getContentSize().height - weaponPoint.y)/m_weaponSprite->getContentSize().height));
	
	TextureController::addSpriteFrames(TexturePathWeapon, m_weaponNumber, this, callfunc_selector(Figure::updateFigure));

}

void Figure::setFigureState(FigureStateType stateType, FigureDirectionType directionType)
{
    bool isChange = false;
    
    if (stateType != FStateNone && stateType != m_stateType)
    {
        m_stateType = stateType;
        isChange = true;
    }
    if (directionType != FDirNone && directionType != m_directionType)
    {
        m_directionType = directionType;
        isChange = true;
    }
    
    if (isChange)
    {
        this->updateFigure();
    }
}

void Figure::updateFigure()
{
    m_frameRate = getFrameRate(m_stateType, m_pathType);
    
    runActions();

	if(m_weaponSprite){
		runWeaponActions();
	}
	
}

void Figure::runActions()
{
	m_bodySprite->stopActionByTag(TAG_ANIMATE);

	string figurePath = TextureController::getTexturePath(m_pathType, m_figureNumber);

	initBodyContentSize(figurePath);

    ActionInterval* animate = this->getRunActionsFromSprite(figurePath);
    
	if(!animate) return;

	animate->setTag(TAG_ANIMATE);

    if (m_stateType == FStateAttack)
    {
        DelayTime* delayTime = DelayTime::create(animate->getDuration()/2);
        CallFunc* callFunc1 = CallFunc::create(CC_CALLBACK_0(Figure::attacking, this));
        CallFunc* callFunc2 = CallFunc::create(CC_CALLBACK_0(Figure::attackCompleted, this));
        Sequence* sequence = Sequence::create(delayTime, callFunc1, delayTime, callFunc2, NULL);

        m_bodySprite->runAction(sequence);
        m_bodySprite->runAction(animate);
    }
    else if (m_stateType == FStateCaster)
    {
        DelayTime* delayTime = DelayTime::create(animate->getDuration()/2);
        CallFunc* callFunc1 = CallFunc::create(CC_CALLBACK_0(Figure::attacking,this));
        CallFunc* callFunc2 = CallFunc::create(CC_CALLBACK_0(Figure::attackCompleted,this));
        Sequence* sequence = Sequence::create(delayTime, callFunc1, delayTime, callFunc2, NULL);

        m_bodySprite->runAction(sequence);
        m_bodySprite->runAction(animate);
    }
    else if (m_stateType == FStateHurt)
    {
        CallFunc* callFunc = CallFunc::create(CC_CALLBACK_0(Figure::hurtCompleted,this));
        Sequence* sequence = Sequence::create(animate, callFunc, NULL);

		m_bodySprite->runAction(sequence);
		m_bodySprite->runAction(animate);
    }
    else if (m_stateType == FStateDeath)
    {
        CallFunc* callFunc = CallFunc::create(CC_CALLBACK_0(Figure::deathCompleted,this));
        Sequence* sequence = Sequence::create(animate, callFunc, NULL);

        m_bodySprite->runAction(sequence);
        m_bodySprite->runAction(animate);
    }
    else
    {
        RepeatForever* repeatForever = RepeatForever::create(animate);
        repeatForever->setTag(TAG_ANIMATE);

		m_bodySprite->runAction(repeatForever);
    }
    
}

void Figure::runWeaponActions()
{
	m_weaponSprite->stopActionByTag(TAG_ANIMATE_WEAPON);

	if(m_weaponNumber <= 0){
		return;
	}

	string weaponPath = TextureController::getTexturePath(TexturePathWeapon, m_weaponNumber);
	log("weaponpath-----------%s",weaponPath.c_str());

	initWeaponContentSize(weaponPath);

	ActionInterval* animate = getWeaponActions(weaponPath);

	if(!animate) return;

	if (m_stateType == FStateAttack)
	{
		animate->setTag(TAG_ANIMATE_WEAPON);
		m_weaponSprite->runAction(animate);
	}
	else if (m_stateType == FStateCaster)
	{
		animate->setTag(TAG_ANIMATE_WEAPON);
		m_weaponSprite->runAction(animate);
	}
	else if (m_stateType == FStateHurt)
	{
		animate->setTag(TAG_ANIMATE_WEAPON);
		m_weaponSprite->runAction(animate);
	}
	else if (m_stateType == FStateDeath)
	{
		animate->setTag(TAG_ANIMATE_WEAPON);
		m_weaponSprite->runAction(animate);
	}
	else
	{
		RepeatForever* repeatForever = RepeatForever::create(animate);
		repeatForever->setTag(TAG_ANIMATE_WEAPON);
		m_weaponSprite->runAction(repeatForever);
		
	}

}

ActionInterval* Figure::getRunActionsFromSprite(std::string& path)
{
	int direction = m_directionType;

	if(is4Direction(path)){
		direction = get4Direction();
	}

	string tag = StringUtils::format("%s_%d_%d_%d", TAG_STR_ANIM_BODY.c_str(), m_figureNumber, m_stateType, direction);
	Animation* animation = AnimationCache::getInstance()->getAnimation(tag);

	if(!animation || animation->getFrames().size() == 0){

		Vector<SpriteFrame*> array;
		int flag = 0;
		do
		{
			
			log("figure--------------%d,%d,%d", m_directionType, direction, flag);
			std::string fullPath = StringUtils::format("%s_%d_%d_%d.png",path.c_str(), m_stateType, direction, flag);
			log("figure--------------%s", fullPath.c_str());
			SpriteFrame* spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(fullPath);

			if(flag == 0 && spriteFrame == NULL){

			}else{

				CC_BREAK_IF(spriteFrame == NULL);

				array.pushBack(spriteFrame);
			}

			flag++;
		}
		while (1);

		animation = Animation::createWithSpriteFrames(array, m_frameRate);
		AnimationCache::getInstance()->addAnimation(animation, tag);
	}

    return Animate::create(animation);
}

ActionInterval* Figure::getWeaponActions(std::string& path)
{
	int direction = m_directionType;

	if(is4DirectionWeapon(path)){
		direction = get4Direction();
	}

	string tag = StringUtils::format("%s_%d_%d_%d", TAG_STR_ANIM_WEAPON.c_str(), m_figureNumber, m_stateType, direction);
	Animation* animation = AnimationCache::getInstance()->getAnimation(tag);

	if(!animation || animation->getFrames().size() == 0){
		
		Vector<SpriteFrame*> array;
		int flag = 0;
		do
		{
			
			log("weapon---------------%d,%d,%d", m_directionType, direction, flag);
			std::string fullPath = StringUtils::format("%s_%d_%d_%d.png",path.c_str(), m_stateType, direction, flag);
			log("weapon---------------%s", fullPath.c_str());
			SpriteFrame* spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(fullPath);

			if(flag == 0 && spriteFrame == NULL){

			}else{

				CC_BREAK_IF(spriteFrame == NULL);

				array.pushBack(spriteFrame);
			}

			flag++;

		}
		while (1);

		animation = Animation::createWithSpriteFrames(array, m_frameRate);
		AnimationCache::getInstance()->addAnimation(animation, tag);
	}
	
	return Animate::create(animation);
}

void Figure::attacking()
{
    if (m_pDelegate)
    {
        m_pDelegate->attacking();
    }
}

void Figure::attackCompleted()
{
    if (m_pDelegate)
    {
        m_pDelegate->attackCompleted();
    }
}

void Figure::hurtCompleted()
{
    if (m_pDelegate)
    {
        m_pDelegate->underAttack();
    }
}

void Figure::deathCompleted()
{
    if (m_pDelegate)
    {
        m_pDelegate->deathActionFinish();
    }
}

void Figure::initBodyContentSize(std::string& figurePath)
{
	m_bodySprite->setContentSize(getFrameSize(figurePath));

	if(!(m_contentSize.width > 0 && m_contentSize.height > 0)){

		this->setContentSize(m_bodySprite->getContentSize());

		m_contentSize = m_bodySprite->getContentSize();

		if (m_pDelegate)
		{
			m_pDelegate->onContentSizeChanged();
		}
	}

	Point bodyPoint = role_texture_pos_map[m_stateType];
	if(bodyPoint.x == 0 && bodyPoint.y == 0){
		m_bodySprite->setAnchorPoint(Vec2(0.5f, 18/m_bodySprite->getContentSize().height));
	}else{
		m_bodySprite->setAnchorPoint(Vec2((m_bodySprite->getContentSize().width - bodyPoint.x)/m_bodySprite->getContentSize().width, 
			(m_bodySprite->getContentSize().height - bodyPoint.y)/m_bodySprite->getContentSize().height));
	}
	m_bodySprite->setPosition(Vec2(this->getContentSize().width/2, 0));
	
	m_shadow->setPosition(Vec2(this->getContentSize().width/2, 0));

}

void Figure::initWeaponContentSize(std::string& weaponPath){

	m_weaponSprite->setContentSize(getFrameSizeWeapon(weaponPath));
	Point weaponPoint = weapon_texture_pos_map[m_stateType];
	m_weaponSprite->setAnchorPoint(Vec2((m_weaponSprite->getContentSize().width - weaponPoint.x)/m_weaponSprite->getContentSize().width, 
		(m_weaponSprite->getContentSize().height - weaponPoint.y)/m_weaponSprite->getContentSize().height));
	m_weaponSprite->setPosition(Vec2(this->getContentSize().width/2, 0));

}

void Figure::setDelegate(FigureDelegate* delegate)
{
    m_pDelegate = delegate;
}

Point Figure::getAnchorPointWithFoot()
{
    float x = this->getContentSize().width/2 / this->getContentSize().width;
    //float y = (this->getContentSize().height - 120) / this->getContentSize().height;
	float y = 1 / this->getContentSize().height;
    return Point(x, y);
}

Point Figure::getAnchorPointWithHand()
{
    float x = this->getContentSize().width/2 / this->getContentSize().width;
    float y = (this->getContentSize().height - 64) / this->getContentSize().height;
    return Point(x, y);
}

Point Figure::getAnchorPointCenter()
{
    return Point(0.5, 0.5);
}

void Figure::setOpacity(GLubyte opacity)
{
    Sprite::setOpacity(opacity);
    
    if (m_bodySprite)
    {
        m_bodySprite->setOpacity(opacity);
    }
    
    if (m_weaponSprite)
    {
        m_weaponSprite->setOpacity(opacity);
    }
}