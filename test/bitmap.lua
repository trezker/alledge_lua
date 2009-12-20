require('luaunit')
require('liballedge_lua')
USE_EXPECTED_ACTUAL_IN_ASSERT_EQUALS = true
assertEqualsDelta = function(expected, actual, delta)
	assert(math.abs(expected-actual)<delta)
end

Test_bitmap = {}

function Test_bitmap:test00_new()
	bitmap = alledge_lua.bitmap.new()
	assertEquals("bitmap", tostring(bitmap):sub(1, 6))
end

function Test_bitmap:test01_load()
	bitmap = alledge_lua.bitmap.new()
	b = bitmap:load("image.png")
	assertEquals(false, b)
end

function Test_bitmap:test02_draw()
	bitmap = alledge_lua.bitmap.new()
--	bitmap:draw(0, 0, 0) //Requires an allegro display...
end

function Test_bitmap:test03_get_opengl_texture()
	bitmap = alledge_lua.bitmap.new()
--	t = bitmap:get_opengl_texture()
--	assertEquals("number", type(t))
end

LuaUnit:run() -- run all tests
