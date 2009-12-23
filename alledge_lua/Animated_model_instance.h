#ifndef allua_animated_model_instance_h
#define allua_animated_model_instance_h

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}
#include "alledge/Animated_model_instance.h"
#include "Scenenode.h"
#include "alledge/shared_ptr.h"

namespace alledge_lua
{

class Animated_model_instance_ud: public Scenenode_ud
{
public:
	Animated_model_instance_ud(shared_ptr<Animated_model_instance> b)
	:Scenenode_ud(b)
	{
	}
};

int register_animated_model_instance (lua_State* L);

shared_ptr<Animated_model_instance> check_animated_model_instance (lua_State *L, int index);
Animated_model_instance_ud* check_animated_model_instance_ud (lua_State *L, int index);
Animated_model_instance_ud* push_animated_model_instance (lua_State *L, shared_ptr<Animated_model_instance> im);

}

#endif
