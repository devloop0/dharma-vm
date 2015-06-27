project("dharma-vm")

set(CMAKE_CXX_FLAGS "--std=c++11")

add_executable(dharma main.cpp src/runtime.cpp src/runtime_function.cpp)
