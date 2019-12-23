project(CarUnitTests VERSION 0.0.1
        DESCRIPTION "Unit tests"
        LANGUAGES CXX)


# Test wrapper
mark_as_advanced(BUILD_GMOCK BUILD_GTEST BUILD_SHARED_LIBS gmock_build_tests
    gtest_build_samples gtest_build_tests gtest_disable_pthreads
    gtest_force_shared_crt gtest_hide_internal_symbols)
set_target_properties(gtest PROPERTIES FOLDER extern)
set_target_properties(gtest_main PROPERTIES FOLDER extern)
set_target_properties(gmock PROPERTIES FOLDER extern)
set_target_properties(gmock_main PROPERTIES FOLDER extern)

add_executable(CarUnitTests testMessages.cpp)
set_target_properties(CarUnitTests PROPERTIES CXX_STANDARD 14)
target_link_libraries(CarUnitTests gtest_main)
gtest_add_tests(TARGET CarUnitTests)