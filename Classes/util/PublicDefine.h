
#ifndef gujian_PublicDefine_h
#define gujian_PublicDefine_h

#include "cocos2d.h"
#include "PublicSourceDefine.h"
#include "CsvUtil.h"

using namespace cocos2d;

#define CREATE_INSTANCE_FUNC(__CLASS__) \
private:\
    static __CLASS__ *_gInstance;\
public:\
static __CLASS__* getInstance()\
{\
    if(!_gInstance)\
    {\
        _gInstance = new __CLASS__();\
        if(_gInstance && _gInstance->init())\
        {\
            _gInstance->autorelease();\
            _gInstance->retain();\
        }\
        else\
        {\
            CC_SAFE_DELETE(_gInstance);\
        }\
    }\
    return _gInstance;\
}

#define DESTROY_INSTANCE_FUNC()\
static void destroyInstance()\
{\
    CC_SAFE_RELEASE_NULL(_gInstance);\
}

#define NOTIFY cocos2d::NotificationCenter::getInstance()

enum CsvRoleProperty
{
    en_role_id = 0,
	en_role_name,
	en_role_weapon,
    en_role_level,
    en_role_hp,
    en_role_mp,
	en_role_attack,
    en_role_speed,
	en_role_visual_range,
	en_role_attack_range,
};

enum CsvMonsterProperty
{
	en_monster_id = 0,
	en_monster_name,
	en_monster_level,
	en_monster_hp,
	en_monster_mp,
	en_monster_attack,
	en_monster_speed,
	en_monster_visual_range,
	en_monster_attack_range,
	en_monster_active,
	en_monster_can_move,
};

enum CsvRolePositionProperty
{
	en_role_texture_id,
	en_role_texture_nick_name,
	en_role_texture_state,
	en_role_texture_center_x,
	en_role_texture_center_y,
};

enum CsvWeponPositionProperty
{
	en_weapon_texture_id,
	en_weapon_texture_nick_name,
	en_weapon_texture_state,
	en_weapon_texture_center_x,
	en_weapon_texture_center_y,
};



#endif
