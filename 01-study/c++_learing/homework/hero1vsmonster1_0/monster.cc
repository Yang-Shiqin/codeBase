#include "monster.hpp"
#include "hero.hpp"

Monster::Monster(){
    this->hp = 5;
    this->atk = 2;
    this->hp_max = 5;
}
Monster::Monster(const int &hp, const int &atk=2): hp(hp), atk(atk){
    this->hp_max = hp;
}

int Monster::getHp(){return this->hp;}
int Monster::getHpmax(){return this->hp_max;}
int Monster::getAtk(){return this->atk;}
void Monster::attacked(const int &damage){
    this->hp -= damage;
    if (this->hp<=0){
        this->hp = 0;
    }
}

bool Monster::attack(Hero &m){    // 普通攻击, 对目标英雄造成atk点伤害
    if(this->hp<=0){
        return false;
    }
    m.attacked(this->atk);
    return true;
}