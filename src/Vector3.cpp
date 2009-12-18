#include "../alledge_lua/Vector3.h"
#include <cstdio>
#include <cstring>

#define VECTOR3_STRING "vector3"

/* Common handlers
 * */
Vector3 alledge_check_vector3 (lua_State *L, int index)
{
  Vector3 *pi, im;
  luaL_checktype(L, index, LUA_TUSERDATA);
  pi = (Vector3*)luaL_checkudata(L, index, VECTOR3_STRING);
  if (pi == NULL)
  	luaL_typerror(L, index, VECTOR3_STRING);
  im = *pi;
  return im;
}

Vector3 *alledge_pushVector3 (lua_State *L, Vector3 im)
{
  Vector3 *pi = (Vector3 *)lua_newuserdata(L, sizeof(Vector3));
  *pi = im;
  luaL_getmetatable(L, VECTOR3_STRING);
  lua_setmetatable(L, -2);
  return pi;
}

/* Constructor and methods
 * */
static int alledge_vector3_new(lua_State *L)
{
	Vector3 v;
	if (lua_gettop(L)==3) {
		v.x = luaL_checknumber(L, 1);
		v.y = luaL_checknumber(L, 2);
		v.z = luaL_checknumber(L, 3);
	}
	else if (lua_gettop(L)==1) {
		Vector3 o = alledge_check_vector3(L, 1);
		v = o;
	}
	alledge_pushVector3(L, v);
	return 1;
}

static int alledge_vector3_length (lua_State *L)
{
	Vector3 *v = (Vector3*)lua_touserdata(L, 1);
	lua_pushnumber(L, v->Length());
	return 1;
}

static int alledge_vector3_squaredlength (lua_State *L)
{
	Vector3 *v = (Vector3*)lua_touserdata(L, 1);
	lua_pushnumber(L, v->SquaredLength());
	return 1;
}

static int alledge_vector3_normalize (lua_State *L)
{
	Vector3 *v = (Vector3*)lua_touserdata(L, 1);
	v->Normalize();
	return 0;
}

static int alledge_vector3_getnormalized (lua_State *L)
{
	Vector3 *v1 = (Vector3*)lua_touserdata(L, 1);
	alledge_pushVector3(L, v1->GetNormalized());
	return 1;
}

static int alledge_vector3_crossproduct (lua_State *L)
{
	Vector3 *v1 = (Vector3*)lua_touserdata(L, 1);
	Vector3 *v2 = (Vector3*)lua_touserdata(L, 2);
	alledge_pushVector3(L, v1->CrossProduct(*v2));
	return 1;
}

static int alledge_vector3_dotproduct (lua_State *L)
{
	Vector3 *v1 = (Vector3*)lua_touserdata(L, 1);
	Vector3 *v2 = (Vector3*)lua_touserdata(L, 2);
	lua_pushnumber(L, v1->DotProduct(*v2));
	return 1;
}

static int alledge_vector3_getangledegrees (lua_State *L)
{
	Vector3 *v1 = (Vector3*)lua_touserdata(L, 1);
	Vector3 *v2 = (Vector3*)lua_touserdata(L, 2);
	lua_pushnumber(L, v1->GetAngleDegree(*v2));
	return 1;
}

static int alledge_vector3_getangleradians (lua_State *L)
{
	Vector3 *v1 = (Vector3*)lua_touserdata(L, 1);
	Vector3 *v2 = (Vector3*)lua_touserdata(L, 2);
	lua_pushnumber(L, v1->GetAngleRadians(*v2));
	return 1;
}

static int alledge_vector3_getproperty (lua_State *L)
{
	Vector3 *v = (Vector3*)lua_touserdata(L, 1);
	const char* i = luaL_checkstring(L, 2);

	switch (*i) { /* lazy! */
		case 'x': case 'r':
			lua_pushnumber(L, v->x);
			break;
		case 'y': case 'g':
			lua_pushnumber(L, v->y);
			break;
		case 'z': case 'b':
			lua_pushnumber(L, v->z);
			break;
		default:
			lua_pushnil(L);
			break;
	}
	return 1;
}

static int alledge_vector3_newindex (lua_State *L)
{
	Vector3 *v = (Vector3*)lua_touserdata(L, 1);
	const char* i = luaL_checkstring(L, 2);

	switch (*i) { /* lazy! */
		case 'x': case 'r':
			v->x = luaL_checknumber(L, 3);
			break;
		case 'y': case 'g':
			v->y = luaL_checknumber(L, 3);
			break;
		case 'z': case 'b':
			v->z = luaL_checknumber(L, 3);
			break;
	}
	return 0;
}

static int alledge_vector3_add (lua_State *L)
{
	Vector3 *v1 = (Vector3*)lua_touserdata(L, 1);
	Vector3 *v2 = (Vector3*)lua_touserdata(L, 2);
	alledge_pushVector3(L, (*v1)+(*v2));
	return 1;
}

static int alledge_vector3_sub (lua_State *L)
{
	Vector3 *v1 = (Vector3*)lua_touserdata(L, 1);
	Vector3 *v2 = (Vector3*)lua_touserdata(L, 2);
	alledge_pushVector3(L, (*v1)-(*v2));
	return 1;
}

static int alledge_vector3_unm (lua_State *L)
{
	Vector3 *v = (Vector3*)lua_touserdata(L, 1);
	alledge_pushVector3(L, -(*v));
	return 1;
}

static int alledge_vector3_mul (lua_State *L)
{
	Vector3 *v = (Vector3*)lua_touserdata(L, 1);
	float s = luaL_checknumber(L, 2);
	alledge_pushVector3(L, (*v)*s);
	return 1;
}

static int alledge_vector3_div (lua_State *L)
{
	Vector3 *v = (Vector3*)lua_touserdata(L, 1);
	float s = luaL_checknumber(L, 2);
	alledge_pushVector3(L, (*v)/s);
	return 1;
}

static int alledge_vector3_eq (lua_State *L)
{
	Vector3 *v1 = (Vector3*)lua_touserdata(L, 1);
	Vector3 *v2 = (Vector3*)lua_touserdata(L, 2);
	lua_pushboolean(L, (*v1)==(*v2));
	return 1;
}

static const luaL_reg alledge_vector3_methods[] = {
	{"new",           alledge_vector3_new},
	{"get",           alledge_vector3_getproperty},
	{"set",           alledge_vector3_newindex},
	{"length",           alledge_vector3_length},
	{"squaredlength",           alledge_vector3_squaredlength},
	{"normalize",           alledge_vector3_normalize},
	{"getnormalized",           alledge_vector3_getnormalized},
	{"crossproduct",           alledge_vector3_crossproduct},
	{"dotproduct",           alledge_vector3_dotproduct},
	{"getangledegrees",           alledge_vector3_getangledegrees},
	{"getangleradians",           alledge_vector3_getangleradians},
	{0,0}
};

static int alledge_vector3_index (lua_State *L)
{
	const char* field = luaL_checkstring(L, 2);
	
	for(int i = 0; alledge_vector3_methods[i].name!=0; ++i)
	{
		if(strcmp(field, alledge_vector3_methods[i].name) == 0)
		{
			lua_pushcfunction (L, alledge_vector3_methods[i].func);
			return 1;
		}
	}
	
	return alledge_vector3_getproperty(L);
}

/* GC and meta
 * */
static int alledge_vector3_tostring (lua_State *L)
{
	lua_pushfstring(L, "vector3: %p", lua_touserdata(L, 1));
	return 1;
}

static const luaL_reg alledge_vector3_meta[] = {
	{"__tostring", alledge_vector3_tostring},
	{"__newindex", alledge_vector3_newindex},
	{"__index", alledge_vector3_index},
	{"__add", alledge_vector3_add},
	{"__sub", alledge_vector3_sub},
	{"__unm", alledge_vector3_unm},
	{"__mul", alledge_vector3_mul},
	{"__div", alledge_vector3_div},
	{"__eq", alledge_vector3_eq},
	{0, 0}
};

/* Register
 * */
int alledge_register_vector3 (lua_State *L)
{
  lua_newtable(L);
  luaL_register(L, NULL, alledge_vector3_methods);  /* create methods table,
                                                add it to the globals */

  luaL_newmetatable(L, VECTOR3_STRING);        /* create metatable for Image,
                                         add it to the Lua registry */
  luaL_register(L, 0, alledge_vector3_meta);  /* fill metatable */

  lua_pushliteral(L, "__metatable");
  lua_pushvalue(L, -3);               /* dup methods table*/
  lua_rawset(L, -3);                  /* hide metatable:
                                         metatable.__metatable = methods */
  lua_pop(L, 1);                      /* drop metatable */

  lua_setfield(L, -2, VECTOR3_STRING);
  return 0;                           /* return methods on the stack */
}
