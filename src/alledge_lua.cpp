
#include "../alledge_lua/Vector3.h"
#include "../alledge_lua/Scenenode.h"
#include "../alledge_lua/Cameranode.h"
#include "../alledge_lua/Transformnode.h"
#include "../alledge_lua/Quadnode.h"
#include "../alledge_lua/Linenode.h"
#include "../alledge_lua/Lightnode.h"
#include "../alledge_lua/Bitmap.h"
#include "../alledge_lua/Animated_model.h"
#include "../alledge_lua/Animated_model_instance.h"
#include "../alledge_lua/Animated_model_node.h"
#include "../alledge_lua/Static_model.h"
#include "../alledge_lua/Static_model_node.h"
#include "../alledge_lua/Heightmap.h"
#include "../alledge_lua/Shader.h"
#include "../alledge_lua/Shader_program.h"
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

static int gl_set_viewport(lua_State *L)
{
	int x = luaL_checknumber(L, 1);
	int y = luaL_checknumber(L, 2);
	int w = luaL_checknumber(L, 3);
	int h = luaL_checknumber(L, 4);
	glViewport(x, y, w, h);
	return 0;
}

static int gl_get(lua_State *L)
{
	int pname = luaL_checknumber(L, 1);
	if(pname == GL_VIEWPORT)
	{
		GLint params[4];
		glGetIntegerv(GL_VIEWPORT, params);
		for(int i=0; i<4; ++i)
			lua_pushnumber(L, params[i]);
		return 4;
	}
	return 0;
}

static int gl_enable(lua_State *L)
{
	int en = luaL_checknumber(L, 1);
	glEnable(en);
	return 0;
}

static int gl_disable(lua_State *L)
{
	int en = luaL_checknumber(L, 1);
	glDisable(en);
	return 0;
}

static int gl_clear(lua_State *L)
{
	int en = luaL_checknumber(L, 1);
	glClear(en);
	return 0;
}

static int gl_colormask(lua_State *L)
{
	bool red = lua_toboolean(L, 1);
	bool green = lua_toboolean(L, 2);
	bool blue = lua_toboolean(L, 3);
	bool alpha = lua_toboolean(L, 4);
	glColorMask(red, green, blue, alpha);
	return 0;
}

static const luaL_reg alledge_lua_gl_methods[] = {
	{"enable", gl_enable},
	{"disable", gl_disable},
	{"clear", gl_clear},
	{"colormask", gl_colormask},
	{"get", gl_get},
	{"set_viewport", gl_set_viewport},
	{NULL, NULL}
};

static int gl_register_enum(lua_State *L)
{
	lua_pushinteger(L, GL_DEPTH_TEST);
	lua_setfield(L, -2, "DEPTH_TEST");

	lua_pushinteger(L, GL_DEPTH_BUFFER_BIT);
	lua_setfield(L, -2, "DEPTH_BUFFER_BIT");

	lua_pushinteger(L, GL_LIGHTING);
	lua_setfield(L, -2, "LIGHTING");

	lua_pushinteger(L, GL_FRAGMENT_SHADER);
	lua_setfield(L, -2, "FRAGMENT_SHADER");

	lua_pushinteger(L, GL_VERTEX_SHADER);
	lua_setfield(L, -2, "VERTEX_SHADER");

	lua_pushinteger(L, GL_VIEWPORT);
	lua_setfield(L, -2, "VIEWPORT");
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
	
	lua_newtable (L);
	luaL_register(L, NULL, alledge_lua_gl_methods);
	gl_register_enum(L);
	lua_setfield(L, -2, "gl");

	register_vector3(L);
	register_scenenode(L);
	register_cameranode(L);
	register_quadnode(L);
	register_linenode(L);
	register_transformnode(L);
	register_lightnode(L);
	register_animated_model(L);
	register_animated_model_instance(L);
	register_animated_model_node(L);
	register_static_model(L);
	register_static_model_node(L);
	register_bitmap(L);
	register_heightmap(L);
	register_shader(L);
	register_shader_program(L);
	lua_pop(L, 1);
	return 0;
}
}

}
