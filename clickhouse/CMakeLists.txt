ADD_LIBRARY (clickhouse-cpp-lib SHARED
    base/coded.cpp
    base/input.cpp
    base/output.cpp
    base/platform.cpp
    base/socket.cpp
    block.cpp
    client.cpp
    columns.cpp
    query.cpp
    type_parser.cpp
    types.cpp
)

ADD_LIBRARY (clickhouse-cpp-lib-static STATIC
    base/coded.cpp
    base/input.cpp
    base/output.cpp
    base/platform.cpp
    base/socket.cpp

    columns/array.cpp
    columns/date.cpp
    columns/factory.cpp
    columns/nullable.cpp
    columns/numeric.cpp
    columns/string.cpp
    columns/tuple.cpp

    types/type_parser.cpp
    types/types.cpp

    block.cpp
    client.cpp
    query.cpp
)

SET_TARGET_PROPERTIES(clickhouse-cpp-lib
    PROPERTIES LINKER_LANGUAGE CXX)

SET_TARGET_PROPERTIES(clickhouse-cpp-lib-static
    PROPERTIES LINKER_LANGUAGE CXX)
