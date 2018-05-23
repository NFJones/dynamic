file(GLOB_RECURSE lib_SRC 
"*.hpp"
"*.cpp")

file(GLOB_RECURSE lib_INCLUDE
"*.hpp")

include_directories("./")

add_library(njones-static STATIC ${lib_SRC})
add_library(njones SHARED ${lib_SRC})

set_target_properties(njones-static PROPERTIES OUTPUT_NAME njones)

install(TARGETS njones 
        RUNTIME DESTINATION bin 
        LIBRARY DESTINATION lib 
        ARCHIVE DESTINATION lib/static)

INSTALL(FILES ${lib_INCLUDE}
        DESTINATION include/njones)
