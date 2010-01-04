#ifndef alledge_bitmap_h
#define alledge_bitmap_h

extern "C" {
#include <lua5.1/lua.h>
#include <lua5.1/lualib.h>
#include <lua5.1/lauxlib.h>
}
#include "alledge/Bitmap.h"
#include "alledge/shared_ptr.h"

namespace alledge_lua
{

#define BITMAP_STRING "bitmap"

class Bitmap_ud
{
public:
	Bitmap_ud(shared_ptr<Bitmap> w)
	{
		bitmap = w;
	}
	shared_ptr<Bitmap> bitmap;
};

int register_bitmap (lua_State* L);

shared_ptr<Bitmap> check_bitmap (lua_State *L, int index);
Bitmap_ud* check_bitmap_ud (lua_State *L, int index);
Bitmap_ud* push_bitmap (lua_State *L, shared_ptr<Bitmap> im);

}

#endif
