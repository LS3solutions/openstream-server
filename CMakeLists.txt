cmake_minimum_required(VERSION 3.5)

project(openstreamapp LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")

list(PREPEND PLATFORM_LIBRARIES
        C:/msys64/mingw64/lib/gcc/x86_64-w64-mingw32/${CMAKE_CXX_COMPILER_VERSION}/libstdc++.a
        C:/msys64/mingw64/lib/gcc/x86_64-w64-mingw32/${CMAKE_CXX_COMPILER_VERSION}/libgcc_s.a
        C:/msys64/mingw64/x86_64-w64-mingw32/lib/libwinpthread.a
        )
list(PREPEND PLATFORM_LIBRARIES
        kernel32
        )

add_subdirectory(openstreamhost)
find_package(Qt5 COMPONENTS Widgets Network REQUIRED)

list(APPEND OPENSTREAM_EXTERNAL_LIBRARIES
		${CMAKE_THREAD_LIBS_INIT}
    )

add_executable(openstreamapp
  WIN32
  sources/main.cpp
  headers/auto_start.h
  sources/auto_start.cpp
  sources/auth_listener_worker.cpp
  headers/auth_listener_worker.h
  sources/auth_pin_handler.cpp
  headers/auth_pin_handler.h
  sources/ConfigurationDialog.cpp
  headers/ConfigurationDialog.h
  sources/ConfigurationManager.cpp
  headers/ConfigurationManager.h
  sources/launcher.cpp
  headers/launcher.h
  openstreamapp.qrc
)

target_link_libraries(openstreamapp PRIVATE Qt5::Widgets Qt5::Network ${OPENSTREAM_EXTERNAL_LIBRARIES})
