# add_library(AnimalLib OBJECT cat.cc dog.cc)   # 生成库
# target_include_directories(AnimalLib PUBLIC .)

add_library(cat OBJECT cat.cc)   # 生成库
target_include_directories(cat PUBLIC .)

add_library(dog OBJECT dog.cc)   # 生成库
target_include_directories(dog PUBLIC .)
