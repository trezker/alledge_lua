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
	scenenode:attach_node(cameranode)
	scenenode:apply()
end

function Test_cameranode:test03_look_at()
	cameranode = alledge_lua.cameranode.new()
	cameranode:look_at(alledge_lua.vector3.new())
end

function Test_cameranode:test04_vectors()
	cameranode = alledge_lua.cameranode.new()
	up = cameranode:get_up()
	front = cameranode:get_front()
	right = cameranode:get_right()
	assertEquals("vector3", tostring(up):sub(1, 7))
	assertEquals("vector3", tostring(front):sub(1, 7))
	assertEquals("vector3", tostring(right):sub(1, 7))
end

function Test_cameranode:test04_position_rotation()
	cameranode = alledge_lua.cameranode.new()
	setpos = alledge_lua.vector3.new(1, 2, 3)
	setrot = alledge_lua.vector3.new(4, 5, 6)
	cameranode:set_position(setpos)
	cameranode:set_rotation(setrot)
	getpos = cameranode:get_position()
	getrot = cameranode:get_rotation()
	assertEquals(setpos, getpos)
	assertEquals(setrot, getrot)
end

function Test_cameranode:test05_set_rotate_around_world_origo()
	cameranode = alledge_lua.cameranode.new()
	cameranode:set_rotate_around_world_origo(true)
end

LuaUnit:run() -- run all tests
