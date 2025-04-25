aux_source_directory(. DIR_SECONDCORE_SRCS)

add_library(SecondCore ${DIR_SECONDCORE_SRCS})

include_directories(../Euzebia3D)

target_link_libraries(
    SecondCore
    PUBLIC 
    Euzebia3D
    pico_stdlib
)