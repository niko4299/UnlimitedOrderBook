set(datastructure_source_files
    limit.cpp
    order.cpp
    trade.hpp
    enums.hpp
    spread.hpp
    snapshot.hpp
)

add_library(data_structures STATIC ${datastructure_source_files})
