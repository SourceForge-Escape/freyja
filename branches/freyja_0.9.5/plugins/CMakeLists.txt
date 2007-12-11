################################################################
# 2007.11.23
# Terry 'Mongoose' Hendrix II <mongooseichiban@gmail.com>
# - CMake conversion from old GNU Makefile.
################################################################# 

add_custom_target( deb
	COMMAND fakeroot dpkg-buildpackage )

add_custom_target( win32 
	COMMAND make -f Makefile.old win32 )

add_custom_target( osx
	COMMAND make -f Makefile.old osx )

INSTALL( DIRECTORY
	../bin/plugins/ui/debug/ DESTINATION /usr/share/freyja/plugins )

INSTALL( DIRECTORY
	../bin/plugins/model/debug/ DESTINATION /usr/share/freyja/plugins/model )

INSTALL( DIRECTORY
	../bin/plugins/image/debug/ DESTINATION /usr/share/freyja/plugins/image )


############################################################################
# Interface plugins
############################################################################

#macro( ui_plugins_macro arg )
#	foreach( plugin 
#			 lineageIIextract 
#			 octree
#			 pakreader
#			 perlinnoise
#			 reverseengineer )
#		exec_program( make ${PLUGIN_DIR}modules/interface/${plugin} ARGS ${arg} )
#	endforeach( plugin )
#endmacro( ui_plugins_macro  )
#

add_custom_target( ui-plugin-lineageIIextract ALL
	COMMAND cd ${PLUGIN_DIR}modules/interface/lineageIIextract && make )

add_custom_target( ui-plugin-octree ALL
	COMMAND cd ${PLUGIN_DIR}modules/interface/octree && make )

add_custom_target( ui-plugin-pakreader ALL
	COMMAND cd ${PLUGIN_DIR}modules/interface/pakreader && make )

add_custom_target( ui-plugin-perlinnoise ALL
	COMMAND cd ${PLUGIN_DIR}modules/interface/perlinnoise && make )

add_custom_target( ui-plugin-reverseengineer ALL
	COMMAND cd ${PLUGIN_DIR}modules/interface/reverseengineer && make )


############################################################################
# Image plugins
############################################################################

add_custom_target( image-plugin-bmp ALL
	COMMAND cd ${PLUGIN_DIR}modules/image/bmp && make )

add_custom_target( image-plugin-dds ALL
	COMMAND cd ${PLUGIN_DIR}modules/image/dds && make )

add_custom_target( image-plugin-jpg ALL
	COMMAND cd ${PLUGIN_DIR}modules/image/jpg && make )

add_custom_target( image-plugin-pcx ALL
	COMMAND cd ${PLUGIN_DIR}modules/image/pcx && make )

add_custom_target( image-plugin-ppm ALL
	COMMAND cd ${PLUGIN_DIR}modules/image/ppm && make )

add_custom_target( image-plugin-png ALL
	COMMAND cd ${PLUGIN_DIR}modules/image/png && make )

add_custom_target( image-plugin-tga ALL
	COMMAND cd ${PLUGIN_DIR}modules/image/tga && make )

#add_custom_target( image-plugins ALL
#	COMMAND make image-plugin-bmp
#	COMMAND make image-plugin-dds
#	COMMAND make image-plugin-jpg
#	COMMAND make image-plugin-pcx
#	COMMAND make image-plugin-ppm
#	COMMAND make image-plugin-png
#	COMMAND make image-plugin-tga
#	COMMENT "Image plugins." )


############################################################################
# Model plugins
############################################################################

add_custom_target( model-plugin-3ds ALL
	COMMAND cd ${PLUGIN_DIR}modules/model/3ds && make )

add_custom_target( model-plugin-ase ALL
	COMMAND cd ${PLUGIN_DIR}modules/model/ase && make )

add_custom_target( model-plugin-bsa ALL
	COMMAND cd ${PLUGIN_DIR}modules/model/bsa && make )

add_custom_target( model-plugin-bvh ALL
	COMMAND cd ${PLUGIN_DIR}modules/model/bvh && make )

add_custom_target( model-plugin-dof ALL
	COMMAND cd ${PLUGIN_DIR}modules/model/dof && make )

#add_custom_target( model-plugin-dxf ALL
#	COMMAND cd ${PLUGIN_DIR}modules/model/dxf && make )

add_custom_target( model-plugin-egg9 ALL
	COMMAND cd ${PLUGIN_DIR}modules/model/egg9 && make )

add_custom_target( model-plugin-et ALL
	COMMAND cd ${PLUGIN_DIR}modules/model/et && make )

add_custom_target( model-plugin-glm ALL
	COMMAND cd ${PLUGIN_DIR}modules/model/glm && make )

add_custom_target( model-plugin-halflife ALL
	COMMAND cd ${PLUGIN_DIR}modules/model/halflife && make )
			 
add_custom_target( model-plugin-llm ALL
	COMMAND cd ${PLUGIN_DIR}modules/model/llm && make )

add_custom_target( model-plugin-lwo ALL
	COMMAND cd ${PLUGIN_DIR}modules/model/lwo && make )

add_custom_target( model-plugin-md2 ALL
	COMMAND cd ${PLUGIN_DIR}modules/model/md2 && make )

add_custom_target( model-plugin-md3 ALL
	COMMAND cd ${PLUGIN_DIR}modules/model/md3 && make )
			 
add_custom_target( model-plugin-md5 ALL
	COMMAND cd ${PLUGIN_DIR}modules/model/md5 && make )

add_custom_target( model-plugin-mdl ALL
	COMMAND cd ${PLUGIN_DIR}modules/model/mdl && make )

add_custom_target( model-plugin-milkshape ALL
	COMMAND cd ${PLUGIN_DIR}modules/model/milkshape && make )

add_custom_target( model-plugin-nad ALL
	COMMAND cd ${PLUGIN_DIR}modules/model/nad && make )

add_custom_target( model-plugin-nif4 ALL
	COMMAND cd ${PLUGIN_DIR}modules/model/nif4 && make )

#add_custom_target( model-plugin-nif5 ALL
#	COMMAND cd ${PLUGIN_DIR}modules/model/nif5 && make )

add_custom_target( model-plugin-nod ALL
	COMMAND cd ${PLUGIN_DIR}modules/model/nod && make )

add_custom_target( model-plugin-obj ALL
	COMMAND cd ${PLUGIN_DIR}modules/model/obj && make )

add_custom_target( model-plugin-psk ALL
	COMMAND cd ${PLUGIN_DIR}modules/model/psk && make )

add_custom_target( model-plugin-sims ALL
	COMMAND cd ${PLUGIN_DIR}modules/model/sims && make )

add_custom_target( model-plugin-skel ALL
	COMMAND cd ${PLUGIN_DIR}modules/model/skel && make )

add_custom_target( model-plugin-skn ALL
	COMMAND cd ${PLUGIN_DIR}modules/model/skn && make )

add_custom_target( model-plugin-smd ALL
	COMMAND cd ${PLUGIN_DIR}modules/model/smd && make )

add_custom_target( model-plugin-tombraider ALL
	COMMAND cd ${PLUGIN_DIR}modules/model/tombraider && make )

add_custom_target( model-plugin-trmesh ALL
	COMMAND cd ${PLUGIN_DIR}modules/model/trmesh && make )
		 
#add_custom_target( model-plugin-user1 ALL
#	COMMAND cd ${PLUGIN_DIR}modules/model/user1 && make )

add_custom_target( model-plugin-utpackage ALL
	COMMAND cd ${PLUGIN_DIR}modules/model/utpackage && make )


# Install rules
#macro( user-install-macro )
##	file( MAKE_DIRECTORY ${USER_DIR} )
#	exec_program( make ARGS -f ${PLUGIN_DIR}Makefile.old user-install )
#endmacro( user-install-macro )
#
#add_custom_command( OUTPUT user-install 
#			COMMAND user-install-macro )

# Pure Wrapper
#exec_program( make ARGS -f ${PLUGIN_DIR}Makefile.old )

