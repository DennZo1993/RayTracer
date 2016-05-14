set (Camera_SRCS Camera.h Camera.cpp)
set (Ray_SRCS Ray.h Ray.cpp)
set (Mesh_SRCS IntersectionResult.h Object3d.h Sphere.h Mesh.h Mesh.cpp)
set (Light_SRCS PointLight.h)

add_library(libRayTracer STATIC ${Camera_SRCS} ${Ray_SRCS} ${Mesh_SRCS} ${Light_SRCS})

target_include_directories(libRayTracer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})