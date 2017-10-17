class TouchPoint : public CCNode
{   
public:
	TouchPoint()
	{
		setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor));
	}

	virtual void draw()
	{
		ccDrawColor4B(m_TouchColor.r, m_TouchColor.g, m_TouchColor.b, 255);
		glLineWidth(3);
		ccDrawLine( ccp(0, 5), ccp(getContentSize().width, 5) );//2个坐标为画线的字父节点中的起点坐标和终点坐标
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

	static TouchPoint* touchPointWithParent(CCNode* pParent)
	{
		TouchPoint* pRet = new TouchPoint();
		pRet->setContentSize(pParent->getContentSize());
		pRet->setAnchorPoint(ccp(0.0f, 0.0f));
		pRet->autorelease();
		return pRet;
	}

private:
	CCPoint m_pTouchPoint;
	ccColor3B m_TouchColor;
};
//先声明上面的类，然后在使用label的地方添加下面代码 
TouchPoint * DrawLine = TouchPoint::touchPointWithParent(label);
DrawLine->setTouchColor(ccWHITE);//画的线的颜色
label->addChild(DrawLine);//将画的先添加到父节点上