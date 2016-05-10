set (Camera_SRCS Camera.h Camera.cpp)
set (Ray_SRCS Ray.h Ray.cpp)

add_library(libRayTracer STATIC ${Camera_SRCS} ${Ray_SRCS})

target_include_directories(libRayTracer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})