#include "../alledge_lua/Shader_program.h"
#include "../alledge_lua/Shader.h"
#include "alledge/Shader_program.h"

#include <stdio.h>

namespace alledge_lua
{

/* Common handlers
 * */
shared_ptr<Shader_program> check_shader_program (lua_State *L, int index)
{
	Shader_program_ud** pi;
	shared_ptr<Shader_program> im;
	luaL_checktype(L, index, LUA_TUSERDATA);

	if(luaL_getmetafield (L, index, "is_shader_program"))
	{
		pi = (Shader_program_ud**)lua_touserdata(L, index);
		return (*pi)->shader_program;
	}

	pi = (Shader_program_ud**)luaL_checkudata(L, index, SHADER_PROGRAM_STRING);
	if (*pi == NULL)
		luaL_typerror(L, index, SHADER_PROGRAM_STRING);
	return (*pi)->shader_program;
}

Shader_program_ud* check_shader_program_ud (lua_State *L, int index)
{
	Shader_program_ud** pi;
	luaL_checktype(L, index, LUA_TUSERDATA);
	pi = (Shader_program_ud**)luaL_checkudata(L, index, SHADER_PROGRAM_STRING);
	if (*pi == NULL)
		luaL_typerror(L, index, SHADER_PROGRAM_STRING);
	return *pi;
}

Shader_program_ud *push_shader_program (lua_State *L, shared_ptr<Shader_program> im)
{
  Shader_program_ud **pi = (Shader_program_ud**)lua_newuserdata(L, sizeof(Shader_program_ud*));
  *pi = new Shader_program_ud(im);
  luaL_getmetatable(L, SHADER_PROGRAM_STRING);
  lua_setmetatable(L, -2);
  return *pi;
}

/* Constructor and methods
 * */
static int shader_program_new(lua_State *L)
{
	push_shader_program(L, shared_ptr<Shader_program>(new Shader_program));
	return 1;
}

static int shader_program_attach_shader(lua_State *L)
{
	shared_ptr<Shader_program> shader_program = check_shader_program(L, 1);
	shared_ptr<Shader> shader = check_shader(L, 2);
	shader_program->Attach_shader(*shader);
	return 0;
}
/*
	void Attach_shader(Shader &shader);
	void Detach_shader(Shader &shader);
	GLuint Get_glprogram();
	GLint Get_uniform_location(const char* name);
	void Link();
	void Use();
*/

static int shader_program_link(lua_State *L)
{
	shared_ptr<Shader_program> shader_program = check_shader_program(L, 1);
	shader_program->Link();
	return 0;
}

static int shader_program_use(lua_State *L)
{
	shared_ptr<Shader_program> shader_program = check_shader_program(L, 1);
	shader_program->Use();
	return 0;
}

static int shader_program_use_fixed(lua_State *L)
{
	glUseProgram(0);
	return 0;
}

static int shader_program_eq (lua_State *L)
{
	shared_ptr<Shader_program> a = check_shader_program(L, 1);
	shared_ptr<Shader_program> b = check_shader_program(L, 2);
	lua_pushboolean(L, a.get() == b.get());
	return 1;
}

static const luaL_reg shader_program_methods[] = {
	{"new", shader_program_new},
	{"attach_shader", shader_program_attach_shader},
	{"link", shader_program_link},
	{"use", shader_program_use},
	{"use_fixed", shader_program_use_fixed},
	{"equals",       shader_program_eq},
	{0,0}
};

/* GC and meta
 * */
static int shader_program_gc (lua_State *L)
{
	Shader_program_ud *pi = (Shader_program_ud*)check_shader_program_ud(L, 1);
//	printf("goodbye shader_program (%p)\n", pi->shader_program.get());
	delete pi;
	return 0;
}

static int shader_program_tostring (lua_State *L)
{
	lua_pushfstring(L, "shader_program: %p", lua_touserdata(L, 1));
	return 1;
}


static const luaL_reg shader_program_meta[] = {
	{"__gc",       shader_program_gc},
	{"__tostring", shader_program_tostring},
	{"__eq",       shader_program_eq},
	{0, 0}
};

/* Register
 * */
int register_shader_program (lua_State *L)
{
	lua_newtable (L);
	luaL_register(L, NULL, shader_program_methods);  /* create methods table,
												add it to the globals */

	luaL_newmetatable(L, SHADER_PROGRAM_STRING);        /* create metatable for Image,
										 add it to the Lua registry */
	luaL_register(L, 0, shader_program_meta);  /* fill metatable */
	lua_pushliteral(L, "__index");
	lua_pushvalue(L, -3);               /* dup methods table*/
	lua_rawset(L, -3);                  /* metatable.__index = methods */
	lua_pushliteral(L, "__metatable");
	lua_pushvalue(L, -3);               /* dup methods table*/
	lua_rawset(L, -3);                  /* hide metatable:
										 metatable.__metatable = methods */
	lua_pop(L, 1);                      /* drop metatable */

	lua_setfield(L, -2, SHADER_PROGRAM_STRING);

	return 0;                           /* return methods on the stack */
}

}
