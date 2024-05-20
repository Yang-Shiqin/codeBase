#ifndef ROLE_HPP
#define ROLE_HPP


class Role{
protected:
    int hp;         // 血量
    int atk;        // 攻击力
    int hp_max;
public:
    Role();
    Role(const int &hp, const int &atk);
    int getHp();
    int getHpmax();
    int getAtk();
    void attacked(const int &damage);
    bool attack(Role &m);
};

class Monster: public Role{
public:
    Monster();
    Monster(const int &hp, const int &atk);
};

class Hero: public Role{
private:
    int mp;         // 蓝量
    int mp_regen;   // 蓝量回复力
    int cold;       // 高阶魔法攻击剩余冷却时间
    int mp_max;
public:
    Hero();
    Hero(const int &hp, const int &mp, const int &atk);
    int getMp();
    int getMpmax();
    int getMpRegen();
    int getCold();
    void setCold(int cold);
    void setMpRegen(int mp_regen);
    void mpRegen();
    bool regeneration();
    bool magicAttackLow(Role &m);
    bool magicAttackHigh(Role &m);
};

#endif