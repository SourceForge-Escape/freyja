# libfreyja
set(Name freyja)
set(Major 0)
set(Minor 9)
set(Micro 5)
set(Revision 14)
set(Email mongooseichiban@gmail.com)

#enable_testing()

add_definitions(
	-DVERSION=\"\\\"lib${Name}-${Major}.${Minor}.${Micro}.${Revision}\\\"\" 
	-DMAJOR_VERSION=${Major}
	-DMINOR_VERSION=${Minor}
	-DMICRO_VERSION=${Micro}
	-DREVISION=${Revision}
	-DBUILD_ID=20070415
	-DEMAIL_ADDRESS=\"\\\"$(Email)\\\"\" 
	-DBUILD_NAME=\"\\\"$(NAME)-$(VERSION)-$(BUILD_ID)\\\"\"
)

# Build options
add_definitions(
	-DEXPERIMENTAL
	-DFREYJAIMAGE_PLUGINS
	-DFREYJA_PLUGINS	
)

# Debug
add_definitions( -DDEBUG -Wall -g )

# RELEASE
#add_definitions( -DRELEASE -DENABLE_FREYJA_ASSERTS -Wall -O2 -finline-functions -fomit-frame-pointer -funroll-loops )

# TinyXML 
add_definitions( -DTINYXML_FOUND )
include_directories( ../tinyxml )
link_directories( ../tinyxml )
link_libraries(	-ltinyxml )

# libhel
add_definitions( -DHEL_FOUND -DHAVE_LIBHEL -DHAVE_HEL )
include_directories( ../libhel )
link_directories( ../libhel )
link_libraries(	-lhel-0.2.2 )

# Lua
FIND_LIBRARY(LUA_FOUND lua5.1)
if (LUA_FOUND)
	add_definitions( -DLUA_FOUND )
	link_libraries( -llua5.1 )
	include_directories( ../lua/lua  )
	message( STATUS "Lua 5.1 library found." )
endif (LUA_FOUND)

# Python
FIND_LIBRARY(PYTHON_FOUND python2.4)
if (PYTHON_FOUND)
	add_definitions( -DPYTHON_FOUND -DUSING_PYTHON )
	link_libraries( -lpython2.4 )
	message( STATUS "Python library found." )
endif (PYTHON_FOUND)


include_directories( ../mstl .  )

set(Objs
	freyja/freyja
	freyja/Bone
	freyja/BoneABI
	freyja/Camera
	freyja/CameraABI
	freyja/FreyjaFSM
	freyja/FreyjaImage
	freyja/FreyjaPakReader
	freyja/FreyjaTexture
	freyja/KeyFrame
	freyja/LegacyABI
	freyja/Light
	freyja/LightABI
	freyja/Material
	freyja/MaterialABI
	freyja/Mesh
	freyja/MeshABI
	freyja/Model
	freyja/ModelABI
	freyja/PakABI
	freyja/PerlinNoise
	freyja/PluginABI
	freyja/Plugin
	freyja/Printer
	freyja/PythonABI
	freyja/LuaABI
	freyja/QueryABI
	freyja/RigidBody
	freyja/Skeleton
	freyja/Track
	freyja/Vertex
)


add_library( ${Name}-${Major}.${Minor}.${Micro} SHARED ${Objs} )

# Packaging
#add_custom_command(TARGET libhel.deb COMMAND dpkg-buildpackage -rfakeroot)


# Unit tests

# Generate a new debian/changelog.
FILE( WRITE debian/changelog "libfreyja (${Major}.${Minor}.${Micro}.${Revision}) feisty; urgency=low\n\n" )
FILE( APPEND debian/changelog "  * Generated release\n\n" )
FILE( APPEND debian/changelog " -- Terry Hendrix (Mongoose) <mongooseichiban@gmail.com>  Thu, 14 Dec 2006 15:52:34 -0500\n\n" )
FILE( APPEND debian/changelog "" )

INSTALL( TARGETS ${Name}-${Major}.${Minor}.${Micro} LIBRARY DESTINATION /usr/lib )



