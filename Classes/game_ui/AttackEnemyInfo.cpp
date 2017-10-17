

#include "AttackEnemyInfo.h"
#include "GameScene.h"
#include "PublicDefine.h"

static int AttackEnemyInfoCount = 0;

AttackEnemyInfo::AttackEnemyInfo()
:m_bloodPro(NULL)
,m_level(NULL)
,m_nickName(NULL)
,m_widget(NULL)
{
   // CCLog("AttackEnemyInfo:%d",AttackEnemyInfoCount++);
}

AttackEnemyInfo::~AttackEnemyInfo()
{
   // CCLog("AttackEnemyInfo:%d",AttackEnemyInfoCount--);
}

bool AttackEnemyInfo::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    m_widget = dynamic_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile("ui/enemyInfo/enemyInfo.json"));
    if (m_widget)
    {
        this->addChild(m_widget);
    }

    m_nickName =static_cast<ui::Text *>(ui::Helper::seekWidgetByName(m_widget,"Label_EnemyName"));

    m_level = dynamic_cast<ui::TextAtlas*>(ui::Helper::seekWidgetByName(m_widget,"enemy_Level"));
    if (m_level)
    {
        m_level->setString("0");
    }
    
    m_bloodPro = dynamic_cast<LoadingBar*>(ui::Helper::seekWidgetByName(m_widget,"LoadingBar_blood"));
    
    return true;
}

void AttackEnemyInfo::showAttackInfo(Monomer* sender)
{
    if (GAME_SCENE->getSelected() == NULL)
        return;
        
    if (!this->isVisible())
    {
        this->setVisible(true);
        m_widget->setTouchEnabled(true);
    }

	int index = CsvUtil::getInstance()->findValueInWithLine(StringUtils::format("%d", sender->getRoleNumber()), en_monster_id, CSVFILE_MONSTER);

	m_nickName->setString(CsvUtil::getInstance()->getText(index, en_monster_name, CSVFILE_MONSTER));
	m_level->setString(CsvUtil::getInstance()->getText(index, en_monster_level, CSVFILE_MONSTER));
    
    this->schedule(schedule_selector(AttackEnemyInfo::updateAttackInfo));
    
}

void AttackEnemyInfo::updateAttackInfo(float delay)
{
	if (GAME_SCENE->getSelected() == NULL)
		return;

	if (m_bloodPro)
	{
		int progress = 100*GAME_SCENE->getSelected()->getBlood()/GAME_SCENE->getSelected()->getBloodCap();
		if (progress < 0)
			progress = 0;
		m_bloodPro->setPercent(progress);
	}
}

void AttackEnemyInfo::hide()
{
    this->unschedule(schedule_selector(AttackEnemyInfo::updateAttackInfo));
    this->setVisible(false);
    m_widget->setTouchEnabled(false);
}