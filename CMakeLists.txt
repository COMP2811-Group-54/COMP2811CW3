cmake_minimum_required(VERSION 3.16)

project(waterQuality VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Charts Core)
qt_standard_project_setup()

qt_add_executable(waterQuality main.cpp POPs.cpp)

target_link_libraries(waterQuality PRIVATE Qt6::Widgets)
target_link_libraries(waterQuality PRIVATE Qt6::Charts)
target_link_libraries(waterQuality PRIVATE Qt6::Core)

set_target_properties(waterQuality PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE OFF
)