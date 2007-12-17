# Build system for magedemo.

set(Name luawrapper)
set(Major 5)
set(Minor 1)
set(Micro 0)
set(Revision 0)
set(Email mongooseichiban@gmail.com)
set(BuildId 1)

enable_testing()

add_definitions(
	-DVERSION=\"\\\"${Name}-${Major}.${Minor}.${Micro}.${Revision}\\\"\" 
	-DMAJOR_VERSION=${Major}
	-DMINOR_VERSION=${Minor}
	-DMICRO_VERSION=${Micro}
	-DREVISION=${Revision}
	-DBUILD_ID=1
	-DEMAIL_ADDRESS=\"\\\"$(Email)\\\"\" 
	-DBUILD_NAME=\"\\\"$(NAME)-$(VERSION)-$(BUILD_ID)\\\"\"
)

add_definitions( -DDEBUG -g	-Wall )

include_directories( ../mstl )
include_directories( lua )

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



# Objects stripped down from various libraries to one executeable for this demo.
set(Objs

	lua/Lua
)

# Executable build rule.
add_library( ${Name} SHARED ${Objs} )

# Generate a new debian/changelog.
FILE( WRITE debian/changelog "lib${Name} (${Major}.${Minor}.${Micro}-${Revision}) feisty; urgency=low\n\n" )
FILE( APPEND debian/changelog "  * Generated release\n\n" )
FILE( APPEND debian/changelog " -- Terry Hendrix (Mongoose) <mongooseichiban@gmail.com>  Thu, 14 Dec 2006 15:52:34 -0500\n\n" )
FILE( APPEND debian/changelog "" )

INSTALL( TARGETS ${Name} LIBRARY DESTINATION /usr/lib )

add_custom_target( deb
	COMMAND fakeroot dpkg-buildpackage )

add_custom_target( win32 
	COMMAND make -f Makefile.Win32 
	COMMENT "luawrapper (win32)" )



