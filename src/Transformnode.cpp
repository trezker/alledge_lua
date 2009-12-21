#include "../alledge_lua/Scenenode.h"
#include "../alledge_lua/Transformnode.h"
#include "../alledge_lua/Vector3.h"
#include "alledge/Transformnode.h"
#include <cstdio>

namespace alledge_lua
{

#define TRANSFORMNODE_STRING "transformnode"

/* Common handlers
 * */
shared_ptr<Transformnode> check_transformnode (lua_State *L, int index)
{
	Transformnode_ud** pi;
	shared_ptr<Transformnode> im;
	luaL_checktype(L, index, LUA_TUSERDATA);
	pi = (Transformnode_ud**)luaL_checkudata(L, index, TRANSFORMNODE_STRING);
	if (*pi == NULL)
		luaL_typerror(L, index, TRANSFORMNODE_STRING);
	im = (*pi)->scenenode;
	return im;
}

Transformnode_ud* check_transformnode_ud (lua_State *L, int index)
{
	Transformnode_ud** pi;
	luaL_checktype(L, index, LUA_TUSERDATA);
	pi = (Transformnode_ud**)luaL_checkudata(L, index, TRANSFORMNODE_STRING);
	if (*pi == NULL)
		luaL_typerror(L, index, TRANSFORMNODE_STRING);
	return *pi;
}

Transformnode_ud *push_transformnode (lua_State *L, shared_ptr<Transformnode> im)
{
  Transformnode_ud **pi = (Transformnode_ud**)lua_newuserdata(L, sizeof(Transformnode_ud*));
  *pi = new Transformnode_ud(im);
  luaL_getmetatable(L, TRANSFORMNODE_STRING);
  lua_setmetatable(L, -2);
  return *pi;
}

/* Constructor and methods
 * */
static int transformnode_new(lua_State *L)
{
	push_transformnode(L, shared_ptr<Transformnode>(new Transformnode));
	return 1;
}

static int transformnode_cast(lua_State *L)
{
	shared_ptr<Scenenode> scenenode = check_scenenode(L, 1);
	shared_ptr<Transformnode> transformnode(scenenode);
	if(transformnode.get())
		push_transformnode(L, transformnode);
	else
		lua_pushnil(L);
	return 1;
}

static int transformnode_get_position(lua_State *L)
{
	shared_ptr<Transformnode> transformnode = check_transformnode(L, 1);
	push_vector3(L, transformnode->Get_position());
	return 1;
}

static int transformnode_get_rotation(lua_State *L)
{
	shared_ptr<Transformnode> transformnode = check_transformnode(L, 1);
	push_vector3(L, transformnode->Get_rotation());
	return 1;
}

static int transformnode_set_position(lua_State *L)
{
	shared_ptr<Transformnode> transformnode = check_transformnode(L, 1);
	Vector3 vector = check_vector3(L, 2);
	transformnode->Set_position(vector);
	return 0;
}

static int transformnode_set_rotation(lua_State *L)
{
	shared_ptr<Transformnode> transformnode = check_transformnode(L, 1);
	Vector3 vector = check_vector3(L, 2);
	transformnode->Set_rotation(vector);
	return 0;
}

static const luaL_reg transformnode_methods[] = {
	{"new", transformnode_new},
	{"cast", transformnode_cast},
	{"get_position", transformnode_get_position},
	{"get_rotation", transformnode_get_rotation},
	{"set_position", transformnode_set_position},
	{"set_rotation", transformnode_set_rotation},
	{0,0}
};

/* GC and meta
 * */
static int transformnode_gc (lua_State *L)
{
	Transformnode_ud *pi = (Transformnode_ud*)check_transformnode_ud(L, 1);
//	printf("goodbye transformnode (%p)\n", pi->scenenode.get());
	delete pi;
	return 0;
}

static int transformnode_tostring (lua_State *L)
{
	lua_pushfstring(L, "transformnode: %p", lua_touserdata(L, 1));
	return 1;
}

static const luaL_reg transformnode_meta[] = {
	{"__gc",       transformnode_gc},
	{"__tostring", transformnode_tostring},
	{0, 0}
};

/* Register
 * */
int register_transformnode (lua_State *L)
{
	lua_newtable (L);
	luaL_register(L, NULL, transformnode_methods);  /* create methods table,
												add it to the globals */

	//Inherit baseclass methods
	luaL_getmetatable (L, TRANSFORMNODE_STRING);
	lua_setmetatable (L, -2);

	luaL_newmetatable(L, TRANSFORMNODE_STRING);        /* create metatable for Image,
										 add it to the Lua registry */
	//Set that this is a scenenode
	lua_pushliteral(L, "is_scenenode");
	lua_pushboolean(L, true);
	lua_rawset(L, -3);

	luaL_register(L, 0, transformnode_meta);  /* fill metatable */
	lua_pushliteral(L, "__index");
	lua_pushvalue(L, -3);               /* dup methods table*/
	lua_rawset(L, -3);                  /* metatable.__index = methods */
	lua_pushliteral(L, "__metatable");
	lua_pushvalue(L, -3);               /* dup methods table*/
	lua_rawset(L, -3);                  /* hide metatable:
										 metatable.__metatable = methods */
	lua_pop(L, 1);                      /* drop metatable */

	lua_setfield(L, -2, TRANSFORMNODE_STRING);

	return 0;                           /* return methods on the stack */
}

}
