#include "../alledge_lua/Scenenode.h"
#include "alledge/Scenenode.h"

#include <stdio.h>

namespace alledge_lua
{

/* Common handlers
 * */
shared_ptr<Scenenode> check_scenenode (lua_State *L, int index)
{
	Scenenode_ud** pi;
	shared_ptr<Scenenode> im;
	luaL_checktype(L, index, LUA_TUSERDATA);

	if(luaL_getmetafield (L, index, "is_scenenode"))
	{
		pi = (Scenenode_ud**)lua_touserdata(L, index);
		return (*pi)->scenenode;
	}

	pi = (Scenenode_ud**)luaL_checkudata(L, index, SCENENODE_STRING);
	if (*pi == NULL)
		luaL_typerror(L, index, SCENENODE_STRING);
	return (*pi)->scenenode;
}

Scenenode_ud* check_scenenode_ud (lua_State *L, int index)
{
	Scenenode_ud** pi;
	luaL_checktype(L, index, LUA_TUSERDATA);
	pi = (Scenenode_ud**)luaL_checkudata(L, index, SCENENODE_STRING);
	if (*pi == NULL)
		luaL_typerror(L, index, SCENENODE_STRING);
	return *pi;
}

Scenenode_ud *push_scenenode (lua_State *L, shared_ptr<Scenenode> im)
{
  Scenenode_ud **pi = (Scenenode_ud**)lua_newuserdata(L, sizeof(Scenenode_ud*));
  *pi = new Scenenode_ud(im);
  luaL_getmetatable(L, SCENENODE_STRING);
  lua_setmetatable(L, -2);
  return *pi;
}

/* Constructor and methods
 * */
static int scenenode_create(lua_State *L)
{
	push_scenenode(L, shared_ptr<Scenenode>(new Scenenode));
	return 1;
}

static int scenenode_attach_node(lua_State *L)
{
	shared_ptr<Scenenode> scenenode = check_scenenode(L, 1);
	shared_ptr<Scenenode> node = check_scenenode(L, 2);
	scenenode->Attach_node(node);
	return 0;
}

static int scenenode_detach_node(lua_State *L)
{
	shared_ptr<Scenenode> scenenode = check_scenenode(L, 1);
	shared_ptr<Scenenode> node = check_scenenode(L, 2);
	scenenode->Detach_node(node);
	return 0;
}

static int scenenode_apply(lua_State *L)
{
	shared_ptr<Scenenode> scenenode = check_scenenode(L, 1);
	scenenode->Apply();
	return 0;
}

static int scenenode_eq (lua_State *L)
{
	shared_ptr<Scenenode> a = check_scenenode(L, 1);
	shared_ptr<Scenenode> b = check_scenenode(L, 2);
	lua_pushboolean(L, a.get() == b.get());
	return 1;
}

static const luaL_reg scenenode_methods[] = {
	{"create", scenenode_create},
	{"attach_node", scenenode_attach_node},
	{"detach_node", scenenode_detach_node},
	{"apply", scenenode_apply},
	{"equals",       scenenode_eq},
	{0,0}
};

/* GC and meta
 * */
static int scenenode_gc (lua_State *L)
{
	Scenenode_ud *pi = (Scenenode_ud*)check_scenenode_ud(L, 1);
//	printf("goodbye scenenode (%p)\n", pi->scenenode.get());
	delete pi;
	return 0;
}

static int scenenode_tostring (lua_State *L)
{
	lua_pushfstring(L, "scenenode: %p", lua_touserdata(L, 1));
	return 1;
}


static const luaL_reg scenenode_meta[] = {
	{"__gc",       scenenode_gc},
	{"__tostring", scenenode_tostring},
	{"__eq",       scenenode_eq},
	{0, 0}
};

/* Register
 * */
int register_scenenode (lua_State *L)
{
	lua_newtable (L);
	luaL_register(L, NULL, scenenode_methods);  /* create methods table,
												add it to the globals */

	luaL_newmetatable(L, SCENENODE_STRING);        /* create metatable for Image,
										 add it to the Lua registry */
	luaL_register(L, 0, scenenode_meta);  /* fill metatable */
	lua_pushliteral(L, "__index");
	lua_pushvalue(L, -3);               /* dup methods table*/
	lua_rawset(L, -3);                  /* metatable.__index = methods */
	lua_pushliteral(L, "__metatable");
	lua_pushvalue(L, -3);               /* dup methods table*/
	lua_rawset(L, -3);                  /* hide metatable:
										 metatable.__metatable = methods */
	lua_pop(L, 1);                      /* drop metatable */

	lua_setfield(L, -2, SCENENODE_STRING);

	return 0;                           /* return methods on the stack */
}

}
