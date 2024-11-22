在 16__SDL_syn_av_player 的基础上实现音视频同步到系统时钟, 增加左右键快进快退
- 退出
- 暂停继续
- 快进快退

## 同步到系统时钟
`av_processor` 已有获取音视频时间戳的函数 `get_video_clock()` 和 `get_audio_clock()`

xxxx增加音频根据时间戳丢帧和插值


`av_SDL` 增加player clock。`this->player_clock` 表示系统时间系的播放开始时间戳, 则当前播放时长(真正代表player clock含义的)为 `av_gettime()-this->player_clock`。其正常播放时不变, 暂停时随系统时间前进, 快进时后退, 快退时前进(以运动的时间 `av_gettime()` 为坐标系, `this->player_clock` 为运动的进度条可能更容易理解其含义)
```cpp
double Player::get_player_clock(){
    // 计算已经播放了多少秒. av_gettime是当前时间戳, player_clock是开始时间戳(会随暂停和快进快退而改变)
    return (double)(av_gettime()-this->player_clock)/1000000.0; // 微秒->秒
}

// is_pause=0(b00): 继续(+), is_pause=2(b10): 暂停(-)
// 暂停时减去当前时刻, 继续时加上当前时刻
void Player::pause_player_clock(int is_pause){
    this->player_clock += (1-is_pause)*av_gettime();
}
```

`av_SDL` 视频已有根据音频时间戳设置delay时长的函数，改为根据播放器时钟(`timer_video_display()` 中 `audio_clock` 改为 `player_clock`)

更详细的说明见笔记《基于FFMpeg+SDL的视频播放器》的“方案3：同步到系统时钟”章节

## 快进快退
`av_SDL.cc` 增加左右键增加/减小系统时钟，因为已经实现基于系统时钟的音视频同步，所以视频音频也会相应快进和快退
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
            case SDLK_SPACE:
                running ^= 2;   // 暂停
                this->pause_player_clock(running & 2);  // 暂停播放器时钟
                break;
            case SDLK_LEFT:     // 快退5s
                this->player_clock += 5000000;
                break;
            case SDLK_RIGHT:    // 快进5s
                this->player_clock -= 5000000;
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
