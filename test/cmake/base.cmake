add_executable(${TESTNAME} ${SRC})
target_link_libraries(${TESTNAME} ${EXTRA_LIBS})
target_compile_features(${TESTNAME} PRIVATE ${NEEDED_FEATURES})
target_include_directories(${TESTNAME} PRIVATE
                            ${CMAKE_CURRENT_BINARY_DIR}
                            ${CMAKE_CURRENT_SOURCE_DIR}/../../app/src)

add_test(${TESTNAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TESTNAME})