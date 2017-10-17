

#include "NpcFigure.h"
//#include "LAlertView.h"
#include "PopupLayer.h"
#include "StringHelper.h"

NpcFigure::NpcFigure(unsigned int roleNumber, NPCDirectionType type)
:m_npcInfo(nullptr)
,m_roleNum(roleNumber)
,m_eTpye(type)
,m_sprite(nullptr)
,m_spriteHigh(nullptr)
,m_shadow(NULL)
,label_nickname(NULL)
,m_nIsDisplay(false)
,m_fSpriteRect(Rect(0, 0, 50, 100))
{
    m_npcInfo = NpcInfoSystem::getNpcInfo(roleNumber);
}

NpcFigure::~NpcFigure()
{
	TextureController::subSpriteFrames(TexturePathNPC, m_npcInfo->getRID());

    m_pArray.clear();
}

void NpcFigure::onEnter()
{
    CCSprite::onEnter();
    
    EventListenerTouchOneByOne * listener=EventListenerTouchOneByOne::create();
    listener->onTouchBegan=CC_CALLBACK_2(NpcFigure::onTouchBegan, this);
    listener->onTouchMoved=CC_CALLBACK_2(NpcFigure::onTouchMoved, this);
    listener->onTouchEnded=CC_CALLBACK_2(NpcFigure::onTouchEnded, this);
    listener->onTouchCancelled=CC_CALLBACK_2(NpcFigure::onTouchCancelled, this);
	listener->setSwallowTouches(true);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void NpcFigure::onExit()
{
    CCSprite::onExit();
    
    //Director::getInstance()->getEventDispatcher()->removeEventListener(this);
    this->hideImageView();
}

NpcFigure* NpcFigure::create(unsigned int roleNumber, NPCDirectionType type)
{
    NpcFigure* npc = new NpcFigure(roleNumber, type);
    if (npc && npc->init())
    {
        npc->autorelease();
        return npc;
    }
    CC_SAFE_DELETE(npc);
    return nullptr;
}

void NpcFigure::displayImageView()
{
    if (m_nIsDisplay == false)
    {
        m_nIsDisplay = true;
        
		TextureController::addSpriteFrames(TexturePathNPC, m_npcInfo->getRID(), this, callfunc_selector(NpcFigure::initWithImageView));

    }
}

void NpcFigure::hideImageView()
{
    if (m_nIsDisplay)
    {
        m_nIsDisplay = false;

        if (m_sprite)
        {
            m_sprite->removeFromParent();
            m_sprite = nullptr;
        }

		if (m_spriteHigh)
        {
            m_spriteHigh->removeFromParent();
			m_spriteHigh = nullptr;
        }
        
		TextureController::subSpriteFrames(TexturePathNPC, m_npcInfo->getRID());

        m_pArray.clear();
    }
}

void NpcFigure::initWithImageView()
{
    if (m_sprite == nullptr)
    {
		CCString* string = CCString::createWithFormat("npc_%u_0_0.png", m_npcInfo->getRID());
        SpriteFrame* spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(string->getCString());

		m_sprite = CCSprite::createWithSpriteFrame(spriteFrame);
		log("---npc------------%f  ,%f-", m_sprite->getContentSize().width, m_sprite->getContentSize().height);
		this->setContentSize(m_sprite->getContentSize());

		m_sprite->setAnchorPoint(Vec2(0.5f, 0));
		m_sprite->setPosition(this->getContentSize().width/2, -15);
        this->addChild(m_sprite, 1);
        
        m_spriteHigh = CCSprite::createWithSpriteFrame(spriteFrame);
        m_spriteHigh->setAnchorPoint(m_sprite->getAnchorPoint());
		m_spriteHigh->setPosition(m_sprite->getPosition());
        this->addChild(m_spriteHigh, 2);
        
        m_sprite->runAction(this->getActions());
        m_spriteHigh->runAction(this->getActions());
        
        m_spriteHigh->setVisible(false);

		m_shadow = Sprite::create("shadow.png");
		m_shadow->setAnchorPoint(Vec2(0.5, 0.5));
		m_shadow->setPosition(Vec2(this->getContentSize().width/2, 0));
		this->addChild(m_shadow, 0);
		m_shadow->setScale(1.0);

		//名称
		Point nickNamePoint = Point(this->getContentSize().width * this->getAnchorPoint().x, -20);
		label_nickname = Label::createWithSystemFont("", "AppleGothic", 15);
		label_nickname->setString(m_npcInfo->getNpcName());
		label_nickname->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
		label_nickname->setPosition(nickNamePoint);
		label_nickname->setColor(Color3B::YELLOW);
		label_nickname->enableOutline(Color4B(124,66,24,255),2);
		this->addChild(label_nickname);

		m_fSpriteRect = Rect(0, 0, this->getContentSize().width, this->getContentSize().height);
    }
}

void NpcFigure::IntelligentDisplay(Rect& showRect, Rect& hideRect)
{
    if (showRect.containsPoint(this->getPosition()))
    {
        this->displayImageView();
    }
    
    if (!hideRect.containsPoint(this->getPosition()))
    {
        this->hideImageView();
    }
}

Action* NpcFigure::getActions()
{
    if (m_pArray.size() == 0)
    {
        int flag = 0;
        do
        {
			CCString* string = CCString::createWithFormat("npc_%u_%d_%d.png", m_npcInfo->getRID(), m_eTpye, flag);
            SpriteFrame* spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(string->getCString());
            CC_BREAK_IF(spriteFrame == nullptr);
            m_pArray.pushBack(spriteFrame);
            flag++;
        }
        while (1);
    }
    
    if (m_pArray.size() > 0)
    {
        Animation* movie = Animation::createWithSpriteFrames(m_pArray, 1 / (float)5);
        Animate* animate = Animate::create(movie);
        RepeatForever* repeatForever = CCRepeatForever::create(animate);
        return repeatForever;
    }

	return nullptr;
}

void NpcFigure::setHighlight()
{
    if (m_spriteHigh->isVisible() == false)
    {
        ccBlendFunc blend = {GL_DST_COLOR, GL_ONE};
        
        m_spriteHigh->setBlendFunc(blend);
        
        m_spriteHigh->setVisible(true);
    }
}

void NpcFigure::setNormal()
{
    if (m_spriteHigh->isVisible() == true)
    {
//        ccBlendFunc blend = {CC_BLEND_SRC, CC_BLEND_DST};
//        m_spriteHigh->setBlendFunc(blend);
        
        m_spriteHigh->setVisible(false);
    }
}

void NpcFigure::setNickName(string& nickname){

	label_nickname->setString(nickname);
}

bool NpcFigure::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	if (m_sprite == nullptr){
		return false;
	}
    
    Point point = pTouch->getLocation();
    point = m_sprite->convertToNodeSpace(point);
    
    if (!m_fSpriteRect.containsPoint(point))
    {
        return false;
    }
    
    this->setHighlight();
    
    return true;
}

void NpcFigure::onTouchMoved(Touch *pTouch, Event *pEvent)
{
    Point point = pTouch->getLocation();
    point = m_sprite->convertToNodeSpace(point);
    
    if (!m_fSpriteRect.containsPoint(point))
    {
        this->setNormal();
    }
    else
    {
        this->setHighlight();
    }
    
}

void NpcFigure::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    Point point = pTouch->getLocation();
    point = m_sprite->convertToNodeSpace(point);
    
    this->setNormal();
    
    PopupLayer* popup = PopupLayer::create("ui/msg_backGround.png");
	popup->setFromName(label_nickname->getString());
    popup->setContentText(m_npcInfo->getSentence().c_str());
    popup->setCallBackFunc(this, callfunc_selector(NpcFigure::buttonCallback));
    popup->addButton(StringHelper::WStrToUTF8(L"确定").c_str(), 0);
    popup->show();
    
}
void NpcFigure::buttonCallback(){
    // 打印 tag 0， 确定，1 ，取消
    //CCLog("button call back. tag: %d", pNode->getTag());
    /*int tag=pNode->getTag();
    if (tag==0) {
        pNode->removeFromParent();
    }*/
}

void NpcFigure::onTouchCancelled(Touch *pTouch, Event *pEvent)
{
    
}

void NpcFigure::alertCallBack(const int number)
{
    if (number == 0)
    {
        
    }
}

Rect NpcFigure::getRect(){
	int off_x = this->getContentSize().width * 0.4f;
	int off_y = this->getContentSize().height * this->getAnchorPoint().y * 0.5f;

	int width = this->getContentSize().width * 0.3f;
	int height = this->getContentSize().height * 0.6f;

	Rect rect;
	rect.origin = Point(off_x, off_y);
	rect.size = Size(width, height);

	int x = this->getContentSize().width * this->getAnchorPoint().x;
	int y = this->getContentSize().height * this->getAnchorPoint().y;

	Point point = Point(x, y);
	point = ccpSub(this->getPosition(), point);

	Rect relustRect = rect;
	relustRect.origin = ccpAdd(point, rect.origin);;

	return relustRect;
}