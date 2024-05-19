#ifndef MONSTER_HPP
#define MONSTER_HPP

class Hero;

class Monster{
private:
    int hp;         // 血量
    int atk;        // 攻击力
    int hp_max;
public:
    Monster();
    Monster(const int &hp, const int &atk);
    int getHp();
    int getHpmax();
    int getAtk();
    void attacked(const int &damage);
    bool attack(Hero &m);
};

#endif
