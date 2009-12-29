require('luaunit')
require('liballedge_lua')
USE_EXPECTED_ACTUAL_IN_ASSERT_EQUALS = true
assertEqualsDelta = function(expected, actual, delta)
	assert(math.abs(expected-actual)<delta)
end

Test_animated_model_node = {}

function Test_animated_model_node:test00_new()
	cameranode = alledge_lua.cameranode.new()
	assertEquals("cameranode", tostring(cameranode):sub(1, 10))
end

function Test_animated_model_node:test01_is_scenenode()
	animated_model_node = alledge_lua.animated_model_node.new()
	scenenode = alledge_lua.scenenode.new()
	scenenode:attach_node(animated_model_node:cast())
end

function Test_animated_model_node:test02_apply()
	animated_model_node = alledge_lua.animated_model_node.new()
	scenenode = alledge_lua.scenenode.new()
	scenenode:attach_node(animated_model_node)
	scenenode:apply()
end

function Test_animated_model_node:test03_set_texture()
--	animated_model_node = alledge_lua.animated_model_node.new()
--	texture = alledge_lua.bitmap.new()
--	animated_model_node:set_texture(texture)
end
--[[
	{"play_animation", animated_model_node_play_animation},
	{"pause_animation", animated_model_node_pause_animation},
	{"animation_has_ended", animated_model_node_animation_has_ended},
	{"set_model", animated_model_node_set_model},
	{"add_model", animated_model_node_add_model},
	{"remove_model", animated_model_node_remove_model},
	{"attach_to_bone", animated_model_node_attach_to_bone},
	{"detach_from_bone", animated_model_node_detach_from_bone},
	{"update", animated_model_node_update},
]]--
LuaUnit:run() -- run all tests
