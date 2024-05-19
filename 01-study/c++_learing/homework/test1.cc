#include <iostream>
#include <vector>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

// 清空屏幕函数
void clearScreen() {
    std::cout << "\033[2J\033[1;1H";
}

// 打印选项函数
void printOptions(const std::vector<std::string>& options, int currentSelection) {
    clearScreen();
    for (size_t i = 0; i < options.size(); ++i) {
        if (i == currentSelection) {
            std::cout << "\033[1;32m> " << options[i] << "\033[0m" << std::endl;  // 绿色高亮
        } else {
            std::cout << "  " << options[i] << std::endl;
        }
    }
}

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

int main() {
    std::vector<std::string> options = {
        "Option 1",
        "Option 2",
        "Option 3"
    };
    int currentSelection = 0;

    // setNonBlocking(true);

    printOptions(options, currentSelection);

    while (true) {
        char ch;
        if (read(STDIN_FILENO, &ch, 1) > 0) {  // 检测按键输入
            if (ch == 27) { // 检测到 ESC 键
                char seq[2];
                if (read(STDIN_FILENO, &seq[0], 1) > 0 && read(STDIN_FILENO, &seq[1], 1) > 0) {
                    if (seq[0] == '[') {
                        if (seq[1] == 'A') {  // 上箭头键
                            currentSelection = (currentSelection - 1 + options.size()) % options.size();
                        } else if (seq[1] == 'B') {  // 下箭头键
                            currentSelection = (currentSelection + 1) % options.size();
                        }
                    }
                }
            } else if (ch == '\n') {  // 回车键
                clearScreen();
                std::cout << "You selected: " << options[currentSelection] << std::endl;
                break;
            }
            printOptions(options, currentSelection);
        }
    }

    // setNonBlocking(false);

    return 0;
}

