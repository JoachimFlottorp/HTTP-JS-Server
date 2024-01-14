#pragma once

#include <string>
class HTTPStatus
{
  public:
    static std::string GetReasonPhrase(int statusCode);

#include "HTTPStatus.inl"
};
