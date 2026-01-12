cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(icon_factory LANGUAGES CXX)

include(FetchContent)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(wxBUILD_SHARED OFF)

message(STATUS "Fetching wxWidgets...")

FetchContent_Declare(
        wxWidgets
        GIT_REPOSITORY https://github.com/wxWidgets/wxWidgets.git
        GIT_TAG v3.3.1
        GIT_SHALLOW ON
)
FetchContent_MakeAvailable(wxWidgets)

set(SRCS App.cpp
        MainFrame.cpp
        MainFrame.h
        IconCanva.cpp
        IconCanva.h
        IconPreview.cpp
        IconPreview.h)

# Add the manifest resource only for MinGW builds to avoid duplicate manifests with MSVC.
if (MINGW)
    list(APPEND SRCS app.rc)
endif()

add_executable(main WIN32 ${SRCS})
target_link_libraries(main PRIVATE wxcore wxnet)
