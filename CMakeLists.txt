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

list(APPEND SUNSHINE_EXTERNAL_LIBRARIES
		${CMAKE_THREAD_LIBS_INIT}
    )

add_executable(openstreamapp
  WIN32
  main.cpp
  auto_start.h
  auto_start.cpp
  auth_listener_worker.cpp
  auth_listener_worker.h
  auth_pin_handler.cpp
  auth_pin_handler.h
  ConfigurationDialog.cpp
  ConfigurationDialog.h
  ConfigurationManager.cpp
  ConfigurationManager.h
  launcher.cpp
  launcher.h
  openstreamapp.qrc
)

target_link_libraries(openstreamapp PRIVATE Qt5::Widgets Qt5::Network ${SUNSHINE_EXTERNAL_LIBRARIES})

