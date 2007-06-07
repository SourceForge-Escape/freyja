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








