add_library(protobuf-c
    STATIC
    ./protobuf-c/protobuf-c.c
    ./protobuf-c/protobuf-c.h
)

add_executable(test main.c struct.pb-c.c)

target_link_libraries(test protobuf-c)