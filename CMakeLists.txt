cmake_minimum_required(VERSION 3.16)

project(waterQuality VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required Qt components
find_package(Qt6 REQUIRED COMPONENTS Widgets Charts Core Network Quick Qml Positioning QuickWidgets Concurrent)

# Standard Qt project setup
qt_standard_project_setup()

# Add executable and sources
qt_add_executable(waterQuality
        main.cpp
        views/Dashboard.cpp
        views/OverviewCards.cpp
        views/PFAs.cpp
        views/POPs.cpp
        views/CD.cpp
        views/CD.hpp
        views/DataPage.cpp
        views/DataPage.hpp
        views/PO.cpp
        views/PO.hpp
        views/GH.cpp
        views/GH.hpp
        utils/csv.hpp
        utils/DataModel.cpp
        utils/DataModel.hpp
        utils/Dataset.cpp
        utils/Dataset.hpp
        utils/Measurement.cpp
        utils/Measurement.hpp
        utils/Compliance.cpp
        utils/Compliance.hpp
        utils/GlobalDataModel.cpp
        utils/GlobalDataModel.hpp
        utils/SamplingPoint.cpp
        utils/SamplingPoint.hpp
)

# Link against Qt libraries
target_link_libraries(waterQuality PRIVATE Qt6::Widgets Qt6::Charts Qt6::Core Qt6::Network Qt6::Quick Qt6::Qml Qt6::Positioning Qt6::QuickWidgets Qt6::Concurrent)

# Set build type if not defined
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

# Set target properties
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

# Ensure all data files are copied to the runtime directory
file(GLOB DATA_FILE "${CMAKE_SOURCE_DIR}/data/*.csv")
add_custom_command(
        TARGET waterQuality POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:waterQuality>/data
        COMMAND ${CMAKE_COMMAND} -E copy ${DATA_FILE} $<TARGET_FILE_DIR:waterQuality>/data
        COMMENT "Copying all data files to runtime directory in the 'data' folder"
)

# Ensure all data files are copied to the runtime directory
file(GLOB QML_FILES "${CMAKE_SOURCE_DIR}/qml/*.qml")
file(GLOB GIF_FILES "${CMAKE_SOURCE_DIR}/qml/*.gif")
add_custom_command(
        TARGET waterQuality POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:waterQuality>/qml
        COMMAND ${CMAKE_COMMAND} -E copy ${QML_FILES} $<TARGET_FILE_DIR:waterQuality>/qml
        COMMAND ${CMAKE_COMMAND} -E copy ${GIF_FILES} $<TARGET_FILE_DIR:waterQuality>/qml
        COMMENT "Copying all qml files to runtime directory in the 'qml' folder"
)
