#ifndef alledge_transformnode_h
#define alledge_transformnode_h

extern "C" {
#include <lua5.1/lua.h>
#include <lua5.1/lualib.h>
#include <lua5.1/lauxlib.h>
}
#include "alledge/Transformnode.h"
#include "Scenenode.h"
#include "alledge/shared_ptr.h"

namespace alledge_lua
{

class Transformnode_ud: public Scenenode_ud
{
public:
	Transformnode_ud(shared_ptr<Transformnode> b)
	:Scenenode_ud(b)
	{
	}
};

int register_transformnode (lua_State* L);

shared_ptr<Transformnode> check_transformnode (lua_State *L, int index);
Transformnode_ud* check_transformnode_ud (lua_State *L, int index);
Transformnode_ud* push_transformnode (lua_State *L, shared_ptr<Transformnode> im);

}

#endif
