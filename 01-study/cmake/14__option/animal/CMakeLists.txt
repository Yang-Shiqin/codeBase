option(USE_CAT "use cat" ON)
if(USE_CAT)
    set(SRC dog.cc cat.cc)
else()
    set(SRC dog.cc)
endif()

add_library(AnimalLib ${SRC})   # 生成库

if(USE_CAT)
    target_compile_definitions(AnimalLib PRIVATE "USE_CAT")
endif()
