#ifndef allua_scenenode_h
#define allua_scenenode_h

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}
#include "alledge/Scenenode.h"
#include "alledge/shared_ptr.h"

namespace adagio
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
