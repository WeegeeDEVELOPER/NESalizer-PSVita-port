## This file is a quick tutorial on writing CMakeLists for targeting the Vita
cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(NESalizer)
include("${VITASDK}/share/vita.cmake" REQUIRED)

set(VITA_APP_NAME "NESalizer")
set(VITA_TITLEID  "WDEV84912")
set(VITA_VERSION  "01.00")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(VITA_MKSFOEX_FLAGS "${VITA_MKSFOEX_FLAGS} -d PARENTAL_LEVEL=1")

enable_language(C ASM)

include_directories(
  ../common # This is for debugScreenPrintf(), you shouldn't need it in your projects
)

# ${CMAKE_CURRENT_BINARY_DIR} lets you use any library currently being built
link_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(${PROJECT_NAME}
  src/apu.cpp
  src/audio.cpp
  src/blip_buf.cpp
  src/common.cpp
  src/controller.cpp
  src/cpu.cpp
  src/gui.cpp
  src/input.cpp
  src/main.cpp
  src/mapper_0.cpp
  src/mapper_1.cpp
  src/mapper_2.cpp
  src/mapper_3.cpp
  src/mapper_4.cpp
  src/mapper_5.cpp
  src/mapper_7.cpp
  src/mapper_9.cpp
  src/mapper_10.cpp
  src/mapper_11.cpp
  src/mapper_13.cpp
  src/mapper_28.cpp
  src/mapper_71.cpp
  src/mapper_232.cpp
  src/mapper.cpp
  src/md5.cpp
  src/menu.cpp
  src/ppu.cpp
  src/rom.cpp
  src/save_states.cpp
  src/sdl_backend.cpp
  src/tables.c
  src/timing.cpp
  #../common/debugScreen.c
)

# Library to link to (drop the -l prefix). This will mostly be stubs.
target_link_libraries(${PROJECT_NAME}
  SDL2_mixer
  SDL2_gfx
  SDL2_image
  SDL2_net
  SDL2_ttf
  SDL2main
  SDL2
  vorbisfile
  vorbis
  FLAC
  mikmod
  mpg123
  ogg
  png
  turbojpeg
  webp
  pthread
  freetype
  SceDisplay_stub
  SceCtrl_stub
  SceAudio_stub
  SceSysmodule_stub
  SceGxm_stub
  SceCommonDialog_stub
  SceAppMgr_stub
  SceTouch_stub
  SceHid_stub
  SceMotion_stub
  SceIofilemgrForDriver_stub 
  SceThreadmgrForKernel_stub
  z
)

vita_create_self(${PROJECT_NAME}.self ${PROJECT_NAME})
vita_create_vpk(${PROJECT_NAME}.vpk ${VITA_TITLEID} ${PROJECT_NAME}.self
  VERSION ${VITA_VERSION}
  NAME ${VITA_APP_NAME}
  FILE sce_sys/icon0.png sce_sys/icon0.png
  FILE sce_sys/livearea/contents/bg.png sce_sys/livearea/contents/bg.png
  FILE sce_sys/livearea/contents/startup.png sce_sys/livearea/contents/startup.png
  FILE sce_sys/livearea/contents/template.xml sce_sys/livearea/contents/template.xml
)
