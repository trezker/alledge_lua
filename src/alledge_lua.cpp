extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include "../alledge_lua/Vector3.h"
#include "../alledge_lua/Scenenode.h"
#include "../alledge_lua/Cameranode.h"
#include "../alledge_lua/Quadnode.h"
#include "../alledge_lua/Bitmap.h"
#include "alledge/View.h"

namespace alledge_lua
{

static int init_perspective_view(lua_State *L)
{
	float fov = luaL_checknumber(L, 1);
	float aspect = luaL_checknumber(L, 2);
	float near = luaL_checknumber(L, 3);
	float far = luaL_checknumber(L, 4);
	Init_perspective_view(fov, aspect, near, far);
	return 0;
}

static int pop_view(lua_State *L)
{
	Pop_view();
	return 0;
}

static const luaL_reg alledge_lua_lib[] = {
	{"init_perspective_view", init_perspective_view},
	{"pop_view", pop_view},
	{NULL, NULL}
};

extern "C"
{
int luaopen_liballedge_lua(lua_State* L)
{
	luaL_register (L, "alledge_lua", alledge_lua_lib);
	register_vector3(L);
	register_scenenode(L);
	register_cameranode(L);
	register_quadnode(L);
	register_bitmap(L);
	lua_pop(L, 1);
	return 0;
}
}

}
