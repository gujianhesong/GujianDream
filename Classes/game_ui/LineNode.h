#include "cocos2d.h"

USING_NS_CC;

class LineNode : public Node
{   
public:
	LineNode()
	{
		setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor));
	}

	virtual void draw(Renderer *renderer, const Mat4& transform, uint32_t flags)
	{
		ccDrawColor4B(m_TouchColor.r, m_TouchColor.g, m_TouchColor.b, 255);
		glLineWidth(3);
		ccDrawLine( ccp(0, 3), ccp(getContentSize().width, 3) );//2个坐标为画线的字父节点中的起点坐标和终点坐标
		glLineWidth(1);

	}

	void setTouchPos(const CCPoint& pt)
	{
		m_pTouchPoint = pt;
	}

	void setTouchColor(ccColor3B color)
	{
		m_TouchColor = color;
	}

	static LineNode* touchPointWithParent(CCNode* pParent)
	{
		LineNode* pRet = new LineNode();
		pRet->setContentSize(pParent->getContentSize());
		pRet->setAnchorPoint(ccp(0.0f, 0.0f));
		pRet->autorelease();
		return pRet;
	}

private:
	CCPoint m_pTouchPoint;
	ccColor3B m_TouchColor;
};
