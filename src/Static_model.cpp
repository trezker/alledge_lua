#include "../alledge_lua/Static_model.h"
#include "../alledge_lua/Bitmap.h"
#include "alledge/Static_model.h"

#include <stdio.h>

namespace alledge_lua
{

/* Common handlers
 * */
shared_ptr<Static_model> check_static_model (lua_State *L, int index)
{
	Static_model_ud** pi;
	shared_ptr<Static_model> im;
	luaL_checktype(L, index, LUA_TUSERDATA);

	if(luaL_getmetafield (L, index, "is_static_model"))
	{
		pi = (Static_model_ud**)lua_touserdata(L, index);
		return (*pi)->static_model;
	}

	pi = (Static_model_ud**)luaL_checkudata(L, index, STATIC_MODEL_STRING);
	if (*pi == NULL)
		luaL_typerror(L, index, STATIC_MODEL_STRING);
	return (*pi)->static_model;
}

Static_model_ud* check_static_model_ud (lua_State *L, int index)
{
	Static_model_ud** pi;
	luaL_checktype(L, index, LUA_TUSERDATA);
	pi = (Static_model_ud**)luaL_checkudata(L, index, STATIC_MODEL_STRING);
	if (*pi == NULL)
		luaL_typerror(L, index, STATIC_MODEL_STRING);
	return *pi;
}

Static_model_ud *push_static_model (lua_State *L, shared_ptr<Static_model> im)
{
  Static_model_ud **pi = (Static_model_ud**)lua_newuserdata(L, sizeof(Static_model_ud*));
  *pi = new Static_model_ud(im);
  luaL_getmetatable(L, STATIC_MODEL_STRING);
  lua_setmetatable(L, -2);
  return *pi;
}

/* Constructor and methods
 * */
static int static_model_new(lua_State *L)
{
	push_static_model(L, shared_ptr<Static_model>(new Static_model));
	return 1;
}

static int static_model_load_model(lua_State *L)
{
	shared_ptr<Static_model> static_model = check_static_model(L, 1);
	const char* filename = luaL_checkstring(L, 2);
	static_model->Load_model(filename);
	return 0;
}

static int static_model_set_texture(lua_State *L)
{
	shared_ptr<Static_model> static_model = check_static_model(L, 1);
	shared_ptr<Bitmap> bitmap = check_bitmap(L, 2);
	static_model->Set_texture(bitmap);
	return 0;
}

static int static_model_eq (lua_State *L)
{
	shared_ptr<Static_model> a = check_static_model(L, 1);
	shared_ptr<Static_model> b = check_static_model(L, 2);
	lua_pushboolean(L, a.get() == b.get());
	return 1;
}

static const luaL_reg static_model_methods[] = {
	{"new", static_model_new},
	{"load_model", static_model_load_model},
	{"set_texture", static_model_set_texture},
	{"equals",       static_model_eq},
	{0,0}
};

/* GC and meta
 * */
static int static_model_gc (lua_State *L)
{
	Static_model_ud *pi = (Static_model_ud*)check_static_model_ud(L, 1);
//	printf("goodbye static_model (%p)\n", pi->static_model.get());
	delete pi;
	return 0;
}

static int static_model_tostring (lua_State *L)
{
	lua_pushfstring(L, "static_model: %p", lua_touserdata(L, 1));
	return 1;
}


static const luaL_reg static_model_meta[] = {
	{"__gc",       static_model_gc},
	{"__tostring", static_model_tostring},
	{"__eq",       static_model_eq},
	{0, 0}
};

/* Register
 * */
int register_static_model (lua_State *L)
{
	lua_newtable (L);
	luaL_register(L, NULL, static_model_methods);  /* create methods table,
												add it to the globals */

	luaL_newmetatable(L, STATIC_MODEL_STRING);        /* create metatable for Image,
										 add it to the Lua registry */
	luaL_register(L, 0, static_model_meta);  /* fill metatable */
	lua_pushliteral(L, "__index");
	lua_pushvalue(L, -3);               /* dup methods table*/
	lua_rawset(L, -3);                  /* metatable.__index = methods */
	lua_pushliteral(L, "__metatable");
	lua_pushvalue(L, -3);               /* dup methods table*/
	lua_rawset(L, -3);                  /* hide metatable:
										 metatable.__metatable = methods */
	lua_pop(L, 1);                      /* drop metatable */

	lua_setfield(L, -2, STATIC_MODEL_STRING);

	return 0;                           /* return methods on the stack */
}

}
