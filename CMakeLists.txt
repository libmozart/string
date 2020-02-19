include_directories(.)

aux_source_directory(mozart++ SRCS)
aux_source_directory(src SRCS_IMPL)

add_library(mpp_string STATIC
        ${SRCS}
        ${SRCS_IMPL}
        )

target_link_libraries(mpp_string mpp_core)
target_link_libraries(mpp_string mpp_foundation)
set_target_properties(mpp_string PROPERTIES LINKER_LANGUAGE CXX)

## test and benchmark targets here
file(GLOB_RECURSE CPP_SRC_LIST tests/test-*.cpp)
foreach(v ${CPP_SRC_LIST})
    string(REGEX MATCH "tests/.*" relative_path ${v})
    string(REGEX REPLACE "tests/" "" target_name ${relative_path})
    string(REGEX REPLACE ".cpp" "" target_name ${target_name})

    add_executable(mpp_string-${target_name} ${v})
    target_link_libraries(mpp_string-${target_name} mpp_string)
    add_test(mpp_string-${target_name} mpp_string-${target_name})
endforeach()
