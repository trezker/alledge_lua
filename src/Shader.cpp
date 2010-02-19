#include "../alledge_lua/Shader.h"
#include "alledge/Shader.h"

#include <stdio.h>

namespace alledge_lua
{

/* Common handlers
 * */
shared_ptr<Shader> check_shader (lua_State *L, int index)
{
	Shader_ud** pi;
	shared_ptr<Shader> im;
	luaL_checktype(L, index, LUA_TUSERDATA);

	if(luaL_getmetafield (L, index, "is_shader"))
	{
		pi = (Shader_ud**)lua_touserdata(L, index);
		return (*pi)->shader;
	}

	pi = (Shader_ud**)luaL_checkudata(L, index, SHADER_STRING);
	if (*pi == NULL)
		luaL_typerror(L, index, SHADER_STRING);
	return (*pi)->shader;
}

Shader_ud* check_shader_ud (lua_State *L, int index)
{
	Shader_ud** pi;
	luaL_checktype(L, index, LUA_TUSERDATA);
	pi = (Shader_ud**)luaL_checkudata(L, index, SHADER_STRING);
	if (*pi == NULL)
		luaL_typerror(L, index, SHADER_STRING);
	return *pi;
}

Shader_ud *push_shader (lua_State *L, shared_ptr<Shader> im)
{
  Shader_ud **pi = (Shader_ud**)lua_newuserdata(L, sizeof(Shader_ud*));
  *pi = new Shader_ud(im);
  luaL_getmetatable(L, SHADER_STRING);
  lua_setmetatable(L, -2);
  return *pi;
}

/* Constructor and methods
 * */
static int shader_new(lua_State *L)
{
	push_shader(L, shared_ptr<Shader>(new Shader));
	return 1;
}

static int shader_load(lua_State *L)
{
	shared_ptr<Shader> shader = check_shader(L, 1);
	const char* filename = luaL_checkstring(L, 2);
	int shader_type = luaL_checkint(L, 3);
	lua_pushboolean(L, shader->Load_shader(filename, shader_type));
	return 1;
}

static int shader_eq (lua_State *L)
{
	shared_ptr<Shader> a = check_shader(L, 1);
	shared_ptr<Shader> b = check_shader(L, 2);
	lua_pushboolean(L, a.get() == b.get());
	return 1;
}

static const luaL_reg shader_methods[] = {
	{"new", shader_new},
	{"load", shader_load},
	{"equals",       shader_eq},
	{0,0}
};

/* GC and meta
 * */
static int shader_gc (lua_State *L)
{
	Shader_ud *pi = (Shader_ud*)check_shader_ud(L, 1);
//	printf("goodbye shader (%p)\n", pi->shader.get());
	delete pi;
	return 0;
}

static int shader_tostring (lua_State *L)
{
	lua_pushfstring(L, "shader: %p", lua_touserdata(L, 1));
	return 1;
}


static const luaL_reg shader_meta[] = {
	{"__gc",       shader_gc},
	{"__tostring", shader_tostring},
	{"__eq",       shader_eq},
	{0, 0}
};

/* Register
 * */
int register_shader (lua_State *L)
{
	lua_newtable (L);
	luaL_register(L, NULL, shader_methods);  /* create methods table,
												add it to the globals */

	luaL_newmetatable(L, SHADER_STRING);        /* create metatable for Image,
										 add it to the Lua registry */
	luaL_register(L, 0, shader_meta);  /* fill metatable */
	lua_pushliteral(L, "__index");
	lua_pushvalue(L, -3);               /* dup methods table*/
	lua_rawset(L, -3);                  /* metatable.__index = methods */
	lua_pushliteral(L, "__metatable");
	lua_pushvalue(L, -3);               /* dup methods table*/
	lua_rawset(L, -3);                  /* hide metatable:
										 metatable.__metatable = methods */
	lua_pop(L, 1);                      /* drop metatable */

	lua_setfield(L, -2, SHADER_STRING);

	return 0;                           /* return methods on the stack */
}

}
