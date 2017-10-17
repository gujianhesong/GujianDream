

#include "SaveSystem.h"
#include "GFileUtils.h"


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

	//readFromSave();

	//test();
}

void SaveSystem::createDefaultSave(){
	ByteBuffer saveBuffer;

	//头部信息
	uint32 type = Save_Header;
	saveBuffer << type;

	HeaderInfo headerInfo;
	headerInfo.id = 10001;
	headerInfo.name = "save_gujian_header";

	headerInfo.writeToBuffer(saveBuffer);

	//角色信息
	type = Save_Role;
	saveBuffer << type;

	RoleInfo roleInfo;
	roleInfo.name = "gujian";
	roleInfo.cur_hp = 954;
	roleInfo.max_mp = 1234;
	roleInfo.cur_mp = 1220;
	roleInfo.max_mp = 2566;

	roleInfo.writeToBuffer(saveBuffer);

	//物品信息
	type = Save_Good;
	saveBuffer << type;

	GoodInfo goodInfo;

	GoodItemInfo* jinchuangyao = new GoodItemInfo;
	jinchuangyao->id = 1001001;
	jinchuangyao->name = "jinchuangyao";
	jinchuangyao->type = Good_AddHp;
	jinchuangyao->affectValue = 1000;
	jinchuangyao->num = 10;
	goodInfo.infoList.push_back(jinchuangyao);

	GoodItemInfo* shexiang = new GoodItemInfo;
	shexiang->id = 1002001;
	shexiang->name = "shexiang";
	shexiang->type = Good_AddMp;
	shexiang->affectValue = 500;
	shexiang->num = 5;
	goodInfo.infoList.push_back(shexiang);

	goodInfo.writeToBuffer(saveBuffer);

	//尾部信息
	type = Save_End;
	saveBuffer << type;

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
	
	//头部信息
	readBuffer >> type;

	_pHeaderInfo = new HeaderInfo();
	_pHeaderInfo->readFromBuffer(readBuffer);

	readBuffer >> type;

	_pRoleInfo = new RoleInfo();
	_pRoleInfo->readFromBuffer(readBuffer);

	readBuffer >> type;

	_pGoodInfo = new GoodInfo();
	_pGoodInfo->readFromBuffer(readBuffer);

	readBuffer >> type;

	_pEndInfo = new EndInfo();
	_pEndInfo->readFromBuffer(readBuffer);

	
	//while(true){

	//	readBuffer >> type;
	//	log("type:aaa  %d", type);
	//	if(type == Save_End){
	//		log("type:1111111");
	//		//尾部信息
	//		_pEndInfo = new EndInfo();
	//		_pEndInfo->readFromBuffer(readBuffer);

	//		//读取完成，停止读取
	//		break;
	//	}

	//	if(type == Save_Role){
	//		log("type:2222222");
	//		//人物信息
	//		_pRoleInfo = new RoleInfo();
	//		_pRoleInfo->readFromBuffer(readBuffer);
	//	}

	//	if(type == Save_Good){
	//		log("type:33333333");
	//		//物品信息
	//		_pGoodInfo = new GoodInfo();
	//		_pGoodInfo->readFromBuffer(readBuffer);
	//	}

	//}
	

	log("header:%s", _pHeaderInfo->name.c_str());
	log("end:%s", _pEndInfo->name.c_str());
	log("roleInfo:%s,%d", _pRoleInfo->name, _pRoleInfo->max_hp);
	log("goodInfo:%d", _pGoodInfo->num);
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

	log("num12:%d,%d,%s,%s",num1,num4,name1.c_str(),name2.c_str());
}

SaveSystem::~SaveSystem(){
	
	_saveSystem = NULL;
}


