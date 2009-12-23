-- Title: bitmap example
-- Demonstrates usage of bitmap functions

require('liballua')
require('liballedge_lua')

allegro5.init()
allegro5.keyboard.install()
allegro5.mouse.install()
allegro5.bitmap.init_image_addon ()

allegro5.display.set_new_flags(allegro5.display.WINDOWED)
display = allegro5.display.create(640, 480)
event_queue = allegro5.event_queue.create()

event_queue:register_event_source(display:get_event_source())
keyboard = allegro5.keyboard.get_event_source()
event_queue:register_event_source(keyboard)
mouse = allegro5.mouse.get_event_source()
event_queue:register_event_source(mouse)


root = alledge_lua.scenenode.new()

camera = alledge_lua.cameranode.new()
camera:set_position(alledge_lua.vector3.new(0, 0, 10));
camera:set_rotation(alledge_lua.vector3.new(0, 0, 0));
root:attach_node(camera);

light = alledge_lua.lightnode.new()
light:set_ambient(.2, .2, .2, 1)
light:set_diffuse(.8, .8, .8, 1)
light:set_position(alledge_lua.vector3.new(10, 10, 10), false)
alledge_lua.scenenode.attach_node(camera, light)

transform = alledge_lua.transformnode.new()
alledge_lua.scenenode.attach_node(light, transform)

animated_model = alledge_lua.animated_model.new()
animated_model:load_model("data/Male.md5mesh")
animated_model:load_animation("data/Male_walk.md5anim", "walk")
animated_model:load_animation("data/Male_run.md5anim", "run")

animated_model_instance = alledge_lua.animated_model_instance.new()
animated_model_instance:set_model(animated_model)
animated_model_instance:play_animation("walk", true)

alledge_lua.scenenode.attach_node(transform, animated_model_instance)


fov = 45
near = 1
far = 1000
width = 640
height = 480

last_time = allegro5.current_time()

b = false

while not quit do
	current_time = allegro5.current_time()
	dt = current_time - last_time
	last_time = current_time

	event = event_queue:get_next_event()
	if event.type == allegro5.display.EVENT_CLOSE or event.type == allegro5.keyboard.EVENT_DOWN and event.keycode == allegro5.keyboard.KEY_ESCAPE then
		quit = true
	end

	if event.type == allegro5.keyboard.EVENT_UP then
		if event.keycode == allegro5.keyboard.KEY_W then
			pos = camera:get_position() + camera:get_front()
			camera:set_position(pos)
		end
		if event.keycode == allegro5.keyboard.KEY_S then
			pos = camera:get_position() - camera:get_front()
			camera:set_position(pos)
		end
		if event.keycode == allegro5.keyboard.KEY_A then
			pos = camera:get_position() - camera:get_right()
			camera:set_position(pos)
		end
		if event.keycode == allegro5.keyboard.KEY_D then
			pos = camera:get_position() + camera:get_right()
			camera:set_position(pos)
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
			rot = camera:get_rotation() + alledge_lua.vector3.new(0, -event.dx, 0)
			camera:set_rotation(rot)
		end
	end
	
	transform:set_rotation(transform:get_rotation() + alledge_lua.vector3.new(10, 10, 0)*dt)
	animated_model_instance:update(dt)

	alledge_lua.init_perspective_view(fov, width/height, near, far)
	alledge_lua.gl.enable(alledge_lua.gl.DEPTH_TEST)
	alledge_lua.gl.enable(alledge_lua.gl.LIGHTING);
	alledge_lua.gl.clear(alledge_lua.gl.DEPTH_BUFFER_BIT)

	root:apply()

	alledge_lua.gl.disable(alledge_lua.gl.DEPTH_TEST)
	alledge_lua.pop_view()

	allegro5.display.flip()
	allegro5.bitmap.clear_to_color (allegro5.color.map_rgba(0, 0, 0, 0))
	allegro5.rest(0.001)
end
