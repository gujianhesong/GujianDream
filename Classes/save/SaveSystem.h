

#ifndef __gujian__SaveSystem__
#define __gujian__SaveSystem__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

#include "ByteBuffer.h"
#include "basic_types.h"
#include "ItemDefine.h"


USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace rapidjson;


// 存储系统                                                                     */
class SaveSystem {

public:

	HeaderInfo* _pHeaderInfo;
	EndInfo* _pEndInfo;

	RoleInfo* _pRoleInfo;
	GoodInfo* _pGoodInfo;
	EquipmentInfo* _pEquipmentInfo;


public:

	SaveSystem();

	virtual ~SaveSystem();

	static SaveSystem* getInstance();

private:

	void test();

	//创建一个默认的存档
	void createDefaultSave();

	//从默认的存档读取数据
	void readFromDefaultSave();

	// 从存档读取数据
	void readFromSave();


};




#endif
