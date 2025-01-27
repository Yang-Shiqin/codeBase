cmake_minimum_required(VERSION 3.20.0)

################## if
set(var TRUE)
if(var)
    message(TRUE)
else()
    message(FALSE)
endif()

if (NOT var OR var)
    message(123)
endif()

if (1 LESS 3)
    message(1 less 3)
endif()

################ for
foreach(var RANGE 3)
    message(${var})
endforeach()

set(list 1 2 3)
foreach(var IN LISTS list ITEMS 4 f)
    message(${var})
endforeach()

# zip
set(l1 one two three)
set(l2 1 2 3)
foreach(var IN ZIP_LISTS l1 l2)
    message("${var_0} ${var_1}")
endforeach()

