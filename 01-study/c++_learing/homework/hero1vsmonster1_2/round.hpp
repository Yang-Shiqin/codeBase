#ifndef ROUND_HPP
#define ROUND_HPP
#include <string>
#include "role.hpp"

class Round1v1{     // 一个英雄vs一个怪物对战回合管理类
private:
    Role *m;
    Hero *h;
    int rounds;
    int whose_turn; // 0为未开始, 1为玩家回合, 2为怪物回合, 3为回合结束
    int m_hp_max;
    int h_hp_max;
    int h_mp_max;
    int hero_choice;    // 玩家选择的英雄技能

    void chooseLine(int no, const std::string &str);   // 选择高亮行, 画图用
    void drawBattle(int round, int hero_hp, int hero_max_hp, int hero_mp, int hero_max_mp, int hero_atk,
                int monster_hp, int monster_max_hp, int monster_atk, int mp_regen, int cold);
public:
    Round1v1(Role *m, Hero *h);

    void showInfo();    // 显示英雄和怪物信息
    void run();         // 开始一场对战
};
#endif