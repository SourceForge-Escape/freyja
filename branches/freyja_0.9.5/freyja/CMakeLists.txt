# freyja
set(Name freyja)
set(Major 0)
set(Minor 9)
set(Micro 5)
set(Revision 14)
set(Email mongooseichiban@gmail.com)
set(Website http://icculus.org/freyja)
set(BuildHost `uname -n -m -s`)
set(Version ${Major}.${Minor}.${Micro}.${Revision})
set(BuildId 9)

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
)

#$(shell freetype-config --cflags)
include_directories( /usr/include/freetype2 )
#$(shell freetype-config --libs)
link_libraries(	-lfreetype -lz )



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


include_directories( ../mstl ../libfreyja ../libhel ../libmgtk ../tinyxml .  )

link_directories( ../libfreyja ../libhel ../libmgtk ../tinyxml )
link_libraries(	-lfreyja-0.9.5 -lhel-0.2.2 -lmgtk-0.1.1 -ltinyxml )

set(Objs
	src/freyja_events
	src/Control
	src/ControlPrinter
	src/Cursor
	src/FreyjaRender
	src/FreyjaControl
	src/FreyjaOpenGL
	src/MaterialControl
	src/OpenGLPrinter
	src/RecentFiles
	src/Texture
)


add_executable( ${Name} ${Objs} )

FILE( WRITE debian/changelog "freyja (${Version}) feisty; urgency=low\n\n" )
FILE( APPEND debian/changelog "  * Generated release\n\n" )
FILE( APPEND debian/changelog " -- Terry Hendrix (Mongoose) <mongooseichiban@gmail.com>  Thu, 14 Dec 2006 15:52:34 -0500\n\n" )
FILE( APPEND debian/changelog "" )


INSTALL( TARGETS ${Name} RUNTIME DESTINATION /usr/bin )
INSTALL( FILES data/freyja-dev.mlisp data/vera.ttf DESTINATION /usr/share/freyja )
INSTALL( DIRECTORY data/icons data/shaders DESTINATION /usr/share/freyja )


