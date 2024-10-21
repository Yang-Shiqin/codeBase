#!/bin/bash

# 清理先前的编译输出
rm -f a.out

gcc 01__log.c -lavutil
./a.out

gcc 02__info_meta.c -lavformat -lavutil
./a.out

gcc 03__extract_audio.c -lavformat -lavutil -lavcodec
./a.out ../data/fly.mp4 ../output/out.aac
ffplay ../output/out.aac

gcc 04__extract_video.c -lavformat -lavutil -lavcodec
./a.out ../data/fly.avi ../output/out.h264
ffplay ../output/out.h264

gcc 05__extract_yuv.c -lavformat -lavutil -lavcodec -lswscale
./a.out ../data/fly.avi ../output/out.yuv
ffplay -video_size 960x400 ../output/out.yuv

gcc 06__extract_pcm.c -lavformat -lavutil -lavcodec
./a.out ../data/fly.avi ../output/out.pcm

gcc 07__SDL_yuv.c -lSDL2
./a.out

g++ 07__extract_yuv_2_SDL.cc -lSDL2 -lavformat -lswscale -lavcodec -lavutil
./a.out ../data/fly.avi

gcc 08__remux.c -lavformat -lavutil -lavcodec
./a.out ../data/fly.avi ../output/fly.mp4
ffplay ../output/out.mp4

gcc 09__cut.c -lavformat -lavutil -lavcodec
./a.out ../data/fly.avi ../output/fly.mp4 2.5 4
ffplay ../output/fly.mp4

gcc 10__encode_video.c -lavutil -lavcodec
./a.out ../output/out.h264 libx264
ffplay ../output/out.h264

gcc 11__encode_audio.c -lavutil -lavcodec
./a.out ../output/out.aac libfdk_aac
ffplay ../output/out.aac
