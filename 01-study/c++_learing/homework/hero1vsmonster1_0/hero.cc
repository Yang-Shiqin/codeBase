#include "hero.hpp"
#include "monster.hpp"

Hero::Hero(){
    this->hp = 5;
    this->mp = 5;
    this->atk = 1;
    this->mp_regen = 1;
    this->cold = 0;
    this->hp_max = 5;
    this->mp_max = 5;
}
Hero::Hero(const int &hp, const int &mp=5, const int &atk=1): hp(hp), mp(mp), atk(atk){
    this->mp_regen = 1;
    this->cold = 0;
    this->hp_max = hp;
    this->mp_max = mp;
}

int Hero::getHp(){return this->hp;}
int Hero::getHpmax(){return this->hp_max;}
int Hero::getMp(){return this->mp;}
int Hero::getMpmax(){return this->mp_max;}
int Hero::getAtk(){return this->atk;}
int Hero::getMpRegen(){return this->mp_regen;}
int Hero::getCold(){return this->cold;}
void Hero::setCold(int cold){this->cold = cold;}
void Hero::setMpRegen(int mp_regen){this->mp_regen = mp_regen;}
void Hero::attacked(const int &damage){
    this->hp -= damage;
    if (this->hp<=0){
        this->hp = 0;
    }
}
void Hero::mpRegen(){
    this->mp += this->mp_regen;
    if (this->mp>this->mp_max){
        this->mp = this->mp_max;
    }
};

bool Hero::attack(Monster &m){    // 普通攻击, 对目标怪物造成atk点伤害
    if(this->hp<=0){
        return false;
    }
    m.attacked(this->atk);
    return true;
}

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

bool Hero::magicAttackLow(Monster &m){    // 低阶魔法攻击, 消耗2点蓝量, 对目标怪物造成2倍atk点伤害
    if(this->mp<2 || this->hp<=0){
        return false;
    }
    this->mp -= 2;
    m.attacked(2*this->atk);
    return true;
}

bool Hero::magicAttackHigh(Monster &m){   // 高阶魔法攻击, 消耗2点蓝量, 对目标怪物造成3倍atk点伤害, 冷却3回合
    if(this->mp<2 || this->cold>0 || this->hp<=0){
        return false;
    }
    this->mp -= 2;
    this->cold = 3;
    m.attacked(3*this->atk);
    return true;
}