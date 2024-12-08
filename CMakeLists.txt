cmake_minimum_required(VERSION 3.16)

project(waterQuality VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Charts Core)
qt_standard_project_setup()

qt_add_executable(waterQuality main.cpp views/Dashboard.cpp views/OverviewCards.cpp views/PFAs.cpp views/POPs.cpp
        utils/csv.hpp
        utils/DataModel.cpp
        utils/DataModel.hpp
        utils/dataset.cpp
        utils/Dataset.hpp
        utils/Measurement.cpp
        utils/Measurement.hpp
        utils/Compliance.cpp
        utils/Compliance.hpp
        views/CD.cpp
        views/CD.hpp
        views/DataPage.cpp
        views/DataPage.hpp
        views/PO.cpp
        views/PO.hpp
        utils/GlobalDataModel.cpp
        utils/GlobalDataModel.hpp)

target_link_libraries(waterQuality PRIVATE Qt6::Widgets)
target_link_libraries(waterQuality PRIVATE Qt6::Charts)
target_link_libraries(waterQuality PRIVATE Qt6::Core)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

set_target_properties(waterQuality PROPERTIES
        WIN32_EXECUTABLE ON
        MACOSX_BUNDLE OFF
)

# Ensure precompiled .qm files are copied to the runtime directory
file(GLOB QM_FILES "${CMAKE_SOURCE_DIR}/languages/*.qm")
add_custom_command(
        TARGET waterQuality POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:waterQuality>/languages
        COMMAND ${CMAKE_COMMAND} -E copy ${QM_FILES} $<TARGET_FILE_DIR:waterQuality>/languages
        COMMENT "Copying precompiled .qm files to runtime directory"
)

# Ensure styles/main.qss is copied to the runtime directory
set(STYLE_FILE "${CMAKE_SOURCE_DIR}/styles/main.qss")
add_custom_command(
        TARGET waterQuality POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:waterQuality>/styles
        COMMAND ${CMAKE_COMMAND} -E copy ${STYLE_FILE} $<TARGET_FILE_DIR:waterQuality>/styles
        COMMENT "Copying main.qss stylesheet to runtime directory in the 'styles' folder"
)