#ifndef alledge_vector3_h
#define alledge_vector3_h

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include <alledge/Vector3.h>

namespace alledge_lua
{

/* Function: allua_register_color
 * Registers Color functionality to the lua state.
 * */
int register_vector3 (lua_State* L);

/* Function: allua_check_color
 * Returns:
 * Pointer to Color instance.
 * */
Vector3 check_vector3 (lua_State *L, int index);
Vector3 *pushVector3 (lua_State *L, Vector3 im);

}

#endif
