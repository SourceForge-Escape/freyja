# Build system for magedemo.

set(Name tinyxml)
set(Major 2)
set(Minor 5)
set(Micro 3)
set(Revision 1)
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

include_directories( tinyxml )


# Objects stripped down from various libraries to one executeable for this demo.
set(Objs

	tinyxml/tinystr
	tinyxml/tinyxml
	tinyxml/tinyxmlerror
	tinyxml/tinyxmlparser
)

# Executable build rule.
add_library( ${Name} SHARED ${Objs} )

# Generate a new debian/changelog.
FILE( WRITE debian/changelog "libtinyxml (${Major}.${Minor}.${Micro}-${Revision}) feisty; urgency=low\n\n" )
FILE( APPEND debian/changelog "  * Generated release\n\n" )
FILE( APPEND debian/changelog " -- Terry Hendrix (Mongoose) <mongooseichiban@gmail.com>  Thu, 14 Dec 2006 15:52:34 -0500\n\n" )
FILE( APPEND debian/changelog "" )

INSTALL( TARGETS ${Name} LIBRARY DESTINATION /usr/lib )


add_custom_target( deb
	COMMAND fakeroot dpkg-buildpackage )

add_custom_target( win32 
	COMMAND make -f Makefile.Win32 )


