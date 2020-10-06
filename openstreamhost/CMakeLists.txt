# Trexbit 2020 m4rkoup@gmail.com

cmake_minimum_required(VERSION 2.8)

project(OpenStreamHost)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

# On MSYS2, building a stand-alone binary that links with ffmpeg is not possible,
# Therefore, ffmpeg, libx264 and libx265 must be build from source
if(WIN32)
        option(OPENSTREAM_STANDALONE "Compile stand-alone binary of Openstream" ON)
        if(OPENSTREAM_STANDALONE)
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
	
                if(NOT DEFINED OPENSTREAM_PREPARED_BINARIES)
                        set(OPENSTREAM_PREPARED_BINARIES "${CMAKE_CURRENT_SOURCE_DIR}/pre-compiled/windows")
		endif()
		list(PREPEND PLATFORM_LIBRARIES
			C:/msys64/mingw64/lib/gcc/x86_64-w64-mingw32/${CMAKE_CXX_COMPILER_VERSION}/libstdc++.a
			C:/msys64/mingw64/x86_64-w64-mingw32/lib/libwinpthread.a
			)

		set(FFMPEG_INCLUDE_DIRS
                        ${OPENSTREAM_PREPARED_BINARIES}/include)
		set(FFMPEG_LIBRARIES
                        ${OPENSTREAM_PREPARED_BINARIES}/lib/libavcodec.a
                        ${OPENSTREAM_PREPARED_BINARIES}/lib/libavdevice.a
                        ${OPENSTREAM_PREPARED_BINARIES}/lib/libavfilter.a
                        ${OPENSTREAM_PREPARED_BINARIES}/lib/libavformat.a
                        ${OPENSTREAM_PREPARED_BINARIES}/lib/libavutil.a
                        ${OPENSTREAM_PREPARED_BINARIES}/lib/libpostproc.a
                        ${OPENSTREAM_PREPARED_BINARIES}/lib/libswresample.a
                        ${OPENSTREAM_PREPARED_BINARIES}/lib/libswscale.a
                        ${OPENSTREAM_PREPARED_BINARIES}/lib/libx264.a
                        ${OPENSTREAM_PREPARED_BINARIES}/lib/libx265.a
			z lzma bcrypt C:/msys64/mingw64/lib/libiconv.a)
	endif()
else()
        set(OPENSTREAM_STANDALONE OFF)
endif()

add_subdirectory(Simple-Web-Server)
add_subdirectory(moonlight-common-c/enet)

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)

if(NOT OPENSTREAM_STANDALONE)
	find_package(FFmpeg REQUIRED)
endif()

list(APPEND OPENSTREAM_COMPILE_OPTIONS -fPIC -Wall -Wno-missing-braces -Wno-maybe-uninitialized -Wno-sign-compare)

if(WIN32)
	add_subdirectory(tools) #This is temporary, only tools for Windows are needed, for now

        list(APPEND OPENSTREAM_DEFINITIONS APPS_JSON="apps_windows.json")
	include_directories(
		ViGEmClient/include)
	set(PLATFORM_TARGET_FILES
                openstreamhost/platform/windows/input.cpp
                openstreamhost/platform/windows/display.h
                openstreamhost/platform/windows/display_base.cpp
                openstreamhost/platform/windows/display_vram.cpp
                openstreamhost/platform/windows/display_ram.cpp
                openstreamhost/platform/windows/audio.cpp
		ViGEmClient/src/ViGEmClient.cpp
		ViGEmClient/include/ViGEm/Client.h
		ViGEmClient/include/ViGEm/Common.h
		ViGEmClient/include/ViGEm/Util.h
		ViGEmClient/include/ViGEm/km/BusShared.h)
	list(PREPEND PLATFORM_LIBRARIES
                kernel32
		winmm
		ksuser
		wsock32
		ws2_32
		iphlpapi
		d3d11 dxgi
		setupapi
		)

	set_source_files_properties(ViGEmClient/src/ViGEmClient.cpp PROPERTIES COMPILE_DEFINITIONS "UNICODE=1;ERROR_INVALID_DEVICE_OBJECT_PARAMETER=650")
	set_source_files_properties(ViGEmClient/src/ViGEmClient.cpp PROPERTIES COMPILE_FLAGS "-Wno-unknown-pragmas -Wno-misleading-indentation -Wno-class-memaccess")
else()
        list(APPEND OPENSTREAM_DEFINITIONS APPS_JSON="apps_linux.json")

	find_package(X11 REQUIRED)
	set(PLATFORM_TARGET_FILES
                openstreamhost/platform/linux/display.cpp
                openstreamhost/platform/linux/input.cpp)
	
	set(PLATFORM_LIBRARIES
		Xfixes
		Xtst
		xcb
		xcb-shm
		xcb-xfixes
		${X11_LIBRARIES}
		evdev
		pulse
		pulse-simple
		)
	
	set(PLATFORM_INCLUDE_DIRS
		${X11_INCLUDE_DIR}
		/usr/include/libevdev-1.0)

        if(NOT DEFINED OPENSTREAM_EXECUTABLE_PATH)
                set(OPENSTREAM_EXECUTABLE_PATH "${CMAKE_CURRENT_BINARY_DIR}/openstreamhost")
	endif()
	configure_file(gen-deb.in gen-deb @ONLY)
        configure_file(openstream.service.in openstream.service @ONLY)
endif()

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS log filesystem REQUIRED)

set(OPENSTREAM_TARGET_FILES
	moonlight-common-c/reedsolomon/rs.c
	moonlight-common-c/reedsolomon/rs.h
	moonlight-common-c/src/Input.h
	moonlight-common-c/src/Rtsp.h
	moonlight-common-c/src/RtspParser.c
	moonlight-common-c/src/Video.h
        openstreamhost/utility.h
        openstreamhost/uuid.h
        openstreamhost/config.h
        openstreamhost/config.cpp
        openstreamhost/main.cpp
        openstreamhost/main.h
        openstreamhost/crypto.cpp
        openstreamhost/crypto.h
        openstreamhost/nvhttp.cpp
        openstreamhost/nvhttp.h
        openstreamhost/rtsp.cpp
        openstreamhost/rtsp.h
        openstreamhost/stream.cpp
        openstreamhost/stream.h
        openstreamhost/video.cpp
        openstreamhost/video.h
        openstreamhost/input.cpp
        openstreamhost/input.h
        openstreamhost/audio.cpp
        openstreamhost/audio.h
        openstreamhost/platform/common.h
        openstreamhost/process.cpp
        openstreamhost/process.h
        openstreamhost/network.cpp
        openstreamhost/network.h
        openstreamhost/move_by_copy.h
        openstreamhost/task_pool.h
        openstreamhost/thread_pool.h
        openstreamhost/thread_safe.h
        openstreamhost/sync.h
        openstreamhost/round_robin.h
	${PLATFORM_TARGET_FILES})

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/Simple-Web-Server
  ${CMAKE_CURRENT_SOURCE_DIR}/moonlight-common-c/enet/include
  ${CMAKE_CURRENT_SOURCE_DIR}/moonlight-common-c/reedsolomon
  ${FFMPEG_INCLUDE_DIRS}
  ${PLATFORM_INCLUDE_DIRS}
)

string(TOUPPER "x${CMAKE_BUILD_TYPE}" BUILD_TYPE)
if("${BUILD_TYPE}" STREQUAL "XDEBUG")
        list(APPEND OPENSTREAM_COMPILE_OPTIONS -O0 -pedantic -ggdb3)
	if(WIN32)
                set_source_files_properties(openstreamhost/nvhttp.cpp PROPERTIES COMPILE_FLAGS -O2)
	endif()
else()
	add_definitions(-DNDEBUG)
        list(APPEND OPENSTREAM_COMPILE_OPTIONS -O3)
endif()

if(NOT OPENSTREAM_ASSETS_DIR)
	set(OPENSTREAM_ASSETS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")
endif()

if(OPENSTREAM_STANDALONE)
	set(OPENSSL_LIBRARIES
		C:/msys64/mingw64/lib/libssl.a
		C:/msys64/mingw64/lib/libcrypto.a)
endif()

list(APPEND OPENSTREAM_EXTERNAL_LIBRARIES
		${CMAKE_THREAD_LIBS_INIT}
		stdc++fs
		enet
		opus
		${FFMPEG_LIBRARIES}
		${Boost_LIBRARIES}
		${OPENSSL_LIBRARIES}
		${PLATFORM_LIBRARIES})

list(APPEND OPENSTREAM_DEFINITIONS OPENSTREAM_ASSETS_DIR="${OPENSTREAM_ASSETS_DIR}")
add_executable(openstreamhost ${OPENSTREAM_TARGET_FILES})
target_link_libraries(openstreamhost ${OPENSTREAM_EXTERNAL_LIBRARIES})
target_compile_definitions(openstreamhost PUBLIC ${OPENSTREAM_DEFINITIONS})
set_target_properties(openstreamhost PROPERTIES CXX_STANDARD 17)

target_compile_options(openstreamhost PRIVATE ${OPENSTREAM_COMPILE_OPTIONS})
