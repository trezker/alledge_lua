require('luaunit')
require('liballedge_lua')
USE_EXPECTED_ACTUAL_IN_ASSERT_EQUALS = true
assertEqualsDelta = function(expected, actual, delta)
	assert(math.abs(expected-actual)<delta)
end

Test_transformnode = {}

function Test_transformnode:test00_new()
	transformnode = alledge_lua.transformnode.new()
	assertEquals("transformnode", tostring(transformnode):sub(1, 13))
end

function Test_transformnode:test01_is_scenenode()
	transformnode = alledge_lua.transformnode.new()
	scenenode = alledge_lua.scenenode.new()
	scenenode:attach_node(transformnode:cast())
end

function Test_transformnode:test02_apply()
	transformnode = alledge_lua.transformnode.new()
	scenenode = alledge_lua.scenenode.new()
	scenenode:attach_node(transformnode)
	scenenode:apply()
end

function Test_transformnode:test04_position_rotation()
	transformnode = alledge_lua.transformnode.new()
	setpos = alledge_lua.vector3.new(1, 2, 3)
	setrot = alledge_lua.vector3.new(4, 5, 6)
	transformnode:set_position(setpos)
	transformnode:set_rotation(setrot)
	getpos = transformnode:get_position()
	getrot = transformnode:get_rotation()
	assertEquals(setpos, getpos)
	assertEquals(setrot, getrot)
end

LuaUnit:run() -- run all tests
