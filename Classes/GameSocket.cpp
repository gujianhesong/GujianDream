//
//  GameSocket.cpp
//  guyue
//
//  Created by dsh on 15/1/8.
//
//

#include "GameSocket.h"
#include "PlayerController.h"
#include "RoleCreateLayer.h"
#include "Player.h"
#include "GameScene.h"
void GameSocket::sendRoleCreate(Node* node, int roleID, std::string nickName, int weaponId)
{
    PlayerController* _playerController = PlayerController::sharePlayerController();
    _playerController->setPlayerNumber(roleID);
    _playerController->setWeaponsNumber(1000);
    _playerController->setPlayerName(nickName);
	_playerController->setWeaponsNumber(weaponId);
    RoleCreateLayer* layer = (RoleCreateLayer*)node;
    layer->joinGame();
}

void GameSocket::attackGroup(Monomer *one, vector<Monomer *> two, int skillNumber)
{
    if (one == NULL || two.empty())
        return;
    
    int hurt = one->getTheAttack();
    if (skillNumber == 2005)
    {
        hurt = hurt * 15;
    }
    float r = CCRANDOM_0_1();
    
    hurt += (int)((hurt/10) * r) - hurt/20;
    
    std::vector<Monomer*>::iterator itr;
    for (itr=two.begin(); itr!=two.end(); itr++)
    {
        if ((*itr)->getBlood() <= 0)
            continue;
        
        int blood = (*itr)->getBlood() - hurt;
        
        blood = MAX(blood, 0);
        
        (*itr)->addAgainstMe(one, blood);
    }
}

void GameSocket::attack(Monomer *one, Monomer *two, int skillNumber)
{
    if (one ==NULL || two == NULL)
        return;
    
    if (two->getBlood() <= 0)
        return;
    
    int hurt = one->getTheAttack();
    if (skillNumber == 1020)
    {
        hurt = hurt * 1.5;
    }
    
    hurt += (int)((hurt/10) * CCRANDOM_0_1()) - hurt/20;
    
    int blood = two->getBlood() - hurt;
    
    blood = MAX(blood, 0);
    
    two->addAgainstMe(one, blood);
}