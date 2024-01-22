#pragma once

#include <duktape.h>

duk_ret_t duk_module_node_peval_main(duk_context* ctx, const char* path);
void duk_module_node_init(duk_context* ctx);
