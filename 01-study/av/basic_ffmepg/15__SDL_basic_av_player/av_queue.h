#pragma once
#include <queue>
#include <algorithm>

extern "C"
{
#include <SDL2/SDL.h>
}

// 流队列，如音/视频编码数据包队列、音/视频帧队列
template <typename T>
class AvQueue
{
private:
    std::queue<T> q;  // 队列
    const std::size_t q_len = 100;     // 队列长度
    SDL_mutex *mutex; // 互斥锁
    SDL_cond *cond;   // 条件变量
public:
    AvQueue();
    AvQueue(std::size_t q_len);
    AvQueue(const AvQueue&) = delete;
    AvQueue& operator=(const AvQueue&) = delete;
    ~AvQueue();
    void push(T element);
    T pop();
    int size(){
        size_t ret;
        SDL_LockMutex(this->mutex);
        ret = this->q.size();
        SDL_UnlockMutex(this->mutex);
        return ret;
    }
};

template <typename T>
class AvBufferQueue{
private:
    T* q;
    const std::size_t q_len = 100;
    std::size_t q_size;
    SDL_mutex *mutex;
    SDL_cond *cond;
    int head;
    int tail;
public:
    AvBufferQueue();
    AvBufferQueue(std::size_t q_len);
    AvBufferQueue(const AvBufferQueue&) = delete;
    AvBufferQueue& operator=(const AvBufferQueue&) = delete;
    ~AvBufferQueue();
    void push(T* element, std::size_t len);
    void pop(T* element, std::size_t len);
    std::size_t size(){
        size_t ret;
        SDL_LockMutex(this->mutex);
        ret = this->q_size;
        SDL_UnlockMutex(this->mutex);
        return ret;
    }
};

template <typename T>
AvQueue<T>::AvQueue(){
    this->mutex = SDL_CreateMutex();
    this->cond = SDL_CreateCond();
}

template <typename T>
AvQueue<T>::AvQueue(std::size_t q_len): q_len(q_len){
    this->mutex = SDL_CreateMutex();
    this->cond = SDL_CreateCond();
}

template <typename T>
AvQueue<T>::~AvQueue(){
    SDL_DestroyMutex(this->mutex);
    SDL_DestroyCond(this->cond);
}

template <typename T>
void AvQueue<T>::push(T element){
    SDL_LockMutex(this->mutex);
    while (this->q.size() >= this->q_len){   // 队列长度大于等于最大长度时等待
        SDL_CondWait(this->cond, this->mutex);
    }
    this->q.push(element);
    SDL_CondBroadcast(this->cond);
    SDL_UnlockMutex(this->mutex);
}

template <typename T>
T AvQueue<T>::pop(){
    SDL_LockMutex(this->mutex);
    while (this->q.empty()){   // 队列长度小于0时等待
        SDL_CondWait(this->cond, this->mutex);
    }
    T ret = this->q.front();
    this->q.pop();
    SDL_CondBroadcast(this->cond);
    SDL_UnlockMutex(this->mutex);
    return ret;
}

template <typename T>
AvBufferQueue<T>::AvBufferQueue(){
    this->q = new T[this->q_len];
    this->q_size = 0;
    this->head = 0;     // 队列头，左边，指向第一个元素
    this->tail = 0;     // 队列尾，右边，指向最后一个元素后一个位置
    this->mutex = SDL_CreateMutex();
    this->cond = SDL_CreateCond();
}

template <typename T>
AvBufferQueue<T>::AvBufferQueue(std::size_t q_len): q_len(q_len){
    this->q = new T[this->q_len];
    this->q_size = 0;
    this->head = 0;
    this->tail = 0;
    this->mutex = SDL_CreateMutex();
    this->cond = SDL_CreateCond();
}

template <typename T>
AvBufferQueue<T>::~AvBufferQueue(){
    delete[] this->q;
    SDL_DestroyMutex(this->mutex);
    SDL_DestroyCond(this->cond);
}

template <typename T>
void AvBufferQueue<T>::push(T* element, std::size_t len){
    SDL_LockMutex(this->mutex);
    while (this->q_size+len >= this->q_len){
        SDL_CondWait(this->cond, this->mutex);
    }
    int l = std::min(len, this->q_len - this->tail);
    memcpy(this->q + this->tail, element, l);
    memcpy(this->q, element + l, len - l);
    this->tail = (this->tail + len) % this->q_len;
    this->q_size += len;
    SDL_CondBroadcast(this->cond);
    SDL_UnlockMutex(this->mutex);
}

template <typename T>
void AvBufferQueue<T>::pop(T* element, std::size_t len){
    SDL_LockMutex(this->mutex);
    while (this->q_size < len){
        SDL_CondWait(this->cond, this->mutex);
    }
    int l = std::min(len, this->q_len - this->head);
    memcpy(element, this->q + this->head, l);
    memcpy(element + l, this->q, len - l);
    this->head = (this->head + len) % this->q_len;
    this->q_size -= len;
    SDL_CondBroadcast(this->cond);
    SDL_UnlockMutex(this->mutex);
}