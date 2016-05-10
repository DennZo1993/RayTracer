file(GLOB SRCS *.cpp)

ADD_EXECUTABLE(testCamera ${SRCS})

TARGET_LINK_LIBRARIES(testCamera
    libRayTracer
    libgtest
    libgmock
)

add_test(NAME testCamera
         COMMAND testCamera)
