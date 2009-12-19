require('luaunit')
require('liballedge_lua')
USE_EXPECTED_ACTUAL_IN_ASSERT_EQUALS = true
assertEqualsDelta = function(expected, actual, delta)
	assert(math.abs(expected-actual)<delta)
end

Test_cameranode = {}

function Test_cameranode:test00_new()
	cameranode = alledge_lua.cameranode.new()
	assertEquals("cameranode", tostring(cameranode):sub(1, 10))
end

function Test_cameranode:test01_is_scenenode()
	cameranode = alledge_lua.cameranode.new()
	scenenode = alledge_lua.scenenode.new()
	scenenode:attach_node(cameranode:cast())
end

function Test_cameranode:test02_apply()
	cameranode = alledge_lua.cameranode.new()
	scenenode = alledge_lua.scenenode.new()
	scenenode:attach_node(cameranode:cast())
	scenenode:apply()
end

LuaUnit:run() -- run all tests
