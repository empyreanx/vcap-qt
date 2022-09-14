cmake_minimum_required(VERSION 3.1.0)

project(vcap-qt VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

#set(CXX "g++")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(CMAKE_BUILD_TYPE Debug)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(vcap-qt
    MainWindow.ui
    Main.cpp
    Application.cpp
    Application.hpp
    Utility.hpp
    Utility.cpp
    IteratorDeleter.cpp
    IteratorDeleter.hpp
    ControlWrapper.hpp
    ControlWrapper.cpp
    BooleanControl.hpp
    BooleanControl.cpp
    MenuControl.hpp
    MenuControl.cpp
    IntegerControl.hpp
    IntegerControl.cpp
    IntegerMenuControl.hpp
    IntegerMenuControl.cpp
    IntegerControl.cpp
    ButtonControl.hpp
    ButtonControl.cpp
    MainWindow.hpp
    MainWindow.cpp
    Stopwatch.hpp
    Stopwatch.cpp
)

# Find V4L2
find_package(V4L2 REQUIRED)

if(NOT V4L2_FOUND)
    message(FATAL_ERROR "V4L2 not found!")
endif()

# Find JANSSON
find_package(Jansson REQUIRED)

if(NOT JANSSON_FOUND)
    message(FATAL_ERROR "Jansson not found!")
endif()

# Find JANSSON
find_package(Vcap REQUIRED)

if(NOT VCAP_FOUND)
    message(FATAL_ERROR "VCAP not found!")
endif()

target_include_directories(vcap-qt PUBLIC ${V4L2_INCLUDE_DIR} ${JANSSON_INCLUDE_DIR} ${VCAP_INCLUDE_DIR} )
target_link_libraries(vcap-qt ${VCAP_LIBRARY} ${V4L2_LIBRARIES} ${JANSSON_LIBRARY}  Qt5::Widgets)

#target_link_libraries(vcap-qt Qt5::Widgets)
