cmake_minimum_required(VERSION 3.28)
project(mailSender)

add_subdirectory(includes/json)
add_subdirectory(includes/Jinja2Cpp)
add_subdirectory(includes/CPP-SMTPClient-library)

add_executable(email-sender src/main.cpp)

target_include_directories(email-sender PRIVATE includes/CPP-SMTPClient-library/src)

target_link_libraries(email-sender PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(email-sender PRIVATE jinja2cpp)
target_link_libraries(email-sender PRIVATE smtpclient)