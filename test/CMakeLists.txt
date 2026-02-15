include(FetchContent)

FetchContent_Declare(
  catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.5.2
)

FetchContent_MakeAvailable(catch2)

add_executable(tests test_basic.cpp)

target_include_directories(tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(tests)
