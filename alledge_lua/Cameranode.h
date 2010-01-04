#ifndef alledge_cameranode_h
#define alledge_cameranode_h

extern "C" {
#include <lua5.1/lua.h>
#include <lua5.1/lualib.h>
#include <lua5.1/lauxlib.h>
}
#include "alledge/Cameranode.h"
#include "Scenenode.h"
#include "alledge/shared_ptr.h"

namespace alledge_lua
{

class Cameranode_ud: public Scenenode_ud
{
public:
	Cameranode_ud(shared_ptr<Cameranode> b)
	:Scenenode_ud(b)
	{
	}
};

int register_cameranode (lua_State* L);

shared_ptr<Cameranode> check_cameranode (lua_State *L, int index);
Cameranode_ud* check_cameranode_ud (lua_State *L, int index);
Cameranode_ud* push_cameranode (lua_State *L, shared_ptr<Cameranode> im);

}

#endif
