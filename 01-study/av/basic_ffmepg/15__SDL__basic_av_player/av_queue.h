#pragma once
#include <queue>

// 流队列，如音/视频编码数据包队列、音/视频帧队列
template <typename T>
class AvQueue
{
private:
    std::queue<T> q;  // 队列
    SDL_mutex *mutex; // 互斥锁
    SDL_cond *cond;   // 条件变量
public:
    AvQueue();
    ~AvQueue();
    void push(T element);
    T pop();
};