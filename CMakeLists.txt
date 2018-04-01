cmake_minimum_required(VERSION 3.9)
project(untitled)

set(CMAKE_CXX_STANDARD 11)


add_executable(person_dispatcher person_dispatcher.cpp
include/logger.h include/measure.h include/resources.h include/msg.h include/semaphore.h include/shm.h message.h constants.h)

add_executable(main main.cpp
        include/logger.h include/measure.h include/resources.h include/msg.h include/semaphore.h include/shm.h message.h constants.h)


add_executable(person person.cpp
        include/logger.h include/measure.h include/resources.h include/msg.h include/semaphore.h include/shm.h constants.h)


add_executable(door door.cpp
        include/logger.h include/measure.h include/resources.h include/msg.h include/semaphore.h include/shm.h constants.h)