#pragma once

#include "HTTPMethod.hpp"

#include <map>
#include <string>

struct HTTPRequestLine
{
    // FIXME: Better defaults.
    HTTPMethod method = HTTPMethod::GET;
    std::string uri = "/";
    std::string version = "HTTP/1.1";

    HTTPRequestLine();
    HTTPRequestLine(HTTPMethod method, std::string uri, std::string version);
};

typedef std::map<std::string, std::string> HTTPRequestHeaders;

class HTTPRequest
{
  public:
    HTTPRequest();
    HTTPRequest(HTTPRequestLine requestLine, HTTPRequestHeaders headers, std::string body);

    ~HTTPRequest();

    void SetHeader(std::string key, std::string value);
    const std::string& GetHeader(const std::string& key) const;
    bool HasHeader(const std::string& key) const;

    HTTPMethod GetMethod() const { return m_RequestLine.method; }

    void DebugLog() const;

  private:
    void SetRequestLine(HTTPRequestLine requestLine);
    void SetHeaders(HTTPRequestHeaders headers);
    void SetBody(std::string body);

  private:
    HTTPRequestLine m_RequestLine;
    HTTPRequestHeaders m_Headers;
    std::string m_Body;

    friend class HTTPRequestParser;
};
