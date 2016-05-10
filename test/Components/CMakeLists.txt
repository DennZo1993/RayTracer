file(GLOB SRCS *.cpp)

ADD_EXECUTABLE(testComponents ${SRCS})

TARGET_LINK_LIBRARIES(testComponents
    libRayTracer
    libgtest
    libgmock
)

add_test(NAME testComponents
         COMMAND testComponents)
