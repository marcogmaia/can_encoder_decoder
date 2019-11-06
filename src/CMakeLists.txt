cmake_minimum_required(VERSION 3.5)
project(canlib)

# add_subdirectory(division)
# set(SOURCE_FILES main.c decoder.c encoder.c)
set(SOURCE_FILES
    decoder.c
    encoder.c
)

# add_executable(can ${SOURCE_FILES})
# target_link_libraries(can can)
# install(TARGETS can DESTINATION ${DIVISIBLE_INSTALL_BIN_DIR})
add_library(canlib STATIC ${SOURCE_FILES})

