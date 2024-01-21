#pragma once

#include "Defines.hpp"
#include "HTTP/HTTPMethod.hpp"

#include <dukglue/dukvalue.h>
#include <duktape.h>

#include <filesystem>
#include <map>
#include <memory>
#include <string>

struct DukDestructor
{
  void operator()(duk_context* ctx) const;
};

typedef std::shared_ptr<duk_context> DukPtr;

struct ScriptRouter
{
  struct Route
  {
	std::string path;
	HTTPMethod method;
	u32 handlerIdx;
  };

  explicit ScriptRouter(std::string prefix);

  duk_ret_t RegisterGet(duk_context*);
  duk_ret_t RegisterPost(duk_context*);
  duk_ret_t RegisterPut(duk_context*);
  duk_ret_t RegisterDelete(duk_context*);

private:
  duk_ret_t Precheck(duk_context*);

private:
  std::string m_Prefix;

  std::vector<Route> m_Routes;
};

class ScriptEngine
{
public:
  static void Initialize();
  static std::shared_ptr<ScriptEngine> The();

  ScriptEngine();
  ~ScriptEngine();

  void Execute(const std::string& script);
  void Execute(const std::string& script, const std::filesystem::path& filename);

  void AddRouter(const std::string& prefix, ScriptRouter* router);

private:
  void JS_FatalError(void* uData, const char* msg);

  void RegisterGlobalObjects();

private:
  DukPtr m_Context;
  std::map<std::string, ScriptRouter*> m_Routers;
};
