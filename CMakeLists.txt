cmake_minimum_required(VERSION 3.5)

project(openstreamapp LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

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
  openstreamapp/main.cpp
  openstreamapp/auto_start.h
  openstreamapp/auto_start.cpp
  openstreamapp/auth_listener_worker.cpp
  openstreamapp/auth_listener_worker.h
  openstreamapp/auth_pin_handler.cpp
  openstreamapp/auth_pin_handler.h
  openstreamapp/ConfigurationDialog.cpp
  openstreamapp/ConfigurationDialog.h
  openstreamapp/ConfigurationManager.cpp
  openstreamapp/ConfigurationManager.h
  openstreamapp/launcher.cpp
  openstreamapp/launcher.h
  openstreamapp/set_priority_class.h
  openstreamapp/set_priority_class.cpp
  openstreamapp/openstreammainwindow.h
  openstreamapp/openstreammainwindow.cpp
  openstreamapp/openstreammainwindow.ui
  openstreamapp/h265configurationdialog.h
  openstreamapp/h265configurationdialog.cpp
  openstreamapp/h265configurationdialog.ui
  openstreamapp/h264cpuconfigurationdialog.h
  openstreamapp/h264cpuconfigurationdialog.cpp
  openstreamapp/h264cpuconfigurationdialog.ui
  openstreamapp/h264nvencconfigurationdialog.h
  openstreamapp/h264nvencconfigurationdialog.cpp
  openstreamapp/h264nvencconfigurationdialog.ui
  openstreamapp/h265nvencconfigurationdialog.h
  openstreamapp/h265nvencconfigurationdialog.cpp
  openstreamapp/h265nvencconfigurationdialog.ui
  openstreamapp/h264amdamfconfigurationdialog.h
  openstreamapp/h264amdamfconfigurationdialog.cpp
  openstreamapp/h264amdamfconfigurationdialog.ui
  openstreamapp/h265amdamfconfigurationdialog.h
  openstreamapp/h265amdamfconfigurationdialog.cpp
  openstreamapp/h265amdamfconfigurationdialog.ui
  openstreamapp/VideoEncoderEnum.h
  openstreamapp.qrc
)

target_link_libraries(openstreamapp PRIVATE Qt5::Widgets Qt5::Network ${OPENSTREAM_EXTERNAL_LIBRARIES})
ADD_CUSTOM_COMMAND (TARGET openstreamapp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/libbrotlicommon.dll ${CMAKE_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/libbrotlidec.dll ${CMAKE_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/libbz2-1.dll ${CMAKE_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/libdouble-conversion.dll ${CMAKE_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/libfreetype-6.dll ${CMAKE_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/libgcc_s_seh-1.dll ${CMAKE_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/libglib-2.0-0.dll ${CMAKE_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/libgraphite2.dll ${CMAKE_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/libharfbuzz-0.dll ${CMAKE_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/libiconv-2.dll ${CMAKE_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/libicudt67.dll ${CMAKE_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/libicuin67.dll ${CMAKE_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/libicuuc67.dll ${CMAKE_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/libintl-8.dll ${CMAKE_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/libpcre-1.dll ${CMAKE_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/libpcre2-16-0.dll ${CMAKE_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/libpng16-16.dll ${CMAKE_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/libstdc++-6.dll ${CMAKE_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/libwinpthread-1.dll ${CMAKE_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/libzstd.dll ${CMAKE_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/zlib1.dll ${CMAKE_BINARY_DIR}
        #Qt libs will be copied by windeployqt
COMMENT "Copying openstream-app binaries" VERBATIM)

get_target_property(QT5_QMAKE_EXECUTABLE Qt5::qmake IMPORTED_LOCATION)
get_filename_component(QT5_WINDEPLOYQT_EXECUTABLE ${QT5_QMAKE_EXECUTABLE} PATH)
set(QT5_WINDEPLOYQT_EXECUTABLE "${QT5_WINDEPLOYQT_EXECUTABLE}/windeployqt.exe")

add_custom_command(TARGET openstreamapp POST_BUILD
   COMMAND ${QT5_WINDEPLOYQT_EXECUTABLE} --qmldir ${CMAKE_SOURCE_DIR} $<TARGET_FILE_DIR:openstreamapp>)

