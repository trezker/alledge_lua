#ifndef alledge_lua_shader_program_h
#define alledge_lua_shader_program_h

extern "C" {
#include <lua5.1/lua.h>
#include <lua5.1/lualib.h>
#include <lua5.1/lauxlib.h>
}
#include "alledge/Shader_program.h"
#include "alledge/shared_ptr.h"

namespace alledge_lua
{

#define SHADER_PROGRAM_STRING "shader_program"

class Shader_program_ud
{
public:
	Shader_program_ud(shared_ptr<Shader_program> w)
	{
		shader_program = w;
	}
	shared_ptr<Shader_program> shader_program;
};

int register_shader_program (lua_State* L);

shared_ptr<Shader_program> check_shader_program (lua_State *L, int index);
Shader_program_ud* check_shader_program_ud (lua_State *L, int index);
Shader_program_ud* push_shader_program (lua_State *L, shared_ptr<Shader_program> im);

}

#endif
