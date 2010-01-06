#include "../alledge_lua/Scenenode.h"
#include "../alledge_lua/Linenode.h"
#include "../alledge_lua/Vector3.h"
#include "alledge/Linenode.h"
#include <stdio.h>

namespace alledge_lua
{

#define LINENODE_STRING "linenode"

/* Common handlers
 * */
shared_ptr<Linenode> check_linenode (lua_State *L, int index)
{
	Linenode_ud** pi;
	shared_ptr<Linenode> im;
	luaL_checktype(L, index, LUA_TUSERDATA);
	pi = (Linenode_ud**)luaL_checkudata(L, index, LINENODE_STRING);
	if (*pi == NULL)
		luaL_typerror(L, index, LINENODE_STRING);
	im = (*pi)->scenenode;
	return im;
}

Linenode_ud* check_linenode_ud (lua_State *L, int index)
{
	Linenode_ud** pi;
	luaL_checktype(L, index, LUA_TUSERDATA);
	pi = (Linenode_ud**)luaL_checkudata(L, index, LINENODE_STRING);
	if (*pi == NULL)
		luaL_typerror(L, index, LINENODE_STRING);
	return *pi;
}

Linenode_ud *push_linenode (lua_State *L, shared_ptr<Linenode> im)
{
  Linenode_ud **pi = (Linenode_ud**)lua_newuserdata(L, sizeof(Linenode_ud*));
  *pi = new Linenode_ud(im);
  luaL_getmetatable(L, LINENODE_STRING);
  lua_setmetatable(L, -2);
  return *pi;
}

/* Constructor and methods
 * */
static int linenode_new(lua_State *L)
{
	push_linenode(L, shared_ptr<Linenode>(new Linenode));
	return 1;
}

static int linenode_cast(lua_State *L)
{
	shared_ptr<Scenenode> scenenode = check_scenenode(L, 1);
	shared_ptr<Linenode> linenode(scenenode);
	if(linenode.get())
		push_linenode(L, linenode);
	else
		lua_pushnil(L);
	return 1;
}

static int linenode_set_line(lua_State *L)
{
	shared_ptr<Linenode> linenode = check_linenode(L, 1);
	Vector3 start = check_vector3(L, 2);
	Vector3 end = check_vector3(L, 3);
	linenode->Set_line(start, end);
	return 0;
}

static int linenode_set_common(lua_State *L, float *v)
{
	if(lua_istable(L, 2))
	{
		for(int i=1; i<=4; ++i)
		{
			lua_pushnumber(L, i);
			lua_gettable(L, 2);
			v[i-1] = luaL_checknumber(L, -1);
		}
		lua_pop(L, 4);
	}
	else if (lua_gettop(L)>=4) {
		v[0] = luaL_checknumber(L, 2);
		v[1] = luaL_checknumber(L, 3);
		v[2] = luaL_checknumber(L, 4);
		if (lua_gettop(L)==5) {
			v[3] = luaL_checknumber(L, 5);
		}
		else
		{
			v[3] = 1;
		}
	}
}

static int linenode_set_color(lua_State *L)
{
	shared_ptr<Linenode> linenode = check_linenode(L, 1);
	float v[4];
	linenode_set_common(L, v);
	linenode->Set_color(v);
	return 0;
}

static const luaL_reg linenode_methods[] = {
	{"new", linenode_new},
	{"cast", linenode_cast},
	{"set_line", linenode_set_line},
	{"set_color", linenode_set_color},
	{0,0}
};

/* GC and meta
 * */
static int linenode_gc (lua_State *L)
{
	Linenode_ud *pi = (Linenode_ud*)check_linenode_ud(L, 1);
//	printf("goodbye linenode (%p)\n", pi->scenenode.get());
	delete pi;
	return 0;
}

static int linenode_tostring (lua_State *L)
{
	lua_pushfstring(L, "linenode: %p", lua_touserdata(L, 1));
	return 1;
}

static const luaL_reg linenode_meta[] = {
	{"__gc",       linenode_gc},
	{"__tostring", linenode_tostring},
	{0, 0}
};

/* Register
 * */
int register_linenode (lua_State *L)
{
	lua_newtable (L);
	luaL_register(L, NULL, linenode_methods);  /* create methods table,
												add it to the globals */

	//Inherit baseclass methods
	luaL_getmetatable (L, LINENODE_STRING);
	lua_setmetatable (L, -2);

	luaL_newmetatable(L, LINENODE_STRING);        /* create metatable for Image,
										 add it to the Lua registry */
	//Set that this is a scenenode
	lua_pushliteral(L, "is_scenenode");
	lua_pushboolean(L, true);
	lua_rawset(L, -3);

	luaL_register(L, 0, linenode_meta);  /* fill metatable */
	lua_pushliteral(L, "__index");
	lua_pushvalue(L, -3);               /* dup methods table*/
	lua_rawset(L, -3);                  /* metatable.__index = methods */
	lua_pushliteral(L, "__metatable");
	lua_pushvalue(L, -3);               /* dup methods table*/
	lua_rawset(L, -3);                  /* hide metatable:
										 metatable.__metatable = methods */
	lua_pop(L, 1);                      /* drop metatable */

	lua_setfield(L, -2, LINENODE_STRING);

	return 0;                           /* return methods on the stack */
}

}
