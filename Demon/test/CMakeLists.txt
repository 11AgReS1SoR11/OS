include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.6.0
)

FetchContent_MakeAvailable(Catch2)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../src)

set(SRC_LIST_TESTS ut.cpp)

add_executable(test ${SRC_LIST_TESTS})

target_include_directories(test PRIVATE ${SOURCE_DIR})
target_link_libraries(test PRIVATE Catch2::Catch2WithMain DemonLib)
