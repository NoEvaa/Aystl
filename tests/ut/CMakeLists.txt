# ==================================================
# Unit tests

project(
    UnitTests
    LANGUAGES CXX
)

set(MAIN_FILE_SUFFIX "main.cc")
set(SOURCE_FILE "test")
set(EXEC_PREFIX "utest_")

get_subdirs(${CMAKE_CURRENT_SOURCE_DIR} subdirs)
foreach(sub_dir IN LISTS subdirs)
    set(sub_path "${CMAKE_CURRENT_SOURCE_DIR}/${sub_dir}")

    # Main files
    file(GLOB_RECURSE main_files
        CONFIGURE_DEPENDS
        "${sub_path}/*.${MAIN_FILE_SUFFIX}"
    )

    # Other files
    file(GLOB_RECURSE other_files
        CONFIGURE_DEPENDS
        "${sub_path}/*.${SOURCE_FILE}.cc"
    )

    # Each main file will be compiled to an executable program
    foreach(main_file IN LISTS main_files)
        string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" main_rpath ${main_file})
        string(REPLACE ".${MAIN_FILE_SUFFIX}" "" main_name ${main_rpath})
        string(REGEX REPLACE "/|\\.|\\\\" "_" main_name ${main_name})
        if(main_name STREQUAL "")
            continue()
        endif()
        set(main_name "${EXEC_PREFIX}${main_name}")

        message(STATUS "Build unit test executable: ${main_name}")
        
        # add executable
        add_executable(${main_name} ${main_file} ${other_files})
        target_include_directories(
            ${main_name}
            PUBLIC ${sub_path}
        )
        target_link_libraries(${main_name} PUBLIC tests::testlib)

        # add test
        add_test(
            NAME ${main_name}
            COMMAND $<TARGET_FILE:${main_name}>
        )
    endforeach()
endforeach()

