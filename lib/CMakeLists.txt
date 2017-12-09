include(AddFlagIfSupported)
include(GetCoverageFlags)

# Source files.
set (
  SOURCES

  Camera.cpp
  Mesh.cpp
  Ray.cpp
  Sphere.cpp
)

# Compiler flags for this target
add_flag_if_supported("-std=c++11"      TARGET_COMPILER_FLAGS)
add_flag_if_supported("-Wall"           TARGET_COMPILER_FLAGS)
add_flag_if_supported("-Wextra"         TARGET_COMPILER_FLAGS)
add_flag_if_supported("-Wpointer-arith" TARGET_COMPILER_FLAGS)
add_flag_if_supported("-Wcast-align"    TARGET_COMPILER_FLAGS)
add_flag_if_supported("-Wswitch-enum"   TARGET_COMPILER_FLAGS)
add_flag_if_supported("-Wuninitialized" TARGET_COMPILER_FLAGS)
add_flag_if_supported("-Werror"         TARGET_COMPILER_FLAGS)

# Disabled (temporarily?) because of glm.
#
# add_flag_if_supported("-Wfloat-equal"   TARGET_COMPILER_FLAGS)
# add_flag_if_supported("-Wshadow"        TARGET_COMPILER_FLAGS)

# Linker flags.
set(TARGET_LINKER_FLAGS "")

# Create static library.
add_library(libRayTracer STATIC ${SOURCES})

target_compile_options(libRayTracer PRIVATE ${TARGET_COMPILER_FLAGS})

if ("${CMAKE_BUILD_TYPE}" MATCHES "Coverage")
  get_coverage_flags(COVERAGE_COMPILER_FLAGS)
  target_compile_options(libRayTracer PRIVATE ${COVERAGE_COMPILER_FLAGS})

  list(APPEND TARGET_LINKER_FLAGS "-lgcov" "--coverage")
endif()

# Link.
target_link_libraries(libRayTracer ${TARGET_LINKER_FLAGS})

target_include_directories(libRayTracer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
