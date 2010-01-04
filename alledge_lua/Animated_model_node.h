#ifndef alledge_animated_model_node_h
#define alledge_animated_model_node_h

extern "C" {
#include <lua5.1/lua.h>
#include <lua5.1/lualib.h>
#include <lua5.1/lauxlib.h>
}
#include "alledge/Animated_model_node.h"
#include "Scenenode.h"
#include "alledge/shared_ptr.h"

namespace alledge_lua
{

class Animated_model_node_ud: public Scenenode_ud
{
public:
	Animated_model_node_ud(shared_ptr<Animated_model_node> b)
	:Scenenode_ud(b)
	{
	}
};

int register_animated_model_node (lua_State* L);

shared_ptr<Animated_model_node> check_animated_model_node (lua_State *L, int index);
Animated_model_node_ud* check_animated_model_node_ud (lua_State *L, int index);
Animated_model_node_ud* push_animated_model_node (lua_State *L, shared_ptr<Animated_model_node> im);

}

#endif
