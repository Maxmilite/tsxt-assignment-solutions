cmake_minimum_required(VERSION 3.11)
 
project(Renderer)

include(FetchContent)

FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG        e69e5f977d458f2650bb346dadf2ad30c5320281)
FetchContent_MakeAvailable(fmt)
FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog
  GIT_TAG        51a0deca2c825f1d4461655a18bb37d6df76646d)
FetchContent_MakeAvailable(spdlog)
FetchContent_Declare(
  yaml-cpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  GIT_TAG        9ce5a25188d83b43dd5cd633f2975be10f5d6608
)
FetchContent_MakeAvailable(yaml-cpp)

# FetchContent_Declare(
#   assimp
#   GIT_REPOSITORY https://github.com/assimp/assimp.git
#   GIT_TAG 9519a62dd20799c5493c638d1ef5a6f484e5faf1 # v5.2.2
# )
# set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
# set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
# set(ASSIMP_INJECT_DEBUG_POSTFIX OFF CACHE BOOL "" FORCE)
# set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
# FetchContent_MakeAvailable(assimp)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/dist)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

set(THIRD_PARTY_H ${CMAKE_SOURCE_DIR}/include)
include_directories(${THIRD_PARTY_H})

file(GLOB_RECURSE DIR_SRCS RELATIVE ${CMAKE_SOURCE_DIR} src/*.cpp src/*.h)

# add include/imgui/*.cpp
file(GLOB IMGUI_SRC ${CMAKE_SOURCE_DIR}/include/imgui/*.cpp)

add_executable(Renderer ${DIR_SRCS} ${IMGUI_SRC})
target_link_libraries(Renderer 
  fmt::fmt 
  spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32> 
  yaml-cpp::yaml-cpp
  "${CMAKE_SOURCE_DIR}/lib/libglfw3.a" 
  "${CMAKE_SOURCE_DIR}/lib/libglad.a" 
  opengl32
)