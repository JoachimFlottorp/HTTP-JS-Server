#pragma once

#include <string>

enum class HTTPMethod
{
    OPTIONS,
    GET,
    HEAD,
    POST,
    PUT,
    DELETE,
    TRACE,
    CONNECT,
};

const std::string& HTTPMethodToString(HTTPMethod);
HTTPMethod HTTPMethodFromString(const std::string&);
