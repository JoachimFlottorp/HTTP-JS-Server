#include "HTTPRequest.hpp"

#include <algorithm>
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

HTTPRequest::HTTPRequest(HTTPRequestLine requestLine, HTTPHeaders headers, std::string body)
    : m_RequestLine(std::move(requestLine)), m_Headers(std::move(headers)), m_Body(std::move(body))
{
}

HTTPRequest::~HTTPRequest()
{
}

void HTTPRequest::DebugLog() const
{
    std::cout << "Method: " << HTTPMethodToString(m_RequestLine.method) << std::endl;
    std::cout << "URI: " << m_RequestLine.uri << std::endl;
    std::cout << "Version: " << m_RequestLine.version << std::endl;

    std::cout << "Headers:" << std::endl;
    for(const auto& [key, value] : m_Headers.m_HeaderStore)
    {
	std::cout << "  " << key << ": " << value << std::endl;
    }

    if(m_Body.empty())
	std::cout << "Body: <empty>" << std::endl;
    else
	std::cout << "Body: " << m_Body << std::endl;
}

std::string HTTPRequest::ToString() const
{
    // FIXME: Less string copies.

    std::string result;

    result += HTTPMethodToString(m_RequestLine.method);
    result += " ";
    result += m_RequestLine.uri;
    result += " ";
    result += m_RequestLine.version;
    result += "\r\n";
    
    result += m_Headers.ToString();

    result += "\r\n";

    result += m_Body;

    return result;
}
