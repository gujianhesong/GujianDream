

#include "BgMapFloorTile.h"

BgMapFloorTile::BgMapFloorTile()
:m_fileName(NULL)
,m_sprite(NULL)
,m_bgMap(NULL)
,m_nIsDisplay(false)
{
    
}

BgMapFloorTile::~BgMapFloorTile()
{
    CC_SAFE_DELETE(m_fileName);
}

void BgMapFloorTile::displayImageView()
{
    if (m_nIsDisplay == false)
    {
        m_nIsDisplay = true;
        Director::getInstance()->getTextureCache()->addImageAsync(m_fileName->getCString(), CC_CALLBACK_1(BgMapFloorTile::initWithImageView,this));
		log("maptile path : %s", m_fileName->getCString());
		checkValuesEnable();
	}
}

void BgMapFloorTile::checkValuesEnable(){

	//显示不可通行区域

	Rect rect = Rect(getPositionX(), getPositionY(), 256, 256);

	if(m_bgMap){
		
		unsigned char** mapGrid = m_bgMap->getMapGrid();
		M_INT m_gridCol = m_bgMap->getGridCol();
		M_INT m_gridRow = m_bgMap->getGridRow();

		M_INT minRowIndex = rect.getMinY() / m_bgMap->getGridSize().height;
		M_INT maxRowIndex = rect.getMaxY() / m_bgMap->getGridSize().height + 1;
		M_INT minColIndex = rect.getMinX() / m_bgMap->getGridSize().width;
		M_INT maxColIndex = rect.getMaxX() / m_bgMap->getGridSize().width + 1;

		minRowIndex = max(minRowIndex, (M_INT)0);
		maxRowIndex = min(maxRowIndex, m_bgMap->getGridCol());
		minColIndex = max(minColIndex, (M_INT)0);
		maxColIndex = min(maxColIndex, m_bgMap->getGridRow());

		if(m_nodeVector.size() == 0){
			for (unsigned int i=minRowIndex; i<maxRowIndex; i++)
			{
				for(unsigned int j=minColIndex; j<maxColIndex; j++){

					if(mapGrid[i][j] == 0){
						continue;
					}

					Point point = Point((j + 0.5)*m_bgMap->getGridSize().width, 
						(i + 0.5)*m_bgMap->getGridSize().height);


					auto node = Scale9Sprite::create("button_1.png");
					node->setOpacity(128);

					if(mapGrid[i][j] == 1){

						node->setColor(Color3B(0xff, 0, 0));
						
					}else if(mapGrid[i][j] == 2){

						node->setColor(Color3B(0, 0xff, 0));

					}else if(mapGrid[i][j] == 0){

						node->setColor(Color3B(0, 0, 0xff));
					}


					node->setContentSize(Size(m_bgMap->getGridSize().width, m_bgMap->getGridSize().height));
					
					node->setOpacity(0xaa);
					node->setAnchorPoint(Point::ANCHOR_MIDDLE);

					Point pointworld = m_bgMap->convertToWorldSpace(point);
					node->setPosition(this->convertToNodeSpace(pointworld));
						
					m_nodeVector.pushBack(node);
				
				}
			}
		}

		for(Node* node : m_nodeVector){
			addChild(node, 10);
		}

	}
}

/*Sprite* BgMapFloorTile::createIndicator()
{
	if(m_textureShade == NULL){
		int size = 256;

		int **pixels=new int *[size];  
		for(int i=0;i<size;++i)  
		{  
			*(pixels+i)=new int[size];   
		}  
  
		for(int i=0;i<size;++i)  
		{  
			for(int j=0;j<size;++j)  
			{  
				*(*(pixels+i)+j)= 0xffffff00;
			}  
		}  
    
		m_textureShade = new Texture2D();
		m_textureShade->initWithData(pixels,20, Texture2D::PixelFormat::RGB888, 1, 1, Size(size, size));
	}
	
	//Sprite* sprite = Sprite::create("ui/shade.jpg");
    return nullptr;
}*/

//bool BgMapFloorTile::checkTheValueEnable(Point point){
//	int x = (int)point.x % 256;
//	int y = (int)point.y % 256;
//
//	if(!m_img){
//		return true;
//	}
//
//	unsigned char *data_ = m_img->getData();
//        
//    unsigned int *pixel = (unsigned int *)data_;
//    pixel = pixel + (y * (int)256)* 1 + x * 1;
//
//	Color4B c = Color4B(0, 0, 0, 0);
//    /*c.r = *pixel & 0xff;
//    c.g = (*pixel >> 8) & 0xff;
//    c.b = (*pixel >> 16) & 0xff;*/
//    c.a = (*pixel >> 24) & 0xff;
//    if (c.a == 0) {
//		log("clickable...");
//        return true;
//    }else{
//		log("not clickable...");;
//        return false;
//    }
//}

void BgMapFloorTile::hideImageView()
{
    if (m_nIsDisplay)
    {
        m_nIsDisplay = false;

		for(Node* node : m_nodeVector){
			removeChild(node);
		}

        if (m_sprite)
        {
            m_sprite->removeFromParent();
            m_sprite = NULL;
        }

        Director::getInstance()->getTextureCache()->removeTextureForKey(m_fileName->getCString());
    }
}

void BgMapFloorTile::initWithImageView(Texture2D* texture)
{
    if (m_sprite == NULL)
    {
        m_sprite = CCSprite::createWithTexture(texture);
        m_sprite->setAnchorPoint(Point::ZERO);
        this->addChild(m_sprite, -1);

    }
}

void BgMapFloorTile::IntelligentDisplay(Rect& showRect, Rect& hideRect)
{
    Point point = ccpAdd(this->getPosition(), OFF_SIZE);
    
    if (showRect.containsPoint(point))
    {
        this->displayImageView();
    }
    
    if (!hideRect.containsPoint(point))
    {
        this->hideImageView();
    }
    
}