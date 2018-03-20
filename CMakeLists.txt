cmake_minimum_required(VERSION 3.9)
project(untitled)

set(CMAKE_CXX_STANDARD 11)

add_executable(main main.cpp
        include/logger.h include/measure.h include/resources.h include/msg.h include/semaforo.h include/shm.h mensaje.h)


add_executable(person persona.cpp
        include/logger.h include/measure.h include/resources.h include/msg.h include/semaforo.h include/shm.h)


add_executable(puerta puerta.cpp
        include/logger.h include/measure.h include/resources.h include/msg.h include/semaforo.h include/shm.h)