#ifndef alledge_matrix4_h
#define alledge_matrix4_h

extern "C" {
#include <lua5.1/lua.h>
#include <lua5.1/lualib.h>
#include <lua5.1/lauxlib.h>
}

#include <alledge/Matrix4.h>

namespace alledge_lua
{

int register_matrix4 (lua_State* L);

Matrix4 check_matrix4 (lua_State *L, int index);
Matrix4 *push_matrix4 (lua_State *L, Matrix4 im);

}

#endif
