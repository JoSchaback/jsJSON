cmake_minimum_required(VERSION 3.10.0)

# set(CMAKE_CXX_STANDARD 23)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

#project name
project (jsJSON)

include(CTest)

add_executable(hello_jsJSON examples/hello_jsJSON.c jsJSON)
add_executable(parsing examples/parsing.c jsJSON)

# Link the math library
# target_link_libraries(usergen m)

add_library(jsJSON jsJSON.c jsJSON.h)
# add_library(json lib/json/json.h lib/json/json.cpp)
# add_library(uuid lib/uuid.h lib/uuid.cpp)

# target_link_libraries(chatter_hub json uuid mongoose)
# target_link_libraries(chatter_client_test json uuid mongoose)

add_test(NAME run_hello_jsJSON_example COMMAND hello_jsJSON)
add_test(NAME run_parsing_example COMMAND parsing)