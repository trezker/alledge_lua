#include "../alledge_lua/Scenenode.h"
#include "../alledge_lua/Animated_model_node.h"
#include "../alledge_lua/Animated_model_instance.h"
#include "../alledge_lua/Vector3.h"
#include "../alledge_lua/Bitmap.h"
#include "alledge/Animated_model_node.h"
#include <stdio.h>

namespace alledge_lua
{

#define ANIMATED_MODEL_NODE_STRING "animated_model_node"

/* Common handlers
 * */
shared_ptr<Animated_model_node> check_animated_model_node (lua_State *L, int index)
{
	Animated_model_node_ud** pi;
	shared_ptr<Animated_model_node> im;
	luaL_checktype(L, index, LUA_TUSERDATA);
	pi = (Animated_model_node_ud**)luaL_checkudata(L, index, ANIMATED_MODEL_NODE_STRING);
	if (*pi == NULL)
		luaL_typerror(L, index, ANIMATED_MODEL_NODE_STRING);
	im = (*pi)->scenenode;
	return im;
}

Animated_model_node_ud* check_animated_model_node_ud (lua_State *L, int index)
{
	Animated_model_node_ud** pi;
	luaL_checktype(L, index, LUA_TUSERDATA);
	pi = (Animated_model_node_ud**)luaL_checkudata(L, index, ANIMATED_MODEL_NODE_STRING);
	if (*pi == NULL)
		luaL_typerror(L, index, ANIMATED_MODEL_NODE_STRING);
	return *pi;
}

Animated_model_node_ud *push_animated_model_node (lua_State *L, shared_ptr<Animated_model_node> im)
{
  Animated_model_node_ud **pi = (Animated_model_node_ud**)lua_newuserdata(L, sizeof(Animated_model_node_ud*));
  *pi = new Animated_model_node_ud(im);
  luaL_getmetatable(L, ANIMATED_MODEL_NODE_STRING);
  lua_setmetatable(L, -2);
  return *pi;
}

/* Constructor and methods
 * */
static int animated_model_node_new(lua_State *L)
{
	push_animated_model_node(L, shared_ptr<Animated_model_node>(new Animated_model_node));
	return 1;
}

static int animated_model_node_cast(lua_State *L)
{
	shared_ptr<Scenenode> scenenode = check_scenenode(L, 1);
	shared_ptr<Animated_model_node> animated_model_node(scenenode);
	if(animated_model_node.get())
		push_animated_model_node(L, animated_model_node);
	else
		lua_pushnil(L);
	return 1;
}

static int animated_model_node_set_model(lua_State *L)
{
	shared_ptr<Animated_model_node> animated_model_node = check_animated_model_node(L, 1);
	shared_ptr<Animated_model_instance> model = check_animated_model_instance(L, 2);
	animated_model_node->Set_model(model);
	return 0;
}

static int animated_model_node_attach_to_bone(lua_State *L)
{
	shared_ptr<Animated_model_node> animated_model_node = check_animated_model_node(L, 1);
	const char* bone = luaL_checkstring(L, 2);
	shared_ptr<Scenenode> node = check_scenenode(L, 3);
	animated_model_node->Attach_to_bone(bone, node);
	return 0;
}

static int animated_model_node_detach_from_bone(lua_State *L)
{
	shared_ptr<Animated_model_node> animated_model_node = check_animated_model_node(L, 1);
	const char* bone = luaL_checkstring(L, 2);
	shared_ptr<Scenenode> node = check_scenenode(L, 3);
	animated_model_node->Detach_from_bone(bone, node);
	return 0;
}

static const luaL_reg animated_model_node_methods[] = {
	{"new", animated_model_node_new},
	{"cast", animated_model_node_cast},
	{"set_model", animated_model_node_set_model},
	{"attach_to_bone", animated_model_node_attach_to_bone},
	{"detach_from_bone", animated_model_node_detach_from_bone},
	{0,0}
};

/* GC and meta
 * */
static int animated_model_node_gc (lua_State *L)
{
	Animated_model_node_ud *pi = (Animated_model_node_ud*)check_animated_model_node_ud(L, 1);
//	printf("goodbye animated_model_node (%p)\n", pi->scenenode.get());
	delete pi;
	return 0;
}

static int animated_model_node_tostring (lua_State *L)
{
	lua_pushfstring(L, "animated_model_node: %p", lua_touserdata(L, 1));
	return 1;
}

static const luaL_reg animated_model_node_meta[] = {
	{"__gc",       animated_model_node_gc},
	{"__tostring", animated_model_node_tostring},
	{0, 0}
};

/* Register
 * */
int register_animated_model_node (lua_State *L)
{
	lua_newtable (L);
	luaL_register(L, NULL, animated_model_node_methods);  /* create methods table,
												add it to the globals */

	//Inherit baseclass methods
	luaL_getmetatable (L, ANIMATED_MODEL_NODE_STRING);
	lua_setmetatable (L, -2);

	luaL_newmetatable(L, ANIMATED_MODEL_NODE_STRING);        /* create metatable for Image,
										 add it to the Lua registry */
	//Set that this is a scenenode
	lua_pushliteral(L, "is_scenenode");
	lua_pushboolean(L, true);
	lua_rawset(L, -3);

	luaL_register(L, 0, animated_model_node_meta);  /* fill metatable */
	lua_pushliteral(L, "__index");
	lua_pushvalue(L, -3);               /* dup methods table*/
	lua_rawset(L, -3);                  /* metatable.__index = methods */
	lua_pushliteral(L, "__metatable");
	lua_pushvalue(L, -3);               /* dup methods table*/
	lua_rawset(L, -3);                  /* hide metatable:
										 metatable.__metatable = methods */
	lua_pop(L, 1);                      /* drop metatable */

	lua_setfield(L, -2, ANIMATED_MODEL_NODE_STRING);

	return 0;                           /* return methods on the stack */
}

}
