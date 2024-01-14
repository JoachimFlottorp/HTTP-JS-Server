#include "HTTPRequest.hpp"

#include <iostream>
#include <utility>

HTTPRequestLine::HTTPRequestLine()
{
}

HTTPRequestLine::HTTPRequestLine(HTTPMethod method, std::string uri, std::string version)
    : method(method), uri(std::move(uri)), version(std::move(version))
{
}

HTTPRequest::HTTPRequest()
{
}

HTTPRequest::HTTPRequest(HTTPRequestLine requestLine, HTTPRequestHeaders headers, std::string body)
    : m_RequestLine(std::move(requestLine)), m_Headers(std::move(headers)), m_Body(std::move(body))
{
}

HTTPRequest::~HTTPRequest()
{
}

void HTTPRequest::SetHeader(std::string key, std::string value)
{
    m_Headers[std::move(key)] = std::move(value);
}

const std::string& HTTPRequest::GetHeader(const std::string& key) const
{
    return m_Headers.at(key);
}

bool HTTPRequest::HasHeader(const std::string& key) const
{
    return m_Headers.find(key) != m_Headers.end();
}

void HTTPRequest::DebugLog() const
{
    std::cout << "Method: " << HTTPMethodToString(m_RequestLine.method) << std::endl;
    std::cout << "URI: " << m_RequestLine.uri << std::endl;
    std::cout << "Version: " << m_RequestLine.version << std::endl;

    std::cout << "Headers:" << std::endl;
    for(const auto& [key, value] : m_Headers)
    {
	std::cout << "  " << key << ": " << value << std::endl;
    }

    if(m_Body.empty())
	std::cout << "Body: <empty>" << std::endl;
    else
	std::cout << "Body: " << m_Body << std::endl;
}

void HTTPRequest::SetRequestLine(HTTPRequestLine requestLine)
{
    m_RequestLine = std::move(requestLine);
}

void HTTPRequest::SetHeaders(HTTPRequestHeaders headers)
{
    m_Headers = std::move(headers);
}

void HTTPRequest::SetBody(std::string body)
{
    m_Body = std::move(body);
}
