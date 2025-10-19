file(GLOB_RECURSE CPPS  ./*.cpp )

add_executable(dynamic_datatype ${CPPS})

target_link_libraries(dynamic_datatype QtNodes)
