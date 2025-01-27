cmake_minimum_required(VERSION 3.10)
# append 列表添加元素
list(APPEND var A B C)
message("var: ${var}") # var: A;B;C
list(APPEND var 1 2)
message(${var})

# length 获取列表元素个数
list(LENGTH var length)
message(length: ${length})

# insert 列表插入元素
list(INSERT var 0 a b)  # 在0位置插入
message(${var})

# remove_item 列表删除元素
list(REMOVE_ITEM var a b)  # 在0位置插入
message(${var})

# find 查找元素
list(FIND var A find_index) # 在var中查找A，将索引存入find_index
message(${find_index})

# reverse 反转list
list(REVERSE var)
message(${var})

# sort 排序list
list(SORT var)
message(${var})
