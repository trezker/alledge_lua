#ifndef alledge_vector3_h
#define alledge_vector3_h

extern "C" {
#include <lua5.1/lua.h>
#include <lua5.1/lualib.h>
#include <lua5.1/lauxlib.h>
}

#include <alledge/Vector3.h>

namespace alledge_lua
{

/* Function: alledge_register_color
 * Registers Color functionality to the lua state.
 * */
int register_vector3 (lua_State* L);

/* Function: alledge_check_color
 * Returns:
 * Pointer to Color instance.
 * */
Vector3 check_vector3 (lua_State *L, int index);
Vector3 *push_vector3 (lua_State *L, Vector3 im);

}

#endif
