set(SOURCE main.cpp
        Network/ListenSocket.cpp
        Network/ClientSocket.cpp
        HTTP/HTTPRequest.cpp
        HTTP/HTTPRequestParser.cpp
        HTTP/HTTPMethod.cpp
        HTTP/HTTPHeaders.cpp
        HTTP/HTTPResponse.cpp
        HTTP/HTTPStatus.cpp
        Scripting/ScriptEngine.cpp
        Scripting/ScriptLogging.cpp
        Scripting/duk_module_node.cpp
)

find_package(duktape CONFIG REQUIRED)
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${DUKTAPE_INCLUDE_DIRS})

## TODO: Clean this.
include_directories(../thirdparty/dukglue/include)

add_executable(${PROJECT_NAME} ${SOURCE})

target_link_libraries(${PROJECT_NAME} ${DUKTAPE_LIBRARY})

if (WIN32)
    target_link_libraries(${PROJECT_NAME} wsock32 ws2_32)
elseif (UNIX)
    message(FATAL_ERROR "Unix is not supported")
endif (WIN32)

# Using target should ensure this is copied each time the project is built
add_custom_target(copy_lib ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/lib $<TARGET_FILE_DIR:${PROJECT_NAME}>/lib
        COMMENT "Copying lib to build directory"
)

add_dependencies(${PROJECT_NAME} copy_lib)
