#include "HTTPHeaders.hpp"
#include "../Defines.hpp"

#include <algorithm>

constexpr auto CRLF = "\r\n";

std::string ConsumeUntil(const std::string& data, u32& index, const std::string& c)
{
  auto startIndex = index;
  auto endIndex = data.find(c, startIndex);

  if(endIndex == std::string_view::npos)
  {
	index = data.size();
	return data.substr(startIndex);
  }

  index = endIndex;
  return data.substr(startIndex, endIndex - startIndex);
}

bool CaseInsensitiveComparer::operator()(const std::string& lhs, const std::string& rhs) const
{
  // clang-format off
    return std::lexicographical_compare(
    lhs.begin(), lhs.end(),
    rhs.begin(), rhs.end(),
    [](char a, char b) { return std::tolower(a) < std::tolower(b); }
    );
  // clang-format on
}

std::pair<HTTPHeaders, u32> HTTPHeaders::FromString(const std::string& data)
{
  //    message-header = field-name ":" [ field-value ]
  //		       field-name     = token
  //	      field-value    = *( field-content | LWS )
  //	      field-content  = <the OCTETs making up the field-value
  //	      and consisting of either *TEXT or combinations
  //	      of token, separators, and quoted-string>

  HTTPHeaders headers {};
  u32 index = 0;

  while(true)
  {
	// 1. Consume header until we reach CRLF.
	auto components = ConsumeUntil(data, index, CRLF);
	if(components.empty())
	{
	  break;
	}

	// 2. Split header into key and value.
	auto colonIndex = components.find(':');

	auto key = components.substr(0, colonIndex);
	auto value = components.substr(colonIndex + 1);

	// 3. Trim whitespace.
	key.erase(0, key.find_first_not_of(' '));
	key.erase(key.find_last_not_of(' ') + 1);

	value.erase(0, value.find_first_not_of(' '));
	value.erase(value.find_last_not_of(' ') + 1);

	// FIXME: Should be placed somewhere else.
	// This should denote end of headers.
	if(key == CRLF)
	{
	  break;
	}

	// 4. Add header to header store.
	headers.Set(std::move(key), std::move(value));

	// 5. Increment index to skip CRLF.
	index += 2;
  }

  return {std::move(headers), index};
}

HTTPHeaders::HTTPHeaders()
{
}

void HTTPHeaders::Set(std::string key, std::string value)
{
  m_HeaderStore[std::move(key)] = std::move(value);
}

const std::string& HTTPHeaders::Get(const std::string& key) const
{
  if(auto it = m_HeaderStore.find(key); it != m_HeaderStore.end())
  {
	return it->second;
  }

  static const std::string empty;
  return empty;
}

bool HTTPHeaders::Has(const std::string& key) const
{
  return m_HeaderStore.find(key) != m_HeaderStore.end();
}

std::string HTTPHeaders::ToString() const
{
  std::string result;

  for(const auto& [key, value] : m_HeaderStore)
  {
	result += ToString(key);
  }

  return result;
}

std::string HTTPHeaders::ToString(const std::string& key) const
{
  // Key: Value\r\n
  return key + ": " + Get(key) + CRLF;
}
