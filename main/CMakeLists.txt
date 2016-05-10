set(Main_SRCS RayTracer.cpp)

add_executable(RayTracer ${Main_SRCS})

target_link_libraries(RayTracer libRayTracer)

install(TARGETS RayTracer DESTINATION bin)