cmake_minimum_required(VERSION 3.29)
project(paho_mqtt_crash)

set(CMAKE_CXX_STANDARD 20)

set(PAHO_MQTT_DIR "${CMAKE_SOURCE_DIR}/external/paho.mqtt.cpp")

if (NOT TARGET paho-mqttpp3-static)
    set(PAHO_BUILD_SHARED OFF CACHE BOOL "" FORCE)
    set(PAHO_BUILD_STATIC ON CACHE BOOL "" FORCE)
    set(PAHO_WITH_MQTT_C ON CACHE BOOL "" FORCE)
    set(PAHO_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
    set(PAHO_WITH_SSL OFF CACHE BOOL "" FORCE)
    set(PAHO_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
    add_subdirectory(${PAHO_MQTT_DIR} EXCLUDE_FROM_ALL)
endif ()

add_executable(paho_mqtt_crash
        main.cpp)

target_include_directories(paho_mqtt_crash PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}
        ${PAHO_MQTT_DIR}/src
)

target_link_libraries(paho_mqtt_crash PRIVATE
        paho-mqttpp3-static
        android
)
