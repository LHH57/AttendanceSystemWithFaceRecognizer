cmake_minimum_required(VERSION 3.20)
project(AttendanceSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

# OpenCV Version: 4.5.3
set(OpenCV_DIR "D:\\MyThings\\Learning\\Code_and_Project\\Libs\\OpenCV_4.5.3\\build-MinGW")
find_package(OpenCV REQUIRED)
message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
include_directories(${OpenCV_INCLUDE_DIRS} srcs qt)

add_executable(AttendanceSystem
        main.cpp
        srcs/FaceRecognizer.cpp
        srcs/attendance.cpp
        srcs/person.cpp
        srcs/filter.cpp
        )

target_link_libraries(AttendanceSystem PRIVATE ${OpenCV_LIBS})

add_executable(bm_test bm.cpp)
