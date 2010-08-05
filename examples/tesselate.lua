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
light:set_position(alledge_lua.vector3.new(0, 10, 0), false)
alledge_lua.scenenode.attach_node(camera, light)

transform = alledge_lua.transformnode.new()
alledge_lua.scenenode.attach_node(light, transform)

texture = alledge_lua.bitmap.new()
texture:load("data/orient.tga")
if not texture then print("orient texture fail") end


Lodtri = { }

function Lodtri:new ()
	o = {}
	setmetatable(o, self)
	self.__index = self
	o:init()
	return o
end

function Lodtri:init()
	self.corners = {}
	self.edges = {}
end

function Lodtri:shares_point(p)
	for l = 1, 3 do
		if self.corners[l] == p then
			return true
		end
	end
	return false
end

function Lodtri:iSharesEdge(t)
	for i = 1, 3 do
		if self.edges[i] == t then
			return i
		end
	end
	return -1
end

function Lodtri:split()
	if self.child then
		return
	end
	if not (self.edges[1] and self.edges[2] and self.edges[3]) then
		self.parent.edges[1]:split()
		self.parent.edges[2]:split()
		self.parent.edges[3]:split()
	end
	np = {}
	np[1] = (self.corners[1] + self.corners[2]) / 2
	np[2] = (self.corners[2] + self.corners[3]) / 2
	np[3] = (self.corners[1] + self.corners[3]) / 2
	for i = 1, 3 do
		if self.edges[i].child then
			np[i]:normalize()
			it0 = self.edges[i]:iSharesEdge(self)
			it1 = it0 + 1 if it1 > 3 then it1 = 1 end
			it3 = it1 + 1 if it3 > 3 then it3 = 1 end
			self.edges[i].child[it0].corners[it1] = np[i]
			self.edges[i].child[it1].corners[it0] = np[i]
			self.edges[i].child[4].corners[it3] = np[i]
		end
	end
	self.child = {}
	self.child[1] = Lodtri:new ()
	self.child[1].corners = { self.corners[1], np[1], np[3] }
	self.child[2] = Lodtri:new ()
	self.child[2].corners = { np[1], self.corners[2], np[2] }
	self.child[3] = Lodtri:new ()
	self.child[3].corners = { np[3], np[2], self.corners[3] }
	self.child[4] = Lodtri:new ()
	self.child[4].corners = { np[2], np[3], np[1] }
	for k, v in pairs(self.child) do
		v.parent = self
	end
	self.child[1].edges[2]=self.child[4]
	self.child[2].edges[3]=self.child[4]
	self.child[3].edges[1]=self.child[4]
	self.child[4].edges = { self.child[3], self.child[1], self.child[2] }

	for i = 1, 3 do
		if self.edges[i] then --non-closed surface trap
			if self.edges[i].child then
				--triangle has children we can point our children too.
				j = i + 1 if j > 3 then j = 1 end
				sharedEdge = self.edges[i]:iSharesEdge(self)
				if sharedEdge ~= -1 then
					k = sharedEdge + 1 if k > 3 then k = 1 end
					self.child[i].edges[i] = self.edges[i].child[k]
					self.child[j].edges[i] = self.edges[i].child[sharedEdge]
					self.edges[i].child[sharedEdge].edges[sharedEdge] = self.child[j]
					self.edges[i].child[k].edges[sharedEdge] = self.child[i]
				end
			end
		end
	end
end

function Lodtri:to_mesh()
	if self.child then
		for k, v in pairs(self.child) do
			v:to_mesh()
		end
	else
		corners[c] = self.corners[1]
		corners[c+1] = self.corners[2]
		corners[c+2] = self.corners[3]
		faces[f] = {c, c+1, c+2}
		c = c + 3
		f = f + 1
	end
end

corners = {
	alledge_lua.vector3.new(1, 0, 0),
	alledge_lua.vector3.new(-1, 0, 0),
	alledge_lua.vector3.new(0, 1, 0),
	alledge_lua.vector3.new(0, -1, 0),
	alledge_lua.vector3.new(0, 0, 1),
	alledge_lua.vector3.new(0, 0, -1),
}

faces = {
	{1, 6, 4},
	{1, 4, 5},
	{1, 3, 6},
	{1, 5, 3},
	{2, 6, 3},
	{2, 4, 6},
	{2, 5, 4},
	{2, 3, 5}
}

lodtris = {}
for k, v in pairs(faces) do
	lodtris[k] = Lodtri:new ()
	lodtris[k].corners = {
		corners[v[1]], corners[v[3]], corners[v[2]], 
	}
end

for k, v in pairs(lodtris) do
	for i = 1, 3 do
		j = i + 1 if j > 3 then j = 1 end
		for k2, v2 in pairs(lodtris) do
			if k~=k2 then
				if v2:shares_point(v.corners[i]) and v2:shares_point(v.corners[j]) then
					v.edges[i] = v2
					break
				end
			end
		end
	end
end

--[[
for k, v in pairs(lodtris) do
	v:split()
	for i = 1, 4 do
		v.child[i]:split()
		for j = 1, 4 do
			v.child[i].child[j]:split()
		end
	end
end
--]]
selected = lodtris[1]
while selected.child do
	selected = selected.child[1]
end
selected.selected = true

c = 1
f = 1
for k, v in pairs(lodtris) do
	v:to_mesh()
end

static_model = alledge_lua.static_model.new()
static_model:set_model_data(corners, faces)
static_model:set_color(1, 1, 1, 1)
static_model:set_texture(texture)
--static_model:show_normals(true)
static_model_node = alledge_lua.static_model_node.new()
static_model_node:set_model(static_model)
alledge_lua.scenenode.attach_node(transform, static_model_node)

selection_model = alledge_lua.static_model.new()
selection_model:set_model_data(selected.corners, {1, 2, 3})
selection_model:set_color(0, 1, 0, 0.5)

selection_transform = alledge_lua.transformnode.new()
alledge_lua.scenenode.attach_node(transform, selection_transform)
selection_transform:set_scale(alledge_lua.vector3.new(1.001, 1.001, 1.001))

selection_model_node = alledge_lua.static_model_node.new()
selection_model_node:set_model(selection_model)
alledge_lua.scenenode.attach_node(selection_transform, selection_model_node)


fov = 45
near = 1
far = 1000
width = 640
height = 480

last_time = allegro5.current_time()

mouse_button = {}
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

		if event.keycode == allegro5.keyboard.KEY_PGDN then
			print ("Woop")
			if selected then
				selected:split()

				c = 1
				f = 1
				for k, v in pairs(lodtris) do
					v:to_mesh()
				end
				static_model:set_model_data(corners, faces)

				selected = selected.child[1]
				selection_model:set_model_data(selected.corners, {1, 2, 3})
			end
		end
	end

	if event.type == allegro5.mouse.EVENT_DOWN then
		mouse_button[event.button] = true
		b = true
	end

	if event.type == allegro5.mouse.EVENT_UP then
		mouse_button[event.button] = false
		b = false
		if event.button == 2 then
			alledge_lua.init_perspective_view(fov, width/height, near, far)
			oglpoint = camera:unproject(event.x, event.y)
			alledge_lua.pop_view()

			matrix = transform:get_matrix()
			print( matrix )
			mindiff = 100
			sel = nil
			for k, v in pairs(lodtris) do
				center = (v.corners[1] + v.corners[2] + v.corners[3]) / 3
				center:transform_by_matrix4(matrix)
				diff = (center - oglpoint):length()
				if diff<mindiff then
					mindiff = diff
					sel = k
				end
			end
			if sel then
				print( "Select ")
				print( sel )
				selected = lodtris[sel]
				while selected.child do
					mindiff = 100
					for k, v in pairs(selected.child) do
						center = (v.corners[1] + v.corners[2] + v.corners[3]) / 3
						center:transform_by_matrix4(matrix)
						diff = (center - oglpoint):length()
						if diff<mindiff then
							mindiff = diff
							sel = k
						end
					end
					print( sel )
					selected = selected.child[sel]
				end
				selection_model:set_model_data(selected.corners, {1, 2, 3})
			else
				print("unselect")
			end
		end
	end

	if event.type == allegro5.mouse.EVENT_AXES then
		if mouse_button[1] then
			rot = transform:get_rotation() + alledge_lua.vector3.new(event.dy, event.dx, 0)
			transform:set_rotation(rot)
		end
	end
	
--	transform:set_rotation(transform:get_rotation() + alledge_lua.vector3.new(10, 10, 0)*dt)

	alledge_lua.init_perspective_view(fov, width/height, near, far)
	alledge_lua.gl.enable(alledge_lua.gl.DEPTH_TEST)
	alledge_lua.gl.enable(alledge_lua.gl.LIGHTING);
	alledge_lua.gl.clear(alledge_lua.gl.DEPTH_BUFFER_BIT)

	root:apply()

	alledge_lua.gl.disable(alledge_lua.gl.DEPTH_TEST)
	alledge_lua.pop_view()

	texture:draw(10, 10, 0)

	allegro5.display.flip()
	allegro5.bitmap.clear_to_color (allegro5.color.map_rgba(0, 0, 0, 0))
	allegro5.rest(0.001)
end
