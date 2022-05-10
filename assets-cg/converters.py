import fxconv
import json
import pathlib
import csv

def convert(input, output, params, target):
	if params["custom-type"] == "map":
		convert_map(input, output, params, target)
		return 0
	elif params["custom-type"] == "capacities":
		convert_capa(input, output, params, target)
		return 0
	elif params["custom-type"] == "monster":
		convert_monster(input, output, params, target)
		return 0
	elif params["custom-type"] == "player_moves":
		convert_player_moves(input, output, params, target)
		return 0
	elif params["custom-type"] == "items":
		convert_items(input, output, params, target)
		return 0
	elif params["custom-type"] == "table_type":
		convert_table_type(input, output, params, target)
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
	TILE_TELEPORTER = 5
	TILE_GRASS = 6
	TILE_ICE = 7

	DIALOG_LAYOUT = "dialog"
	TELEPORTER_LAYOUT = "teleporter"
	ZONE_LAYOUT = "zone"

	data = json.load(open(input, "r"))

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
	tileset_size = data_tileset.get("columns")
	tileset.close()

	tile_value = {}
	tile_type = {
		"air":TILE_AIR, 
		"solid":TILE_SOLID, 
		"door_in":TILE_DOOR_IN, 
		"door_out":TILE_DOOR_OUT, 
		"talkable":TILE_TALKABLE, 
		"bridge":TILE_BRIDGE, 
		"teleporter":TILE_TELEPORTER, 
		"grass":TILE_GRASS, 
		"ice":TILE_ICE
	}
	#create a dictionnary {tile id:type}
	for i in data_tileset["tiles"]:
		id = i["id"]+1
		type = i["type"]

		value = tile_type.get(type) if type in tile_type else TILE_AIR
		tile_value[id] = value

	#Extract from the json the width, height
	w, h = data["width"], data["height"]

	#nbTileLayer is the number of "true" layers (without ObjectsLayer)
	nbTilelayer = ["data" in i for i in data["layers"]].count(True)
	objectLayers = data["layers"][nbTilelayer:len(data["layers"])]

	nbDialog = 0
	nbTelep = 0
	nbZone = 0
	structMap = fxconv.Structure()
	dialogs = fxconv.Structure()
	teleporter = fxconv.Structure()
	zone = fxconv.Structure()

	for layer in objectLayers:
		if layer.get("name") == DIALOG_LAYOUT:
			nbDialog = len(layer["objects"])
			dialogs = parseDialog(layer)
		elif layer.get("name") == TELEPORTER_LAYOUT:
			nbTelep = len(layer["objects"])
			teleporter = parseTeleporter(layer)
		elif layer.get("name") == ZONE_LAYOUT:
			nbZone = len(layer["objects"])
			zone = parseZone(layer)
		else:
			print("UNKNOWN LAYER FOUND : " + layer.get("name"))

	structMap += fxconv.u32(w) + fxconv.u32(h) + fxconv.u32(nbTilelayer) + fxconv.u32(nbDialog) + fxconv.u32(nbTelep) + fxconv.u32(nbZone)
	structMap += fxconv.ref(f"img_{nameTilesetFree}")
	structMap += fxconv.u32(tileset_size)
	structMap += fxconv.ptr(dialogs)
	structMap += fxconv.ptr(teleporter)
	structMap += fxconv.ptr(zone)

	#generation of the collision map (take the maximum of the layer except for bridges)
	#bridges are always walkable
	info_map = bytes()

	maxValue = 0
	bridge = False
	for x in range(w*h):
		for i in range(nbTilelayer):
			value = tile_value.get(data["layers"][i]["data"][x])
			#attention : priorité aux valeurs hautes : un bloc d'herbe sera prioritaire sur un bloc solide
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

def parseDialog(layer):
	dialogs = fxconv.Structure()
	for i in layer["objects"]:
		dialogs += fxconv.u32(int(i["x"]/i["width"]))
		#Tiled seem to start at the bottom y of the object
		dialogs += fxconv.u32(int(i["y"]/i["width"])-1)

		try:
			for j in i["properties"]:
				if(j["value"] == ""): j["value"] = " "
				dialogs += fxconv.string(j["value"])
		except KeyError:
			dialogs += fxconv.string("default name")
			dialogs += fxconv.string("default text")
	return dialogs

def parseTeleporter(layer):
	teleporter = fxconv.Structure()
	for i in layer["objects"]:
		teleporter += fxconv.u32(int(i["x"]/i["width"]))
		#Tiled seem to start at the bottom y of the object
		teleporter += fxconv.u32(int(i["y"]/i["width"])-1)

		try:
			if len(i["properties"]) < 2:
				raise Exception("parseTeleporter() : Un téléporteur est mal configuré")
			if len(i["properties"]) == 2:
				print("parseTeleporter() : passage automatique idMap = -1 sur téléporteur x = " + str(i["properties"][0]["value"]) + ", y = " + str(i["properties"][1]["value"]))
				teleporter += fxconv.u32(-1)
			for j in i["properties"]:
				teleporter += fxconv.u32(j["value"])
		except KeyError	:
			raise Exception("parseTeleporter() : Un téléporteur est mal configuré")
	return teleporter

def parseZone(layer):
	zone = fxconv.Structure()
	for i in layer["objects"]:
		origin = (int(i['x']/16), int(i['y']/16))
		to = (int(origin[0]+i['width']/16)-1, int(origin[1]+i['height']/16)-1)

		zone += fxconv.u32(origin[0])
		zone += fxconv.u32(origin[1])
		zone += fxconv.u32(to[0])
		zone += fxconv.u32(to[1])

		event = bytes(i["properties"][0]["value"], "utf-8")
		event += bytes(128 - len(event))
		zone += event #event

		monsters = bytes()
		zone += fxconv.u32(int(i["properties"][1]["value"]) if i["properties"][1]["value"] != "" else -1) #level
		monster_list_raw = []
		if i["properties"][2]["value"] != "":  monster_list_raw = i["properties"][2]["value"].split(";") #monster list
		monster_list = []
		#x-y notation generate an array
		for i in monster_list_raw: 
			if "-" in i:
				a = i.split("-")
				monster_list.extend(list(range(int(a[0]),int(a[1])+1)))
			else:
				monster_list.append(int(i))
		zone += fxconv.u32(len(monster_list))

		for j in monster_list:
			monsters += fxconv.u16(int(j))

		zone += fxconv.ptr(monsters)
	return zone

def convert_capa(input, output, params, target):
	liste_file = list(pathlib.Path(input).parent.glob('*.json'))

	capacities = fxconv.Structure()
	capacities += fxconv.u32(len(liste_file))
	for f in liste_file:
		file = open(f,"r")
		data = json.load(file)
		move = fxconv.Structure()

		try:
			categorie = data["categorie"]
			id_categorie = ["STATUT", "PHYSICAL", "SPECIAL"]
			
			move += fxconv.string(data["name"])
			move += fxconv.string(data["type"])
			move += fxconv.u32(data["id"])
			move += fxconv.u32(id_categorie.index(categorie))
			move += fxconv.u32(data["pp"])
			move += fxconv.u32(data["pp"])

			if categorie=="PHYSICAL" or categorie=="SPECIAL":
				move += fxconv.u32(data["atk"])
				move += fxconv.u32(data["precision"])
				move += fxconv.u32(0) + fxconv.u32(0) + fxconv.u32(0)
			elif categorie=="STATUT":
				move += fxconv.u32(0) + fxconv.u32(100)
				move += fxconv.u32(data["boost_atk"])
				move += fxconv.u32(data["boost_def"])
				move += fxconv.u32(data["boost_hp"])
		except KeyError:
			raise Exception(f"convert_capa() : La capacité {data['name']} est mal configurée")

		capacities += fxconv.ptr(move)

	fxconv.elf(capacities, output, "_" + params["name"], **target)

def convert_monster(input, output, params, target):
	liste_file = list(pathlib.Path(input).parent.glob('*.json'))

	monsters = fxconv.Structure()
	monsters += fxconv.u32(len(liste_file))
	for f in liste_file:
		file = open(f,"r")
		data = json.load(file)
		stats = fxconv.Structure()

		if len(data["stats"]) != 6: raise Exception(f"convert_monster : Les statistiques de {data['name']} sont mauvaises")
		stats+=fxconv.string(data["type"])
		stats+=fxconv.u32(data["stats"]["atk"])
		stats+=fxconv.u32(data["stats"]["def"])
		stats+=fxconv.u32(data["stats"]["pv"])
		stats+=fxconv.u32(data["stats"]["spe_atk"])
		stats+=fxconv.u32(data["stats"]["spe_def"])
		stats+=fxconv.u32(1) # level, will be calculated later
		stats+=fxconv.u32(data["stats"]["xp"])
		stats+=fxconv.u32(data["stats"]["pv"])

		moves = bytes()
		for i in data["moves"]:
			moves+=fxconv.u16(i)

		monster = fxconv.Structure()
		monster += fxconv.string(data["name"])
		monster += fxconv.ptr(f"img_{data['sprite']}")
		monster += fxconv.u32(data["id"])
		monster += fxconv.u32(len(data["moves"]))
		monster += fxconv.ptr(stats)
		monster += fxconv.ptr(moves)

		monsters += fxconv.ptr(monster)

	fxconv.elf(monsters, output, "_" + params["name"], **target)

def convert_player_moves(input, output, params, target):
	levelupplayer = fxconv.Structure()
	data = open(input, 'r').readlines()
	levelupplayer += fxconv.u32(len(data))
	for i in data:
		levelup = fxconv.Structure()
		levelup += fxconv.u32(int(i.split(":")[0]))
		levelup += fxconv.u32(int(i.split(":")[1]))

		levelupplayer += fxconv.ptr(levelup)
	fxconv.elf(levelupplayer, output, "_" + params["name"], **target)

def convert_items(input, output, params, target):
	liste_file = list(pathlib.Path(input).parent.glob('*.json'))

	items = fxconv.Structure()
	items += fxconv.u32(len(liste_file))
	for f in liste_file:
		file = open(f,"r")
		data = json.load(file)
		item = fxconv.Structure()
		try:
			item += fxconv.string(data["name"])
			item += fxconv.u32(data["id"])
			item += fxconv.string(data["description"])
			item += fxconv.ptr(f"img_{data['sprite']}")
			item += fxconv.string(";".join(data["action"]))
			items += fxconv.ptr(item)
		except KeyError:
			raise Exception(f"convert_items() : L'item {data['name']} est mal configuré")

	fxconv.elf(items, output, "_" + params["name"], **target)

def convert_table_type(input, output, params, target):
	data = csv.DictReader(open(input,'r'))

	table_type = fxconv.Structure()
	for i in data:
		type = fxconv.Structure()
		type += fxconv.string(i["type"])
		type += fxconv.u32(list(i).index(i["type"]))

		taille = len(i)
		b,l,n = [],[],[]
		for j in i:
			id = list(i).index(j)
			if(i[j]=="2"): b.append(id)
			if(i[j]=="0,5"): l.append(id)
			if(i[j]=="0"): n.append(id)
		for a in range(len(b),taille):b.append(0)
		for a in range(len(l),taille):l.append(0)
		for a in range(len(n),taille):n.append(0)

		type += b"".join(fxconv.u32(value) for value in b)
		type += b"".join(fxconv.u32(value) for value in l)
		type += b"".join(fxconv.u32(value) for value in n)

		table_type += fxconv.ptr(type)

	fxconv.elf(table_type, output, "_" + params["name"], **target)