cmake_minimum_required(VERSION 2.8)

project(OpenStreamHost)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(Simple-Web-Server)
add_subdirectory(moonlight-common-c/enet)

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)

########### FFMPEG #################
# We link against our build of FFmpeg with nvenc support enabled compilation
set(SUNSHINE_PREPARED_BINARIES "${CMAKE_CURRENT_SOURCE_DIR}/pre-compiled-ffmpeg/windows")
set(FFMPEG_INCLUDE_DIRS
                        ${SUNSHINE_PREPARED_BINARIES}/include)
set(FFMPEG_LIBRARIES
        ${SUNSHINE_PREPARED_BINARIES}/lib/libavcodec.dll.a
        ${SUNSHINE_PREPARED_BINARIES}/lib/libavdevice.dll.a
        ${SUNSHINE_PREPARED_BINARIES}/lib/libavfilter.dll.a
        ${SUNSHINE_PREPARED_BINARIES}/lib/libavformat.dll.a
        ${SUNSHINE_PREPARED_BINARIES}/lib/libavutil.dll.a
        ${SUNSHINE_PREPARED_BINARIES}/lib/libpostproc.dll.a
        ${SUNSHINE_PREPARED_BINARIES}/lib/libswresample.dll.a
        ${SUNSHINE_PREPARED_BINARIES}/lib/libswscale.dll.a)
########### /FFMPEG ################

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

# This copy the shared libs to the output dir when building.
# TODO: Research how to improve this. CMake **might** provide a more
#   idiomatic and safe way of doing this.
ADD_CUSTOM_COMMAND (TARGET openstreamhost POST_BUILD
        # COPY .dll file from Msys2 system packages.
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/libaom.dll ${CMAKE_BINARY_DIR}/openstreamhost
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/libcelt0-2.dll ${CMAKE_BINARY_DIR}/openstreamhost
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/libcrypto-1_1-x64.dll ${CMAKE_BINARY_DIR}/openstreamhost
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/libdav1d.dll ${CMAKE_BINARY_DIR}/openstreamhost
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/libgcc_s_seh-1.dll ${CMAKE_BINARY_DIR}/openstreamhost
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/libgsm.dll ${CMAKE_BINARY_DIR}/openstreamhost
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/libiconv-2.dll ${CMAKE_BINARY_DIR}/openstreamhost
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/liblzma-5.dll ${CMAKE_BINARY_DIR}/openstreamhost
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/libmfx-1.dll ${CMAKE_BINARY_DIR}/openstreamhost
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/libmp3lame-0.dll ${CMAKE_BINARY_DIR}/openstreamhost
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/libogg-0.dll ${CMAKE_BINARY_DIR}/openstreamhost
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/libopencore-amrnb-0.dll ${CMAKE_BINARY_DIR}/openstreamhost
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/libopencore-amrwb-0.dll ${CMAKE_BINARY_DIR}/openstreamhost
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/libopenjp2-7.dll ${CMAKE_BINARY_DIR}/openstreamhost
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/libopus-0.dll ${CMAKE_BINARY_DIR}/openstreamhost
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/libspeex-1.dll ${CMAKE_BINARY_DIR}/openstreamhost
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/libssl-1_1-x64.dll ${CMAKE_BINARY_DIR}/openstreamhost
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/libstdc++-6.dll ${CMAKE_BINARY_DIR}/openstreamhost
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/libtheoradec-1.dll ${CMAKE_BINARY_DIR}/openstreamhost
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/libtheoraenc-1.dll ${CMAKE_BINARY_DIR}/openstreamhost
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/libvorbis-0.dll ${CMAKE_BINARY_DIR}/openstreamhost
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/libvorbis-0.dll ${CMAKE_BINARY_DIR}/openstreamhost
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/libvorbisenc-2.dll ${CMAKE_BINARY_DIR}/openstreamhost
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/libvpx-1.dll ${CMAKE_BINARY_DIR}/openstreamhost
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/libvulkan-1.dll ${CMAKE_BINARY_DIR}/openstreamhost
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/libwavpack-1.dll ${CMAKE_BINARY_DIR}/openstreamhost
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/libwebp-7.dll ${CMAKE_BINARY_DIR}/openstreamhost
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/libwebpmux-3.dll ${CMAKE_BINARY_DIR}/openstreamhost
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/libwinpthread-1.dll ${CMAKE_BINARY_DIR}/openstreamhost
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/libx264-159.dll ${CMAKE_BINARY_DIR}/openstreamhost
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/libx265.dll ${CMAKE_BINARY_DIR}/openstreamhost
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/xvidcore.dll ${CMAKE_BINARY_DIR}/openstreamhost
        COMMAND ${CMAKE_COMMAND} -E copy C:/msys64/mingw64/bin/zlib1.dll ${CMAKE_BINARY_DIR}/openstreamhost
### COPY PRE-COMPILED FFMPEG
        COMMAND ${CMAKE_COMMAND} -E copy ${SUNSHINE_PREPARED_BINARIES}/bin/avcodec-59.dll ${CMAKE_BINARY_DIR}/openstreamhost
        COMMAND ${CMAKE_COMMAND} -E copy ${SUNSHINE_PREPARED_BINARIES}/bin/avutil-57.dll ${CMAKE_BINARY_DIR}/openstreamhost
        COMMAND ${CMAKE_COMMAND} -E copy ${SUNSHINE_PREPARED_BINARIES}/bin/swresample-4.dll ${CMAKE_BINARY_DIR}/openstreamhost
        COMMAND ${CMAKE_COMMAND} -E copy ${SUNSHINE_PREPARED_BINARIES}/bin/swscale-6.dll ${CMAKE_BINARY_DIR}/openstreamhost
COMMENT "Copying host (sunshine) binaries" VERBATIM)
