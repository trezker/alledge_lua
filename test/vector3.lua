require('luaunit')
require('liballedge_lua')
USE_EXPECTED_ACTUAL_IN_ASSERT_EQUALS = true
assertEqualsDelta = function(expected, actual, delta)
	assert(math.abs(expected-actual)<delta)
end

Test_vector3 = {}

function Test_vector3:test00_new()
	vector0 = alledge_lua.vector3.new()
	vector1 = alledge_lua.vector3.new(1, 2, 3)
	vector2 = alledge_lua.vector3.new(vector1)
	assertEquals("vector3", tostring(vector0):sub(1, 7))
	assertEquals(vector0.x, 0)
	assertEquals(vector0.y, 0)
	assertEquals(vector0.z, 0)

	assertEquals(vector1.x, 1)
	assertEquals(vector1.y, 2)
	assertEquals(vector1.z, 3)

	assertEquals(vector2.x, 1)
	assertEquals(vector2.y, 2)
	assertEquals(vector2.z, 3)
end

function Test_vector3:test01_getset()
	vector0 = alledge_lua.vector3.new()
	vector0.x, vector0.y, vector0.z = 2, 3, 5
	assertEquals(vector0.x, 2)
	assertEquals(vector0.y, 3)
	assertEquals(vector0.z, 5)
end

function Test_vector3:test02_add()
	vector0 = alledge_lua.vector3.new(1, 2, 5)
	vector1 = alledge_lua.vector3.new(2, 4, 10)
	vector2 = vector0 + vector1
	assertEquals(vector2.x, 3)
	assertEquals(vector2.y, 6)
	assertEquals(vector2.z, 15)
end

function Test_vector3:test02_sub()
	vector0 = alledge_lua.vector3.new(1, 2, 5)
	vector1 = alledge_lua.vector3.new(2, 4, 10)
	vector2 = vector0 - vector1
	assertEquals(vector2.x, -1)
	assertEquals(vector2.y, -2)
	assertEquals(vector2.z, -5)
end

function Test_vector3:test02_unm()
	vector0 = alledge_lua.vector3.new(1, 2, 5)
	vector1 = -vector0
	assertEquals(vector1.x, -1)
	assertEquals(vector1.y, -2)
	assertEquals(vector1.z, -5)
end

function Test_vector3:test03_mul()
	vector0 = alledge_lua.vector3.new(1, 2, 5)
	vector1 = vector0 * 3
	assertEquals(vector1.x, 3)
	assertEquals(vector1.y, 6)
	assertEquals(vector1.z, 15)
end

function Test_vector3:test03_div()
	vector0 = alledge_lua.vector3.new(1, 2, 5)
	vector1 = vector0 / .2
	assertEquals(vector1.x, 5)
	assertEquals(vector1.y, 10)
	assertEquals(vector1.z, 25)
end

function Test_vector3:test03_equals()
	vector0 = alledge_lua.vector3.new(1, 2, 5)
	vector1 = alledge_lua.vector3.new(1, 2, 5)
	assertEquals(vector0, vector1)
end

function Test_vector3:test04_length()
	vector0 = alledge_lua.vector3.new(3, 4, 5)
	assertEquals(vector0:squaredlength(), 50)
	assertEqualsDelta(vector0:length(), math.sqrt(50), 0.000001)
end

function Test_vector3:test04_normalize()
	vector0 = alledge_lua.vector3.new(3, 4, 5)
	vector1 = vector0:getnormalized()
	vector0:normalize()
	assertEquals(vector0:length(), 1)
	assertEquals(vector1:length(), 1)
end

function Test_vector3:test05_products()
	vector0 = alledge_lua.vector3.new(3, 4, 5)
	vector1 = alledge_lua.vector3.new(9, 8, -7)
	cross = vector0:crossproduct(vector1)
	dot = vector0:dotproduct(vector1)
	assertEquals("vector3", tostring(cross):sub(1, 7))
	assertEquals(type(dot), "number")
end

function Test_vector3:test06_angles()
	vector0 = alledge_lua.vector3.new(3, 4, 5)
	vector1 = alledge_lua.vector3.new(9, 8, -7)
	degrees = vector0:getangledegrees(vector1)
	radians = vector0:getangleradians(vector1)
	assertEquals(type(degrees), "number")
	assertEquals(type(radians), "number")
end

LuaUnit:run() -- run all tests
