#include "../alledge_lua/Scenenode.h"
#include "../alledge_lua/Static_model_node.h"
#include "../alledge_lua/Static_model.h"
#include "alledge/Static_model_node.h"
#include <stdio.h>

namespace alledge_lua
{

#define STATIC_MODEL_NODE_STRING "static_model_node"

/* Common handlers
 * */
shared_ptr<Static_model_node> check_static_model_node (lua_State *L, int index)
{
	Static_model_node_ud** pi;
	shared_ptr<Static_model_node> im;
	luaL_checktype(L, index, LUA_TUSERDATA);
	pi = (Static_model_node_ud**)luaL_checkudata(L, index, STATIC_MODEL_NODE_STRING);
	if (*pi == NULL)
		luaL_typerror(L, index, STATIC_MODEL_NODE_STRING);
	im = (*pi)->scenenode;
	return im;
}

Static_model_node_ud* check_static_model_node_ud (lua_State *L, int index)
{
	Static_model_node_ud** pi;
	luaL_checktype(L, index, LUA_TUSERDATA);
	pi = (Static_model_node_ud**)luaL_checkudata(L, index, STATIC_MODEL_NODE_STRING);
	if (*pi == NULL)
		luaL_typerror(L, index, STATIC_MODEL_NODE_STRING);
	return *pi;
}

Static_model_node_ud *push_static_model_node (lua_State *L, shared_ptr<Static_model_node> im)
{
  Static_model_node_ud **pi = (Static_model_node_ud**)lua_newuserdata(L, sizeof(Static_model_node_ud*));
  *pi = new Static_model_node_ud(im);
  luaL_getmetatable(L, STATIC_MODEL_NODE_STRING);
  lua_setmetatable(L, -2);
  return *pi;
}

/* Constructor and methods
 * */
static int static_model_node_new(lua_State *L)
{
	push_static_model_node(L, shared_ptr<Static_model_node>(new Static_model_node));
	return 1;
}

static int static_model_node_cast(lua_State *L)
{
	shared_ptr<Scenenode> scenenode = check_scenenode(L, 1);
	shared_ptr<Static_model_node> static_model_node(scenenode);
	if(static_model_node.get())
		push_static_model_node(L, static_model_node);
	else
		lua_pushnil(L);
	return 1;
}

static int static_model_node_set_model(lua_State *L)
{
	shared_ptr<Static_model_node> static_model_node = check_static_model_node(L, 1);
	shared_ptr<Static_model> model = check_static_model(L, 2);
	static_model_node->Set_model(model);
	return 0;
}

static const luaL_reg static_model_node_methods[] = {
	{"new", static_model_node_new},
	{"cast", static_model_node_cast},
	{"set_model", static_model_node_set_model},
	{0,0}
};

/* GC and meta
 * */
static int static_model_node_gc (lua_State *L)
{
	Static_model_node_ud *pi = (Static_model_node_ud*)check_static_model_node_ud(L, 1);
//	printf("goodbye static_model_node (%p)\n", pi->scenenode.get());
	delete pi;
	return 0;
}

static int static_model_node_tostring (lua_State *L)
{
	lua_pushfstring(L, "static_model_node: %p", lua_touserdata(L, 1));
	return 1;
}

static const luaL_reg static_model_node_meta[] = {
	{"__gc",       static_model_node_gc},
	{"__tostring", static_model_node_tostring},
	{0, 0}
};

/* Register
 * */
int register_static_model_node (lua_State *L)
{
	lua_newtable (L);
	luaL_register(L, NULL, static_model_node_methods);  /* create methods table,
												add it to the globals */

	//Inherit baseclass methods
	luaL_getmetatable (L, STATIC_MODEL_NODE_STRING);
	lua_setmetatable (L, -2);

	luaL_newmetatable(L, STATIC_MODEL_NODE_STRING);        /* create metatable for Image,
										 add it to the Lua registry */
	//Set that this is a scenenode
	lua_pushliteral(L, "is_scenenode");
	lua_pushboolean(L, true);
	lua_rawset(L, -3);

	luaL_register(L, 0, static_model_node_meta);  /* fill metatable */
	lua_pushliteral(L, "__index");
	lua_pushvalue(L, -3);               /* dup methods table*/
	lua_rawset(L, -3);                  /* metatable.__index = methods */
	lua_pushliteral(L, "__metatable");
	lua_pushvalue(L, -3);               /* dup methods table*/
	lua_rawset(L, -3);                  /* hide metatable:
										 metatable.__metatable = methods */
	lua_pop(L, 1);                      /* drop metatable */

	lua_setfield(L, -2, STATIC_MODEL_NODE_STRING);

	return 0;                           /* return methods on the stack */
}

}
