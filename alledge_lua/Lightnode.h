#ifndef allua_lightnode_h
#define allua_lightnode_h

extern "C" {
#include <lua5.1/lua.h>
#include <lua5.1/lualib.h>
#include <lua5.1/lauxlib.h>
}
#include "alledge/Lightnode.h"
#include "Scenenode.h"
#include "alledge/shared_ptr.h"

namespace alledge_lua
{

class Lightnode_ud: public Scenenode_ud
{
public:
	Lightnode_ud(shared_ptr<Lightnode> b)
	:Scenenode_ud(b)
	{
	}
};

int register_lightnode (lua_State* L);

shared_ptr<Lightnode> check_lightnode (lua_State *L, int index);
Lightnode_ud* check_lightnode_ud (lua_State *L, int index);
Lightnode_ud* push_lightnode (lua_State *L, shared_ptr<Lightnode> im);

}

#endif
