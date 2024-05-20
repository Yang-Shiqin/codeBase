#include <iostream>
#include "role.hpp"
using namespace std;

// 角色父类
Role::Role(){   // 默认构造函数, 血量5, 攻击力1
    this->hp = 5;
    this->atk = 2;
    this->hp_max = 5;
}
Role::Role(const int &hp, const int &atk=2): hp(hp), atk(atk){
    this->hp_max = hp;
}

int Role::getHp(){return this->hp;}
int Role::getHpmax(){return this->hp_max;}
int Role::getAtk(){return this->atk;}

void Role::attacked(const int &damage){   // 被攻击, 血量减少damage点
    this->hp -= damage;
    if (this->hp<=0){
        this->hp = 0;
    }
}

bool Role::attack(Role &m){    // 普通攻击, 对目标造成atk点伤害
    if(this->hp<=0){
        return false;
    }
    m.attacked(this->atk);
    return true;
}

// 怪物子类
Monster::Monster(): Role(){}
Monster::Monster(const int &hp, const int &atk=1): Role(hp, atk){}

void Monster::attacked(const int &damage){   // 被攻击, 血量减少damage点
    Role::attacked(damage);
    cout << "monster has been attacked, hp -" << damage << endl;
}

// 英雄子类
Hero::Hero(){   // 默认构造函数, 血量5, 蓝量5, 攻击力1
    this->hp = 5;
    this->mp = 5;
    this->atk = 1;
    this->mp_regen = 1;
    this->cold = 0;
    this->hp_max = 5;
    this->mp_max = 5;
}
Hero::Hero(const int &hp, const int &mp=5, const int &atk=1): Role(hp, atk), mp(mp){
    this->mp_regen = 1;
    this->cold = 0;
    this->hp_max = hp;
    this->mp_max = mp;
}

int Hero::getMp(){return this->mp;}
int Hero::getMpmax(){return this->mp_max;}
int Hero::getMpRegen(){return this->mp_regen;}
int Hero::getCold(){return this->cold;}
void Hero::setCold(int cold){this->cold = cold;}
void Hero::setMpRegen(int mp_regen){this->mp_regen = mp_regen;}
void Hero::mpRegen(){
    this->mp += this->mp_regen;
    if (this->mp>this->mp_max){
        this->mp = this->mp_max;
    }
};

bool Hero::regeneration(){        // 再生, 消耗1点蓝量, 回复3点血量
    if(this->mp<1 || this->hp<=0){
        return false;
    }
    this->mp--;
    this->hp += 3;
    if (this->hp>5){
        this->hp = 5;
    }
    return true;
}

bool Hero::magicAttackLow(Role &m){    // 低阶魔法攻击, 消耗2点蓝量, 对目标怪物造成2倍atk点伤害
    if(this->mp<2 || this->hp<=0){
        return false;
    }
    this->mp -= 2;
    m.attacked(2*this->atk);
    return true;
}

bool Hero::magicAttackHigh(Role &m){   // 高阶魔法攻击, 消耗2点蓝量, 对目标怪物造成3倍atk点伤害, 冷却3回合
    if(this->mp<2 || this->cold>0 || this->hp<=0){
        return false;
    }
    this->mp -= 2;
    this->cold = 3;
    m.attacked(3*this->atk);
    return true;
}

void Hero::attacked(const int &damage){   // 被攻击, 血量减少damage点
    Role::attacked(damage);
    cout << "hero has been attacked, hp -" << damage << endl;
}