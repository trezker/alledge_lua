require('luaunit')
require('liballedge_lua')
USE_EXPECTED_ACTUAL_IN_ASSERT_EQUALS = true
assertEqualsDelta = function(expected, actual, delta)
	assert(math.abs(expected-actual)<delta)
end

Test_quadnode = {}

function Test_quadnode:test00_new()
	cameranode = alledge_lua.cameranode.new()
	assertEquals("cameranode", tostring(cameranode):sub(1, 10))
end

function Test_quadnode:test01_is_scenenode()
	quadnode = alledge_lua.quadnode.new()
	scenenode = alledge_lua.scenenode.new()
	scenenode:attach_node(quadnode:cast())
end

function Test_quadnode:test02_apply()
	quadnode = alledge_lua.quadnode.new()
	scenenode = alledge_lua.scenenode.new()
	scenenode:attach_node(quadnode)
	scenenode:apply()
end

function Test_quadnode:test03_set_texture()
	quadnode = alledge_lua.quadnode.new()
	texture = alledge_lua.bitmap.new()
	quadnode:set_texture(texture)
end

function Test_quadnode:test04_set_corners()
	quadnode = alledge_lua.quadnode.new()
	corners = {
		alledge_lua.vector3.new(1, 1, 0),
		alledge_lua.vector3.new(1, -1, 0),
		alledge_lua.vector3.new(-1, -1, 0),
		alledge_lua.vector3.new(-1, 1, 0),
	}
	quadnode:set_corners(corners)
end

LuaUnit:run() -- run all tests
