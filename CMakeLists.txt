cmake_minimum_required(VERSION 3.16)
project(WatermelonScene VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

add_library(scene_core STATIC
    graphicsitems.h graphicsitems.cpp
    watermelonscene.h watermelonscene.cpp
    mainwindow.h mainwindow.cpp
)
target_link_libraries(scene_core PUBLIC Qt${QT_VERSION_MAJOR}::Widgets)
target_include_directories(scene_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
if(MSVC)
    target_compile_options(scene_core PUBLIC /utf-8 /W4)
else()
    target_compile_options(scene_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(WatermelonScene WIN32 MACOSX_BUNDLE main.cpp)
target_link_libraries(WatermelonScene PRIVATE scene_core)
set_target_properties(WatermelonScene PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER "ru.student.watermelonscene"
    MACOSX_BUNDLE_BUNDLE_NAME "WatermelonScene"
    MACOSX_BUNDLE_BUNDLE_VERSION "1.0"
    MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0"
)

# Tests are optional: the normal project needs only the Widgets module.
option(BUILD_LAB_TESTS "Build automated scene and mouse interaction tests" OFF)
if(BUILD_LAB_TESTS)
    enable_testing()
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test)
    add_executable(scene_tests tests/tst_scene.cpp)
    target_link_libraries(scene_tests PRIVATE scene_core Qt${QT_VERSION_MAJOR}::Test)
    add_test(NAME scene_tests COMMAND scene_tests)
    set_tests_properties(scene_tests PROPERTIES
        ENVIRONMENT "QT_QPA_PLATFORM=offscreen"
        TIMEOUT 60
    )
endif()
