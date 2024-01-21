#include "ScriptEngine.hpp"
#include "Defines.hpp"
#include "ScriptLogging.hpp"

#include <dukglue/dukglue.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <utility>

static std::shared_ptr<ScriptEngine> instance;

void DukDestructor::operator()(duk_context* ctx) const
{
  duk_destroy_heap(ctx);
}

void ScriptEngine::Initialize()
{
#ifdef assert
  assert(instance == nullptr);
#endif

  instance = std::make_shared<ScriptEngine>();
  instance->RegisterGlobalObjects();
}

std::shared_ptr<ScriptEngine> ScriptEngine::The()
{
#ifdef assert
  assert(instance != nullptr);
#endif

  return instance;
}

ScriptEngine::ScriptEngine()
{
  auto fatalHandler = [](void* uData, const char* msg) { ScriptEngine::The()->JS_FatalError(uData, msg); };

  auto* dukCtx = duk_create_heap(nullptr, nullptr, nullptr, nullptr, fatalHandler);
  m_Context = DukPtr(dukCtx, DukDestructor());
}

ScriptEngine::~ScriptEngine()
{
  for(auto& [prefix, router] : m_Routers)
  {
	delete router;
  }
}

DukPtr ScriptEngine::GetContext()
{
  return m_Context;
}

void ScriptEngine::Execute(const std::string& script)
{
  i32 result = duk_peval_string(m_Context.get(), script.c_str());
  if(result != 0)
  {
	std::cout << "Failed to execute JavaScript: " << duk_safe_to_string(m_Context.get(), -1) << std::endl;
	duk_pop(m_Context.get());
  }
}

void ScriptEngine::Execute(const std::string& script, const std::filesystem::path& filename)
{
  auto filenameStr = filename.string();
  duk_push_string(m_Context.get(), filenameStr.c_str());
  i32 result = duk_pcompile_lstring_filename(m_Context.get(), 0, script.c_str(), script.size());
  if(result != 0)
  {
	auto* msg = duk_safe_to_string(m_Context.get(), -1);
	std::cout << "Failed to compile JavaScript file (" << filename << "): " << msg << std::endl;
	duk_pop(m_Context.get());
  }

  result = duk_pcall(m_Context.get(), 0);
  if(result != 0)
  {
	if(duk_is_error(m_Context.get(), -1))
	{
	  // get the stack trace
	  duk_get_prop_string(m_Context.get(), -1, "stack");
	  auto* msg = duk_safe_to_string(m_Context.get(), -1);

	  std::cout << "Failed to execute JavaScript file (" << filename << "): " << msg << std::endl;
	  duk_pop(m_Context.get());
	}
	else
	{
	  auto* msg = duk_safe_to_string(m_Context.get(), -1);
	  std::cout << "Failed to execute JavaScript file (" << filename << "): " << msg << std::endl;
	}
  }

  duk_pop(m_Context.get());
}

void ScriptEngine::JS_FatalError(void* uData, const char* msg)
{
  std::cout << "Caught a fatal exception in JavaScript land: " << msg << std::endl;
}

void ScriptEngine::RegisterGlobalObjects()
{
  // Load native objects.
  {
	// ScriptLogging
	dukglue_register_constructor<ScriptLogging>(m_Context.get(), "ScriptLogging");
	dukglue_register_method_varargs(m_Context.get(), &ScriptLogging::Debug, "debug");
	dukglue_register_method_varargs(m_Context.get(), &ScriptLogging::Log, "log");
	dukglue_register_method_varargs(m_Context.get(), &ScriptLogging::Warn, "warn");
	dukglue_register_method_varargs(m_Context.get(), &ScriptLogging::Error, "error");

	// Router
	// FIXME: These should not be variadic.
	//		  Can't figure out how to pass a JavaScript function to C++ land.
	//		  (Argument 1: expected native object (missing type_info))
	dukglue_register_constructor<ScriptRouter, std::string>(m_Context.get(), "Router");
	dukglue_register_method_varargs(m_Context.get(), &ScriptRouter::RegisterGet, "get");
	dukglue_register_method_varargs(m_Context.get(), &ScriptRouter::RegisterPost, "post");
	dukglue_register_method_varargs(m_Context.get(), &ScriptRouter::RegisterPut, "put");
	dukglue_register_method_varargs(m_Context.get(), &ScriptRouter::RegisterDelete, "delete");
  }

  // FIXME: Load better.
  for(auto& entry : std::filesystem::directory_iterator("lib"))
  {
	const auto& path = entry.path();
	auto filename = path.filename();
	auto ext = path.extension();

	if(ext != ".js")
	  continue;

	std::ifstream file(path);
	std::string script((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	Execute(script, filename);
  }
}

void ScriptEngine::AddRouter(const std::string& prefix, ScriptRouter* router)
{
  m_Routers[prefix] = router;
}

const RouterMap& ScriptEngine::GetRouters() const
{
  return m_Routers;
}

ScriptRouter::ScriptRouter(std::string prefix) : m_Prefix(std::move(prefix))
{
  // FIXME: This should be changed.
  // FIXME: Should not pass raw pointer.

  // Register the router.
  ScriptEngine::The()->AddRouter(m_Prefix, this);
}

duk_ret_t ScriptRouter::RegisterGet(duk_context* ctx)
{
  return Register(ctx, HTTPMethod::GET);
}

duk_ret_t ScriptRouter::RegisterPost(duk_context* ctx)
{
  return Register(ctx, HTTPMethod::POST);
}

duk_ret_t ScriptRouter::RegisterPut(duk_context* ctx)
{
  return Register(ctx, HTTPMethod::PUT);
}

duk_ret_t ScriptRouter::RegisterDelete(duk_context* ctx)
{
  return Register(ctx, HTTPMethod::DELETE);
}

duk_ret_t ScriptRouter::Register(duk_context* ctx, HTTPMethod method)
{
  auto check = Precheck(ctx);
  if(check != 0)
	return check;

  auto path = duk_get_string(ctx, 0);

  // Place the function into the globalThis.__functionStore array.
  duk_get_global_string(ctx, "__functionStore");
  duk_dup(ctx, 1);

  auto idx = duk_get_length(ctx, -2);

  duk_put_prop_index(ctx, -2, idx);
  duk_pop(ctx);

  // Register the route.
  m_Routes.push_back({path, method, static_cast<u32>(idx)});

  // Return "this"
  duk_push_this(ctx);

  return 1;
}

duk_ret_t ScriptRouter::Precheck(duk_context* ctx)
{
  // Check that there are two arguments
  if(duk_get_top(ctx) != 2)
  {
	duk_error(ctx, DUK_RET_TYPE_ERROR, "Expected two arguments.");
	return DUK_RET_TYPE_ERROR;
  }

  // Check that the first argument is a string
  if(!duk_is_string(ctx, 0))
  {
	duk_error(ctx, DUK_RET_TYPE_ERROR, "Expected string as first argument.");
	return DUK_RET_TYPE_ERROR;
  }

  // Check that the second argument is a function
  if(!duk_is_function(ctx, 1))
  {
	duk_error(ctx, DUK_RET_TYPE_ERROR, "Expected function as second argument.");
	return DUK_RET_TYPE_ERROR;
  }

  return 0;
}

const std::vector<ScriptRouter::Route>& ScriptRouter::GetRoutes()
{
  return m_Routes;
}
