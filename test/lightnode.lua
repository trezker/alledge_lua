require('luaunit')
require('liballedge_lua')
USE_EXPECTED_ACTUAL_IN_ASSERT_EQUALS = false
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

function Test_lightnode:test04_components()
	lightnode = alledge_lua.lightnode.new()
	setambient = {1, 2, 3, 4}
	lightnode:set_ambient(setambient)
	getambient = lightnode:get_ambient()
	setdiffuse = {5, 6, 7, 8}
	lightnode:set_diffuse(setdiffuse)
	getdiffuse = lightnode:get_diffuse()
	setspecular = {9, 10, 11, 12}
	lightnode:set_specular(setspecular)
	getspecular = lightnode:get_specular()
	for i = 1, 4 do
		assertEquals(setambient[i], getambient[i])
		assertEquals(setdiffuse[i], getdiffuse[i])
		assertEquals(setspecular[i], getspecular[i])
	end
end

LuaUnit:run() -- run all tests
