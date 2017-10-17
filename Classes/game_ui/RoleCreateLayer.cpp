//
//  RoleCreateLayer.cpp
//  guyue
//
//  Created by dsh on 15/1/7.
//
//

#include "RoleCreateLayer.h"
#include "Figure.h"
#include "Monomer.h"
#include "GameLoading.h"
#include "GameSocket.h"
#include "TextureController.h"
#include "StringHelper.h"
#include "CsvUtil.h"
#include "PublicDefine.h"



RoleCreateLayer::RoleCreateLayer()
:m_nickName("")
,m_selected(NULL)
{
}

RoleCreateLayer::~RoleCreateLayer()
{
	m_roleObjVector.clear();
	m_roleArray.clear();
	Director::getInstance()->getTextureCache()->removeAllTextures();
}

bool RoleCreateLayer::init()
{
    if (!Layer::init()) {
        return false;
    }

    Size winSize = Director::getInstance()->getWinSize();
    //SimpleAudioEngine::getInstance()->playBackgroundMusic("music/loading.mp3",true);
    
    Sprite * bj=Sprite::create("bjrole.png");
    bj->setPosition(winSize.width/2,winSize.height/2);
    this->addChild(bj);
	std::wstring str = L"血腥、暴力、耐打";
	Label *Label1 = Label::createWithSystemFont(StringHelper::WStrToUTF8(str), "Arial", 20);
    Label1->setPosition(Point(170,100));
    Label1->setColor(Color3B::RED);
    Label1->enableOutline(Color4B(124,66,24,255),2);
    this->addChild(Label1);
    
	std::wstring str2 = L"法术，高魔";
	Label *Label2 = Label::createWithSystemFont(StringHelper::WStrToUTF8(str2), "Arial", 20);
    Label2->setPosition(Point(470,100));
    Label2->setColor(Color3B::ORANGE);
    Label2->enableOutline(Color4B(124,66,24,255),2);
    this->addChild(Label2);
    
	std::wstring str3 = L"强力、强力、还是强力";
	Label *Label3 = Label::createWithSystemFont(StringHelper::WStrToUTF8(str3), "Arial", 20);
    Label3->setPosition(Point(800,100));
    Label3->setColor(Color3B::YELLOW);
    Label3->enableOutline(Color4B(124,66,24,255),2);
    this->addChild(Label3);
    
	Point points[] = {Point(150,160), Point(470,160), Point(800,160)};

	for(int i=0; i<3; i++){
		RoleObj role;
		role.p=points[i];
		role.number=CsvUtil::getInstance()->getInt(i, en_role_id, CSVFILE_ROLE);
		role.nickname = CsvUtil::getInstance()->getText(i, en_role_name, CSVFILE_ROLE);
		role.weaponId = CsvUtil::getInstance()->getInt(i, en_role_weapon, CSVFILE_ROLE);

		m_roleObjVector.push_back(role);
	}
    
    for (unsigned int i =0; i < 3; i++)
    {
        ControlButton* button = ControlButton::create();
        button->setHighlighted(false);
        button->setPreferredSize(Size(120, 200));
		button->setPosition(m_roleObjVector.at(i).p);
        this->addChild(button);
        button->setTag(i);
        button->addTargetWithActionForControlEvents(this,cccontrol_selector(RoleCreateLayer::setSelector),Control::EventType::TOUCH_DOWN);
        
        Figure* monomer = Figure::create(TexturePathFigure, m_roleObjVector.at(i).number);
		monomer->setWeapon(m_roleObjVector.at(i).weaponId);
        monomer->setScale(1);
        monomer->setPosition(button->getContentSize()/2);
        monomer->setColor(Color3B(127, 127, 127));
        button->addChild(monomer);
        m_roleArray.pushBack(monomer);
        
    }
    
    MenuItemImage* back = MenuItemImage::create(
                    "enter_n.png",	//正常图片
                    "enter_h.png",	//按下图片
                    CC_CALLBACK_1(RoleCreateLayer::callBackEnter, this) //被点击时回调的方法
                    );
	//设置其位置
    back->setPosition(Point(winSize.width+200,winSize.height-10));
    //创建菜单回想
    Menu* menu = Menu::create(back,NULL);
    //设置其位置
    menu->setPosition(Point::ZERO);
    menu->setScale(0.5);
    menu->setTag(999);
    menu->setVisible(false);
    //将其添加到布景中
    this->addChild(menu, 2);


    return true;
}

void RoleCreateLayer::callBackEnter(Ref* obj)
{
    
	GameSocket::sendRoleCreate(this, m_roleObjVector.at(m_index).number, m_roleObjVector.at(m_index).nickname, m_roleObjVector.at(m_index).weaponId);//SOCKET
}

Scene * RoleCreateLayer::createScene()
{
    auto scene=Scene::create();
    auto layer=RoleCreateLayer::create();
    scene->addChild(layer);
    return scene;
}

void RoleCreateLayer::randomNickName(Ref* pSender)
{
//    if (m_textField)
//    {
//        int num = CCRANDOM_0_1() * 0xffffff;
//        char str[20];
//        sprintf(str, "随机%d",num);
//        m_textField->setText(str);
//    }
}

void RoleCreateLayer::sendMessage(Ref *pSender)
{
//    if (0 == strcmp(m_textField->getStringValue(), ""))
//    {
//        CCMessageBox("提示", "您还没有取名哦!");
//        return;
//    }
//    GameSocket::sendRoleCreate(this, m_roleID, m_textField->getStringValue());//SOCKET
//    m_textField->setTouchEnabled(true);
}

void RoleCreateLayer::setSelector(Ref *pSender, Control::EventType controlEvent)
{
    ControlButton* btn = dynamic_cast<ControlButton*>(pSender);
    if (btn)
    {
		m_index = btn->getTag();
        if (m_selected)
        {
            Figure* monomer = (Figure*)m_selected;
            ControlButton* button = (ControlButton*)monomer->getParent();
            monomer->setFigureState(FStateStand, FDirDown);
            monomer->setColor(ccc3(127, 127, 127));
            ScaleTo* scaleTo = ScaleTo::create(0.15f, 1.0f);
            Sequence* actions = Sequence::create(scaleTo, NULL);
            monomer->runAction(actions);
            button->setTouchEnabled(true);
        }
        
        for (int i=0; i<m_roleArray.size(); i++)
        {
            Figure* monomer = (Figure*)m_roleArray.at(i);
            ControlButton* button = (ControlButton*)monomer->getParent();
            if (btn==(button))
            {
                button->setTouchEnabled(false);
                this->runActionsForFigure(monomer);
                m_selected = monomer;

				monomer->setColor(Color3B(255, 255, 255));
            }
        }
        
        this->getChildByTag(999)->setVisible(true);
    }
    

}

void RoleCreateLayer::runActionsForFigure(cocos2d::Ref *pSender)
{
    Figure* monomer = (Figure*)pSender;
    
	monomer->setFigureState(FStateWalk, FDirDown);
    
    /*DelayTime* delayTime = DelayTime::create(2.f);
    CCCallFuncO* finish = CCCallFuncO::create(this, callfuncO_selector(RoleCreateLayer::runActionsForFigure), monomer);
    Sequence* actions = Sequence::create(delayTime, finish, NULL);
    monomer->runAction(actions);*/
}

void RoleCreateLayer::joinGame()
{
   
    GameLoading::runGameLoading(101, 1);//地图200  传送编号1
    this->removeFromParent();
    
}