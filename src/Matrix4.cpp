#include "../alledge_lua/Matrix4.h"
#include "../alledge_lua/Matrix4.h"
#include <cstdio>
#include <cstring>

namespace alledge_lua
{

#define MATRIX4_STRING "matrix4"

/* Common handlers
 * */
Matrix4 check_matrix4 (lua_State *L, int index)
{
  Matrix4 *pi, im;
  luaL_checktype(L, index, LUA_TUSERDATA);
  pi = (Matrix4*)luaL_checkudata(L, index, MATRIX4_STRING);
  if (pi == NULL)
  	luaL_typerror(L, index, MATRIX4_STRING);
  im = *pi;
  return im;
}

Matrix4 *push_matrix4 (lua_State *L, Matrix4 im)
{
  Matrix4 *pi = (Matrix4 *)lua_newuserdata(L, sizeof(Matrix4));
  *pi = im;
  luaL_getmetatable(L, MATRIX4_STRING);
  lua_setmetatable(L, -2);
  return pi;
}

/* Constructor and methods
 * */
static int matrix4_new(lua_State *L)
{
	Matrix4 m;
	if (lua_gettop(L)==16) {
		float v[16];
		for(int i = 0; i<16; ++i)
		{
			v[i] = luaL_checknumber(L, i+1);
		}
		m.Set(v);
	}
	else if (lua_gettop(L)==1) {
		//TODO: Accept a table
		Matrix4 o = check_matrix4(L, 1);
		m = o;
	}
	push_matrix4(L, m);
	return 1;
}

static int matrix4_zero (lua_State *L)
{
	Matrix4 *v = (Matrix4*)lua_touserdata(L, 1);
	v->Zero();
	return 1;
}

static int matrix4_add (lua_State *L)
{
	Matrix4 *v1 = (Matrix4*)lua_touserdata(L, 1);
	Matrix4 *v2 = (Matrix4*)lua_touserdata(L, 2);
	push_matrix4(L, (*v1)+(*v2));
	return 1;
}

static int matrix4_sub (lua_State *L)
{
	Matrix4 *v1 = (Matrix4*)lua_touserdata(L, 1);
	Matrix4 *v2 = (Matrix4*)lua_touserdata(L, 2);
	push_matrix4(L, (*v1)-(*v2));
	return 1;
}

static int matrix4_mul (lua_State *L)
{
	Matrix4 *v = (Matrix4*)lua_touserdata(L, 1);
	float s = luaL_checknumber(L, 2);
	push_matrix4(L, (*v)*s);
	return 1;
}

static int matrix4_div (lua_State *L)
{
	Matrix4 *v = (Matrix4*)lua_touserdata(L, 1);
	float s = luaL_checknumber(L, 2);
	push_matrix4(L, (*v)/s);
	return 1;
}

static int matrix4_eq (lua_State *L)
{
	Matrix4 *v1 = (Matrix4*)lua_touserdata(L, 1);
	Matrix4 *v2 = (Matrix4*)lua_touserdata(L, 2);
	lua_pushboolean(L, (*v1)==(*v2));
	return 1;
}

static const luaL_reg matrix4_methods[] = {
	{"new",           matrix4_new},
	{"zero",           matrix4_zero},
	{0,0}
};

// GC and meta
static int matrix4_tostring (lua_State *L)
{
	lua_pushfstring(L, "matrix4: %p", lua_touserdata(L, 1));
	return 1;
}

static const luaL_reg matrix4_meta[] = {
	{"__tostring", matrix4_tostring},
	{"__add", matrix4_add},
	{"__sub", matrix4_sub},
	{"__mul", matrix4_mul},
	{"__div", matrix4_div},
	{"__eq", matrix4_eq},
	{0, 0}
};

/* Register
 * */
int register_matrix4 (lua_State *L)
{
  lua_newtable(L);
  luaL_register(L, NULL, matrix4_methods);  /* create methods table,
                                                add it to the globals */

  luaL_newmetatable(L, MATRIX4_STRING);        /* create metatable for Image,
                                         add it to the Lua registry */
  luaL_register(L, 0, matrix4_meta);  /* fill metatable */

  lua_pushliteral(L, "__metatable");
  lua_pushvalue(L, -3);               /* dup methods table*/
  lua_rawset(L, -3);                  /* hide metatable:
                                         metatable.__metatable = methods */
  lua_pop(L, 1);                      /* drop metatable */

  lua_setfield(L, -2, MATRIX4_STRING);
  return 0;                           /* return methods on the stack */
}

}
