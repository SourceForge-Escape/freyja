# Create the libhel library.

set(Name hel)
set(Major 0)
set(Minor 2)
set(Micro 2)
set(Revision 4)

enable_testing()

add_definitions(
	-DVERSION=\"\\\"libhel-${Major}.${Minor}.${Micro}.${Revision}\\\"\" 
	-DMAJOR_VERSION=${Major}
	-DMINOR_VERSION=${Minor}
	-DMICRO_VERSION=${Micro}
	-DREVISION=${Revision}
	-DBUILD_ID=20070415
)

add_definitions(
	-DEXPERIMENTAL
)

# DEBUG
#add_definitions( -DDEBUG -g -Wall )

# RELEASE
add_definitions( -DRELEASE -Wall -O2 -finline-functions -fomit-frame-pointer -funroll-loops )


# NOTE: cmake 2.6 ( unreleased ) is required for cross compiling.
if (WIN32)
#	@-mkdir -p $(BUILD_WIN32_DIR)
#	$(MAKE) w32targets BUILDDIR=$(BUILD_WIN32_DIR) \
#	CC=$(WIN32_CC) \
#	CPP=$(WIN32_CPP)  \
#	LD=$(WIN32_LD)  \
#	MODULE="hel0" \
#	SHLIBEXT=".dll" \
#	CFLAGS="$(BASE_CFLAGS) $(WIN32_INC_DIR) -DHELEXPORTSCLASS" \
#	LD_FLAGS=" -shared -lstdc++ -lm" \
#	LIBS="-lm -lstdc++ $(WIN32_LIB_DIR) -lpthreadGC2"

#$(BUILDDIR)/$(MODULE).dll : $(OBJS)
#	$(CC) -shared -o $(BUILDDIR)/$(MODULE).dll \
#    -Wl,--out-implib=$(BUILDDIR)/lib$(MODULE).a \
#    -Wl,--export-all-symbols \
#    -Wl,--enable-auto-import \
#    -Wl,--whole-archive $(OBJS) \
#    -Wl,--no-whole-archive $(LIBS)

	#cmake_cxx_compiler("i586-mingw32msvc-g++ -mms-bitfields")
	#add_definitions(-Wl,--export-all-symbols -Wl,--enable-auto-import)

endif(WIN32)	


#######################################################################
# Dependencies
#######################################################################

# mstl
include_directories( ../mstl . )

# TinyXML 
add_definitions( -DTINYXML_FOUND )
include_directories( ../tinyxml )
link_directories( ../tinyxml )
link_libraries(	-ltinyxml )


#######################################################################
# Objects
#######################################################################

link_directories(../bin/libhel)

set(Objs
	hel/math 
	hel/BoundingVolume 
	hel/CollisionObject 
	hel/Mass 
	hel/Mat44 
	hel/Octree 
	hel/Quat 
	hel/Ray 
	hel/Simulation 
	hel/Spring 
	hel/ViewVolume 
)

add_library( ${Name}-${Major}.${Minor}.${Micro} SHARED ${Objs} )


#######################################################################
# Packaging
#######################################################################

#add_custom_command(TARGET libhel.deb COMMAND dpkg-buildpackage -rfakeroot)

# Generate a new debian/changelog.
FILE( WRITE debian/changelog "libhel (${Major}.${Minor}.${Micro}.${Revision}) feisty; urgency=low\n\n" )
FILE( APPEND debian/changelog "  * Generated release\n\n" )
FILE( APPEND debian/changelog " -- Terry Hendrix (Mongoose) <mongooseichiban@gmail.com>  Thu, 14 Dec 2006 15:52:34 -0500\n\n" )
FILE( APPEND debian/changelog "" )

# Generate install rule.
INSTALL( TARGETS ${Name}-${Major}.${Minor}.${Micro} LIBRARY DESTINATION /usr/lib )


#######################################################################
# Unit tests
#######################################################################

# Remember to run `export LD_LIBRARY_PATH=.` before running CTest!

# Mat44
add_executable( mat44 hel/unit/mat44 )
set_target_properties( mat44 PROPERTIES OUTPUT_NAME unit/mat44 )
set_target_properties( mat44 PROPERTIES COMPILE_FLAGS 
	"-DUNIT_TEST_MAT44" )
set_target_properties( mat44 PROPERTIES LINK_FLAGS 
	"-L. -l${Name}-${Major}.${Minor}.${Micro} -lpthread" )
add_test( mat44 unit/mat44 unit/mat44-results.xml )


# Ray
add_executable( ray hel/unit/ray )
set_target_properties( ray PROPERTIES OUTPUT_NAME unit/ray )
set_target_properties( ray PROPERTIES COMPILE_FLAGS 
	"-DUNIT_TEST_RAY" )
set_target_properties( ray PROPERTIES LINK_FLAGS 
	"-L. -l${Name}-${Major}.${Minor}.${Micro} -lpthread" )
add_test( ray unit/ray unit/ray-results.xml )



