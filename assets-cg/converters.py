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
	info_map = bytearray()

	#create a dictionnary between tile-type
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
	w = data["layers"][0]["width"]
	h = data["layers"][0]["height"]
	nblayer = len(data["layers"])

	o = fxconv.ObjectData()
	o += fxconv.u32(w) + fxconv.u32(h) + fxconv.u32(nblayer)
	o += fxconv.ref("img_tileset")

	#To preserve performance, max 2 layers
	if(nblayer <= 2):
		#generate the array of tiles from the layer
		for i in range(nblayer):
			tiles = data["layers"][i]["data"]

			byte_tiles = bytearray()
			for j in tiles:
				byte_tiles += bytearray(fxconv.u16(j))
			o += fxconv.ref(byte_tiles)

		#generation of the collision map (take the maximum of the layer except for bridges)
		#bridges is always walkable
		for x in range(w*h):
			value1 = tile_value.get(data["layers"][0]["data"][x])
			if(nblayer >= 2):
				value2 = tile_value.get(data["layers"][1]["data"][x])
				if value1 == None: value1 = TILE_AIR
				if value2 == None: value2 = TILE_AIR
				if value2 == TILE_BRIDGE: value1 = value2 = TILE_AIR
				info_map += bytearray(fxconv.u16(max(value1, value2)))
			else:
				if value1 == TILE_BRIDGE: value1 = TILE_AIR
				info_map += bytearray(fxconv.u16(value1))
		o += fxconv.ref(info_map)
	else:
		raise fxconv.FxconvError(f"There is too much layer ! {nblayer} found for a max of 2")

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