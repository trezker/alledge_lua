#ifndef alledge_scenenode_h
#define alledge_scenenode_h

extern "C" {
#include <lua5.1/lua.h>
#include <lua5.1/lualib.h>
#include <lua5.1/lauxlib.h>
}
#include "alledge/Scenenode.h"
#include "alledge/shared_ptr.h"

namespace alledge_lua
{

#define SCENENODE_STRING "scenenode"

class Scenenode_ud
{
public:
	Scenenode_ud(shared_ptr<Scenenode> w)
	{
		scenenode = w;
	}
	shared_ptr<Scenenode> scenenode;
};

int register_scenenode (lua_State* L);

shared_ptr<Scenenode> check_scenenode (lua_State *L, int index);
Scenenode_ud* check_scenenode_ud (lua_State *L, int index);
Scenenode_ud* push_scenenode (lua_State *L, shared_ptr<Scenenode> im);

}

#endif
