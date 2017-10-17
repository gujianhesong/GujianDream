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
		ccDrawLine( ccp(0, 5), ccp(getContentSize().width, 5) );//2������Ϊ���ߵ��ָ��ڵ��е����������յ�����
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
//������������࣬Ȼ����ʹ��label�ĵط����������� 
TouchPoint * DrawLine = TouchPoint::touchPointWithParent(label);
DrawLine->setTouchColor(ccWHITE);//�����ߵ���ɫ
label->addChild(DrawLine);//����������ӵ����ڵ���