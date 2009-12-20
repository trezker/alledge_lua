#ifndef allua_quadnode_h
#define allua_quadnode_h

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}
#include "alledge/Quadnode.h"
#include "Scenenode.h"
#include "alledge/shared_ptr.h"

namespace alledge_lua
{

class Quadnode_ud: public Scenenode_ud
{
public:
	Quadnode_ud(shared_ptr<Quadnode> b)
	:Scenenode_ud(b)
	{
	}
};

int register_quadnode (lua_State* L);

shared_ptr<Quadnode> check_quadnode (lua_State *L, int index);
Quadnode_ud* check_quadnode_ud (lua_State *L, int index);
Quadnode_ud* push_quadnode (lua_State *L, shared_ptr<Quadnode> im);

}

#endif
