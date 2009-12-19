#include "../alledge_lua/Scenenode.h"
#include "../alledge_lua/Cameranode.h"
#include "../alledge_lua/Vector3.h"
#include "alledge/Cameranode.h"
#include <stdio.h>

namespace alledge_lua
{

#define CAMERANODE_STRING "cameranode"

/* Common handlers
 * */
shared_ptr<Cameranode> check_cameranode (lua_State *L, int index)
{
	Cameranode_ud** pi;
	shared_ptr<Cameranode> im;
	luaL_checktype(L, index, LUA_TUSERDATA);
	pi = (Cameranode_ud**)luaL_checkudata(L, index, CAMERANODE_STRING);
	if (*pi == NULL)
		luaL_typerror(L, index, CAMERANODE_STRING);
	im = (*pi)->scenenode;
	return im;
}

Cameranode_ud* check_cameranode_ud (lua_State *L, int index)
{
	Cameranode_ud** pi;
	luaL_checktype(L, index, LUA_TUSERDATA);
	pi = (Cameranode_ud**)luaL_checkudata(L, index, CAMERANODE_STRING);
	if (*pi == NULL)
		luaL_typerror(L, index, CAMERANODE_STRING);
	return *pi;
}

Cameranode_ud *push_cameranode (lua_State *L, shared_ptr<Cameranode> im)
{
  Cameranode_ud **pi = (Cameranode_ud**)lua_newuserdata(L, sizeof(Cameranode_ud*));
  *pi = new Cameranode_ud(im);
  luaL_getmetatable(L, CAMERANODE_STRING);
  lua_setmetatable(L, -2);
  return *pi;
}

/* Constructor and methods
 * */
static int cameranode_new(lua_State *L)
{
	push_cameranode(L, shared_ptr<Cameranode>(new Cameranode));
	return 1;
}

static int cameranode_cast(lua_State *L)
{
	shared_ptr<Scenenode> scenenode = check_scenenode(L, 1);
	shared_ptr<Cameranode> cameranode(scenenode);
	if(cameranode.get())
		push_cameranode(L, cameranode);
	else
		lua_pushnil(L);
	return 1;
}
/*
	void Set_rotate_around_world_origo(bool t);
*/
static int cameranode_look_at(lua_State *L)
{
	shared_ptr<Cameranode> cameranode = check_cameranode(L, 1);
	Vector3 vector = check_vector3(L, 2);
	cameranode->Look_at(vector);
	return 0;
}

static int cameranode_get_up(lua_State *L)
{
	shared_ptr<Cameranode> cameranode = check_cameranode(L, 1);
	push_vector3(L, cameranode->Get_up());
	return 1;
}

static int cameranode_get_front(lua_State *L)
{
	shared_ptr<Cameranode> cameranode = check_cameranode(L, 1);
	push_vector3(L, cameranode->Get_front());
	return 1;
}

static int cameranode_get_right(lua_State *L)
{
	shared_ptr<Cameranode> cameranode = check_cameranode(L, 1);
	push_vector3(L, cameranode->Get_right());
	return 1;
}

static int cameranode_get_position(lua_State *L)
{
	shared_ptr<Cameranode> cameranode = check_cameranode(L, 1);
	push_vector3(L, cameranode->Get_position());
	return 1;
}

static int cameranode_get_rotation(lua_State *L)
{
	shared_ptr<Cameranode> cameranode = check_cameranode(L, 1);
	push_vector3(L, cameranode->Get_rotation());
	return 1;
}

static int cameranode_set_position(lua_State *L)
{
	shared_ptr<Cameranode> cameranode = check_cameranode(L, 1);
	Vector3 vector = check_vector3(L, 2);
	cameranode->Set_position(vector);
	return 0;
}

static int cameranode_set_rotation(lua_State *L)
{
	shared_ptr<Cameranode> cameranode = check_cameranode(L, 1);
	Vector3 vector = check_vector3(L, 2);
	cameranode->Set_rotation(vector);
	return 0;
}

static int cameranode_set_rotate_around_world_origo(lua_State *L)
{
	shared_ptr<Cameranode> cameranode = check_cameranode(L, 1);
	bool t = lua_toboolean(L, 2);
	cameranode->Set_rotate_around_world_origo(t);
	return 0;
}

static const luaL_reg cameranode_methods[] = {
	{"new", cameranode_new},
	{"cast", cameranode_cast},
	{"look_at", cameranode_look_at},
	{"get_up", cameranode_get_up},
	{"get_front", cameranode_get_front},
	{"get_right", cameranode_get_right},
	{"get_position", cameranode_get_position},
	{"get_rotation", cameranode_get_rotation},
	{"set_position", cameranode_set_position},
	{"set_rotation", cameranode_set_rotation},
	{"set_rotate_around_world_origo", cameranode_set_rotate_around_world_origo},
	{0,0}
};

/* GC and meta
 * */
static int cameranode_gc (lua_State *L)
{
	Cameranode_ud *pi = (Cameranode_ud*)check_cameranode_ud(L, 1);
//	printf("goodbye cameranode (%p)\n", pi->scenenode.get());
	delete pi;
	return 0;
}

static int cameranode_tostring (lua_State *L)
{
	lua_pushfstring(L, "cameranode: %p", lua_touserdata(L, 1));
	return 1;
}

static const luaL_reg cameranode_meta[] = {
	{"__gc",       cameranode_gc},
	{"__tostring", cameranode_tostring},
	{0, 0}
};

/* Register
 * */
int register_cameranode (lua_State *L)
{
	lua_newtable (L);
	luaL_register(L, NULL, cameranode_methods);  /* create methods table,
												add it to the globals */

	//Inherit baseclass methods
	luaL_getmetatable (L, CAMERANODE_STRING);
	lua_setmetatable (L, -2);

	luaL_newmetatable(L, CAMERANODE_STRING);        /* create metatable for Image,
										 add it to the Lua registry */
	//Set that this is a scenenode
	lua_pushliteral(L, "is_scenenode");
	lua_pushboolean(L, true);
	lua_rawset(L, -3);

	luaL_register(L, 0, cameranode_meta);  /* fill metatable */
	lua_pushliteral(L, "__index");
	lua_pushvalue(L, -3);               /* dup methods table*/
	lua_rawset(L, -3);                  /* metatable.__index = methods */
	lua_pushliteral(L, "__metatable");
	lua_pushvalue(L, -3);               /* dup methods table*/
	lua_rawset(L, -3);                  /* hide metatable:
										 metatable.__metatable = methods */
	lua_pop(L, 1);                      /* drop metatable */

	lua_setfield(L, -2, CAMERANODE_STRING);

	return 0;                           /* return methods on the stack */
}

}
