set(SOURCE_FILE main.cpp)
set(TARGET_NAME main)

set(COMPILE_FLAGS "-fsycl -w ${WIN_FLAG}")
set(LINK_FLAGS "-fsycl")
set(USE_AVX_INSTRUCTIONS true)
set(USE_SSE2_INSTRUCTIONS true)
set(USE_SSE4_INSTRUCTIONS true)

add_executable(${TARGET_NAME} ${SOURCE_FILE})
set_target_properties(${TARGET_NAME} PROPERTIES COMPILE_FLAGS "${COMPILE_FLAGS}")
set_target_properties(${TARGET_NAME} PROPERTIES LINK_FLAGS "${LINK_FLAGS}")

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories("/opt/intel/oneapi/dpl/latest/linux")
target_link_libraries(main ${OpenCV_LIBS})
target_link_libraries(main dlib::dlib)
find_package(VPL REQUIRED)
target_link_libraries(main VPL::dispatcher)

