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
camera:set_position(alledge_lua.vector3.new(20, 5, 20));
camera:set_rotation(alledge_lua.vector3.new(0, 0, 0));
root:attach_node(camera);

light = alledge_lua.lightnode.new()
light:set_ambient(.2, .2, .2, 1)
light:set_diffuse(.8, .8, .8, 1)
light:set_position(alledge_lua.vector3.new(1, 1, 1), true)
alledge_lua.scenenode.attach_node(camera, light)

transform = alledge_lua.transformnode.new()
alledge_lua.scenenode.attach_node(light, transform)

line_start = alledge_lua.vector3.new(-1, -1, 1);
line_end = alledge_lua.vector3.new(1, -1, 1);

line_node = alledge_lua.linenode.new()
line_node:set_line(line_start, line_end)
line_node:set_color(1, 1, 1, 1)
alledge_lua.scenenode.attach_node(transform, line_node);

--Heightmap setup
texture = alledge_lua.bitmap.new()
texture:load("data/darwinian.png");
texture2 = alledge_lua.bitmap.new()
texture2:load("data/Colormap.png");
splat_texture = alledge_lua.bitmap.new()
splat_texture:load("data/splat_texture.png");

if not (texture and texture2 and splat_texture) then
	print("Texture missing")
end

heightmap = alledge_lua.heightmap.new()
heightmap:set_tilesize(1);
heightmap:resize(50, 30);
heightmap:set_texture(texture, 0);
heightmap:set_texture(texture2, 1);
heightmap:set_splat_texture(splat_texture);
alledge_lua.scenenode.attach_node(transform, heightmap);

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

		if event.keycode == allegro5.keyboard.KEY_R then
			pos = camera:get_position() + camera:get_up()
			camera:set_position(pos)
		end
		if event.keycode == allegro5.keyboard.KEY_F then
			pos = camera:get_position() - camera:get_up()
			camera:set_position(pos)
		end
	end

	if event.type == allegro5.mouse.EVENT_DOWN then
		if event.button == 1 then
			lmb = true
		end
		if event.button == 2 then
			rmb = true
		end
	end

	if event.type == allegro5.mouse.EVENT_UP then
		if event.button == 1 then
			lmb = false
		end
		if event.button == 2 then
			rmb = false
		end
	end

	if event.type == allegro5.mouse.EVENT_AXES then
		if lmb then
			rot = camera:get_rotation() + alledge_lua.vector3.new(-event.dy, -event.dx, 0)
			camera:set_rotation(rot)
		end
		mouse_x = event.x
		mouse_y = event.y
	end

	if rmb then
		alledge_lua.init_perspective_view(fov, width/height, near, far)
		oglpoint = camera:unproject(mouse_x, mouse_y)
		alledge_lua.pop_view()

--		std::cout<<"oglpoint x, y, z: "<<oglpoint.x<<", "<<oglpoint.y<<", "<<oglpoint.z<<std::endl;
		curve = {-1, -.7, 0, .3, 0}
		heightmap:apply_brush(oglpoint.x, oglpoint.z, 10, 1*dt, 5, curve);
	end
	
--	transform:set_rotation(transform:get_rotation() + alledge_lua.vector3.new(10, 10, 0)*dt)

	alledge_lua.init_perspective_view(fov, width/height, near, far)
	alledge_lua.gl.enable(alledge_lua.gl.DEPTH_TEST)
	alledge_lua.gl.enable(alledge_lua.gl.LIGHTING);
	alledge_lua.gl.clear(alledge_lua.gl.DEPTH_BUFFER_BIT)

	root:apply()

	alledge_lua.gl.disable(alledge_lua.gl.LIGHTING);
	alledge_lua.gl.disable(alledge_lua.gl.DEPTH_TEST)
	alledge_lua.pop_view()

	allegro5.display.flip()
	allegro5.bitmap.clear_to_color (allegro5.color.map_rgba(0, 0, 0, 0))
	allegro5.rest(0.001)
end
