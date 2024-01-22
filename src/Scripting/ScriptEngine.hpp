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

  const std::vector<Route>& GetRoutes();

private:
  duk_ret_t Register(duk_context*, HTTPMethod);
  duk_ret_t Precheck(duk_context*);

private:
  std::string m_Prefix;

  std::vector<Route> m_Routes;
};

typedef std::map<std::string, ScriptRouter*> RouterMap;

class ScriptEngine
{
public:
  static void Initialize();
  static std::shared_ptr<ScriptEngine> The();

  ScriptEngine();
  ~ScriptEngine();

  DukPtr GetContext();

  void Execute(const std::string& script);
  void Execute(const std::string& script, const std::filesystem::path& filename);

  void AddRouter(const std::string& prefix, ScriptRouter* router);
  const RouterMap& GetRouters() const;

private:
  void JS_FatalError(void* uData, const char* msg);
  static duk_ret_t JS_ResolveModule(duk_context* ctx);
  static duk_ret_t JS_LoadModule(duk_context* ctx);

  void RegisterGlobalObjects();

private:
  DukPtr m_Context;
  RouterMap m_Routers;
};
