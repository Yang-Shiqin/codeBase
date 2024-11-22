在 15__SDL_basic_av_player 的基础上实现更精细的同步(视频同步到音频), 增加空格暂停继续

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

## 同步到音频
`av_processor` 增加获取音视频时间戳的函数
```cpp
// 计算视频时钟, 单位为s
double AvProcessor::get_video_clock(AVFrame* frame){
    return frame->pts*av_q2d(this->fmt_ctx->streams[this->v_index]->time_base);
}

// 计算音频时钟, 单位为s
double AvProcessor::get_audio_clock(){
    // 公式: 当前帧实际时间 = pts x time_base - 已解码未播放字节/(channels x 样本位深 x sample_rate)
    double audio_clock = this->next_pts * av_q2d(this->fmt_ctx->streams[this->a_index]->time_base); // 下一音频包的pts
    audio_clock -= (double)(this->audio_chunk.size()) / 
        (double)(this->a_codec_ctx->channels*this->a_codec_ctx->sample_rate*
        av_get_bytes_per_sample(this->a_codec_ctx->sample_fmt));
    return audio_clock;
}
```

`av_SDL` 增加根据时间戳设置delay时长的函数
```cpp
// 原本调用video_display的地方改为调用timer_video_display

int Player::timer_video_display(){
    static double first_delay = 0;  // 用来同步音视频第一个帧所需的延迟
    static int flag = 0;
    // 1. 从队列中取出视频帧
    if (this->frame==nullptr)
        this->frame = this->processor->video_frame_pop();
    if (!this->frame) {
        av_log(NULL, AV_LOG_ERROR, "frame is NULL\n");
        return (this->invalid = VIDEO_FRAME_BROKE);
    }
    // 2. 计算视频同步到音频需要的延迟(下一视频帧时间戳-音频帧时间戳, <0则说明视频慢了, 应加速播放)
    double delay = this->processor->get_video_clock(this->frame) - this->processor->get_audio_clock()-first_delay;
    if (!flag){     // 只记录第一次的延迟
        first_delay = delay;
        flag = 1;
    }
    av_log(NULL, AV_LOG_DEBUG, "delay: %f\n", delay);
    
    if (delay <= 0){    // 视频慢了
        this->video_display(this->frame);   // 显示视频
        this->frame = nullptr;
        SDL_AddTimer(1, video_timer, this->processor);  // 1ms后再次调用timer_video_display
    }else{  // 视频快了
        // delay是s为单位, 而SDL_AddTimer是ms为单位, 所以delay*1000, +0.5是向上取整, 防止下次调用没到时间又重复delay了很小一个时间
        SDL_AddTimer((int)(delay*1000+0.5), video_timer, this->processor);  // 延迟快了的时间后再次调用timer_video_display
    }
    return 0;
}

// 播放一帧视频
int Player::video_display(AVFrame* frame){
    // 2. 更新纹理
    SDL_UpdateYUVTexture(this->texture, NULL, frame->data[0], frame->linesize[0], 
        frame->data[1], frame->linesize[1], frame->data[2], frame->linesize[2]);
    // 3. 清空渲染器
    SDL_RenderClear(this->renderer);
    // 4. 拷贝纹理到渲染器
    SDL_RenderCopy(this->renderer, this->texture, NULL, NULL);
    // 5. 显示
    SDL_RenderPresent(this->renderer);
    // 6. 释放帧
    av_freep(&frame);
    return 0;
}
```

更详细的说明见笔记《基于FFMpeg+SDL的视频播放器》的“方案2：视频同步到音频”章节

## 暂停继续
`av_SDL.cc` 增加空格暂停继续音频, 因为已经实现较为精细的音视频同步，所以视频也会相应暂停和继续
```cpp
int Player::play(){
    ...
    int running = 1;    // 第1位是是否播放, 第2位是是否暂停
    while(running){
        SDL_WaitEvent(&this->event);
        switch (this->event.type)
        {
        case SDL_QUIT:  // 退出事件
            this->processor->stop();
            SDL_WaitThread(demux_tid, nullptr);
            running = 0;
            break;
        case SDL_KEYDOWN:   // 键盘事件
            switch (event.key.keysym.sym){
            case SDLK_SPACE:                // 增加键盘事件中的空格事件处理
                running ^= 2;   // 暂停
                SDL_PauseAudio(running & 2);    // 暂停音频
                break;
            }
            break;
        case SDL_USEREVENT: // 视频定时播放事件
            this->timer_video_display();
            break;
        default:
            break;
        }
    }
    ...
}
```