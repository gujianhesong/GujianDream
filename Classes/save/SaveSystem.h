

#ifndef __gujian__SaveSystem__
#define __gujian__SaveSystem__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

#include "byte_buffer.h"


USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;
using namespace rapidjson;


/************************************************************************/
/* 存储系统                                                                     */
/************************************************************************/
class HeaderInfo;
class EndInfo;
class RoleInfo;
class GoodInfo;
class GoodItemInfo;

class SaveSystem {

public:

	HeaderInfo* _pHeaderInfo;
	EndInfo* _pEndInfo;

	RoleInfo* _pRoleInfo;
	GoodInfo* _pGoodInfo;

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

/************************************************************************/
/* 物品类型                                                                     */
/************************************************************************/
enum GoodType
{
	Good_Unkonw,
	Good_AddHp,
	Good_AddMp,
	Good_Baotu
};

/************************************************************************/
/* 保存内容类型                                                                     */
/************************************************************************/
enum SaveType
{
	Save_Header = 1000,
	Save_End,
	Save_Role,
	Save_Good,
	Save_Task
};

class BaseInfo{

	enum InfoType
	{
		Unkonw,
		RoleInfo,
		GoodInfo
	};

public:

	BaseInfo(){

	}

	~BaseInfo(){

	}

public:

	virtual void readFromBuffer(ByteBuffer &buffer){

	}

	virtual void writeToBuffer(ByteBuffer &buffer){

	}

};

/************************************************************************/
/* 主角信息                                                                     */
/************************************************************************/
class RoleInfo : public BaseInfo{
public:
	string name;
	uint32 cur_hp;
	uint32 max_hp;
	uint32 cur_mp;
	uint32 max_mp;

public:

	RoleInfo(){
		name = "";
		cur_hp = 0;
		max_hp = 0;
		cur_mp = 0;
		max_mp = 0;
	}

	~RoleInfo(){

	}

	virtual void readFromBuffer(ByteBuffer &buffer){
		buffer >> name;
		buffer >> cur_hp;
		buffer >> max_hp;
		buffer >> cur_mp;
		buffer >> max_mp;

		log("read, %s,%d,%d,%d",name.c_str(),cur_hp,max_hp, cur_mp);
	}

	virtual void writeToBuffer(ByteBuffer &buffer){
		buffer << name;
		buffer << cur_hp;
		buffer << max_hp;
		buffer << cur_mp;
		buffer << max_mp;
	}

};

/************************************************************************/
/* 物品Item信息                                                               */
/************************************************************************/
class GoodItemInfo : public BaseInfo{

public:
	uint32 id;
	string name;
	uint8 type;
	uint32 affectValue;
	uint8 num;

public:

	GoodItemInfo(){
		id = 0;
		name = "";
		type = Good_Unkonw;
		affectValue = 0;
		num = 0;
	}

	~GoodItemInfo(){

	}

	virtual void readFromBuffer(ByteBuffer &buffer){
		buffer >> id;
		buffer >> name;
		buffer >> type;
		buffer >> affectValue;
		buffer >> num;

		log("read, %d,%s,%d,%d,%d",id,name.c_str(),type,affectValue, num);
	}

	virtual void writeToBuffer(ByteBuffer &buffer){
		buffer << id;
		buffer << name;
		buffer << type;
		buffer << affectValue;
		buffer << num;

		log("write, %d,%s,%d,%d,%d",id,name.c_str(),type,affectValue, num);
	}

};

/************************************************************************/
/* 物品信息                                                               */
/************************************************************************/
class GoodInfo : public BaseInfo{
public:
	
	uint8 num;
	std::vector<GoodItemInfo*> infoList;

public:

	GoodInfo(){
		infoList.clear();
		num = 0;
	}

	~GoodInfo(){
		infoList.clear();
	}

	virtual void readFromBuffer(ByteBuffer &buffer){
		buffer >> num;
		log("---num=====%d", num);
		for(int i=0; i<num; i++){
			GoodItemInfo* itemInfo = new GoodItemInfo;
			itemInfo->readFromBuffer(buffer);

			infoList.push_back(itemInfo);
		}

	}

	virtual void writeToBuffer(ByteBuffer &buffer){
		buffer << infoList.size();
		for(GoodItemInfo* itemInfo : infoList){
			itemInfo->writeToBuffer(buffer);
		}
	}

};

class HeaderInfo : public BaseInfo{

public:
	uint32 id;
	string name;

public:

	HeaderInfo(){
		id = 0;
		name = "";
	}

	~HeaderInfo(){

	}

	virtual void readFromBuffer(ByteBuffer &buffer){
		buffer >> id;
		buffer >> name;
	}

	virtual void writeToBuffer(ByteBuffer &buffer){
		buffer << id;
		buffer << name;
	}

};

class EndInfo : public BaseInfo{

public:
	uint32 id;
	string name;

public:

	EndInfo(){
		id = 0;
		name = "";
	}

	~EndInfo(){

	}

	virtual void readFromBuffer(ByteBuffer &buffer){
		buffer >> id;
		buffer >> name;
	}

	virtual void writeToBuffer(ByteBuffer &buffer){
		buffer << id;
		buffer << name;
	}

};


#endif
