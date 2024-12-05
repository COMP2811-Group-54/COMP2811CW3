cmake_minimum_required(VERSION 3.16)

project(waterQuality VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Charts Core)
qt_standard_project_setup()

qt_add_executable(waterQuality 
                    main.cpp 
                    Dashboard.cpp 
                    OverviewCards.cpp 
                    PFAs.cpp 
                    POPs.cpp 
                    PO.cpp 
                    DataPage.cpp 
                    CD.cpp 
                    utils/DataModel.cpp 
                    utils/dataset.cpp 
                    utils/Measurement.cpp)

target_link_libraries(waterQuality PRIVATE Qt6::Widgets)
target_link_libraries(waterQuality PRIVATE Qt6::Charts)
target_link_libraries(waterQuality PRIVATE Qt6::Core)

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
