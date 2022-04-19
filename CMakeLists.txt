cmake_minimum_required(VERSION 3.1.0)

project(vcap-qt VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(vcap-qt
    MainWindow.ui
    Main.cpp
    Utils.hpp
    Utils.cpp
    ControlWrapper.hpp
    ControlWrapper.cpp
    BooleanControl.hpp
    BooleanControl.cpp
    MenuControl.hpp
    MenuControl.cpp
    IntegerControl.hpp
    IntegerControl.cpp
    ButtonControl.hpp
    ButtonControl.cpp
    IntegerMenuControl.hpp
    IntegerMenuControl.cpp
    MainWindow.hpp
    MainWindow.cpp
)

target_link_libraries(vcap-qt Qt5::Widgets)
