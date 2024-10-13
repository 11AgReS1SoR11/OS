set(SRC_LIST Demon.cpp Logger.cpp Reader.cpp)

add_library(DemonLib STATIC ${SRC_LIST})

target_include_directories(DemonLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
