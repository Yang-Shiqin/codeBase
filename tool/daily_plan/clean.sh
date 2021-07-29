#!/bin/bash

filename=daily_plan.md

line=`grep -n "##" ${filename} | awk -F ":" '{print $1}'`
line_num=(`echo ${line} | tr ' ' ' '`)   # line按空格划分到数组(shell数组就是空格划分)
part1=${line_num[0]}    # 时刻表
part2=${line_num[1]}    # today's work
part3=${line_num[2]}    # TODO
part4=${line_num[3]}    # good job!

sed -i "${part1},${part2} s/- \[x\]/- \[ \]/g" ${filename}
sed -i "${part2},${part4}{/- \[x\]/d;}" ${filename}

