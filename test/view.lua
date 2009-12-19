require('luaunit')
require('liballedge_lua')
USE_EXPECTED_ACTUAL_IN_ASSERT_EQUALS = true
assertEqualsDelta = function(expected, actual, delta)
	assert(math.abs(expected-actual)<delta)
end

Test_view = {}

function Test_view:test00_view()
	alledge_lua.init_perspective_view(90, 800/600, 1, 1000)
	alledge_lua.pop_view()
end

LuaUnit:run() -- run all tests
