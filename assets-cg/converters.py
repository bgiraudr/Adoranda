import fxconv
import csv

def convert(input, output, params, target):
	if params["custom-type"] == "map":
		convert_map(input, output, params, target)
		return 0
	else:
		return 1

def convert_map(input, output, params, target):
	with open(input, "r") as csvData:
		tiles = []
		csvReader = csv.reader(csvData)
		for row in csvReader:
			tiles.append(row)

	w = max(len(i) for i in tiles)
	h = len(tiles)

	encoded_tiles = bytearray()
	for(y, i) in enumerate(tiles):
		for(x, j) in enumerate(i):
			encoded_tiles += bytearray(fxconv.u16((int)(j)+1))

	o = fxconv.ObjectData()
	o += fxconv.u32(w) + fxconv.u32(h)
	o += fxconv.ref("img_tileset")
	o += fxconv.ref(encoded_tiles)

	fxconv.elf(o, output, "_" + params["name"], **target)