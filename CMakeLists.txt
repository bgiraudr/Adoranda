# Configure with [fxsdk build-fx] or [fxsdk build-cg], which provide the
# toolchain file and module path of the fxSDK

cmake_minimum_required(VERSION 3.15)
project(MyAddin)

include(GenerateG1A)
include(GenerateG3A)
include(Fxconv)
include_directories(include)
find_package(Gint 2.1 REQUIRED)
fxconv_declare_converters(assets-cg/converters.py)

add_custom_command(
	OUTPUT "${CMAKE_CURRENT_LIST_DIR}/assets-cg/maps/testCarte.json"
	       "${CMAKE_CURRENT_LIST_DIR}/assets-cg/maps/inside/interior_1.json"
	COMMENT "Convert tmx map to json"
	COMMAND tiled --export-tileset json tileset.tsx tileset.json
	COMMAND tiled --export-map json testCarte.tmx testCarte.json
	COMMAND tiled --export-map json inside/interior_1.tmx inside/interior_1.json
	WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/assets-cg/maps/
	DEPENDS assets-cg/maps/testCarte.tmx assets-cg/tileset.png assets-cg/maps/tileset.tsx assets-cg/converters.py assets-cg/maps/inside/interior_1.tmx)

set(SOURCES
	src/main.c
	src/character.c
	src/vec2.c
	src/game.c
	src/engine.c
	src/camera.c
	src/map.c
	src/player.c
	src/util.c
	src/animation.c
)

set(ASSETS_cg
	assets-cg/tileset.png
	assets-cg/maps/testCarte.json
	assets-cg/maps/inside/interior_1.json
	assets-cg/spritesheet.png
	assets-cg/characters/Tituya.char
	assets-cg/characters/Lephenixnoir.char
	assets-cg/characters/KikooDX.char
	assets-cg/characters/Massena.char
	assets-cg/characters/Tituya2.char
	assets-cg/characters/PancarteVille.char
)

fxconv_declare_assets(${ASSETS} ${ASSETS_fx} ${ASSETS_cg} WITH_METADATA)

add_executable(myaddin ${SOURCES} ${ASSETS} ${ASSETS_${FXSDK_PLATFORM}})
target_compile_options(myaddin PRIVATE -Wall -Wextra -Os)
target_link_libraries(myaddin Gint::Gint)
target_link_options(myaddin PRIVATE -Wl,-Map=map)

if("${FXSDK_PLATFORM_LONG}" STREQUAL fxCG50)
	generate_g3a(TARGET myaddin OUTPUT "MyAddin.g3a"
		NAME "MyAddin" ICONS assets-cg/icon-uns.png assets-cg/icon-sel.png)
endif()
