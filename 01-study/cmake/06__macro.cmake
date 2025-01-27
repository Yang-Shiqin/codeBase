cmake_minimum_required(VERSION 3.20.0)

macro(Test var)
    set(var "new")
endmacro()

set(var "value")
message(${var})
Test(var)
message(${var})
