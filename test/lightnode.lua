require('luaunit')
require('liballedge_lua')
USE_EXPECTED_ACTUAL_IN_ASSERT_EQUALS = true
assertEqualsDelta = function(expected, actual, delta)
	assert(math.abs(expected-actual)<delta)
end

Test_lightnode = {}

function Test_lightnode:test00_new()
	lightnode = alledge_lua.lightnode.new()
	assertEquals("lightnode", tostring(lightnode):sub(1, 9))
end

function Test_lightnode:test01_is_scenenode()
	lightnode = alledge_lua.lightnode.new()
	scenenode = alledge_lua.scenenode.new()
	scenenode:attach_node(lightnode:cast())
end

function Test_lightnode:test02_apply()
	lightnode = alledge_lua.lightnode.new()
	scenenode = alledge_lua.scenenode.new()
	scenenode:attach_node(lightnode)
	scenenode:apply()
end

function Test_lightnode:test03_position()
	lightnode = alledge_lua.lightnode.new()
	setpos = alledge_lua.vector3.new(1, 2, 3)
	lightnode:set_position(setpos, true)
--	getpos = lightnode:get_position()
--	assertEquals(setpos, getpos)
end

LuaUnit:run() -- run all tests
