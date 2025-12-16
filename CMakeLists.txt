cmake_minimum_required(VERSION 3.14)
project(Lab05_Stack_FixedVector)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(main_app 
    src/fixed_vector_memory_resource.cpp 
    main.cpp
)
target_include_directories(main_app PRIVATE include)

add_executable(run_tests 
    src/fixed_vector_memory_resource.cpp 
    test/test01.cpp
)
target_include_directories(run_tests PRIVATE include)
target_link_libraries(run_tests GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(run_tests)
