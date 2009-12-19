extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include "../alledge_lua/Vector3.h"
#include "../alledge_lua/Scenenode.h"
#include "../alledge_lua/Cameranode.h"

namespace alledge_lua
{

static const luaL_reg alledge_lua_lib[] = {
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
	lua_pop(L, 1);
	return 0;
}
}

}
