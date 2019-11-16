# project (Tutorial)

# include project source
# include_directories(${PROJECT_SOURCE_DIR}/../include)
# include_directories(${PROJECT_SOURCE_DIR}/../src)
# include_directories(${PROJECT_SOURCE_DIR}/../include/LedDriver)
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/include)

# include mocks only for test code
# include_directories(${PROJECT_SOURCE_DIR}/../mocks)

# include CppUTest headers
# include_directories($ENV{CPPUTEST_HOME}/include)
# find_library(cpputest CppUTest)

# add cpputest library
# add_library(imp_cpputest STATIC IMPORTED)
# set_property(TARGET imp_cpputest PROPERTY
#              IMPORTED_LOCATION $ENV{CPPUTEST_HOME}/src/CppUTest/libCppUTest.a)

# build test library for LedDriver
set(SRC_FILES
    "${PROJECT_SOURCE_DIR}/src/encoder.c"
    "${PROJECT_SOURCE_DIR}/src/decoder.c"
    "${PROJECT_SOURCE_DIR}/src/utils.c"
    "./encoder/test_encoder.cpp"
    "./test_utils.cpp"
)
add_library(test_encoder 
    ${SRC_FILES}
)

add_executable(RunAllTests RunAllTests.cpp)
target_link_libraries(RunAllTests CppUTest CppUTestExt test_encoder)