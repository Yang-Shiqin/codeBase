/***********增加多态(可能两个英雄互殴, 只是对面英雄只会攻击(因为没玩家操作))
 * 角色类
 *  英雄类
 *  怪物类
 * 回合管理类
 **********/


#include <iostream>
#include "role.hpp"
#include "round.hpp"

using namespace std;

int main()
{
    Role *m = new Monster();
    Hero *h = new Hero(5, 5, 1);
    Round1v1 r(m, h);
    r.run();
    return 0;
}

