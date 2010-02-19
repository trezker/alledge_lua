#ifndef alledge_lua_shader_h
#define alledge_lua_shader_h

extern "C" {
#include <lua5.1/lua.h>
#include <lua5.1/lualib.h>
#include <lua5.1/lauxlib.h>
}
#include "alledge/Shader.h"
#include "alledge/shared_ptr.h"

namespace alledge_lua
{

#define SHADER_STRING "shader"

class Shader_ud
{
public:
	Shader_ud(shared_ptr<Shader> w)
	{
		shader = w;
	}
	shared_ptr<Shader> shader;
};

int register_shader (lua_State* L);

shared_ptr<Shader> check_shader (lua_State *L, int index);
Shader_ud* check_shader_ud (lua_State *L, int index);
Shader_ud* push_shader (lua_State *L, shared_ptr<Shader> im);

}

#endif
