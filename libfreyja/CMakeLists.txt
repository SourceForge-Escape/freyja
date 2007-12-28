# libfreyja
set(Name freyja)
set(Major 0)
set(Minor 10)
set(Micro 0)
set(Revision 0)
set(Email mongooseichiban@gmail.com)
EXEC_PROGRAM( date ARGS +%Y%m%d.%H%M OUTPUT_VARIABLE BuildDate )

#enable_testing()

add_definitions(
	-DVERSION=\"\\\"lib${Name}-${Major}.${Minor}.${Micro}.${Revision}\\\"\" 
	-DMAJOR_VERSION=${Major}
	-DMINOR_VERSION=${Minor}
	-DMICRO_VERSION=${Micro}
	-DREVISION=${Revision}
	-DBUILD_ID=${BuildDate}
	-DEMAIL_ADDRESS=\"\\\"$(Email)\\\"\" 
	-DBUILD_NAME=\"\\\"$(NAME)-$(VERSION)-$(BUILD_ID)\\\"\"
)

# Build options
add_definitions(
	-DEXPERIMENTAL
	-DFREYJAIMAGE_PLUGINS
	-DFREYJA_PLUGINS	
)

# Common
add_definitions( -Wall -DENABLE_FREYJA_ASSERTS )

# Debug
#add_definitions( -DDEBUG -g )

# RELEASE
add_definitions( -DRELEASE -O2 -finline-functions -fomit-frame-pointer -funroll-loops )

# Lua
FIND_LIBRARY( LUA_FOUND NAMES lua5.1 lua-5.1 )
FIND_PATH( LUA_INCLUDE_PATH lua.h  PATH_SUFFIXES lua5.1 )
FIND_FILE( LUA_INCLUDE lua.h PATH_SUFFIXES lua5.1 )
FIND_FILE( LUALIB_INCLUDE lualib.h PATH_SUFFIXES lua5.1 )
FIND_FILE( LAUXLIB_INCLUDE lauxlib.h PATH_SUFFIXES lua5.1 )
if (LUA_FOUND)

	add_definitions( -DLUA_FOUND -DLUA_FOUND_INCLUDE )
	include_directories( ${LUA_INCLUDE_PATH} )
	link_libraries( ${LUA_FOUND} )
	message( STATUS "Lua 5.1 library found.\n\t${LUA_FOUND}\n\t${LUA_INCLUDE_PATH}\n\t${LUA_INCLUDE}\n\t${LUALIB_INCLUDE}\n\t${LAUXLIB_INCLUDE}" )

elseif ( NOT LUA_FOUND )

	message(FATAL_ERROR "The library LUA 5.1 was not found.  Ensure you have the development libraries installed, and the files are in the correct search path.")

endif (LUA_FOUND)

# LuaWrapper 
add_definitions( -DLUAWRAPPER_FOUND )
include_directories( ../lua )
link_directories( ../lua )
link_libraries(	-lluawrapper )

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
	freyja/AnimationTrack
	freyja/Bone
	freyja/BoneABI
	freyja/Camera
	freyja/CameraABI
	freyja/FileABI
	freyja/FloatArray
	freyja/Keyframe
	freyja/Light
	freyja/LightABI
	freyja/LuaABI
	freyja/Material
	freyja/MaterialABI
	freyja/Mesh
	freyja/MeshABI
	freyja/Metadata
	freyja/Node
	freyja/NodeABI
	freyja/PakABI
	freyja/PakReader
	freyja/PerlinNoise
	freyja/PixelBuffer
	freyja/PluginABI
	freyja/Plugin
	freyja/Printer
	freyja/PythonABI
	freyja/Renderable
	freyja/RigidBody
	freyja/QueryABI
	freyja/Scene
	freyja/SceneABI
	freyja/SceneNodeABI
	freyja/Skeleton
	freyja/SkeletonABI 
	freyja/Vertex
)


add_library( ${Name}-${Major}.${Minor}.${Micro} SHARED ${Objs} )

# Packaging
#add_custom_command(TARGET libhel.deb COMMAND dpkg-buildpackage -rfakeroot)


# Unit tests



add_custom_target( zip
	COMMAND zip -r ../lib${Name}-${Major}.${Minor}.${Micro}.${Revision}-${BuildDate}.zip . )

add_custom_target( deb
	COMMAND fakeroot dpkg-buildpackage )

# Generate a new debian/changelog.
FILE( WRITE debian/changelog "libfreyja (${Major}.${Minor}.${Micro}.${Revision}) feisty; urgency=low\n\n" )
FILE( APPEND debian/changelog "  * Generated release\n\n" )
FILE( APPEND debian/changelog " -- Terry Hendrix (Mongoose) <mongooseichiban@gmail.com>  Thu, 14 Dec 2006 15:52:34 -0500\n\n" )
FILE( APPEND debian/changelog "" )

INSTALL( TARGETS ${Name}-${Major}.${Minor}.${Micro} LIBRARY DESTINATION /usr/lib )


add_custom_target( win32 
	COMMAND make -f Makefile.Win32 )

