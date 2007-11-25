# freyja
set(Name freyja)
set(Major 0)
set(Minor 9)
set(Micro 5)
set(Revision 17)
set(Email mongooseichiban@gmail.com)
set(Website http://icculus.org/freyja)
set(BuildHost `uname -n -m -s`)
set(Version ${Major}.${Minor}.${Micro}.${Revision})
set(BuildId 1)

#enable_testing()

add_definitions(
	-DVERSION=\"\\\"${Name}-${Version}-${BuildId}\\\"\"
	-DMAJOR_VERSION=${Major}
	-DMINOR_VERSION=${Minor}
	-DMICRO_VERSION=${Micro}
	-DREVISION=${Revision}
	-DBUILD_ID=20070415
	-DEMAIL_ADDRESS=\"\\\"${Email}\\\"\" 
	-DBUILD_NAME=\"\\\"${Name}-${Version}-${BuildId}\\\"\"
	-DPROJECT_URL=\"\\\"${Website}\\\"\" 
	-DPROGRAM_NAME=\"\\\"${Name}\\\"\" 
	-DBUILD_HOST=\"\\\"${BuildHost}\\\"\"
	-DPROGRAM_NAME=\"\\\"${Name}\\\"\" 

	-DUSING_GTK_2_0 -DHAVE_FREYJA_IMAGE -DHAVE_OPENGL
	-DFORCE_ICON_TOOLBAR -DMGTKEXPORTSCLASS 
)

# DEBUG
add_definitions( -DDEBUG -g -Wall -O0 )

# RELEASE
#add_definitions( -DRELEASE -Wall -O2 -finline-functions -fomit-frame-pointer -funroll-loops )

add_definitions(
	-DEXPERIMENTAL
	-DHAVE_FREYJA_IMAGE 
	-DHAVE_OPENGL 
	-DHAVE_GTKGL 
	-DHAVE_HEL
	-DHAVE_FREETYPE2 
	-DTINYXML_FOUND
	-DHAVE_GTKGLEXT
	-DLUAWRAPPER_FOUND
)

#$(shell freetype-config --cflags)
include_directories( /usr/include/freetype2 )
#$(shell freetype-config --libs)
link_libraries(	-lfreetype -lz )


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


# OpenGL Framework and freetype2 for MacOS
#if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
if(APPLE)
	FIND_LIBRARY( OPENGL_LIBRARIES NAMES OpenGL)
	link_libraries( ${OPENGL_LIBRARIES} )
	include_directories(  /usr/X11R6/include  /usr/X11R6/include/freetype2  )
	link_directories( /usr/X11R6/lib )
else(APPLE)
	add_definitions( -DUSING_OPENGL_EXT )
endif(APPLE)


include_directories( ../mstl ../libfreyja ../libhel ../libmgtk ../tinyxml ../lua .  )

link_directories( ../libfreyja ../libhel ../libmgtk ../tinyxml )
link_libraries(	-lfreyja-0.9.5 -lhel-0.2.2 -lmgtk-0.1.1 -ltinyxml )

set(Objs
	src/freyja_events
	src/freyja3d_scenegraph
	src/misc
	src/Control
	src/ControlPrinter
	src/Cursor
	src/FreyjaRender
	src/FreyjaControl
	src/FreyjaOpenGL
	src/MaterialControl
	src/OpenGLPrinter
	src/Plugins
	src/RecentFiles
	src/Texture
)

add_executable( ${Name} ${Objs} )


add_custom_target( deb
	COMMAND fakeroot dpkg-buildpackage )

FILE( WRITE debian/changelog "freyja (${Version}) feisty; urgency=low\n\n" )
FILE( APPEND debian/changelog "  * Generated release\n\n" )
FILE( APPEND debian/changelog " -- Terry Hendrix (Mongoose) <mongooseichiban@gmail.com>  Thu, 14 Dec 2006 15:52:34 -0500\n\n" )
FILE( APPEND debian/changelog "" )


INSTALL( TARGETS ${Name} RUNTIME DESTINATION /usr/bin )
INSTALL( FILES data/freyja-dev.mlisp data/vera.ttf DESTINATION /usr/share/freyja )
INSTALL( DIRECTORY data/icons data/shaders DESTINATION /usr/share/freyja )


add_custom_target( win32 
	COMMAND make -f Makefile.Win32 )


