cmake_minimum_required(VERSION 4.0)
project(lab_5)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenCV REQUIRED)

add_executable(lab_5 main.cpp
        ImageInterface.cpp
        ImageEffect.cpp
        EffectManager.cpp)
target_link_libraries(lab_5 ${OpenCV_LIBS})
target_include_directories(lab_5 PRIVATE ${OpenCV_INCLUDE_DIRS})