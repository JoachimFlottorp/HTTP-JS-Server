#pragma once

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
    static HTTPHeaders FromString(const std::string& data);

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
