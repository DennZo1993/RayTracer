set (Camera_SRCS Camera.h Camera.cpp)

add_library(libRayTracer STATIC ${Camera_SRCS})

target_include_directories(libRayTracer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})