#include "../alledge_lua/Animated_model.h"
#include "alledge/Animated_model.h"

#include <stdio.h>

namespace alledge_lua
{

/* Common handlers
 * */
shared_ptr<Animated_model> check_animated_model (lua_State *L, int index)
{
	Animated_model_ud** pi;
	shared_ptr<Animated_model> im;
	luaL_checktype(L, index, LUA_TUSERDATA);

	if(luaL_getmetafield (L, index, "is_animated_model"))
	{
		pi = (Animated_model_ud**)lua_touserdata(L, index);
		return (*pi)->animated_model;
	}

	pi = (Animated_model_ud**)luaL_checkudata(L, index, ANIMATED_MODEL_STRING);
	if (*pi == NULL)
		luaL_typerror(L, index, ANIMATED_MODEL_STRING);
	return (*pi)->animated_model;
}

Animated_model_ud* check_animated_model_ud (lua_State *L, int index)
{
	Animated_model_ud** pi;
	luaL_checktype(L, index, LUA_TUSERDATA);
	pi = (Animated_model_ud**)luaL_checkudata(L, index, ANIMATED_MODEL_STRING);
	if (*pi == NULL)
		luaL_typerror(L, index, ANIMATED_MODEL_STRING);
	return *pi;
}

Animated_model_ud *push_animated_model (lua_State *L, shared_ptr<Animated_model> im)
{
  Animated_model_ud **pi = (Animated_model_ud**)lua_newuserdata(L, sizeof(Animated_model_ud*));
  *pi = new Animated_model_ud(im);
  luaL_getmetatable(L, ANIMATED_MODEL_STRING);
  lua_setmetatable(L, -2);
  return *pi;
}

/* Constructor and methods
 * */
static int animated_model_new(lua_State *L)
{
	push_animated_model(L, shared_ptr<Animated_model>(new Animated_model));
	return 1;
}

static int animated_model_load_model(lua_State *L)
{
	shared_ptr<Animated_model> animated_model = check_animated_model(L, 1);
	const char* filename = luaL_checkstring(L, 2);
	animated_model->Load_model(filename);
	return 0;
}

static int animated_model_load_animation(lua_State *L)
{
	shared_ptr<Animated_model> animated_model = check_animated_model(L, 1);
	const char* filename = luaL_checkstring(L, 2);
	const char* name = luaL_checkstring(L, 3);
	animated_model->Load_animation(filename, name);
	return 0;
}

static int animated_model_eq (lua_State *L)
{
	shared_ptr<Animated_model> a = check_animated_model(L, 1);
	shared_ptr<Animated_model> b = check_animated_model(L, 2);
	lua_pushboolean(L, a.get() == b.get());
	return 1;
}

static const luaL_reg animated_model_methods[] = {
	{"new", animated_model_new},
	{"load_model", animated_model_load_model},
	{"load_animation", animated_model_load_animation},
	{"equals",       animated_model_eq},
	{0,0}
};

/* GC and meta
 * */
static int animated_model_gc (lua_State *L)
{
	Animated_model_ud *pi = (Animated_model_ud*)check_animated_model_ud(L, 1);
//	printf("goodbye animated_model (%p)\n", pi->animated_model.get());
	delete pi;
	return 0;
}

static int animated_model_tostring (lua_State *L)
{
	lua_pushfstring(L, "animated_model: %p", lua_touserdata(L, 1));
	return 1;
}


static const luaL_reg animated_model_meta[] = {
	{"__gc",       animated_model_gc},
	{"__tostring", animated_model_tostring},
	{"__eq",       animated_model_eq},
	{0, 0}
};

/* Register
 * */
int register_animated_model (lua_State *L)
{
	lua_newtable (L);
	luaL_register(L, NULL, animated_model_methods);  /* create methods table,
												add it to the globals */

	luaL_newmetatable(L, ANIMATED_MODEL_STRING);        /* create metatable for Image,
										 add it to the Lua registry */
	luaL_register(L, 0, animated_model_meta);  /* fill metatable */
	lua_pushliteral(L, "__index");
	lua_pushvalue(L, -3);               /* dup methods table*/
	lua_rawset(L, -3);                  /* metatable.__index = methods */
	lua_pushliteral(L, "__metatable");
	lua_pushvalue(L, -3);               /* dup methods table*/
	lua_rawset(L, -3);                  /* hide metatable:
										 metatable.__metatable = methods */
	lua_pop(L, 1);                      /* drop metatable */

	lua_setfield(L, -2, ANIMATED_MODEL_STRING);

	return 0;                           /* return methods on the stack */
}

}
