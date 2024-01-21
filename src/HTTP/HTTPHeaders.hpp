#pragma once

#include "Defines.hpp"
#include <map>
#include <string>

namespace
{
  struct CaseInsensitiveComparer
  {
	bool operator()(const std::string& lhs, const std::string& rhs) const;
  };
} // namespace

class HTTPHeaders
{
public:
  /**
   * @brief Converts a string to a HTTPHeaders object.
   * @param data
   * @return {HTTPHeaders, u32}
   * 	- HTTPHeaders: The HTTPHeaders object.
   * 	- u32: The size of the headers in bytes.
   */
  static std::pair<HTTPHeaders, u32> FromString(const std::string& data);

public:
  HTTPHeaders();

  void Set(std::string key, std::string value);
  [[nodiscard]] const std::string& Get(const std::string& key) const;
  [[nodiscard]] bool Has(const std::string& key) const;

  /**
   * @brief Converts the headers to a string.
   */
  [[nodiscard]] std::string ToString() const;

  /**
   * @brief Converts a single header to a string.
   */
  [[nodiscard]] std::string ToString(const std::string& key) const;

private:
  std::map<std::string, std::string, CaseInsensitiveComparer> m_HeaderStore;

  friend class HTTPRequest;
};
