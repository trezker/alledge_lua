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

--light = alledge_lua.lightnode.new()
--transform = alledge_lua.transformnode.new()

texture = alledge_lua.bitmap.new()
texture:load("data/darwinian.png")
quads = {}

corners = {
	{
		alledge_lua.vector3.new(1, 1, -1),
		alledge_lua.vector3.new(-1, 1, -1),
		alledge_lua.vector3.new(-1, -1, -1),
		alledge_lua.vector3.new(1, -1, -1)
	},
	{
		alledge_lua.vector3.new(1, -1, 1),
		alledge_lua.vector3.new(-1, -1, 1),
		alledge_lua.vector3.new(-1, 1, 1),
		alledge_lua.vector3.new(1, 1, 1)
	},
	{
		alledge_lua.vector3.new(-1, 1, 1),
		alledge_lua.vector3.new(-1, -1, 1),
		alledge_lua.vector3.new(-1, -1, -1),
		alledge_lua.vector3.new(-1, 1, -1)
	},
	{
		alledge_lua.vector3.new(1, 1, -1),
		alledge_lua.vector3.new(1, -1, -1),
		alledge_lua.vector3.new(1, -1, 1),
		alledge_lua.vector3.new(1, 1, 1)
	},
	{
		alledge_lua.vector3.new(1, 1, 1),
		alledge_lua.vector3.new(-1, 1, 1),
		alledge_lua.vector3.new(-1, 1, -1),
		alledge_lua.vector3.new(1, 1, -1)
	},
	{
		alledge_lua.vector3.new(1, -1, -1);
		alledge_lua.vector3.new(-1, -1, -1);
		alledge_lua.vector3.new(-1, -1, 1);
		alledge_lua.vector3.new(1, -1, 1);
	}
}

for i = 1, 6 do
	quads[i] = alledge_lua.quadnode.new()
	quads[i]:set_corners(corners[i])
	quads[i]:set_texture(texture)
	alledge_lua.scenenode.attach_node(camera, quads[i]);
end

fov = 45
near = 1
far = 1000
width = 640
height = 480

last_time = allegro5.current_time()

b = false

while not quit do
	event = event_queue:get_next_event()
	if event.type == allegro5.display.EVENT_CLOSE or event.type == allegro5.keyboard.EVENT_DOWN and event.keycode == allegro5.keyboard.KEY_ESCAPE then
		quit = true
	end

	if event.type == allegro5.keyboard.EVENT_UP then
		if event.keycode == allegro5.keyboard.KEY_S then
			pos = camera:get_position() + camera:get_front()
			camera:set_position(pos)
		end
		if event.keycode == allegro5.keyboard.KEY_W then
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
			rot = camera:get_rotation() + alledge_lua.vector3.new(0, event.dx, 0)
			camera:set_rotation(rot)
		end
	end

	texture:draw(10, 10, 0)
	
	--Todo: some gl depth and stuff need to be done.
	alledge_lua.init_perspective_view(fov, width/height, near, far)
	root:apply()
	alledge_lua.pop_view()

	allegro5.display.flip()
	allegro5.bitmap.clear_to_color (allegro5.color.map_rgba(0, 0, 0, 0))
	allegro5.rest(0.001)
end
