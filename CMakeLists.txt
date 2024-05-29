cmake_minimum_required(VERSION 3.10)
project(gloss)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/lexer
    ${CMAKE_SOURCE_DIR}/src/parser
    ${CMAKE_SOURCE_DIR}/src/parser/interpreter
    ${CMAKE_SOURCE_DIR}/src/parser/ASTs
    ${CMAKE_SOURCE_DIR}/src/parser/visitors
)

set(SRC_FILES 
    ./src/lexer/lexer.cpp 
    ./src/lexer/token.cpp 
    ./src/alerts.cpp 
    ./src/parser/parser.cpp 
    ./src/parser/interpreter/interpreter.cpp 
    ./src/parser/interpreter/environment.cpp
)

set(TEST_SRC_FILES 
    test/unit-tests/lexer-tests.cpp 
    # test/unit-tests/gloss-tests.cpp 
)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

add_executable(test_runner ${TEST_SRC_FILES} ${SRC_FILES})

target_link_libraries(test_runner gtest gtest_main)

enable_testing()
add_test(NAME unit_tests COMMAND test_runner)
