#pragma once

#include <duktape.h>

#include <string>

/**
 * @brief Logging class for JavaScript land. Implements the <b>globalThis.console</b> object.
 */
class ScriptLogging
{
public:
  duk_ret_t Debug(duk_context*);
  duk_ret_t Log(duk_context*);
  duk_ret_t Warn(duk_context*);
  duk_ret_t Error(duk_context*);

  duk_ret_t Helper(duk_context*, const std::string& type, std::string_view colour = "");
};
