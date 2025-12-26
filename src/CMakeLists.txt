add_executable(ufix.ut ufix.ut.cpp)
target_include_directories(ufix.ut PUBLIC "${PROJECT_SOURCE_DIR}/src/")
add_test(NAME ufix.ut COMMAND ufix.ut)
