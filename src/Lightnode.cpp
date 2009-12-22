#include "../alledge_lua/Scenenode.h"
#include "../alledge_lua/Lightnode.h"
#include "../alledge_lua/Vector3.h"
#include "alledge/Lightnode.h"
#include <cstdio>

namespace alledge_lua
{

#define LIGHTNODE_STRING "lightnode"

/* Common handlers
 * */
shared_ptr<Lightnode> check_lightnode (lua_State *L, int index)
{
	Lightnode_ud** pi;
	shared_ptr<Lightnode> im;
	luaL_checktype(L, index, LUA_TUSERDATA);
	pi = (Lightnode_ud**)luaL_checkudata(L, index, LIGHTNODE_STRING);
	if (*pi == NULL)
		luaL_typerror(L, index, LIGHTNODE_STRING);
	im = (*pi)->scenenode;
	return im;
}

Lightnode_ud* check_lightnode_ud (lua_State *L, int index)
{
	Lightnode_ud** pi;
	luaL_checktype(L, index, LUA_TUSERDATA);
	pi = (Lightnode_ud**)luaL_checkudata(L, index, LIGHTNODE_STRING);
	if (*pi == NULL)
		luaL_typerror(L, index, LIGHTNODE_STRING);
	return *pi;
}

Lightnode_ud *push_lightnode (lua_State *L, shared_ptr<Lightnode> im)
{
  Lightnode_ud **pi = (Lightnode_ud**)lua_newuserdata(L, sizeof(Lightnode_ud*));
  *pi = new Lightnode_ud(im);
  luaL_getmetatable(L, LIGHTNODE_STRING);
  lua_setmetatable(L, -2);
  return *pi;
}

/* Constructor and methods
 * */
static int lightnode_new(lua_State *L)
{
	push_lightnode(L, shared_ptr<Lightnode>(new Lightnode));
	return 1;
}

static int lightnode_cast(lua_State *L)
{
	shared_ptr<Scenenode> scenenode = check_scenenode(L, 1);
	shared_ptr<Lightnode> lightnode(scenenode);
	if(lightnode.get())
		push_lightnode(L, lightnode);
	else
		lua_pushnil(L);
	return 1;
}
/*
static int lightnode_get_position(lua_State *L)
{
	shared_ptr<Lightnode> lightnode = check_lightnode(L, 1);
	push_vector3(L, lightnode->Get_position());
	return 1;
}
*/

static int lightnode_set_position(lua_State *L)
{
	shared_ptr<Lightnode> lightnode = check_lightnode(L, 1);
	Vector3 vector = check_vector3(L, 2);
	bool directional = lua_toboolean(L, 3);
	lightnode->Set_position(vector, directional);
	return 0;
}

static int lightnode_set_common(lua_State *L, float *v)
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
	else if (lua_gettop(L)>=3) {
		v[0] = luaL_checknumber(L, 2);
		v[1] = luaL_checknumber(L, 3);
		v[2] = luaL_checknumber(L, 4);
		if (lua_gettop(L)==4) {
			v[3] = luaL_checknumber(L, 5);
		}
		else
		{
			v[3] = 1;
		}
	}
}

static int lightnode_set_ambient(lua_State *L)
{
	shared_ptr<Lightnode> lightnode = check_lightnode(L, 1);
	float v[4];
	lightnode_set_common(L, v);
	lightnode->Set_ambient(v);
	return 0;
}

static int lightnode_set_diffuse(lua_State *L)
{
	shared_ptr<Lightnode> lightnode = check_lightnode(L, 1);
	float v[4];
	lightnode_set_common(L, v);
	lightnode->Set_diffuse(v);
	return 0;
}

static int lightnode_set_specular(lua_State *L)
{
	shared_ptr<Lightnode> lightnode = check_lightnode(L, 1);
	float v[4];
	lightnode_set_common(L, v);
	lightnode->Set_specular(v);
	return 0;
}

static int lightnode_get_ambient(lua_State *L)
{
	shared_ptr<Lightnode> lightnode = check_lightnode(L, 1);
	float v[4];
	lightnode->Get_ambient(v);
	lua_newtable(L);
	for(int i=0; i<4; ++i)
	{
		lua_pushnumber(L, v[i]);
		lua_rawseti (L, -2, i+1);
	}
	return 1;
}

static int lightnode_get_diffuse(lua_State *L)
{
	shared_ptr<Lightnode> lightnode = check_lightnode(L, 1);
	float v[4];
	lightnode->Get_diffuse(v);
	lua_newtable(L);
	for(int i=0; i<4; ++i)
	{
		lua_pushnumber(L, v[i]);
		lua_rawseti (L, -2, i+1);
	}
	return 1;
}

static int lightnode_get_specular(lua_State *L)
{
	shared_ptr<Lightnode> lightnode = check_lightnode(L, 1);
	float v[4];
	lightnode->Get_specular(v);
	lua_newtable(L);
	for(int i=0; i<4; ++i)
	{
		lua_pushnumber(L, v[i]);
		lua_rawseti (L, -2, i+1);
	}
	return 1;
}

static const luaL_reg lightnode_methods[] = {
	{"new", lightnode_new},
	{"cast", lightnode_cast},
//	{"get_position", lightnode_get_position},
	{"set_position", lightnode_set_position},
	{"set_ambient", lightnode_set_ambient},
	{"set_diffuse", lightnode_set_diffuse},
	{"set_specular", lightnode_set_specular},
	{"get_ambient", lightnode_get_ambient},
	{"get_diffuse", lightnode_get_diffuse},
	{"get_specular", lightnode_get_specular},
	{0,0}
};

/* GC and meta
 * */
static int lightnode_gc (lua_State *L)
{
	Lightnode_ud *pi = (Lightnode_ud*)check_lightnode_ud(L, 1);
//	printf("goodbye lightnode (%p)\n", pi->scenenode.get());
	delete pi;
	return 0;
}

static int lightnode_tostring (lua_State *L)
{
	lua_pushfstring(L, "lightnode: %p", lua_touserdata(L, 1));
	return 1;
}

static const luaL_reg lightnode_meta[] = {
	{"__gc",       lightnode_gc},
	{"__tostring", lightnode_tostring},
	{0, 0}
};

/* Register
 * */
int register_lightnode (lua_State *L)
{
	lua_newtable (L);
	luaL_register(L, NULL, lightnode_methods);  /* create methods table,
												add it to the globals */

	//Inherit baseclass methods
	luaL_getmetatable (L, LIGHTNODE_STRING);
	lua_setmetatable (L, -2);

	luaL_newmetatable(L, LIGHTNODE_STRING);        /* create metatable for Image,
										 add it to the Lua registry */
	//Set that this is a scenenode
	lua_pushliteral(L, "is_scenenode");
	lua_pushboolean(L, true);
	lua_rawset(L, -3);

	luaL_register(L, 0, lightnode_meta);  /* fill metatable */
	lua_pushliteral(L, "__index");
	lua_pushvalue(L, -3);               /* dup methods table*/
	lua_rawset(L, -3);                  /* metatable.__index = methods */
	lua_pushliteral(L, "__metatable");
	lua_pushvalue(L, -3);               /* dup methods table*/
	lua_rawset(L, -3);                  /* hide metatable:
										 metatable.__metatable = methods */
	lua_pop(L, 1);                      /* drop metatable */

	lua_setfield(L, -2, LIGHTNODE_STRING);

	return 0;                           /* return methods on the stack */
}

}
