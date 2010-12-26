require('luaunit')
require('liballedge_lua')
USE_EXPECTED_ACTUAL_IN_ASSERT_EQUALS = true
assertEqualsDelta = function(expected, actual, delta)
	assert(math.abs(expected-actual)<delta)
end

Test_animated_model = {}

function Test_animated_model:test00_new()
	animated_model = alledge_lua.animated_model.new()
	assertEquals("animated_model", tostring(animated_model):sub(1, 14))
end

function Test_animated_model:test01_load()
--	animated_model = alledge_lua.animated_model.new()
--	b = animated_model:load("image.png")
--	assertEquals(false, b)
end

LuaUnit:run() -- run all tests
