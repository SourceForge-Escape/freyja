# Create the libhel library.

set(Name hel)
set(Major 0)
set(Minor 2)
set(Micro 2)
set(Revision 4)

#enable_testing()

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

# Debug build
add_definitions( -DDEBUG -g -Wall )

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

	cmake_cxx_compiler("i586-mingw32msvc-g++ -mms-bitfields")
	add_definitions(-Wl,--export-all-symbols -Wl,--enable-auto-import)

#elseif(OSX)
#	@-mkdir -p $(BUILD_DEBUG_DIR)
#	$(MAKE) osxtargets BUILDDIR=$(BUILD_DEBUG_DIR) \
#	LD_FLAGS="$(LD_FLAGS) " \
#	SHLIBLDFLAGS="$(LD_FLAGS) -dynamiclib -fPIC" \
#	CFLAGS="$(DEBUG_CFLAGS)"

#$(BUILDDIR)/$(NAME)$(API).dynlib : $(PROG_OBJS)
#	$(CC) $(LD_FLAGS) -L/usr/local/lib -lhel0 \
#	-L/usr/lib -lstdc++ -fPIC -dynamiclib \
#	-o $(BUILDDIR)/$(MODULE).dynlib \
#	$(PROG_OBJS)
endif(WIN32)	

include_directories( ../mstl . )

link_directories(../bin/libhel)

set(Objs
	hel/math 
	hel/BoundingVolume 
	hel/CollisionObject 
	hel/Mass 
	hel/Mat44 
	hel/Quat 
	hel/Ray 
	hel/Simulation 
	hel/Spring 
	hel/ViewVolume 
)

add_library( ${Name}-${Major}.${Minor}.${Micro} SHARED ${Objs} )

# Packaging
#add_custom_command(TARGET libhel.deb COMMAND dpkg-buildpackage -rfakeroot)


# Unit tests
add_test(matrix matrix_test)
add_test(quat quat_test)
add_test(ray ray_test)
add_test(vec3 vec3_test)


#If the ENABLE_TESTING command has been run, this command adds a test target to the current directory. If ENABLE_TESTING has not been run, this command does nothing. The tests are run by the testing subsystem by executing Exename with the specified arguments. Exename can be either an executable built by this project or an arbitrary executable on the system (like tclsh). The test will be run with the current working directory set to the CMakeList.txt files corresponding directory in the binary tree. 



# ADD_LIBRARY(foo SHARED ${foo_sources})
# ADD_LIBRARY(foo-static STATIC ${foo_sources})
# // The library target "foo" already has a default OUTPUT_NAME of "foo", so we don't need to change it.
# // The library target "foo-static" has a default OUTPUT_NAME of "foo-static", so change it.
# SET_TARGET_PROPERTIES(foo-static PROPERTIES OUTPUT_NAME "foo")




