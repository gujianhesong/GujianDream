

#include "SaveSystem.h"
#include "GFileUtils.h"
#include "NameUtil.h"


static SaveSystem* _saveSystem = nullptr;

const string _savePath = "default_game.sav";

SaveSystem* SaveSystem::getInstance(){
	if (_saveSystem == nullptr){
		_saveSystem = new SaveSystem();
	}
	return _saveSystem;
}

SaveSystem::SaveSystem():
	_pHeaderInfo(nullptr),
	_pEndInfo(nullptr),
	_pRoleInfo(nullptr),
	_pGoodInfo(nullptr)
{
	createDefaultSave();

	readFromSave();

	//test();
}

void SaveSystem::createDefaultSave(){
	ByteBuffer saveBuffer;

	//头部信息
	HeaderInfo headerInfo;
	headerInfo.id = 10001;
	headerInfo.name = "save_gujian_header";

	headerInfo.writeToBuffer(saveBuffer);

	//角色信息
	RoleInfo roleInfo;
	roleInfo.name = "gujian";
	roleInfo.level = 1;
	roleInfo.exp = 0;
	roleInfo.cur_hp = 400;
	roleInfo.max_hp = 500;
	roleInfo.cur_mp = 200;
	roleInfo.max_mp = 300;
	roleInfo.attack = 500;
	roleInfo.defence = 230;
	roleInfo.magic = 400;
	roleInfo.magic_defence = 300;
	roleInfo.speed = 100;

	roleInfo.writeToBuffer(saveBuffer);

	//物品信息
	GoodInfo goodInfo;

	GoodItemInfo jinchuangyao;
	jinchuangyao.id = 1001001;
	jinchuangyao.name = "jinchuangyao";
	jinchuangyao.type = Good_AddHp;
	jinchuangyao.affectValue = 1000;
	jinchuangyao.num = 10;
	goodInfo.infoList.push_back(jinchuangyao);

	GoodItemInfo shexiang;
	shexiang.id = 1002001;
	shexiang.name = "shexiang";
	shexiang.type = Good_AddMp;
	shexiang.affectValue = 500;
	shexiang.num = 5;
	goodInfo.infoList.push_back(shexiang);

	goodInfo.writeToBuffer(saveBuffer);

	//装备信息
	EquipmentInfo equipmentInfo;

	// 武器
	EquipmentItemInfo weapon;
	weapon.id = 1002001;
	weapon.name = "duanjian";
	weapon.type = Equipment_Weapon;
	weapon.affect_attack = 102;
	equipmentInfo.infoList.push_back(weapon);

	// 帽子
	EquipmentItemInfo head;
	head.id = 1002001;
	head.name = "bumao";
	head.type = Equipment_Head;
	head.affect_defence = 50;
	equipmentInfo.infoList.push_back(head);

	// 项链
	EquipmentItemInfo neck;
	neck.id = 1002001;
	neck.name = "wuseshi";
	neck.type = Equipment_Neck;
	neck.affect_magic = 30;
	equipmentInfo.infoList.push_back(neck);

	// 衣服
	EquipmentItemInfo cloth;
	cloth.id = 1002001;
	cloth.name = "buyi";
	cloth.type = Equipment_Cloth;
	cloth.affect_defence = 80;
	equipmentInfo.infoList.push_back(cloth);

	// 腰带
	EquipmentItemInfo belt;
	belt.id = 1002001;
	belt.name = "buyaodai";
	belt.type = Equipment_Belt;
	belt.affect_hp = 180;
	belt.affect_defence = 20;
	equipmentInfo.infoList.push_back(belt);

	// 鞋子
	EquipmentItemInfo shoes;
	shoes.id = 1002001;
	shoes.name = "buxie";
	shoes.type = Equipment_Shoes;
	shoes.affect_speed = 20;
	shoes.affect_defence = 20;
	equipmentInfo.infoList.push_back(shoes);

	equipmentInfo.writeToBuffer(saveBuffer);

	//尾部信息
	EndInfo endInfo;
	endInfo.id = 10001;
	endInfo.name = "save_gujian_end";

	endInfo.writeToBuffer(saveBuffer);

	//保存到文件
	GFileUtils::saveBuffer(_savePath, saveBuffer);
}

void SaveSystem::readFromDefaultSave(){

	
}

void SaveSystem::readFromSave(){

	ByteBuffer readBuffer = GFileUtils::readBuffer(_savePath);

	uint32 type = 0;
	
	while(true){

		readBuffer >> type;
		Glog("type:aaa  %d", type);

		if(type == Save_Header){
			//头部信息
			_pHeaderInfo = new HeaderInfo();
			_pHeaderInfo->readFromBuffer(readBuffer);
		}

		if(type == Save_End){
			//尾部信息
			_pEndInfo = new EndInfo();
			_pEndInfo->readFromBuffer(readBuffer);

			//读取完成，停止读取
			break;
		}

		if(type == Save_Role){
			Glog("type:2222222");
			//人物信息
			_pRoleInfo = new RoleInfo();
			_pRoleInfo->readFromBuffer(readBuffer);
		}

		if(type == Save_Good){
			Glog("type:33333333");
			//物品信息
			_pGoodInfo = new GoodInfo();
			_pGoodInfo->readFromBuffer(readBuffer);
		}

		if(type == Save_Equipment){
			Glog("type:444");
			//装备信息
			_pEquipmentInfo = new EquipmentInfo();
			_pEquipmentInfo->readFromBuffer(readBuffer);
		}

	}

	Glog("%s", _pRoleInfo->toString().c_str());
	Glog("%s", _pGoodInfo->toString().c_str());
	Glog("%s", _pEquipmentInfo->toString().c_str());
}

void SaveSystem::test(){
	rapidjson::Document m_doc;
	std::string jsonpath = FileUtils::getInstance()->fullPathForFilename("game_data/game.save");
	std::string contentStr = FileUtils::getInstance()->getStringFromFile(jsonpath);
	printf("%s\n",contentStr.c_str());

	ByteBuffer saveBuffer;
	uint8 num1 = 100;
	uint16 num2 = 100;
	uint32 num3 = 100;
	uint32 num4 = 100;
	std::string name1 = "hesong";
	std::string name2 = "mianyan";

	saveBuffer << num1;
	saveBuffer << num2;
	saveBuffer << num3;
	saveBuffer << num4;
	saveBuffer << name1;
	saveBuffer << name2;

	string savePath = "game.sav";

	GFileUtils::saveBuffer(savePath, saveBuffer);

	ByteBuffer readBuffer = GFileUtils::readBuffer(savePath);

	readBuffer >> num1;
	readBuffer >> num2;
	readBuffer >> num3;
	readBuffer >> num4;
	readBuffer >> name1;
	readBuffer >> name2;

	Glog("num12:%d,%d,%s,%s",num1,num4,name1.c_str(),name2.c_str());
}

SaveSystem::~SaveSystem(){
	
	_saveSystem = NULL;
}


