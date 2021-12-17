cmake_minimum_required(VERSION 3.20)
project(FaceRecognizerDemo)

set(CMAKE_CXX_STANDARD 17)

# OpenCV Version: 4.5.3
set(OpenCV_DIR "D:\\MyThings\\Learning\\Code_and_Project\\Libs\\OpenCV_4.5.3\\build-MinGW")

find_package(OpenCV REQUIRED)

#message(STATUS "OpenCV library status:")
#message(STATUS "    version: ${OpenCV_VERSION}")
#message(STATUS "    libraries: ${OpenCV_LIBS}")
#message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

include_directories(${OpenCV_INCLUDE_DIRS} srcs)

add_executable(FaceRecognizerDemo
        main.cpp
        srcs/FaceRecognizer.cpp
        srcs/attendance.cpp
        srcs/person.cpp
        srcs/filter.cpp
        )
target_link_libraries(FaceRecognizerDemo PRIVATE ${OpenCV_LIBS})
