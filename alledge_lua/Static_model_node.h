#ifndef alledge_static_model_node_h
#define alledge_static_model_node_h

extern "C" {
#include <lua5.1/lua.h>
#include <lua5.1/lualib.h>
#include <lua5.1/lauxlib.h>
}
#include "alledge/Static_model_node.h"
#include "Scenenode.h"
#include "alledge/shared_ptr.h"

namespace alledge_lua
{

class Static_model_node_ud: public Scenenode_ud
{
public:
	Static_model_node_ud(shared_ptr<Static_model_node> b)
	:Scenenode_ud(b)
	{
	}
};

int register_static_model_node (lua_State* L);

shared_ptr<Static_model_node> check_static_model_node (lua_State *L, int index);
Static_model_node_ud* check_static_model_node_ud (lua_State *L, int index);
Static_model_node_ud* push_static_model_node (lua_State *L, shared_ptr<Static_model_node> im);

}

#endif
