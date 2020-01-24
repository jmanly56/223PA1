cmake_minimum_required(VERSION 3.5)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) #use with YCM
add_executable(PA1 main.cpp game.cpp list.h game.h profile.h profile.cpp)
target_compile_options(PA1 PRIVATE -Wall -W -Wextra -pedantic -Wno-unused-parameter -Wsign-compare  -march=native -O1 -pipe -fno-plt -DNDEBUG -g -fwrapv)
target_precompile_headers(PA1 PRIVATE list.h game.h profile.h)
