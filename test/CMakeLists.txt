file(GLOB_RECURSE test_SRC
"*.hpp"
"*.cpp"
)

include_directories("./")
include_directories("../src")

set(CMAKE_PREFIX_PATH ${CMAKE_CURRENT_LIST_DIR})
find_package(CxxTest)

if(CXXTEST_FOUND)
    include_directories(${CXXTEST_INCLUDE_DIR})
    enable_testing()

    CXXTEST_ADD_TEST(test-njones test_njones.cpp ${test_SRC})
	target_link_libraries(test-njones libnjones.a)
	add_dependencies(test-njones njones)
endif()
