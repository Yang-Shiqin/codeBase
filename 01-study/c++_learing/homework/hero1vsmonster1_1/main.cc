/***********增加继承, 重新整理了下代码
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
    Monster m;
    Hero h;
    Round1v1 r(&m, &h);
    r.run();
    return 0;
}

