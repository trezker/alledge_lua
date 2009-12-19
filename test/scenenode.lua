require('luaunit')
require('liballedge_lua')
USE_EXPECTED_ACTUAL_IN_ASSERT_EQUALS = true
assertEqualsDelta = function(expected, actual, delta)
	assert(math.abs(expected-actual)<delta)
end

Test_scenenode = {}

function Test_scenenode:test00_new()
	scenenode = alledge_lua.scenenode.new()
	assertEquals("scenenode", tostring(scenenode):sub(1, 9))
end

function Test_scenenode:test01_attach()
	scenenode = alledge_lua.scenenode.new()
	childnode = alledge_lua.scenenode.new()
	scenenode:attach_node(childnode)
	scenenode:detach_node(childnode)
end

function Test_scenenode:test02_apply()
	scenenode = alledge_lua.scenenode.new()
	scenenode:apply()
end

LuaUnit:run() -- run all tests
