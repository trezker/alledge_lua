#include "../alledge_lua/Bitmap.h"
#include "alledge/Bitmap.h"

#include <stdio.h>

namespace alledge_lua
{

/* Common handlers
 * */
shared_ptr<Bitmap> check_bitmap (lua_State *L, int index)
{
	Bitmap_ud** pi;
	shared_ptr<Bitmap> im;
	luaL_checktype(L, index, LUA_TUSERDATA);

	if(luaL_getmetafield (L, index, "is_bitmap"))
	{
		pi = (Bitmap_ud**)lua_touserdata(L, index);
		return (*pi)->bitmap;
	}

	pi = (Bitmap_ud**)luaL_checkudata(L, index, BITMAP_STRING);
	if (*pi == NULL)
		luaL_typerror(L, index, BITMAP_STRING);
	return (*pi)->bitmap;
}

Bitmap_ud* check_bitmap_ud (lua_State *L, int index)
{
	Bitmap_ud** pi;
	luaL_checktype(L, index, LUA_TUSERDATA);
	pi = (Bitmap_ud**)luaL_checkudata(L, index, BITMAP_STRING);
	if (*pi == NULL)
		luaL_typerror(L, index, BITMAP_STRING);
	return *pi;
}

Bitmap_ud *push_bitmap (lua_State *L, shared_ptr<Bitmap> im)
{
  Bitmap_ud **pi = (Bitmap_ud**)lua_newuserdata(L, sizeof(Bitmap_ud*));
  *pi = new Bitmap_ud(im);
  luaL_getmetatable(L, BITMAP_STRING);
  lua_setmetatable(L, -2);
  return *pi;
}

/* Constructor and methods
 * */
static int bitmap_new(lua_State *L)
{
	push_bitmap(L, shared_ptr<Bitmap>(new Bitmap));
	return 1;
}

static int bitmap_load(lua_State *L)
{
	shared_ptr<Bitmap> bitmap = check_bitmap(L, 1);
	const char* filename = luaL_checkstring(L, 2);
	lua_pushboolean(L, bitmap->Load(filename));
	return 1;
}

static int bitmap_draw(lua_State *L)
{
	shared_ptr<Bitmap> bitmap = check_bitmap(L, 1);
	float x = luaL_checknumber(L, 2);
	float y = luaL_checknumber(L, 3);
	int flags = luaL_checknumber(L, 4);
	bitmap->Draw(x, y, flags);
	return 0;
}

static int bitmap_draw_scaled(lua_State *L)
{
	shared_ptr<Bitmap> bitmap = check_bitmap(L, 1);
	float x = luaL_checknumber(L, 2);
	float y = luaL_checknumber(L, 3);
	float width = luaL_checknumber(L, 4);
	float height = luaL_checknumber(L, 5);
	int flags = luaL_checknumber(L, 6);
	bitmap->Draw_scaled(x, y, width, height, flags);
	return 0;
}

static int bitmap_set_target(lua_State *L)
{
	shared_ptr<Bitmap> bitmap = check_bitmap(L, 1);
	bitmap->Set_target();
	return 0;
}

static int bitmap_get_opengl_texture(lua_State *L)
{
	shared_ptr<Bitmap> bitmap = check_bitmap(L, 1);
	lua_pushnumber(L, bitmap->get_opengl_texture());
	return 1;
}

static int bitmap_eq (lua_State *L)
{
	shared_ptr<Bitmap> a = check_bitmap(L, 1);
	shared_ptr<Bitmap> b = check_bitmap(L, 2);
	lua_pushboolean(L, a.get() == b.get());
	return 1;
}

static const luaL_reg bitmap_methods[] = {
	{"new", bitmap_new},
	{"load", bitmap_load},
	{"draw", bitmap_draw},
	{"draw_scaled", bitmap_draw_scaled},
	{"set_target", bitmap_set_target},
	{"get_opengl_texture", bitmap_get_opengl_texture},
	{"equals",       bitmap_eq},
	{0,0}
};

/* GC and meta
 * */
static int bitmap_gc (lua_State *L)
{
	Bitmap_ud *pi = (Bitmap_ud*)check_bitmap_ud(L, 1);
//	printf("goodbye bitmap (%p)\n", pi->bitmap.get());
	delete pi;
	return 0;
}

static int bitmap_tostring (lua_State *L)
{
	lua_pushfstring(L, "bitmap: %p", lua_touserdata(L, 1));
	return 1;
}


static const luaL_reg bitmap_meta[] = {
	{"__gc",       bitmap_gc},
	{"__tostring", bitmap_tostring},
	{"__eq",       bitmap_eq},
	{0, 0}
};

/* Register
 * */
int register_bitmap (lua_State *L)
{
	lua_newtable (L);
	luaL_register(L, NULL, bitmap_methods);  /* create methods table,
												add it to the globals */

	luaL_newmetatable(L, BITMAP_STRING);        /* create metatable for Image,
										 add it to the Lua registry */
	luaL_register(L, 0, bitmap_meta);  /* fill metatable */
	lua_pushliteral(L, "__index");
	lua_pushvalue(L, -3);               /* dup methods table*/
	lua_rawset(L, -3);                  /* metatable.__index = methods */
	lua_pushliteral(L, "__metatable");
	lua_pushvalue(L, -3);               /* dup methods table*/
	lua_rawset(L, -3);                  /* hide metatable:
										 metatable.__metatable = methods */
	lua_pop(L, 1);                      /* drop metatable */

	lua_setfield(L, -2, BITMAP_STRING);

	return 0;                           /* return methods on the stack */
}

}
