import fxconv
import json

def convert(input, output, params, target):
	if params["custom-type"] == "map":
		convert_map(input, output, params, target)
		return 0
	else:
		return 1

def convert_map(input, output, params, target):
	TILE_BRIDGE = -1 #only for bridge detection to avoid solid behind
	TILE_AIR = 0
	TILE_SOLID = 1
	TILE_DOOR_IN = 2
	TILE_DOOR_OUT = 3
	TILE_TALKABLE = 4

	with open(input, "r") as jsonData:
		data = json.load(jsonData)

	#find the tileset in use. it's a relative path (like ../tileset.tsx)
	nameTileset = data["tilesets"][0]["source"].replace(".tsx","")
	#the name of the tileset without the .something
	nameTilesetFree = nameTileset.split("/")[-1]
	#count the number of "back" (cd ..) to locate the tileset on the computer
	nbRetour = nameTileset.count("..")+1
	#create the tileset absolute path 
	tilesetPath = "/".join(input.split("/")[:-nbRetour]) + "/" + nameTileset.split("/")[-1] + ".json"

	tileset = open(tilesetPath, "r")
	data_tileset = json.load(tileset)
	tileset.close()

	tile_value = {}
	#create a dictionnary {tile id:type}
	for i in data_tileset["tiles"]:
		id = i["id"]+1
		type = i["type"]

		if type == "air":
			value = TILE_AIR
		elif type == "solid":
			value = TILE_SOLID
		elif type == "door_in":
			value = TILE_DOOR_IN
		elif type == "door_out":
			value = TILE_DOOR_OUT
		elif type == "talkable":
			value = TILE_TALKABLE
		elif type == "bridge":
			value = TILE_BRIDGE
		else:
			value = TILE_AIR

		tile_value[id] = value

	#Extract from the json the width, height
	w, h = data["width"], data["height"]
	indexObjectlayer = None

	#nbTileLayer is the number of "true" layers (without ObjectsLayer)
	nbTilelayer = len(data["layers"])
	for i in range(nbTilelayer):
		try:
			data["layers"][i]["data"]
			nbTilelayer = i+1
		except KeyError:
			indexObjectlayer = i
			break

	if indexObjectlayer != None: 
		nbDialog = len(data["layers"][indexObjectlayer]["objects"])
	else:
		nbDialog = 0

	structMap = fxconv.Structure()
	structMap += fxconv.u32(w) + fxconv.u32(h) + fxconv.u32(nbTilelayer) + fxconv.u32(nbDialog)
	structMap += fxconv.ref(f"img_{nameTilesetFree}")

	dialogs = fxconv.Structure()
	# print(indexObjectlayer)
	if indexObjectlayer != None:
		#generate all of the dialog
		for i in data["layers"][indexObjectlayer]["objects"]:
			dialogs += fxconv.u32(int(i["x"]/i["width"]))
			#Tiled seem to start at the bottom y of the object
			#So if tile is 16 px wide, you would start at line y = 1
			dialogs += fxconv.u32(int(i["y"]/i["width"])-1)

			try:
				for j in i["properties"]:
					if(j["value"] == ""): j["value"] = " "
					dialogs += fxconv.string(j["value"])
			except KeyError:
				dialogs += fxconv.string("default name")
				dialogs += fxconv.string("default text")

	structMap += fxconv.ptr(dialogs)

	#generation of the collision map (take the maximum of the layer except for bridges)
	#bridges are always walkable
	info_map = bytes()

	maxValue = 0
	bridge = False
	for x in range(w*h):
		for i in range(nbTilelayer):
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
	structMap += fxconv.ptr(info_map)

	#generate the array of tiles from the layer
	for i in range(nbTilelayer):
		layer_data = bytes()
		layer = data["layers"][i]
		for tile in layer["data"]:
			layer_data += fxconv.u16(tile)

		structMap += fxconv.ptr(layer_data)

	#generate !
	fxconv.elf(structMap, output, "_" + params["name"], **target)