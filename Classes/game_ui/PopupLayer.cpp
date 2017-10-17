//
//  MAlert.cpp
//  guyue
//
//  Created by dsh on 15/1/18.
//
//

#include "PopupLayer.h"
#include "LineNode.h"

USING_NS_CC;


// 构造函数中变量设初值

PopupLayer::PopupLayer()
{

	m_contentPadding = 0;
	m_contentPaddingTop = 0;
	m_pMenu = NULL;
	m_callbackListener = NULL;
	m_callback = NULL;
	m_sfBackGround = NULL;
	m_s9BackGround = NULL;
	m_ltContentText = NULL;
	m_ltTitle = NULL;
	m_ltFromName = NULL;
}

//释放
PopupLayer::~PopupLayer()
{

	CC_SAFE_RELEASE(m_pMenu);
	CC_SAFE_RELEASE(m_sfBackGround);
	CC_SAFE_RELEASE(m_s9BackGround);
	CC_SAFE_RELEASE(m_ltContentText);
	CC_SAFE_RELEASE(m_ltTitle);
	CC_SAFE_RELEASE(m_ltFromName);

}

//初始化

bool PopupLayer::init()
{

	if ( !Layer::init() ){
		return false;
	}

	this->setContentSize(Size::ZERO);

	EventListenerTouchOneByOne * listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan=CC_CALLBACK_2(PopupLayer::onTouchBegan, this);
	//屏蔽下面层的事件
	listener->setSwallowTouches(true);
	this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	return true;

}

bool PopupLayer::onTouchBegan( Touch *pTouch, Event *pEvent ){
	return true;
}

//创建一个弹出层，给背景精灵变量赋值
PopupLayer* PopupLayer::create( const char* backgroundImage ){

	PopupLayer* popup = PopupLayer::create();
	popup->setSpriteBackGround(Sprite::create(backgroundImage));
	popup->setSprite9BackGround(Scale9Sprite::create(backgroundImage));

	//初始化需要的Menu
	Menu* menu = CCMenu::create();
	menu->setPosition(Vec2::ZERO);
	popup->setMenuButton(menu);  //set()方法

	return popup;

}

//给标题变量赋值
void PopupLayer::setTitle( const char* title, int fontsize ){
	LabelTTF* ltfTitle = CCLabelTTF::create(title, "Arial", fontsize);
	ltfTitle->setColor(Color3B::WHITE);
	setLabelTitle(ltfTitle);

}

//给文本变量赋值
void PopupLayer::setContentText( const char* text){
	LabelTTF* content = CCLabelTTF::create(text, "Arial", 14);
	content->setColor(Color3B::WHITE);
	setLabelContentText(content);
	m_contentPadding = 20;
	m_contentPaddingTop = 60;

}

void PopupLayer::setFromName(std::string name){
	
	LabelTTF* content = CCLabelTTF::create(name, "Arial", 25);
	content->setColor(Color3B::YELLOW);
	setLabelFromName(content);
}

//给下层层变量和回调函数变量赋值
void PopupLayer::setCallBackFunc( Ref* target, SEL_CallFunc callfun ){
	m_callbackListener = target;
	m_callback = callfun;

}

//给menu菜单变量添加Item
bool PopupLayer::addButton(const char* title, int tag ){

	auto label = Label::create();
	label->setSystemFontSize(14);
	label->setString(title);
	label->setColor(Color3B::YELLOW);

	auto labelSize = label->getContentSize();

	// 创建图片菜单按钮
	auto normalSprite = Scale9Sprite::create(Rect(29, 21, 1, 1), "ui/btn_bg_normal.png");
	normalSprite->setContentSize(Size(labelSize.width + 40, 40));

	auto pressedSprite = Scale9Sprite::create(Rect(29, 21, 1, 1), "ui/btn_bg_pressed.png");
	pressedSprite->setContentSize(normalSprite->getContentSize());

	auto menuItem = MenuItemSprite::create(normalSprite, pressedSprite,
		CC_CALLBACK_1(PopupLayer::buttonCallBack, this));

	menuItem->setTag(tag);
	menuItem->setPosition(Vec2(getSpriteBackGround()->getContentSize().width/2, 10));
	menuItem->setAnchorPoint(Vec2(0.5, 0));

	// 添加文字说明并设置位置
	auto menuSize = menuItem->getContentSize();

	label->setPosition(Point(menuSize.width/2, menuSize.height/2));
	menuItem->addChild(label);
	getMenuButton()->addChild(menuItem);

	return true;

}
void PopupLayer::show()
{
	this->setPosition(ccp(0, 0));
	Director::getInstance()->getRunningScene()->addChild(this, 0xffff);

}


//销毁弹出框，传递参数node给下层
void PopupLayer::buttonCallBack( Ref* pSender ){
	Node* node = (Node*)pSender;
	//CCLog("touch tag: %d", node->getTag());
	if (m_callback && m_callbackListener)
	{
		//执行HelloWorld层的回调函数，传递node参数
		(m_callbackListener->*m_callback)();
	}
	this->removeFromParentAndCleanup(true);

}

//全部参数都设定好后，在运行时动态加载
void PopupLayer::onEnter(){
	Layer::onEnter();

	Size winSize = Director::getInstance()->getWinSize();
	Point center = Point(winSize.width/2, winSize.height/3);

	Size contentSize;
	if (getContentSize().equals(Size::ZERO))
	{
		getSpriteBackGround()->setPosition(center);
		this->addChild(getSpriteBackGround(),0,0);
		contentSize = getSpriteBackGround()->getTexture()->getContentSize();

		//添加按钮，并根据Item的个数设置其位置
		getSpriteBackGround()->addChild(getMenuButton());

	}else{
		Scale9Sprite* background = getSprite9BackGround();
		background->setContentSize(getContentSize());
		background->setPosition(center);
		this->addChild(background, 0);
		contentSize = getContentSize();

		//添加按钮，并根据Item的个数设置其位置
		getSprite9BackGround()->addChild(getMenuButton());

	}

	//显示文本内容
	if (getLabelContentText()){
		LabelTTF* ltf = getLabelContentText();
		ltf->setPosition(center);
		ltf->setDimensions(Size(contentSize.width - m_contentPadding*2, contentSize.height - m_contentPaddingTop));
		ltf->setHorizontalAlignment(TextHAlignment::LEFT);
		this->addChild(ltf);

	}

	//显示人物头像
	auto head = Sprite::create("head/npc_1002.png");
	head->setAnchorPoint(Vec2(0, 0));
	head->setPosition(Vec2(0, getSpriteBackGround()->getContentSize().height));
	getSpriteBackGround()->addChild(head);

	//显示人物名称
	if (getLabelFromName()){
		LabelTTF* ltf = getLabelFromName();
		ltf->setAnchorPoint(Vec2(0, 0));
		ltf->setPosition(Vec2(head->getPositionX() + head->getContentSize().width, 
			getSpriteBackGround()->getContentSize().height));
		getSpriteBackGround()->addChild(ltf);
	}

	//弹出效果
	Sequence *popupActions = Sequence::create(FadeIn::create(1.0), NULL);

	this->runAction(popupActions);

}


//退出
void PopupLayer::onExit(){
	Layer::onExit();

}
