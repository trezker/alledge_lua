#include "../alledge_lua/Scenenode.h"
#include "../alledge_lua/Quadnode.h"
#include "../alledge_lua/Vector3.h"
#include "../alledge_lua/Bitmap.h"
#include "alledge/Quadnode.h"
#include <stdio.h>

namespace alledge_lua
{

#define QUADNODE_STRING "quadnode"

/* Common handlers
 * */
shared_ptr<Quadnode> check_quadnode (lua_State *L, int index)
{
	Quadnode_ud** pi;
	shared_ptr<Quadnode> im;
	luaL_checktype(L, index, LUA_TUSERDATA);
	pi = (Quadnode_ud**)luaL_checkudata(L, index, QUADNODE_STRING);
	if (*pi == NULL)
		luaL_typerror(L, index, QUADNODE_STRING);
	im = (*pi)->scenenode;
	return im;
}

Quadnode_ud* check_quadnode_ud (lua_State *L, int index)
{
	Quadnode_ud** pi;
	luaL_checktype(L, index, LUA_TUSERDATA);
	pi = (Quadnode_ud**)luaL_checkudata(L, index, QUADNODE_STRING);
	if (*pi == NULL)
		luaL_typerror(L, index, QUADNODE_STRING);
	return *pi;
}

Quadnode_ud *push_quadnode (lua_State *L, shared_ptr<Quadnode> im)
{
  Quadnode_ud **pi = (Quadnode_ud**)lua_newuserdata(L, sizeof(Quadnode_ud*));
  *pi = new Quadnode_ud(im);
  luaL_getmetatable(L, QUADNODE_STRING);
  lua_setmetatable(L, -2);
  return *pi;
}

/* Constructor and methods
 * */
static int quadnode_new(lua_State *L)
{
	push_quadnode(L, shared_ptr<Quadnode>(new Quadnode));
	return 1;
}

static int quadnode_cast(lua_State *L)
{
	shared_ptr<Scenenode> scenenode = check_scenenode(L, 1);
	shared_ptr<Quadnode> quadnode(scenenode);
	if(quadnode.get())
		push_quadnode(L, quadnode);
	else
		lua_pushnil(L);
	return 1;
}

static int quadnode_set_texture(lua_State *L)
{
	shared_ptr<Quadnode> quadnode = check_quadnode(L, 1);
	shared_ptr<Bitmap> bitmap = check_bitmap(L, 2);
	quadnode->Set_texture(bitmap);
	return 0;
}

static int quadnode_set_corners(lua_State *L)
{
	shared_ptr<Quadnode> quadnode = check_quadnode(L, 1);
	Vector3 corners[4];
	if(lua_istable(L, 2))
	{
		for(int i=1; i<=4; ++i)
		{
			lua_pushnumber(L, i);
			lua_gettable(L, 2);
			corners[i-1] = check_vector3(L, -1);
		}
		lua_pop(L, 4);
	}
	quadnode->Set_corners(corners);
	return 0;
}

static const luaL_reg quadnode_methods[] = {
	{"new", quadnode_new},
	{"cast", quadnode_cast},
	{"set_texture", quadnode_set_texture},
	{"set_corners", quadnode_set_corners},
	{0,0}
};

/* GC and meta
 * */
static int quadnode_gc (lua_State *L)
{
	Quadnode_ud *pi = (Quadnode_ud*)check_quadnode_ud(L, 1);
//	printf("goodbye quadnode (%p)\n", pi->scenenode.get());
	delete pi;
	return 0;
}

static int quadnode_tostring (lua_State *L)
{
	lua_pushfstring(L, "quadnode: %p", lua_touserdata(L, 1));
	return 1;
}

static const luaL_reg quadnode_meta[] = {
	{"__gc",       quadnode_gc},
	{"__tostring", quadnode_tostring},
	{0, 0}
};

/* Register
 * */
int register_quadnode (lua_State *L)
{
	lua_newtable (L);
	luaL_register(L, NULL, quadnode_methods);  /* create methods table,
												add it to the globals */

	//Inherit baseclass methods
	luaL_getmetatable (L, QUADNODE_STRING);
	lua_setmetatable (L, -2);

	luaL_newmetatable(L, QUADNODE_STRING);        /* create metatable for Image,
										 add it to the Lua registry */
	//Set that this is a scenenode
	lua_pushliteral(L, "is_scenenode");
	lua_pushboolean(L, true);
	lua_rawset(L, -3);

	luaL_register(L, 0, quadnode_meta);  /* fill metatable */
	lua_pushliteral(L, "__index");
	lua_pushvalue(L, -3);               /* dup methods table*/
	lua_rawset(L, -3);                  /* metatable.__index = methods */
	lua_pushliteral(L, "__metatable");
	lua_pushvalue(L, -3);               /* dup methods table*/
	lua_rawset(L, -3);                  /* hide metatable:
										 metatable.__metatable = methods */
	lua_pop(L, 1);                      /* drop metatable */

	lua_setfield(L, -2, QUADNODE_STRING);

	return 0;                           /* return methods on the stack */
}

}
