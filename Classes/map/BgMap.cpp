

#include "BgMap.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include "Player.h"
#include "GameScene.h"
#include "GameLoading.h"
#include "PortalSprite.h"
#include "SimpleAudioEngine.h"
using namespace std;
using namespace rapidjson;
#define RADIUS_PASSAGEWAY 64

#define TYPE_PROTAL 100
#define TYPE_BORN_POINT 101
#define TYPE_NPC 200
#define TYPE_ENEMY 300
#define TYPE_BACKGROUND_MUSIC 400

#pragma -
#pragma JSON

#pragma -
#pragma BgMap

/*
 20150106 段生辉 修改json解析
*/

void BgMap::readGirdData()
{
    rapidjson::Document json;
    std::string jsonpath = FileUtils::getInstance()->fullPathForFilename(StringUtils::format("map/s%d/data_gird_%d.json", m_mapID, m_mapID));
    std::string contentStr = FileUtils::getInstance()->getStringFromFile(jsonpath);
    json.Parse<0>(contentStr.c_str());
    
    unsigned int width = json["mapW"].GetInt();
    unsigned int height = json["mapH"].GetInt();
    m_bgSize = Size(width, height);
    
    unsigned int gWidth = json["mapGridW"].GetInt();
    unsigned int gHeight = json["mapGridH"].GetInt();

    m_gridSize = Size(gWidth, gHeight);

    MapPoint::setGridSize(m_gridSize);

    m_gridRow = (unsigned int)ceilf(width/m_gridSize.width);//有多少列
    m_gridCol = (unsigned int)ceilf(height/m_gridSize.height);//有多少行
    
    this->initGridData(json["mapFlagArr"]);
    
    unsigned int iWidth = json["divideBlockW"].GetInt();//划分块
    unsigned int iHeight = json["divideBlockH"].GetInt();

    m_imageSize = Size(iWidth, iHeight);

    m_imageRow = (unsigned int)ceilf(width/m_imageSize.width);
    m_imageCol = (unsigned int)ceilf(height/m_imageSize.height);

	
}

void BgMap::readGoodsData()
{
    rapidjson::Document json;
    std::string jsonpath = FileUtils::getInstance()->fullPathForFilename(StringUtils::format("map/s%d/data_goods_%d.json", m_mapID, m_mapID));
    std::string contentStr = FileUtils::getInstance()->getStringFromFile(jsonpath);
    json.Parse<0>(contentStr.c_str());

    if (json.IsNull() || !json["items"].IsArray())
    {
        return ;
    }

    unsigned int size = json["items"].Size();
    for (unsigned int i=0; i<size; i++)
    {
        rapidjson::Value & goods = json["items"][i];

		goods["itemPosX"].SetDouble(goods["itemPosX"].GetDouble() + 90);
        
        switch (goods["itemType"].GetInt())
        {
            
            case TYPE_PROTAL:
            {
                //传送
                int key = goods["itemSN"].GetInt();
                int mapID = goods["portalToMap"].GetInt();
                int born = goods["portalToPos"].GetInt();
                Point point = Point(goods["itemPosX"].GetDouble(), goods["itemPosY"].GetDouble());
                
				PortalInformation info;
				info.key = key;
				info.mapID = mapID;
				info.born = born;
				info.point = point;
				
				m_mapPassageway.push_back(info);
            }
            break;
            case TYPE_BORN_POINT:
            {
                //出生地
                int born = goods["itemSN"].GetInt();
                Point point = Point(goods["itemPosX"].GetDouble(), goods["itemPosY"].GetDouble());
                m_mapBornPoint[born] = point;
            }
                break;
            case TYPE_NPC:
            {
                //npc
                int key = goods["itemSN"].GetInt();
                NPCDirectionType direction = (NPCDirectionType)goods["npcDirection"].GetInt();
                Point point = Point(goods["itemPosX"].GetDouble(), goods["itemPosY"].GetDouble());

				NpcInformation info;
				info.key = key;
				info.directionType = direction;
				info.point = point;

				m_npcInfoVec.push_back(info);
            }
                break;
            case TYPE_ENEMY:
            {
                // 敌人
                unsigned int key = goods["itemSN"].GetUint();
                Point point = Point(goods["itemPosX"].GetDouble(), goods["itemPosY"].GetDouble());
                m_enemyMap.insert(std::map<unsigned int, Point>::value_type(key, point));
            }
                break;
            case TYPE_BACKGROUND_MUSIC:
            {
                //背景音乐
               /* unsigned int key = goods["itemSN"].GetUint();
                char s[20];
                sprintf(s, "music/map_music/%d.mp3", key);
                m_backGroundMusic = s;*/
            }
                break;
            default:
                break;
        }
    }
}

BgMap::BgMap(int mapID)
:m_grid(NULL)
,m_gridRow(0)
,m_gridCol(0)
,m_gridSize(Size::ZERO)
,m_imageRow(0)
,m_imageCol(0)
,m_imageSize(Size::ZERO)
,m_bgSize(Size::ZERO)
,m_mapID(mapID)
,m_playerLead(NULL)
,m_delegate(NULL)
{
    this->readGirdData();
    this->readGoodsData();
}

BgMap::~BgMap()
{
    for (unsigned int i=0; i<m_gridCol; i++)
    {
        delete[] m_grid[i];
    }
    delete[] m_grid;
}

BgMap* BgMap::createWithPath(int mapID)
{
    BgMap* map = new BgMap(mapID);
    if (map && map->init())
    {
        map->autorelease();
        return map;
    }
    CC_SAFE_DELETE(map);
    return NULL;
}

bool BgMap::init()
{
    if (!Node::init())
        return false;

    char str[24];
    sprintf(str, "map/s%d/min_s%d.jpg", m_mapID, m_mapID);
    m_sMap = Sprite::create(str);

	if(m_sMap){
		m_sMap->setAnchorPoint(Vec2::ZERO);
		this->addChild(m_sMap, BgMap::getZOrderZero(this));
		//m_sMap->setScale(10/3.0f);
	}

    //初始化地图块
    this->initBgMapFloorTile();
    //生成传送门
    this->initBgMapPassagewayImage();
    
    this->initNpcFigure();

    this->updateImageDisplay();

    return true; 
}

void BgMap::initGridData(rapidjson::Value & arr)
{
    m_grid = new unsigned char*[m_gridCol];
    for (unsigned int i=0; i<m_gridCol; i++)
    {
        m_grid[i] = new unsigned char[m_gridRow];
    }
    
    int size = arr.Size();
    for (unsigned int i=0; i<size; i++)
    {
		m_grid[i/m_gridRow][i%m_gridRow] = arr[i].GetInt();
    }
}

void BgMap::initBgMapFloorTile()
{
    for (unsigned int i=0; i<m_imageCol; i++)
    {
		std::vector<BgMapFloorTile*> vector;

        for (unsigned int j=0; j<m_imageRow; j++)
        {
            BgMapFloorTile* floorTile = BgMapFloorTile::create();
			floorTile->setCurrBgMap(this);
            floorTile->setFloorTileFileName(CCString::createWithFormat("map/s%d/s%d_%d_%d.jpg", m_mapID, m_mapID, i, j));
			floorTile->setPosition(Point(m_imageSize.width*j, m_imageSize.height*i));
            this->addChild(floorTile, BgMap::getZOrderZero(this));//z轴
			
            vector.push_back(floorTile);
        }

		m_floorTileVec.push_back(vector);
    }
}

void BgMap::initBgMapPassagewayImage()
{
    vector<struct PortalInformation>::iterator itr;
    for (itr=m_mapPassageway.begin(); itr!=m_mapPassageway.end(); itr++)
    {
        Point point = MapPoint((*itr).point).getCCPointValue();
        
        //生成传送门
        float value = BgMap::getZOrder(point);//z轴
        
        PortalSprite* sprite = PortalSprite::createWithPortalSprite("trans-");
        sprite->setPosition(point);
        this->addChild(sprite, value);
    }
}

void BgMap::initNpcFigure()
{
    vector<struct NpcInformation>::iterator itr;
    for (itr=m_npcInfoVec.begin(); itr!=m_npcInfoVec.end(); itr++)
    {
        NpcFigure* npc = NpcFigure::create(itr->key, itr->directionType);
		npc->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        npc->setPosition(itr->point);
        this->addChild(npc, this->getZOrder(npc->getPosition()));
        
        m_npcFigureVec.push_back(npc);
    }
}

void BgMap::updateImageDisplay()
{
    Rect rect = this->getShowRect();

    Rect rect2 = this->getHideRect();
    
    /*std::vector<BgMapFloorTile*>::iterator itr;
    for (itr=m_floorTileVec.begin(); itr!=m_floorTileVec.end(); itr++)
    {
        (*itr)->IntelligentDisplay(rect, rect2);
    }*/

	M_INT minRowIndex = rect2.getMinY() / m_imageSize.height;
	M_INT maxRowIndex = rect2.getMaxY() / m_imageSize.height + 1;
	M_INT minColIndex = rect2.getMinX() / m_imageSize.width;
	M_INT maxColIndex = rect2.getMaxX() / m_imageSize.width + 1;

	minRowIndex = max(minRowIndex, (M_INT)0);
	maxRowIndex = min(maxRowIndex, (M_INT)m_imageCol);
	minColIndex = max(minColIndex, (M_INT)0);
	maxColIndex = min(maxColIndex, (M_INT)m_imageRow);

	for (unsigned int i=minRowIndex; i<maxRowIndex; i++)
	{
		for(unsigned int j=minColIndex; j<maxColIndex; j++){

			m_floorTileVec.at(i).at(j)->IntelligentDisplay(rect, rect2);
				
		}
	}
    
    std::vector<NpcFigure*>:: iterator itr2;
    for (itr2=m_npcFigureVec.begin(); itr2!=m_npcFigureVec.end(); itr2++)
    {
        (*itr2)->IntelligentDisplay(rect, rect2);
    }
    
    if (m_delegate)
    {
        m_delegate->updateImageDisplay(rect, rect2);
    }
    
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

void BgMap::updateMapPosition(float delay)
{
    if (m_playerLead == NULL)
        return;

    Size winSize = Director::getInstance()->getWinSize();
    Point pMap = ccpSub(winSize/2, m_playerLead->getPosition());
    
    pMap.x = MAX(pMap.x, WINSIZE.width-this->getBgSize().width);
    pMap.y = MAX(pMap.y, WINSIZE.height-this->getBgSize().height);
    pMap.x = MIN(pMap.x, 0);
    pMap.y = MIN(pMap.y, 0);
    
    this->setPosition(pMap);
    
    this->updateImageDisplay();
}

unsigned char BgMap::getCurrentGridValue(const MapPoint& point)
{
    M_INT row = point.x;
    M_INT col = point.z;
    unsigned char relust = 1;
    
    if (1 < col && col < m_gridCol-1 && 1 < row &&row < m_gridRow-1)
    {
        relust = m_grid[col][row];
    }
    
    return relust;
}

PortalInformation BgMap::isMapPassageway(Node *node)
{
    vector<struct PortalInformation>::iterator itr;
    for (itr=m_mapPassageway.begin(); itr!=m_mapPassageway.end(); itr++)
    {
        struct PortalInformation mapPassageway = *itr;
        Point point = mapPassageway.point;
        if (ccpDistance(point, node->getPosition()) <= RADIUS_PASSAGEWAY)
        {
            return mapPassageway;
        }
    }
    return PortalInformationZero;
}

std::map<unsigned int, Point>& BgMap::getEnemeyMap()
{
    return m_enemyMap;
}

void BgMap::addChildPlayerLead(Node* node, int bornPoint)
{
    map<int, Point>::iterator itr = m_mapBornPoint.find(bornPoint);
    Point point = MapPoint((*itr).second).getCCPointValue();
    this->addChildPlayerLead(node, point);
}

void BgMap::addChildPlayerLead(Node* node, Point point)
{
    float value = BgMap::getZOrder(this->getPosition());//z轴
    m_playerLead = node;
    m_playerLead->setPosition(MapPoint(point).getCCPointValue());
    this->addChild(m_playerLead, value);
    this->updateMapPosition();
}

 Rect BgMap::getShowRect()
{
    Rect rect = Rect::ZERO;
    rect.origin = ccpSub(Vec2::ZERO, OFF_SIZE);
    rect.origin = ccpSub(rect.origin, this->getPosition());
    rect.size = ccpAdd(WINSIZE, OFF_SIZE * 2);
    return rect;
}

 Rect BgMap::getHideRect()
{
    Rect rect = Rect::ZERO;
    rect.origin = ccpSub(Point::ZERO, OFF_SIZE * 2);
    rect.origin = ccpSub(rect.origin, this->getPosition());
    rect.size = ccpAdd(WINSIZE, OFF_SIZE * 4);
    return rect;
}

int BgMap::getMapID()
{
    return m_mapID;
}

bool BgMap::isTouchNpcAndNotNeedMoveToNpc(Vec2 touchPos){

	std::vector<NpcFigure*>::iterator iterator;
	for(iterator = m_npcFigureVec.begin(); iterator != m_npcFigureVec.end(); iterator++){
		NpcFigure* npc = *iterator;
		Rect rect = npc->getRect();

		if(npc->getRect().containsPoint(touchPos)){
			Player* player = Player::sharePlayer();
			if(npc->getPosition().distance(player->getPosition()) < 100){
				return true;
			}
		}
	}
	
	return false;
}

