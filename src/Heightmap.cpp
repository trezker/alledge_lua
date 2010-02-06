#include "../alledge_lua/Scenenode.h"
#include "../alledge_lua/Heightmap.h"
#include "../alledge_lua/Vector3.h"
#include "../alledge_lua/Bitmap.h"
#include "alledge/Heightmap.h"
#include <stdio.h>

namespace alledge_lua
{

#define HEIGHTMAP_STRING "heightmap"

/* Common handlers
 * */
shared_ptr<Heightmap> check_heightmap (lua_State *L, int index)
{
	Heightmap_ud** pi;
	shared_ptr<Heightmap> im;
	luaL_checktype(L, index, LUA_TUSERDATA);
	pi = (Heightmap_ud**)luaL_checkudata(L, index, HEIGHTMAP_STRING);
	if (*pi == NULL)
		luaL_typerror(L, index, HEIGHTMAP_STRING);
	im = (*pi)->scenenode;
	return im;
}

Heightmap_ud* check_heightmap_ud (lua_State *L, int index)
{
	Heightmap_ud** pi;
	luaL_checktype(L, index, LUA_TUSERDATA);
	pi = (Heightmap_ud**)luaL_checkudata(L, index, HEIGHTMAP_STRING);
	if (*pi == NULL)
		luaL_typerror(L, index, HEIGHTMAP_STRING);
	return *pi;
}

Heightmap_ud *push_heightmap (lua_State *L, shared_ptr<Heightmap> im)
{
  Heightmap_ud **pi = (Heightmap_ud**)lua_newuserdata(L, sizeof(Heightmap_ud*));
  *pi = new Heightmap_ud(im);
  luaL_getmetatable(L, HEIGHTMAP_STRING);
  lua_setmetatable(L, -2);
  return *pi;
}

/* Constructor and methods
 * */
static int heightmap_new(lua_State *L)
{
	push_heightmap(L, shared_ptr<Heightmap>(new Heightmap));
	return 1;
}

static int heightmap_cast(lua_State *L)
{
	shared_ptr<Scenenode> scenenode = check_scenenode(L, 1);
	shared_ptr<Heightmap> heightmap(scenenode);
	if(heightmap.get())
		push_heightmap(L, heightmap);
	else
		lua_pushnil(L);
	return 1;
}

static int heightmap_set_splat_texture(lua_State *L)
{
	shared_ptr<Heightmap> heightmap = check_heightmap(L, 1);
	shared_ptr<Bitmap> bitmap = check_bitmap(L, 2);
	heightmap->Set_splat_texture(bitmap);
	return 0;
}

static int heightmap_set_texture(lua_State *L)
{
	shared_ptr<Heightmap> heightmap = check_heightmap(L, 1);
	shared_ptr<Bitmap> bitmap = check_bitmap(L, 2);
	int channel = luaL_checkint(L, 3);
	heightmap->Set_texture(bitmap, channel);
	return 0;
}

static int heightmap_load(lua_State *L)
{
	shared_ptr<Heightmap> heightmap = check_heightmap(L, 1);
	const char* filename = luaL_checkstring(L, 2);
	heightmap->Load(filename);
	return 0;
}

static int heightmap_set_texture_scale(lua_State *L)
{
	shared_ptr<Heightmap> heightmap = check_heightmap(L, 1);
	float s = luaL_checknumber(L, 2);
	heightmap->Set_texture_scale(s);
	return 0;
}

static int heightmap_set_tilesize(lua_State *L)
{
	shared_ptr<Heightmap> heightmap = check_heightmap(L, 1);
	float s = luaL_checknumber(L, 2);
	heightmap->Set_tilesize(s);
	return 0;
}

static int heightmap_resize(lua_State *L)
{
	shared_ptr<Heightmap> heightmap = check_heightmap(L, 1);
	int width = luaL_checkint(L, 2);
	int height = luaL_checkint(L, 3);
	heightmap->Resize(width, height);
	return 0;
}

static int heightmap_get_height(lua_State *L)
{
	shared_ptr<Heightmap> heightmap = check_heightmap(L, 1);
	int x = luaL_checkint(L, 2);
	int z = luaL_checkint(L, 3);
	lua_pushnumber(L, heightmap->Get_height(x, z));
	return 1;
}

static int heightmap_get_tilesize(lua_State *L)
{
	shared_ptr<Heightmap> heightmap = check_heightmap(L, 1);
	lua_pushnumber(L, heightmap->Get_tilesize());
	return 1;
}

static int heightmap_get_size_x(lua_State *L)
{
	shared_ptr<Heightmap> heightmap = check_heightmap(L, 1);
	lua_pushnumber(L, heightmap->Get_size_x());
	return 1;
}

static int heightmap_get_size_z(lua_State *L)
{
	shared_ptr<Heightmap> heightmap = check_heightmap(L, 1);
	lua_pushnumber(L, heightmap->Get_size_z());
	return 1;
}

static int heightmap_recalc_normals(lua_State *L)
{
	shared_ptr<Heightmap> heightmap = check_heightmap(L, 1);
	heightmap->Recalc_normals();
	return 0;
}

/*
void Apply_brush(float x, float z, float brush_size, float brush_pressure, const float *brush, int brush_points);
*/
static int heightmap_apply_brush(lua_State *L)
{
	shared_ptr<Heightmap> heightmap = check_heightmap(L, 1);
	float x = luaL_checknumber(L, 2);
	float z = luaL_checknumber(L, 3);
	float brush_size = luaL_checknumber(L, 4);
	float brush_pressure = luaL_checknumber(L, 5);
	int brush_points = luaL_checkint(L, 6);
	float brush[brush_points];
	int i = 1;
	for(int i=0; i<brush_points; ++i)
	{
		lua_pushinteger(L, i+1);
		lua_gettable(L, 7);
		brush[i] = luaL_checknumber(L, -1);
	}
	lua_pop(L, brush_points);
	heightmap->Apply_brush(x, z, brush_size, brush_pressure, brush, brush_points);
	return 0;
}

static const luaL_reg heightmap_methods[] = {
	{"new", heightmap_new},
	{"cast", heightmap_cast},
	{"set_splat_texture", heightmap_set_splat_texture},
	{"set_texture", heightmap_set_texture},
	{"load", heightmap_load},
	{"set_texture_scale", heightmap_set_texture_scale},
	{"set_tilesize", heightmap_set_tilesize},
	{"resize", heightmap_resize},
	{"get_height", heightmap_get_height},
	{"get_tilesize", heightmap_get_tilesize},
	{"get_size_x", heightmap_get_size_x},
	{"get_size_z", heightmap_get_size_z},
	{"recalc_normals", heightmap_recalc_normals},
	{"apply_brush", heightmap_apply_brush},
	{0,0}
};

/* GC and meta
 * */
static int heightmap_gc (lua_State *L)
{
	Heightmap_ud *pi = (Heightmap_ud*)check_heightmap_ud(L, 1);
//	printf("goodbye heightmap (%p)\n", pi->scenenode.get());
	delete pi;
	return 0;
}

static int heightmap_tostring (lua_State *L)
{
	lua_pushfstring(L, "heightmap: %p", lua_touserdata(L, 1));
	return 1;
}

static const luaL_reg heightmap_meta[] = {
	{"__gc",       heightmap_gc},
	{"__tostring", heightmap_tostring},
	{0, 0}
};

/* Register
 * */
int register_heightmap (lua_State *L)
{
	lua_newtable (L);
	luaL_register(L, NULL, heightmap_methods);  /* create methods table,
												add it to the globals */

	//Inherit baseclass methods
	luaL_getmetatable (L, HEIGHTMAP_STRING);
	lua_setmetatable (L, -2);

	luaL_newmetatable(L, HEIGHTMAP_STRING);        /* create metatable for Image,
										 add it to the Lua registry */
	//Set that this is a scenenode
	lua_pushliteral(L, "is_scenenode");
	lua_pushboolean(L, true);
	lua_rawset(L, -3);

	luaL_register(L, 0, heightmap_meta);  /* fill metatable */
	lua_pushliteral(L, "__index");
	lua_pushvalue(L, -3);               /* dup methods table*/
	lua_rawset(L, -3);                  /* metatable.__index = methods */
	lua_pushliteral(L, "__metatable");
	lua_pushvalue(L, -3);               /* dup methods table*/
	lua_rawset(L, -3);                  /* hide metatable:
										 metatable.__metatable = methods */
	lua_pop(L, 1);                      /* drop metatable */

	lua_setfield(L, -2, HEIGHTMAP_STRING);

	return 0;                           /* return methods on the stack */
}

}
