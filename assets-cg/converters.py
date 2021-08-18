import fxconv
import json

def convert(input, output, params, target):
	if params["custom-type"] == "map":
		convert_map(input, output, params, target)
		return 0
	elif params["custom-type"] == "character":
		convert_character(input, output, params, target)
		return 0
	else:
		return 1

def convert_map(input, output, params, target):
	TILE_AIR = 0
	TILE_SOLID = 1
	TILE_DOOR = 2
	TILE_CHARACTER = 3

	TILE_BRIDGE = -1 #only for bridge detection to avoid solid behind

	with open(input, "r") as jsonData:
		data = json.load(jsonData)

	#find the tileset using the location of the input
	tileset = open(input.replace(input.split("/")[-1], "tileset.json"), "r")
	data_tileset = json.load(tileset)
	tileset.close()

	tile_value = {}

	#create a dictionnary between tile id-type
	for i in data_tileset["tiles"]:
		try:
			id = i["id"]+1
			type = i["type"]

			if type == "air":
				value = TILE_AIR
			elif type == "solid":
				value = TILE_SOLID
			elif type == "door":
				value = TILE_DOOR
			elif type == "character":
				value = TILE_CHARACTER
			elif type == "bridge":
				value = TILE_BRIDGE
			else:
				value = TILE_AIR

			tile_value[id] = value
		except KeyError:
			pass

	#Extract from the json the width, height and layers of the map
	w, h = data["width"], data["height"]
	nblayer = len(data["layers"])

	o = fxconv.ObjectData()
	o += fxconv.u32(w) + fxconv.u32(h) + fxconv.u32(nblayer)
	o += fxconv.ref("img_tileset")

	#generation of the collision map (take the maximum of the layer except for bridges)
	#bridges are always walkable
	info_map = bytes()

	maxValue = 0
	bridge = False
	for x in range(w*h):
		for i in range(nblayer):
			value = tile_value.get(data["layers"][i]["data"][x])
			if value == None: value = TILE_AIR
			if value > maxValue: maxValue = value
			if value == TILE_BRIDGE:
				maxValue = TILE_AIR
				bridge = True
			if bridge:
				if value != TILE_AIR:
					maxValue = value
		info_map += fxconv.u16(maxValue)
		maxValue = 0
		bridge = False
	o += fxconv.ref(info_map)

	#generate the array of tiles from the layer
	for layer in data["layers"]:
		layer_data = bytes()

		for tile in layer["data"]:
			layer_data += fxconv.u16(tile)
		
		o += fxconv.ref(layer_data)

	#generate !
	fxconv.elf(o, output, "_" + params["name"], **target)

def convert_character(input, output, params, target):
	with open(input,"r") as dialog:
		file = dialog.read().splitlines()

	o = fxconv.ObjectData()
	o += fxconv.u32((int)(file[0])) + fxconv.u32((int)(file[1]))
	o += fxconv.ref(bytes(file[2], 'utf-8') + bytes(1)) #bytes(1) is necessary to end a char
	o += fxconv.ref(bytes(file[3], 'utf-8') + bytes(1))
	
	fxconv.elf(o, output, "_" + params["name"], **target)