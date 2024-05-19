# ifndef HERO_HPP
# define HERO_HPP
class Monster;

class Hero{
private:
    int hp;         // 血量
    int mp;         // 蓝量
    int atk;        // 攻击力
    int mp_regen;   // 蓝量回复力
    int cold;       // 高阶魔法攻击剩余冷却时间
    int hp_max;
    int mp_max;
public:
    Hero();
    Hero(const int &hp, const int &mp, const int &atk);
    int getHp();
    int getHpmax();
    int getMp();
    int getMpmax();
    int getAtk();
    int getMpRegen();
    int getCold();
    void setCold(int cold);
    void setMpRegen(int mp_regen);
    void attacked(const int &damage);
    void mpRegen();
    bool attack(Monster &m);
    bool regeneration();
    bool magicAttackLow(Monster &m);
    bool magicAttackHigh(Monster &m);
};

# endif