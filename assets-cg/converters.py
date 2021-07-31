import fxconv
import json

def convert(input, output, params, target):
	if params["custom-type"] == "map":
		convert_map(input, output, params, target)
		return 0
	else:
		return 1

def convert_map(input, output, params, target):
	TILE_AIR = 0
	TILE_SOLID = 1
	TILE_DOOR = 2

	with open(input, "r") as jsonData:
		data = json.load(jsonData)

	tileset = open(input.replace(input.split("/")[-1], "tileset.json"), "r")
	data_tileset = json.load(tileset)
	tileset.close()

	tile_value = {}
	info_map = bytearray()

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
			else:
				value = TILE_AIR

			tile_value[id] = value
		except KeyError:
			pass

	w = data["layers"][0]["width"]
	h = data["layers"][0]["height"]
	nblayer = len(data["layers"])

	o = fxconv.ObjectData()
	o += fxconv.u32(w) + fxconv.u32(h) + fxconv.u32(nblayer)
	o += fxconv.ref("img_tileset")

	if(nblayer <= 2):
		for i in range(nblayer):
			tiles = data["layers"][i]["data"]

			byte_tiles = bytearray()
			for j in tiles:
				if i == nblayer-1: 
					value = tile_value.get(j)
					if value == None: value = TILE_AIR
					info_map += bytearray(fxconv.u16(value))
				byte_tiles += bytearray(fxconv.u16(j))
			o += fxconv.ref(byte_tiles)
		o += fxconv.ref(info_map)
	else:
		raise fxconv.FxconvError(f"There is too much layer ! {nblayer} found for a max of 2")

	fxconv.elf(o, output, "_" + params["name"], **target)