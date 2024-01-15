#include "HTTPResponse.hpp"
#include "HTTPStatus.hpp"

HTTPStatusLine::HTTPStatusLine(std::string version, int statusCode, std::string reasonPhrase)
				: version(std::move(version)), statusCode(statusCode), reasonPhrase(std::move(reasonPhrase))
{
}

HTTPResponse::HTTPResponse(HTTPStatusLine statusLine, HTTPHeaders headers, std::string body)
				: m_StatusLine(std::move(statusLine)), m_Headers(std::move(headers)), m_Body(std::move(body))
{
}

std::string HTTPResponse::ToString() const
{
				std::string result;

				result += m_StatusLine.version;
				result += " ";
				result += std::to_string(m_StatusLine.statusCode);
				result += " ";
				result += m_StatusLine.reasonPhrase;
				result += "\r\n";

				result += m_Headers.ToString();
				result += "\r\n";

				result += m_Body;

				return result;
}

HTTPResponse& HTTPResponse::SetStatusCode(int statusCode)
{
				m_StatusLine.statusCode = statusCode;
				m_StatusLine.reasonPhrase = HTTPStatus::GetReasonPhrase(statusCode);

				return *this;
}

HTTPHeaders& HTTPResponse::GetHeaders()
{
				return m_Headers;
}

HTTPResponse& HTTPResponse::SetHeader(const std::string& key, const std::string& value)
{
				m_Headers.Set(key, value);

				return *this;
}

HTTPResponse& HTTPResponse::SetBody(std::string body)
{
				m_Body = std::move(body);
				m_Headers.Set("Content-Length", std::to_string(m_Body.size()));

				return *this;
}

HTTPResponse& HTTPResponse::AddBody(const std::string& body)
{
				m_Body += body;
				m_Headers.Set("Content-Length", std::to_string(m_Body.size()));

				return *this;
}