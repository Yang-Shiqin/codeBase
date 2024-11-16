在 Basic AV Player 的基础上实现更精细的同步

~~增加堆 `av_queue.h`、`av_processor.h`~~

## 本来想加堆，但似乎没用
`av_queue.h`:
```cpp
/* 支持多线程的单个元素进队出队的队列或堆，用于如音/视频编码数据包队列、视频帧队列(需按pts排序) */
// 但解码器似乎会自己维护pts，所以不需要排序, 所以堆也没用
template <typename T, typename Container = std::queue<T>>
class AvQueue
{
private:
    Container q;    // 队列
    const std::size_t q_len = 100;  // 队列长度
    SDL_mutex *mutex;   // 互斥锁
    SDL_cond *cond;     // 条件变量
    int running = 1;    // 用于外部停止队列的阻塞
public:
    AvQueue();
    AvQueue(std::size_t q_len);
    // 禁用拷贝构造函数和赋值运算符(浅拷贝对于互斥锁和条件变量是不安全的)
    AvQueue(const AvQueue&) = delete;
    AvQueue& operator=(const AvQueue&) = delete;
    ~AvQueue();
    void push(T element);   // 进队
    T pop();                // 出队
    int size(){
        return this->q.size();
    }
    void stop(){    // 用于外部停止队列的阻塞
        this->running = 0;
        SDL_CondBroadcast(this->cond);
    }
};

template <typename T, typename Container>
AvQueue<T, Container>::AvQueue(){
    this->mutex = SDL_CreateMutex();
    this->cond = SDL_CreateCond();
}

template <typename T, typename Container>
AvQueue<T, Container>::AvQueue(std::size_t q_len): q_len(q_len){
    this->mutex = SDL_CreateMutex();
    this->cond = SDL_CreateCond();
}

template <typename T, typename Container>
AvQueue<T, Container>::~AvQueue(){
    SDL_DestroyMutex(this->mutex);
    SDL_DestroyCond(this->cond);
}

// 入队, 将element放入this->q, 并唤醒等待的线程(this->running=0时不保证正确性)
template <typename T, typename Container>
void AvQueue<T, Container>::push(T element){
    SDL_LockMutex(this->mutex);
    while (this->running && this->q.size() >= this->q_len){   // 队列长度大于等于最大长度时等待
        SDL_CondWait(this->cond, this->mutex);
    }
    if (!this->running){
        SDL_UnlockMutex(this->mutex);
        return;
    }
    this->q.push(element);
    SDL_CondBroadcast(this->cond);
    SDL_UnlockMutex(this->mutex);
}

// 出队, 从this->q pop出一个元素, 并唤醒等待的线程(this->running=0时不保证正确性)
template <typename T, typename Container>
T AvQueue<T, Container>::pop(){
    SDL_LockMutex(this->mutex);
    while (this->running && this->q.empty()){   // 队列长度小于0时等待
        SDL_CondWait(this->cond, this->mutex);
    }
    if (!this->running){
        SDL_UnlockMutex(this->mutex);
        return T();
    }
    T ret;
    if constexpr (std::is_same_v<Container, std::queue<T>>) {  // C++17 或更高
        ret = this->q.front();  // 对于 std::queue 使用 front()
    } else {
        ret = this->q.top();    // 对于 std::priority_queue 使用 top()
    }
    this->q.pop();
    SDL_CondBroadcast(this->cond);
    SDL_UnlockMutex(this->mutex);
    return ret;
}
```

`av_processor.h`
```cpp
// 自定义比较函数，按 pts 升序排列（最小堆）
struct ComparePTS {
    bool operator()(const AVPacket* frame1, const AVPacket* frame2) const {
    // bool operator()(const AVFrame* frame1, const AVFrame* frame2) const {
        if (!frame1 || !frame2) return false;  // 空帧比较
        return frame1->pts > frame2->pts;  // 小的 pts 优先
    }
};

class AvProcessor{...
AvQueue<AVPacket*, std::priority_queue<AVPacket*, std::vector<AVPacket*>, ComparePTS>> v_pkt_queue{100};    // 视频编码数据包队列
...}
```

## 同步
`av_processor` 增加获取音视频时间戳的函数

`av_SDL` 增加根据时间戳设置delay时长的函数