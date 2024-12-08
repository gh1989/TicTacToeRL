cmake_minimum_required(VERSION 3.10)
project(MinimalRL)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
set(Torch_DIR "/opt/libtorch/share/cmake/Torch")
find_package(Torch REQUIRED)
file(GLOB SOURCES
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)
add_executable(MinimalRL ${SOURCES})
target_link_libraries(MinimalRL "${TORCH_LIBRARIES}")
target_include_directories(MinimalRL PRIVATE ${CMAKE_SOURCE_DIR}/src)
set_target_properties(MinimalRL PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")
message(STATUS "Torch libraries: ${TORCH_LIBRARIES}")
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")

# tests
set(TEST_COMMON src/environment.cpp src/game.cpp src/agent.cpp src/trainer.cpp)
add_executable(test_environment tests/test_environment.cpp ${TEST_COMMON})
target_link_libraries(test_environment "${TORCH_LIBRARIES}")
target_include_directories(test_environment PRIVATE ${CMAKE_SOURCE_DIR}/src)
add_executable(test_agent tests/test_agent.cpp ${TEST_COMMON})
target_link_libraries(test_agent "${TORCH_LIBRARIES}")
target_include_directories(test_agent PRIVATE ${CMAKE_SOURCE_DIR}/src)
add_executable(test_trainer tests/test_trainer.cpp ${TEST_COMMON})
target_link_libraries(test_trainer "${TORCH_LIBRARIES}")
target_include_directories(test_trainer PRIVATE ${CMAKE_SOURCE_DIR}/src)

