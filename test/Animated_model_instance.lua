require('luaunit')
require('liballedge_lua')
USE_EXPECTED_ACTUAL_IN_ASSERT_EQUALS = true
assertEqualsDelta = function(expected, actual, delta)
	assert(math.abs(expected-actual)<delta)
end

Test_animated_model_instance = {}

function Test_animated_model_instance:test00_new()
	cameranode = alledge_lua.cameranode.new()
	assertEquals("cameranode", tostring(cameranode):sub(1, 10))
end

function Test_animated_model_instance:test01_is_scenenode()
	animated_model_instance = alledge_lua.animated_model_instance.new()
	scenenode = alledge_lua.scenenode.new()
	scenenode:attach_node(animated_model_instance:cast())
end

function Test_animated_model_instance:test02_apply()
	animated_model_instance = alledge_lua.animated_model_instance.new()
	scenenode = alledge_lua.scenenode.new()
	scenenode:attach_node(animated_model_instance)
	scenenode:apply()
end

function Test_animated_model_instance:test03_set_texture()
--	animated_model_instance = alledge_lua.animated_model_instance.new()
--	texture = alledge_lua.bitmap.new()
--	animated_model_instance:set_texture(texture)
end
--[[
	{"play_animation", animated_model_instance_play_animation},
	{"pause_animation", animated_model_instance_pause_animation},
	{"animation_has_ended", animated_model_instance_animation_has_ended},
	{"set_model", animated_model_instance_set_model},
	{"add_model", animated_model_instance_add_model},
	{"remove_model", animated_model_instance_remove_model},
	{"attach_to_bone", animated_model_instance_attach_to_bone},
	{"detach_from_bone", animated_model_instance_detach_from_bone},
	{"update", animated_model_instance_update},
]]--
LuaUnit:run() -- run all tests
