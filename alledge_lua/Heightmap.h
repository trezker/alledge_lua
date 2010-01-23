#ifndef alledge_heightmap_h
#define alledge_heightmap_h

extern "C" {
#include <lua5.1/lua.h>
#include <lua5.1/lualib.h>
#include <lua5.1/lauxlib.h>
}
#include "alledge/Heightmap.h"
#include "Scenenode.h"
#include "alledge/shared_ptr.h"

namespace alledge_lua
{

class Heightmap_ud: public Scenenode_ud
{
public:
	Heightmap_ud(shared_ptr<Heightmap> b)
	:Scenenode_ud(b)
	{
	}
};

int register_heightmap (lua_State* L);

shared_ptr<Heightmap> check_heightmap (lua_State *L, int index);
Heightmap_ud* check_heightmap_ud (lua_State *L, int index);
Heightmap_ud* push_heightmap (lua_State *L, shared_ptr<Heightmap> im);

}

#endif
