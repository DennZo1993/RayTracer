include(AddFlagIfSupported)
include(GetCoverageFlags)

set (
  SOURCES

  RayTracer.cpp
)

# Compiler flags for this target
add_flag_if_supported("-std=c++11"      TARGET_COMPILER_FLAGS)
add_flag_if_supported("-Wall"           TARGET_COMPILER_FLAGS)
add_flag_if_supported("-Wextra"         TARGET_COMPILER_FLAGS)
add_flag_if_supported("-Wshadow"        TARGET_COMPILER_FLAGS)
add_flag_if_supported("-Wpointer-arith" TARGET_COMPILER_FLAGS)
add_flag_if_supported("-Wcast-align"    TARGET_COMPILER_FLAGS)
add_flag_if_supported("-Wswitch-enum"   TARGET_COMPILER_FLAGS)
add_flag_if_supported("-Wuninitialized" TARGET_COMPILER_FLAGS)
add_flag_if_supported("-Werror"         TARGET_COMPILER_FLAGS)
add_flag_if_supported("-Wfloat-equal"   TARGET_COMPILER_FLAGS)

set(TARGET_LINKER_FLAGS "")

add_executable(RayTracer ${SOURCES})

target_compile_options(RayTracer PRIVATE ${TARGET_COMPILER_FLAGS})

if ("${CMAKE_BUILD_TYPE}" MATCHES "Coverage")
  get_coverage_flags(COVERAGE_COMPILER_FLAGS)
  target_compile_options(libRayTracer PRIVATE ${COVERAGE_COMPILER_FLAGS})

  list(APPEND TARGET_LINKER_FLAGS "-lgcov" "--coverage")
endif()

target_include_directories(RayTracer PRIVATE ../lib)
target_link_libraries(RayTracer libRayTracer ${TARGET_LINKER_FLAGS})

install(TARGETS RayTracer DESTINATION bin)
