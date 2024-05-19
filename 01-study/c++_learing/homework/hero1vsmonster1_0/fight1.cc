/***********
 * 英雄类
 * 怪物类
 * 回合管理类
 **********/


#include <iostream>
#include <iomanip> // 用于控制输出格式
#include <string> // 用于字符串操作
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include "hero.hpp"
#include "monster.hpp"
using namespace std;

// 设置终端为非阻塞模式
void setNonBlocking(bool enable) {
    struct termios ttystate;
    tcgetattr(STDIN_FILENO, &ttystate);
    if (enable) {
        ttystate.c_lflag &= ~ICANON;
        ttystate.c_cc[VMIN] = 1;
    } else {
        ttystate.c_lflag |= ICANON;
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
    fcntl(STDIN_FILENO, F_SETFL, enable ? O_NONBLOCK : 0);
}


class Round1v1{     // 一个英雄vs一个怪物对战回合管理类
private:
    Monster *m;
    Hero *h;
    int rounds;
    int whose_turn; // 0为未开始, 1为玩家回合, 2为怪物回合, 3为回合结束
    int m_hp_max;
    int h_hp_max;
    int h_mp_max;
    int hero_choice;    // 玩家选择的英雄技能
public:
    Round1v1(Monster *m, Hero *h): m(m), h(h){
        this->rounds = 0;
        this->whose_turn = 0;
        this->m_hp_max = m->getHpmax();
        this->h_hp_max = h->getHpmax();
        this->h_mp_max = h->getMpmax();
        this->hero_choice = 0;
    }

    void chooseLine(int no, const string &str){
        if (this->hero_choice == no) {
            cout << "\033[1;32m" << str << "\033[0m" << endl;
        } else {
            cout << str << endl;
        }
    }

    void drawBattle(int round, int hero_hp, int hero_max_hp, int hero_mp, int hero_max_mp, int hero_atk,
                int monster_hp, int monster_max_hp, int monster_atk, int mp_regen, int cold) {
        cout << "\033[2J\033[1;1H";  // 清屏
        cout << "press q to exit" << endl;
        cout << "┌────────────────────────────────────────────────┐" << endl;
        cout << "|                   Round " << left << setw(23) << round << "|" << endl;
        cout << "├───────────────────────┬────────────────────────┤" << endl;
        cout << "|          Hero         |        Monster         |" << endl;
        cout << "| HP:  " << left << setw(17) << to_string(hero_hp)+"/"+to_string(hero_max_hp)
        << "| HP:  " << left << setw(17) << to_string(monster_hp)+"/"+to_string(monster_max_hp) << " |" << endl;
        cout << "| MP:  " << left << setw(17) << to_string(hero_mp)+"/"+to_string(hero_max_mp)
        << "| ATK: " << left << setw(18) << monster_atk << "|" << endl;
        cout << "| ATK: " << left << setw(12) << hero_atk << " O   |                        |" << endl;
        cout << "| MP_regen: " << left << setw(7) << mp_regen << "/|\\  |                /\\/\\    |" << endl;
        cout << "| cold: " << left << setw(11) << cold << "/ \\  |              >(o  o)<  |" << endl;
        cout << "├───────────────────────┴────────────────────────┤" << endl;
        this->chooseLine(0, "| 1. Normal Attack                               |");
        this->chooseLine(1, "| 2. Magic Attack(low): mp-2                     |");
        this->chooseLine(2, "| 3. Magic Attack(high): mp-2                    |");
        this->chooseLine(3, "| 4. Regeneration: mp-1                          |");
        cout << "└────────────────────────────────────────────────┘" << endl;
    }

    void showInfo(){
        drawBattle(this->rounds, this->h->getHp(), this->h_hp_max, 
            this->h->getMp(), this->h_mp_max, this->h->getAtk(),
            this->m->getHp(), this->m_hp_max, this->m->getAtk(), 
            this->h->getMpRegen(), this->h->getCold());
    }

    void run(){
        char ch;
        setNonBlocking(true);   // 设置终端为非阻塞模式(即不等待用户输入回车立即返回)
        while(true){
            if (read(STDIN_FILENO, &ch, 1) > 0) { 
                if (ch == 'q') {
                    break;
                } else if (ch == '\n') {
                    std::cout << "You pressed Enter." << std::endl;
                } else if (ch == 27) { // 检测到 ESC 键
                    char seq[2];
                    if (read(STDIN_FILENO, &seq[0], 1) > 0 && read(STDIN_FILENO, &seq[1], 1) > 0) {
                        if (seq[0] == '[') {
                            if (seq[1] == 'A') {  // 上箭头键
                                this->hero_choice = (this->hero_choice - 1 + 4) % 4;
                            } else if (seq[1] == 'B') {  // 下箭头键
                                this->hero_choice = (this->hero_choice + 1) % 4;
                            }
                        }
                    }
                }
                this->showInfo();
            }
            if (ch == '\n') {  // 回车键
                this->rounds++;         // 回合数加1
                if (this->rounds % 10 == 0) { // 每10回合英雄回蓝能力加1
                    this->h->setMpRegen(this->h->getMpRegen()+1);
                }
                this->h->setCold(max(0, this->h->getCold()-1));    // 英雄技能冷却时间减1
                this->h->mpRegen();     // 英雄蓝量回复
                bool suc = false;
                switch (this->hero_choice)  // 根据玩家选择的技能进行相应操作
                {
                case 0:
                    suc = this->h->attack(*this->m);
                    break;
                case 1:
                    suc = this->h->magicAttackLow(*this->m);
                    break;
                case 2:
                    suc = this->h->magicAttackHigh(*this->m);
                    break;
                case 3:
                    suc = this->h->regeneration();
                    break;
                default:
                    break;
                }
                this->showInfo();   // 显示战斗信息
                if (suc) {
                    cout << "You selected: " << this->hero_choice << "and succeed"<< endl;
                }else{
                    cout << "You selected: " << this->hero_choice << "and failed"<< endl;
                }
                sleep(1);
                whose_turn = 2;     // 轮到怪物回合
                suc = this->m->attack(*this->h);    // 怪物攻击英雄
                this->showInfo();   // 显示战斗信息
                if (suc) {
                    cout << "Monster attack you and succeed"<< endl;
                    whose_turn = 1;         // 轮到英雄回合
                }else{
                    cout << "Monster died and you win"<< endl;
                    this->whose_turn = 3;   // 回合结束
                    break;
                }
                sleep(1);
                ch = 0;
            }

        }
        setNonBlocking(false);  // 设置终端为阻塞模式
    }
};

int main()
{
    Monster m;
    Hero h;
    Round1v1 r(&m, &h);
    r.showInfo();
    r.run();
    // char c;
    // c = getchar();
    // cout << c << endl;
    return 0;
}

