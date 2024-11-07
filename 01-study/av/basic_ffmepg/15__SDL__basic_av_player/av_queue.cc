#include "av_queue.h"

template <typename T>
AvQueue<T>::AvQueue(){
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
    while (this->q.size() > 100){   // 队列长度大于100时等待
        SDL_CondWait(this->cond, this->mutex);
    }
    this->q.push(element);
    SDL_CondBroadcast(this->cond);
    SDL_UnlockMutex(this->mutex);
}

template <typename T>
T AvQueue<T>::pop(){
    SDL_LockMutex(this->mutex);
    while (this->q.size() <= 0){   // 队列长度大于100时等待
        SDL_CondWait(this->cond, this->mutex);
    }
    T ret = this->q.front();
    this->q.pop();
    SDL_CondBroadcast(this->cond);
    SDL_UnlockMutex(this->mutex);
}
