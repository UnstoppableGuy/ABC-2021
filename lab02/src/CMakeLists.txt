project(riscv_data)

file(GLOB SRC
        "*.h"
        "*.cpp"
        )
list(REMOVE_ITEM SRC "main.cpp")

add_executable(riscv_sim ${SRC} main.cpp)

add_library(riscv_lib STATIC ${SRC})
