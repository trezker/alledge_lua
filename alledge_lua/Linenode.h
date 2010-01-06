#ifndef alledge_linenode_h
#define alledge_linenode_h

extern "C" {
#include <lua5.1/lua.h>
#include <lua5.1/lualib.h>
#include <lua5.1/lauxlib.h>
}
#include "alledge/Linenode.h"
#include "Scenenode.h"
#include "alledge/shared_ptr.h"

namespace alledge_lua
{

class Linenode_ud: public Scenenode_ud
{
public:
	Linenode_ud(shared_ptr<Linenode> b)
	:Scenenode_ud(b)
	{
	}
};

int register_linenode (lua_State* L);

shared_ptr<Linenode> check_linenode (lua_State *L, int index);
Linenode_ud* check_linenode_ud (lua_State *L, int index);
Linenode_ud* push_linenode (lua_State *L, shared_ptr<Linenode> im);

}

#endif
