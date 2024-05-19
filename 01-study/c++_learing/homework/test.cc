#include <iostream>
#include <iomanip> // 用于控制输出格式
#include <string> // 用于字符串操作
using namespace std;

void drawBattle(int round, int hero_hp, int hero_max_hp, int hero_mp, int hero_max_mp, int hero_atk,
                int monster_hp, int monster_max_hp, int monster_atk, int mp_regen, int cold) {
    cout << "┌────────────────────────────────────────────────┐" << endl;
    cout << "|                   Round " << left << setw(23) << round << "|" << endl;
    cout << "├───────────────────────┬────────────────────────┤" << endl;
    cout << "|          Hero         |        Monster         |" << endl;
    cout << "| HP:  " << left << setw(17) << to_string(hero_hp)+"/"+to_string(hero_max_hp)
     << "| HP:  " << left << setw(17) << to_string(monster_hp)+"/"+to_string(monster_max_hp) << " |" << endl;
    cout << "| MP:  " << left << setw(17) << to_string(hero_mp)+"/"+to_string(hero_max_mp)
     << "| ATK: " << left << setw(18) << monster_atk << "|" << endl;
    cout << "| ATK: " << left << setw(17) << hero_atk << "|                        |" << endl;
    cout << "| MP_regen: " << left << setw(12) << mp_regen << "|                /\\/\\    |" << endl;
    cout << "| cold: " << left << setw(16) << cold << "|              >(o  o)<  |" << endl;
    cout << "├───────────────────────┴────────────────────────┤" << endl;
    cout << "| 1. Normal Attack                               |" << endl;
    cout << "| 2. Magic Attack(low): mp-2                     |" << endl;
    cout << "| 3. Magic Attack(high): mp-2                    |" << endl;
    cout << "| 4. Regeneration: mp-1                          |" << endl;
    cout << "└────────────────────────────────────────────────┘" << endl;
}

int main() {
    // 假设这些是具体数值
    int round = 11111;
    int hero_hp = 1000;
    int hero_max_hp = 150;
    int hero_mp = 100;
    int hero_max_mp = 1000;
    int hero_atk = 20;
    int monster_hp = 20000;
    int monster_max_hp = 200;
    int monster_atk = 30;
    int mp_regen = 5;
    int cold = 2;

    // 绘制战斗图
    drawBattle(round, hero_hp, hero_max_hp, hero_mp, hero_max_mp, hero_atk,
               monster_hp, monster_max_hp, monster_atk, mp_regen, cold);

    return 0;
}

