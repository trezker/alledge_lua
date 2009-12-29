-- Title: bitmap example
-- Demonstrates usage of bitmap functions

require('liballua')
require('liballedge_lua')

allegro5.init()
allegro5.keyboard.install()
allegro5.mouse.install()
allegro5.bitmap.init_image_addon ()
allegro5.font.init_addon ()
allegro5.font.init_ttf_addon ()

allegro5.display.set_new_flags(allegro5.display.WINDOWED)
display = allegro5.display.create(640, 480)
event_queue = allegro5.event_queue.create()

event_queue:register_event_source(display:get_event_source())
keyboard = allegro5.keyboard.get_event_source()
event_queue:register_event_source(keyboard)
mouse = allegro5.mouse.get_event_source()
event_queue:register_event_source(mouse)

font = allegro5.font.load_ttf("data/DejaVuSans.ttf", 23, 0)



root = alledge_lua.scenenode.new()

camera = alledge_lua.cameranode.new()
camera:set_position(alledge_lua.vector3.new(0, 50, 50));
camera:set_rotation(alledge_lua.vector3.new(-45, 0, 0));
root:attach_node(camera);

light = alledge_lua.lightnode.new()
light:set_ambient(.2, .2, .2, 1)
light:set_diffuse(.8, .8, .8, 1)
light:set_position(alledge_lua.vector3.new(100, 100, 100), false)
alledge_lua.scenenode.attach_node(camera, light)

transform = alledge_lua.transformnode.new()
alledge_lua.scenenode.attach_node(light, transform)

animated_model = alledge_lua.animated_model.new()
animated_model:load_model("data/Male.md5mesh")
animated_model:load_animation("data/Male_walk.md5anim", "walk")
animated_model:load_animation("data/Male_run.md5anim", "run")
animated_model:load_animation("data/Male_breathe.md5anim", "breathe")

	print("So far")	
breathe_instance = alledge_lua.animated_model_instance.new()
breathe_instance:set_model(animated_model)
breathe_instance:play_animation("breathe", true)

walk_instance = alledge_lua.animated_model_instance.new()
walk_instance:set_model(animated_model)
walk_instance:play_animation("walk", true)

animated_model_node = alledge_lua.animated_model_node.new()
animated_model_node:set_model(breathe_instance)
alledge_lua.scenenode.attach_node(transform, animated_model_node)

men = {}

i = 0
for z = 1, 25 do
	for x = 1, 40 do
		i = i+1
		men[i] = {}
		men[i].transform = alledge_lua.transformnode.new()
		men[i].transform:set_rotation(alledge_lua.vector3.new(0, 90, 0))
		men[i].transform:set_position(alledge_lua.vector3.new((-x)*10, 0, (z-3)*10))
		alledge_lua.scenenode.attach_node(light, men[i].transform)
		men[i].animated_model_node = alledge_lua.animated_model_node.new()
		men[i].animated_model_node:set_model(walk_instance)
		alledge_lua.scenenode.attach_node(men[i].transform, men[i].animated_model_node)
	end
end


fov = 45
near = 1
far = 1000
width = 640
height = 480

last_time = allegro5.current_time()
fps = 0
fps_counter = 0
last_fps = last_time
b = false

update = function(dt)
	if move_forward then
		pos = camera:get_position() + camera:get_front()*10 *dt
		camera:set_position(pos)
	end
	if move_backward then
		pos = camera:get_position() - camera:get_front()*10 *dt
		camera:set_position(pos)
	end
	if move_left then
		pos = camera:get_position() - camera:get_right()*10 *dt
		camera:set_position(pos)
	end
	if move_right then
		pos = camera:get_position() + camera:get_right()*10 *dt
		camera:set_position(pos)
	end
	if move_up then
		pos = camera:get_position() + camera:get_up()*10 *dt
		camera:set_position(pos)
	end
	if move_down then
		pos = camera:get_position() - camera:get_up()*10 *dt
		camera:set_position(pos)
	end
end

while not quit do
	current_time = allegro5.current_time()
	dt = current_time - last_time
	last_time = current_time

	if current_time - last_fps > 1 then
		fps = fps_counter
		fps_counter = 0
		last_fps = current_time
	end

	fps_counter = fps_counter + 1

	event = event_queue:get_next_event()
	while event.type do
		if event.type == allegro5.display.EVENT_CLOSE or event.type == allegro5.keyboard.EVENT_DOWN and event.keycode == allegro5.keyboard.KEY_ESCAPE then
			quit = true
		end

		if event.type == allegro5.keyboard.EVENT_UP then
			if event.keycode == allegro5.keyboard.KEY_W then
				move_forward = false
			end
			if event.keycode == allegro5.keyboard.KEY_S then
				move_backward = false
			end
			if event.keycode == allegro5.keyboard.KEY_A then
				move_left = false
			end
			if event.keycode == allegro5.keyboard.KEY_D then
				move_right = false
			end
			if event.keycode == allegro5.keyboard.KEY_R then
				move_up = false
			end
			if event.keycode == allegro5.keyboard.KEY_F then
				move_down = false
			end
		end
		if event.type == allegro5.keyboard.EVENT_DOWN then
			if event.keycode == allegro5.keyboard.KEY_W then
				move_forward = true
			end
			if event.keycode == allegro5.keyboard.KEY_S then
				move_backward = true
			end
			if event.keycode == allegro5.keyboard.KEY_A then
				move_left = true
			end
			if event.keycode == allegro5.keyboard.KEY_D then
				move_right = true
			end
			if event.keycode == allegro5.keyboard.KEY_R then
				move_up = true
			end
			if event.keycode == allegro5.keyboard.KEY_F then
				move_down = true
			end
		end

		if event.type == allegro5.mouse.EVENT_DOWN then
			b = true
		end

		if event.type == allegro5.mouse.EVENT_UP then
			b = false
		end

		if event.type == allegro5.mouse.EVENT_AXES then
			if b then
				rot = camera:get_rotation() + alledge_lua.vector3.new(-event.dy, -event.dx, 0)
				camera:set_rotation(rot)
			end
		end
	
		event = event_queue:get_next_event()
	end

	update(dt)
	
--	transform:set_rotation(transform:get_rotation() + alledge_lua.vector3.new(10, 10, 0)*dt)
--	animated_model_node:update(dt)
	walk_instance:update(dt)
	breathe_instance:update(dt)

	v = men[1]
	for k, v in pairs(men) do
		new_position = v.transform:get_position() + alledge_lua.vector3.new(5, 0, 0)*dt
		if new_position.x > 200 then
			new_position.x = -200
		end
		v.transform:set_position(new_position);
--		v.animated_model_node:update(dt)
	end

	alledge_lua.init_perspective_view(fov, width/height, near, far)
	alledge_lua.gl.enable(alledge_lua.gl.DEPTH_TEST)
	alledge_lua.gl.enable(alledge_lua.gl.LIGHTING);
	alledge_lua.gl.clear(alledge_lua.gl.DEPTH_BUFFER_BIT)

	root:apply()

	alledge_lua.gl.disable(alledge_lua.gl.LIGHTING);
	alledge_lua.gl.disable(alledge_lua.gl.DEPTH_TEST)
	alledge_lua.pop_view()

	allegro5.primitives.draw_filled_rectangle(5, 5, 120, 40, allegro5.color.map_rgb(255, 0, 0))
	font:draw_text (10, 10, 0, "FPS: " .. fps)


	allegro5.display.flip()
	allegro5.bitmap.clear_to_color (allegro5.color.map_rgba(0, 0, 0, 0))
	allegro5.rest(0.001)
end
