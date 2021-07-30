import fxconv
import json

def convert(input, output, params, target):
	if params["custom-type"] == "map":
		convert_map(input, output, params, target)
		return 0
	else:
		return 1

def convert_map(input, output, params, target):
	"""with open(input, "r") as csvData:
		tiles = []
		csvReader = csv.reader(csvData)
		for row in csvReader:
			tiles.append(row)

	w = max(len(i) for i in tiles)
	h = len(tiles)

	encoded_tiles = bytearray()
	for(y, i) in enumerate(tiles):
		for(x, j) in enumerate(i):
			encoded_tiles += bytearray(fxconv.u16((int)(j)+1))"""

	with open(input, "r") as jsonData:
		data = json.load(jsonData)

		w = data["layers"][0]["width"]
		h = data["layers"][0]["height"]

		o = fxconv.ObjectData()
		o += fxconv.u32(w) + fxconv.u32(h)
		o += fxconv.ref("img_tileset")

		for i in range(len(data["layers"])):
			tiles = data["layers"][i]["data"]

			byte_tiles = bytearray()
			for j in tiles:
				byte_tiles += bytearray(fxconv.u16(j))
			o += fxconv.ref(byte_tiles)

	fxconv.elf(o, output, "_" + params["name"], **target)