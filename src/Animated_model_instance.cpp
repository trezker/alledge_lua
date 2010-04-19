#include "../alledge_lua/Scenenode.h"
#include "../alledge_lua/Animated_model_instance.h"
#include "../alledge_lua/Animated_model.h"
#include "../alledge_lua/Vector3.h"
#include "../alledge_lua/Bitmap.h"
#include "alledge/Animated_model_instance.h"
#include <stdio.h>

namespace alledge_lua
{

#define ANIMATED_MODEL_INSTANCE_STRING "animated_model_instance"

/* Common handlers
 * */
shared_ptr<Animated_model_instance> check_animated_model_instance (lua_State *L, int index)
{
	Animated_model_instance_ud** pi;
	shared_ptr<Animated_model_instance> im;
	luaL_checktype(L, index, LUA_TUSERDATA);
	pi = (Animated_model_instance_ud**)luaL_checkudata(L, index, ANIMATED_MODEL_INSTANCE_STRING);
	if (*pi == NULL)
		luaL_typerror(L, index, ANIMATED_MODEL_INSTANCE_STRING);
	im = (*pi)->scenenode;
	return im;
}

Animated_model_instance_ud* check_animated_model_instance_ud (lua_State *L, int index)
{
	Animated_model_instance_ud** pi;
	luaL_checktype(L, index, LUA_TUSERDATA);
	pi = (Animated_model_instance_ud**)luaL_checkudata(L, index, ANIMATED_MODEL_INSTANCE_STRING);
	if (*pi == NULL)
		luaL_typerror(L, index, ANIMATED_MODEL_INSTANCE_STRING);
	return *pi;
}

Animated_model_instance_ud *push_animated_model_instance (lua_State *L, shared_ptr<Animated_model_instance> im)
{
  Animated_model_instance_ud **pi = (Animated_model_instance_ud**)lua_newuserdata(L, sizeof(Animated_model_instance_ud*));
  *pi = new Animated_model_instance_ud(im);
  luaL_getmetatable(L, ANIMATED_MODEL_INSTANCE_STRING);
  lua_setmetatable(L, -2);
  return *pi;
}

/* Constructor and methods
 * */
static int animated_model_instance_new(lua_State *L)
{
	push_animated_model_instance(L, shared_ptr<Animated_model_instance>(new Animated_model_instance));
	return 1;
}

static int animated_model_instance_play_animation(lua_State *L)
{
	shared_ptr<Animated_model_instance> animated_model_instance = check_animated_model_instance(L, 1);
	const char* name = luaL_checkstring(L, 2);
	bool loop = lua_toboolean(L, 3);
	animated_model_instance->Play_animation(name, loop);
	return 0;
}

static int animated_model_instance_pause_animation(lua_State *L)
{
	shared_ptr<Animated_model_instance> animated_model_instance = check_animated_model_instance(L, 1);
	bool loop = lua_toboolean(L, 2);
	animated_model_instance->Pause_animation(loop);
	return 0;
}

static int animated_model_instance_animation_has_ended(lua_State *L)
{
	shared_ptr<Animated_model_instance> animated_model_instance = check_animated_model_instance(L, 1);
	lua_pushboolean(L, animated_model_instance->Animation_has_ended());
	return 1;
}

static int animated_model_instance_set_model(lua_State *L)
{
	shared_ptr<Animated_model_instance> animated_model_instance = check_animated_model_instance(L, 1);
	shared_ptr<Animated_model> model = check_animated_model(L, 2);
	animated_model_instance->Set_model(model);
	return 0;
}
/*
static int animated_model_instance_add_model(lua_State *L)
{
	shared_ptr<Animated_model_instance> animated_model_instance = check_animated_model_instance(L, 1);
	shared_ptr<Animated_model> model = check_animated_model(L, 2);
	animated_model_instance->Add_model(model);
	return 0;
}

static int animated_model_instance_remove_model(lua_State *L)
{
	shared_ptr<Animated_model_instance> animated_model_instance = check_animated_model_instance(L, 1);
	shared_ptr<Animated_model> model = check_animated_model(L, 2);
	animated_model_instance->Remove_model(model);
	return 0;
}

static int animated_model_instance_attach_to_bone(lua_State *L)
{
	shared_ptr<Animated_model_instance> animated_model_instance = check_animated_model_instance(L, 1);
	const char* bone = luaL_checkstring(L, 2);
	shared_ptr<Scenenode> node = check_scenenode(L, 3);
	animated_model_instance->Attach_to_bone(bone, node);
	return 0;
}

static int animated_model_instance_detach_from_bone(lua_State *L)
{
	shared_ptr<Animated_model_instance> animated_model_instance = check_animated_model_instance(L, 1);
	const char* bone = luaL_checkstring(L, 2);
	shared_ptr<Scenenode> node = check_scenenode(L, 3);
	animated_model_instance->Detach_from_bone(bone, node);
	return 0;
}
*/
static int animated_model_instance_update(lua_State *L)
{
	shared_ptr<Animated_model_instance> animated_model_instance = check_animated_model_instance(L, 1);
	float dt = luaL_checknumber(L, 2);
	animated_model_instance->Update(dt);
	return 0;
}

static int animated_model_instance_set_color(lua_State *L)
{
	shared_ptr<Animated_model_instance> model = check_animated_model_instance(L, 1);

	float v[4];
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
	model->Set_color(v);
}

static int animated_model_get_low_corner(lua_State *L)
{
	shared_ptr<Animated_model_instance> animated_model_instance = check_animated_model_instance(L, 1);
	Vector3 corner = animated_model_instance->Get_low_corner();
	push_vector3(L, corner);
	return 1;
}

static int animated_model_get_high_corner(lua_State *L)
{
	shared_ptr<Animated_model_instance> animated_model_instance = check_animated_model_instance(L, 1);
	Vector3 corner = animated_model_instance->Get_high_corner();
	push_vector3(L, corner);
	return 1;
}

static const luaL_reg animated_model_instance_methods[] = {
	{"new", animated_model_instance_new},
	{"play_animation", animated_model_instance_play_animation},
	{"pause_animation", animated_model_instance_pause_animation},
	{"animation_has_ended", animated_model_instance_animation_has_ended},
	{"set_model", animated_model_instance_set_model},
/*	{"add_model", animated_model_instance_add_model},
	{"remove_model", animated_model_instance_remove_model},
	{"attach_to_bone", animated_model_instance_attach_to_bone},
	{"detach_from_bone", animated_model_instance_detach_from_bone},
*/	{"update", animated_model_instance_update},
	{"set_color", animated_model_instance_set_color},
	{"get_low_corner", animated_model_get_low_corner},
	{"get_high_corner", animated_model_get_high_corner},
	{0,0}
};

/* GC and meta
 * */
static int animated_model_instance_gc (lua_State *L)
{
	Animated_model_instance_ud *pi = (Animated_model_instance_ud*)check_animated_model_instance_ud(L, 1);
//	printf("goodbye animated_model_instance (%p)\n", pi->scenenode.get());
	delete pi;
	return 0;
}

static int animated_model_instance_tostring (lua_State *L)
{
	lua_pushfstring(L, "animated_model_instance: %p", lua_touserdata(L, 1));
	return 1;
}

static const luaL_reg animated_model_instance_meta[] = {
	{"__gc",       animated_model_instance_gc},
	{"__tostring", animated_model_instance_tostring},
	{0, 0}
};

/* Register
 * */
int register_animated_model_instance (lua_State *L)
{
	lua_newtable (L);
	luaL_register(L, NULL, animated_model_instance_methods);  /* create methods table,
												add it to the globals */

	//Inherit baseclass methods
	luaL_getmetatable (L, ANIMATED_MODEL_INSTANCE_STRING);
	lua_setmetatable (L, -2);

	luaL_newmetatable(L, ANIMATED_MODEL_INSTANCE_STRING);        /* create metatable for Image,
										 add it to the Lua registry */

	luaL_register(L, 0, animated_model_instance_meta);  /* fill metatable */
	lua_pushliteral(L, "__index");
	lua_pushvalue(L, -3);               /* dup methods table*/
	lua_rawset(L, -3);                  /* metatable.__index = methods */
	lua_pushliteral(L, "__metatable");
	lua_pushvalue(L, -3);               /* dup methods table*/
	lua_rawset(L, -3);                  /* hide metatable:
										 metatable.__metatable = methods */
	lua_pop(L, 1);                      /* drop metatable */

	lua_setfield(L, -2, ANIMATED_MODEL_INSTANCE_STRING);

	return 0;                           /* return methods on the stack */
}

}
