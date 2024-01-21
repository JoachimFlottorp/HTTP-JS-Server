#include "ScriptLogging.hpp"

#include <duktape.h>

#include <iostream>
#include <string>

constexpr std::string_view ANSI_RESET = "\033[0m";
constexpr std::string_view ANSI_RED = "\033[31m";
constexpr std::string_view ANSI_GREEN = "\033[32m";
constexpr std::string_view ANSI_YELLOW = "\033[33m";
constexpr std::string_view ANSI_CYAN = "\033[36m";

duk_ret_t ScriptLogging::Debug(duk_context* ctx)
{
  return Helper(ctx, "DBG", ANSI_CYAN);
}

duk_ret_t ScriptLogging::Log(duk_context* ctx)
{
  return Helper(ctx, "INF", ANSI_GREEN);
}

duk_ret_t ScriptLogging::Warn(duk_context* ctx)
{
  return Helper(ctx, "WRN", ANSI_YELLOW);
}

duk_ret_t ScriptLogging::Error(duk_context* ctx)
{
  return Helper(ctx, "ERR", ANSI_RED);
}

duk_ret_t ScriptLogging::Helper(duk_context* ctx, const std::string& type, std::string_view colour)
{
  // FIXME: Should maybe support fancy stringify.
  std::string message;

  for(int i = 0; i < duk_get_top(ctx); ++i)
  {
	message += duk_safe_to_string(ctx, i);
  }

  std::cout << colour << "[" << type << "] " << ANSI_RESET << message << std::endl;

  return DUK_ERR_NONE;
}
