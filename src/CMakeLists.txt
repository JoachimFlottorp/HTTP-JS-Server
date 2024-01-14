set(SOURCE main.cpp
        Network/ListenSocket.cpp
        Network/ClientSocket.cpp
        HTTP/HTTPRequest.cpp
        HTTP/HTTPRequestParser.cpp
        HTTP/HTTPMethod.cpp
        HTTP/HTTPHeaders.cpp
        HTTP/HTTPResponse.cpp
        HTTP/HTTPStatus.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE})

if (WIN32)
    target_link_libraries(${PROJECT_NAME} wsock32 ws2_32)
elseif (UNIX)
    message(FATAL_ERROR "Unix is not supported")
endif (WIN32)
