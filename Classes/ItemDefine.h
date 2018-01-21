#ifndef __gujian__ItemDefine__
#define __gujian__ItemDefine__

#include "ByteBuffer.h"
#include "basic_types.h"

#include "cocos2d.h"

using namespace std;
USING_NS_CC;

// 保存内容类型                                                                     */
enum SaveType
{
	Save_Header = 1000,
	Save_End,
	Save_Role,
	Save_Good,
	Save_Equipment,
	Save_Task
};

// 物品类型
enum GoodType
{
	Good_Unkonw,
	Good_AddHp,
	Good_AddMp,
	Good_Baotu
};

// 装备类型
enum EquipmentType
{
	Equipment_Unknown,   //未知
	Equipment_Weapon,   //武器
	Equipment_Neck, //项链
	Equipment_Head,     //头盔
	Equipment_Cloth,  //衣服
	Equipment_Belt,     //腰带
	Equipment_Shoes,    //鞋子
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

	virtual string toString(){
		return "";
	}


};

// 开头信息
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
		buffer << (uint32)Save_Header;

		buffer << id;
		buffer << name;
	}

};

// 结尾信息
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
		buffer << (uint32)Save_End;

		buffer << id;
		buffer << name;
	}

};

// 主角信息
class RoleInfo : public BaseInfo{
public:
	string name;
	uint16 level; //等级
	uint64 exp;   //经验
	uint32 cur_hp; //当前血量
	uint32 max_hp; //最大血量
	uint32 cur_mp; //当前蓝量
	uint32 max_mp; //最大蓝量
	uint32 attack; //攻击
	uint32 defence; //防御
	uint32 magic; //灵力
	uint32 magic_defence; //法术防御
	uint32 speed; //速度

public:

	RoleInfo(){
		name = "";
		level = 0;
		exp = 0;
		cur_hp = 0;
		max_hp = 0;
		cur_mp = 0;
		max_mp = 0;
		attack = 0;
		defence = 0;
		magic = 0;
		magic_defence = 0;
		speed = 0;
	}

	~RoleInfo(){

	}

	virtual void readFromBuffer(ByteBuffer &buffer){
		buffer >> name;
		buffer >> level;
		buffer >> exp;
		buffer >> cur_hp;
		buffer >> max_hp;
		buffer >> cur_mp;
		buffer >> max_mp;
		buffer >> attack;
		buffer >> defence;
		buffer >> magic;
		buffer >> magic_defence;
		buffer >> speed;
	}

	virtual void writeToBuffer(ByteBuffer &buffer){
		buffer << (uint32)Save_Role;

		buffer << name;
		buffer << level;
		buffer << exp;
		buffer << cur_hp;
		buffer << max_hp;
		buffer << cur_mp;
		buffer << max_mp;
		buffer << attack;
		buffer << defence;
		buffer << magic;
		buffer << magic_defence;
		buffer << speed;
	}

	virtual string toString(){
		std::ostringstream oss;

		oss << "RoleInfo====  ";
		oss << "  name : " << name;
		oss << ", level : " << level;
		oss << ", exp : " << exp;
		oss << ", cur_hp : " << cur_hp;
		oss << ", max_hp : " << max_hp;
		oss << ", cur_mp : " << cur_mp;
		oss << ", max_mp : " << max_mp;
		oss << ", attack : " << attack;
		oss << ", defence : " << defence;
		oss << ", magic : " << magic;
		oss << ", magic_defence : " << magic_defence;
		oss << ", speed : " << speed;

		return oss.str();
	}

};

// 物品Item信息
class GoodItemInfo : public BaseInfo{

public:
	uint32 id;
	string name;
	uint16 type;
	uint32 affectValue;
	uint16 num;


public:

	GoodItemInfo(){
		id = 0;
		name = "";
		type = (uint16)Good_Unkonw;
		affectValue = 0;
	}

	~GoodItemInfo(){

	}

	virtual void readFromBuffer(ByteBuffer &buffer){
		buffer >> id;
		buffer >> name;
		buffer >> type;
		buffer >> affectValue;
		buffer >> num;
	}

	virtual void writeToBuffer(ByteBuffer &buffer){
		buffer << id;
		buffer << name;
		buffer << type;
		buffer << affectValue;
		buffer << num;
	}

	virtual string toString(){
		std::ostringstream oss;

		oss << "GoodItemInfo====  ";
		oss << "  id : " << id;
		oss << ", name : " << name;
		oss << ", type : " << type;
		oss << ", affectValue : " << affectValue;
		oss << ", num : " << num;

		return oss.str();
	}

};

// 物品信息
class GoodInfo : public BaseInfo{
public:
	
	uint32 num;
	std::vector<GoodItemInfo> infoList;

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
		for(int i=0; i<num; i++){
			GoodItemInfo itemInfo;
			itemInfo.readFromBuffer(buffer);

			infoList.push_back(itemInfo);
		}
	}

	virtual void writeToBuffer(ByteBuffer &buffer){
		buffer << (uint32)Save_Good;

		size_t size = infoList.size();
		buffer << size;
		for(GoodItemInfo itemInfo : infoList){
			itemInfo.writeToBuffer(buffer);
		}
	}

	virtual string toString(){
		std::ostringstream oss;
		oss << "GoodInfo====  ";
		oss << "num : " << infoList.size() << "\n";
		for(GoodItemInfo itemInfo : infoList){
			oss << itemInfo.toString();
			oss << "\n";
		}
		return oss.str();
	}

};

// 装备Item信息
class EquipmentItemInfo : public BaseInfo{

public:
	uint32 id;
	string name;
	uint16 type;
	uint32 affect_attack; //攻击
	uint32 affect_magic; //灵力
	uint32 affect_hp; //血量
	uint32 affect_defence; //防御
	uint32 affect_speed; //速度

public:

	EquipmentItemInfo(){
		id = 0;
		name = "";
		type = (uint16)Equipment_Unknown;
		affect_attack = 0;
		affect_magic = 0;
		affect_hp = 0;
		affect_defence = 0;
		affect_speed = 0;
	}

	~EquipmentItemInfo(){

	}

	virtual void readFromBuffer(ByteBuffer &buffer){
		buffer >> id;
		buffer >> name;
		buffer >> type;
		buffer >> affect_attack;
		buffer >> affect_magic;
		buffer >> affect_hp;
		buffer >> affect_defence;
		buffer >> affect_speed;
	}

	virtual void writeToBuffer(ByteBuffer &buffer){
		buffer << id;
		buffer << name;
		buffer << type;
		buffer << affect_attack;
		buffer << affect_magic;
		buffer << affect_hp;
		buffer << affect_defence;
		buffer << affect_speed;
	}

	virtual string toString(){
		std::ostringstream oss;

		oss << "EquipmentItemInfo====  ";
		oss << "  id : " << id;
		oss << ", name : " << name;
		oss << ", type : " << type;
		oss << ", affect_attack : " << affect_attack;
		oss << ", affect_magic : " << affect_magic;
		oss << ", affect_hp : " << affect_hp;
		oss << ", affect_defence : " << affect_defence;
		oss << ", affect_speed : " << affect_speed;

		return oss.str();
	}

};

// 装备信息
class EquipmentInfo : public BaseInfo{
public:
	
	uint32 num;
	std::vector<EquipmentItemInfo> infoList;

public:

	EquipmentInfo(){
		infoList.clear();
		num = 0;
	}

	~EquipmentInfo(){
		infoList.clear();
	}

	virtual void readFromBuffer(ByteBuffer &buffer){
		buffer >> num;
		for(int i=0; i<num; i++){
			EquipmentItemInfo itemInfo;
			itemInfo.readFromBuffer(buffer);

			infoList.push_back(itemInfo);
		}
	}

	virtual void writeToBuffer(ByteBuffer &buffer){
		buffer << (uint32)Save_Equipment;

		size_t size = infoList.size();
		buffer << size;
		for(EquipmentItemInfo itemInfo : infoList){
			itemInfo.writeToBuffer(buffer);
		}
	}

	virtual string toString(){
		std::ostringstream oss;
		oss << "EquipmentInfo====  ";
		oss << "num : " << infoList.size() << "\n";
		for(EquipmentItemInfo itemInfo : infoList){
			oss << itemInfo.toString();
			oss << "\n";
		}
		return oss.str();
	}


};

#endif