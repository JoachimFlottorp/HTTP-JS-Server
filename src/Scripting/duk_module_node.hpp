#pragma once

#include <duktape.h>

#include <filesystem>

duk_ret_t duk_module_node_peval_main(duk_context* ctx, const std::filesystem::path& path);
void duk_module_node_init(duk_context* ctx);
