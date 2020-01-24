cmake_minimum_required(VERSION 3.5)
        set(CMAKE_EXPORT_COMPILE_COMMANDS ON) #use with YCM
        add_executable(PA1 main.cpp game.cpp list.h game.h profile.h profile.cpp) #add files as you go
        target_compile_options(PA1 PRIVATE -Wall -W -Wextra -pedantic -Wno-unused-parameter -Wsign-compare -march=native -O -pipe -fno-plt -DNDEBUG -g -fwrapv)
