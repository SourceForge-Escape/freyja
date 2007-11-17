--======================================================================
-- 
-- Project : freyja3d
-- Author  : Terry 'Mongoose' Hendrix II
-- Website : http://icculus.org/~freyja
-- Email   : mongooseichiban@gmail.com
-- Object  : 
-- Comments: Test script for freyja3d UI using lua.
--           Mostly for testing gtk+ bindings and UI design.
-- 
---- History ---------------------------------------------------------- 
--
-- 2007.11.15:
-- Terry 'Mongoose' Hendrix II, 
-- Created
--======================================================================

print("-- freyja3d.lua --")


function freyja3d_ui_menubar( vbox )

	menubar = mgtk_menubar()
	mgtk_box_pack( vbox, menubar )

	--handlebox = mgtk_handlebox( 1 )
	--mgtk_box_pack( vbox, handlebox )
	--mgtk_box_pack( handlebox, menubar )

	-- File
	submenu = mgtk_submenu("_File")
	mgtk_append_menu( menubar, submenu )

	mgtk_append_menu( submenu, mgtk_menu_item( "_New", "eNewFile", "C-n", "gtk-new" ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "Ap_pend Model...", "eAppendFile", "gtk-paste" ) )
	mgtk_append_menu( submenu, mgtk_menu_separator() )
	mgtk_append_menu( submenu, mgtk_submenu( "R_ecent Models...", "eRecentFiles", "recent_models_bind" ) )
	mgtk_append_menu( submenu, mgtk_menu_separator() )
	mgtk_append_menu( submenu, mgtk_menu_item( "_Save Model", "eSaveFileModel", "C-s", "gtk-save" ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "Save Model _As...", "eSaveModel", "S-C-s", "gtk-save-as" ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "_Revert Model", "eRevertFile", "gtk-revert-to-saved" ) )
	mgtk_append_menu( submenu, mgtk_menu_separator() )
	mgtk_append_menu( submenu, mgtk_menu_item( "Proper_ties", "eInfo", "gtk-properties" ) )
	mgtk_append_menu( submenu, mgtk_menu_separator() )
	mgtk_append_menu( submenu, mgtk_submenu( "_Import Model...", "eImportFile" ) )
	mgtk_append_menu( submenu, mgtk_submenu( "_Export Model...", "eExportFile" ) )
	mgtk_append_menu( submenu, mgtk_menu_separator() )
	mgtk_append_menu( submenu, mgtk_menu_item( "_Close", "eCloseFile", "C-w", "gtk-close" ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "_Quit", "eShutdown", "C-q", "gtk-quit" ) )

	-- Edit
	submenu = mgtk_submenu( "_Edit" )
	mgtk_append_menu( menubar, submenu )
	mgtk_append_menu( submenu, mgtk_menu_item( "Take _Screenshot", "eScreenShot", "gtk-screenshot" ) )
	mgtk_append_menu( submenu, mgtk_menu_separator() )
	mgtk_append_menu( submenu, mgtk_menu_item( "_Undo", "eUndo", "C-z", "gtk-undo" ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "_Redo", "eRedo", "S-C-z", "gtk-redo" ) )
	mgtk_append_menu( submenu, mgtk_menu_separator() )
	mgtk_append_menu( submenu, mgtk_menu_item( "Cu_t", "eCut", "C-x", "gtk-cut" ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "_Copy", "eCopy", "C-c", "gtk-copy" ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "_Paste", "ePaste", "C-v", "gtk-paste" ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "_Delete", "eDelete", "S-Delete", "gtk-delete" ) )
	mgtk_append_menu( submenu, mgtk_menu_separator() )
	mgtk_append_menu( submenu, mgtk_menu_item( "Select _All", "eSelectAll", "C-a", "gtk-select-table" ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "Unselect A_ll", "eUnSelectAll", "S-C-a", "gtk-select-table" ) )
	mgtk_append_menu( submenu, mgtk_menu_separator() )
	mgtk_append_menu( submenu, mgtk_menu_item( "D_uplicate", "eDupeObject", "C-d", "gtk-convert" ) )
	mgtk_append_menu( submenu, mgtk_menu_separator() )
	mgtk_append_menu( submenu, mgtk_menu_item( "Pr_eferences", "ePreferencesDialog", "gtk-preferences" ) )

	-- View
	submenu = mgtk_submenu( "_View" )
	mgtk_append_menu( menubar, submenu )
	mgtk_append_menu( submenu, mgtk_menu_separator() )
	mgtk_append_menu( submenu, mgtk_menu_item( "_Fullscreen", "eFullscreen", "F11", "gtk-fullscreen" ) )
--	mgtk_append_menu( submenu, mgtk_menu_item_check( "_Viewports", "eViewports", 0 ) )
	mgtk_append_menu( submenu, mgtk_menu_separator() )
--	mgtk_append_menu( submenu, mgtk_menu_item_check( "_Grid", "eRenderGrid", 1 ) )
	submenu2 = mgtk_submenu( "_Mesh" )
	mgtk_append_menu( submenu, submenu2 )
--	mgtk_append_menu( submenu2, mgtk_menu_item_check( "_Points", "eRenderVertex", 0 ) )
--	mgtk_append_menu( submenu2, mgtk_menu_item_check( "_Wireframe", "eRenderWireframe", 0 ) )
--	mgtk_append_menu( submenu2, mgtk_menu_item_check( "_Normals", "eRenderNormals", 0 ) )
--	mgtk_append_menu( submenu2, mgtk_menu_item_check( "_Faces", "eRenderFace", 1 ) )
--	mgtk_append_menu( submenu2, mgtk_menu_item_check( "_Lighting", "eRenderLighting", 1 ) )
--	mgtk_append_menu( submenu2, mgtk_menu_item_check( "_Material", "eRenderMaterial", 1 ) )
--	mgtk_append_menu( submenu2, mgtk_menu_item_check( "_Bounding Volume", "eRenderBbox", 1 ) )
--	mgtk_append_menu( submenu2, mgtk_menu_item_check( "_Skeleton", "eRenderSkeleton", 1 ) )
--	mgtk_append_menu( submenu2, mgtk_menu_item_check( "_Pick Ray", "eRenderPickRay", 1 ) )
	submenu2 = mgtk_submenu( "_Joint" )
	mgtk_append_menu( submenu, submenu2 )
--	mgtk_append_menu( submenu2, mgtk_menu_item_check( "_Point", "ePointJoint", 1 ) )
--	mgtk_append_menu( submenu2, mgtk_menu_item_check( "_Sphere", "eSphereJoint", 0 ) )
--	mgtk_append_menu( submenu2, mgtk_menu_item_check( "_Axes", "eAxisJoint", 0 ) )
	submenu2 = mgtk_submenu( "_Bone" )
	mgtk_append_menu( submenu, submenu2 )
--	mgtk_append_menu( submenu2, mgtk_menu_item_check( "_ZBuffer", "eRenderBoneZClear", 0 ) )
--	mgtk_append_menu( submenu2, mgtk_menu_item_check( "_Line", "eLineBone", 0 ) )
--	mgtk_append_menu( submenu2, mgtk_menu_item_check( "_PolyMesh", "ePolyMeshBone", 1 ) )
	submenu2 = mgtk_submenu( "_Create" )
	mgtk_append_menu( submenu, submenu2 )
	submenu2 = mgtk_submenu( "_Componets" )
	mgtk_append_menu( submenu, submenu2 )
	mgtk_append_menu( submenu2, mgtk_menu_item( "_Vertex", "eVertexNew" ) )
	mgtk_append_menu( submenu2, mgtk_menu_item( "_Face", "ePolygonNew" ) )
	mgtk_append_menu( submenu2, mgtk_menu_item( "_Mesh", "eMeshNew" ) )
	mgtk_append_menu( submenu2, mgtk_menu_separator() )
	mgtk_append_menu( submenu2, mgtk_menu_item( "_Bone", "eBoneNew", "C-B", "gtk-bone" ) )
	submenu2 = mgtk_submenu( "P_olygon Primatives" )
	mgtk_append_menu( submenu, submenu2 )
	mgtk_append_menu( submenu2, mgtk_menu_item( "_Cube", "eGenerateCube", "gtk-cube" ) )
	mgtk_append_menu( submenu2, mgtk_menu_item( "_Plane", "eGeneratePlane", "gtk-plane" ) )
	mgtk_append_menu( submenu2, mgtk_menu_item( "_Tube", "eGenerateTube", "gtk-tube" ) )
	mgtk_append_menu( submenu2, mgtk_menu_item( "C_ircle", "eGenerateCircle", "gtk-circle" ) )
	mgtk_append_menu( submenu2, mgtk_menu_item( "C_one", "eGenerateCone", "gtk-cone" ) )
	mgtk_append_menu( submenu2, mgtk_menu_item( "C_ylinder", "eGenerateCylinder", "gtk-cylinder" ) )
	mgtk_append_menu( submenu2, mgtk_menu_item( "_Sphere", "eGenerateSphere", "gtk-sphere" ) )


	-- Modify
	submenu = mgtk_submenu( "_Modify" )
	mgtk_append_menu( menubar, submenu )
	submenu2 = mgtk_submenu( "_Transforms" )
	mgtk_append_menu( submenu, submenu2 )
	mgtk_append_menu( submenu2, mgtk_menu_item( "Move", "eMoveObject", "F5", "gtk-move" ) )
	mgtk_append_menu( submenu2, mgtk_menu_item( "Rotate", "eRotateObject", "F6", "gtk-rotate" ) )
	mgtk_append_menu( submenu2, mgtk_menu_item( "Scale", "eScaleObject", "F7", "gtk-scale" ) )
	mgtk_append_menu( submenu2, mgtk_menu_item( "Select", "eSelect", "F8", "gtk-select" ) )
	mgtk_append_menu( submenu2, mgtk_menu_item( "Unselect", "eUnselect", "F9", "gtk-unselect" ) )
	submenu2 = mgtk_submenu( "_UV Mapping" )
	mgtk_append_menu( submenu, submenu2 )
	mgtk_append_menu( submenu2, mgtk_menu_item( "_Planar", "eMeshTexcoordPlaneProj", "gtk-uv" ) )
	mgtk_append_menu( submenu2, mgtk_menu_item( "_Cylindrical", "eMeshTexcoordCylindrical", "gtk-uv" ) )
	mgtk_append_menu( submenu2, mgtk_menu_item( "_Spherical", "eMeshTexcoordSpherical", "gtk-uv" ) )
	mgtk_append_menu( submenu2, mgtk_menu_separator() )
	submenu2 = mgtk_submenu( "_Vertex" )
	mgtk_append_menu( submenu, submenu2 )
	mgtk_append_menu( submenu2, mgtk_menu_item( "_Delete", "eVertexDelete" ) )
	mgtk_append_menu( submenu2, mgtk_menu_item( "_Combine", "eVertexCombine" ) )
	submenu2 = mgtk_submenu( "V_ertices..." )
	mgtk_append_menu( submenu, submenu2 )
	mgtk_append_menu( submenu2, mgtk_menu_item( "_Unselect", "eMeshUnselectVertices" ) )
	mgtk_append_menu( submenu2, mgtk_menu_separator() )
	mgtk_append_menu( submenu2, mgtk_menu_item( "_Clear weights", "eNone" ) )
	mgtk_append_menu( submenu2, mgtk_menu_item( "_View weights", "eNone" ) )
	mgtk_append_menu( submenu2, mgtk_menu_separator() )
	mgtk_append_menu( submenu2, mgtk_menu_item( "_Delete", "eNone" ) )
	mgtk_append_menu( submenu2, mgtk_menu_separator() )
	mgtk_append_menu( submenu2, mgtk_menu_item( "Mirror _X", "eMirrorMeshX" ) )
	mgtk_append_menu( submenu2, mgtk_menu_item( "Mirror _Y", "eMirrorMeshY" ) )
	mgtk_append_menu( submenu2, mgtk_menu_item( "Mirror _Z", "eMirrorMeshZ" ) )
	submenu2 = mgtk_submenu( "_Face" )
	mgtk_append_menu( submenu, submenu2 )
	mgtk_append_menu( submenu2, mgtk_menu_item( "_Select", "ePolygonSelect" ) )
	mgtk_append_menu( submenu2, mgtk_menu_item( "_Delete", "ePolygonDelete" ) )
	mgtk_append_menu( submenu2, mgtk_menu_item( "_Split", "ePolygonSplit" ) )
	mgtk_append_menu( submenu2, mgtk_menu_item( "_Collapse", "eCollapseFace" ) )
	mgtk_append_menu( submenu2, mgtk_menu_item( "_Extrude", "eExtrude", "F4", "gtk-polygon" ) )
	submenu2 = mgtk_submenu( "F_aces..." )
	mgtk_append_menu( submenu, submenu2 )
	mgtk_append_menu( submenu2, mgtk_menu_item( "_Unselect faces", "eMeshUnselectFaces" ) )
	mgtk_append_menu( submenu2, mgtk_menu_separator() )
	mgtk_append_menu( submenu2, mgtk_menu_item( "_Groups", "eSmoothingGroupsDialog" ) )
	mgtk_append_menu( submenu2, mgtk_menu_item( "_Smooth", "eSelectedFacesGenerateNormals" ) )
	mgtk_append_menu( submenu2, mgtk_menu_separator() )
	mgtk_append_menu( submenu2, mgtk_menu_item( "Clear alpha", "eClearSelectedFacesAlpha" ) )
	mgtk_append_menu( submenu2, mgtk_menu_item( "Set alpha", "eSetSelectedFacesAlpha" ) )
	mgtk_append_menu( submenu2, mgtk_menu_separator() )
	mgtk_append_menu( submenu2, mgtk_menu_item( "Select _vertices", "eSelectVerticesByFaces" ) )
	mgtk_append_menu( submenu2, mgtk_menu_separator() )
	mgtk_append_menu( submenu2, mgtk_menu_item( "Mirror _X", "eMirrorFacesX" ) )
	mgtk_append_menu( submenu2, mgtk_menu_item( "Mirror _Y", "eMirrorFacesY" ) )
	mgtk_append_menu( submenu2, mgtk_menu_item( "Mirror _Z", "eMirrorFacesZ" ) )
	mgtk_append_menu( submenu2, mgtk_menu_separator() )
	mgtk_append_menu( submenu2, mgtk_menu_item( "_Delete", "eSelectedFacesDelete" ) )
	submenu2 = mgtk_submenu( "_Mesh" )
	mgtk_append_menu( submenu, submenu2 )
	mgtk_append_menu( submenu2, mgtk_menu_item( "_Select", "eMeshSelect" ) )
	mgtk_append_menu( submenu2, mgtk_menu_item( "_Delete", "eMeshDelete" ) )
	mgtk_append_menu( submenu2, mgtk_menu_item( "_Tesselate", "eMeshTesselate" ) )
	mgtk_append_menu( submenu2, mgtk_menu_item( "_Generate Normals", "eMeshGenerateNormals" ) )
	mgtk_append_menu( submenu2, mgtk_menu_item( "_Flip Normals", "eMeshFlipNormals" ) )
	mgtk_append_menu( submenu2, mgtk_menu_item( "Mirror _X", "eMirrorMeshX" ) )
	mgtk_append_menu( submenu2, mgtk_menu_item( "Mirror _Y", "eMirrorMeshY" ) )
	mgtk_append_menu( submenu2, mgtk_menu_item( "Mirror _Z", "eMirrorMeshZ" ) )
	mgtk_append_menu( submenu2, mgtk_menu_item( "Snap-_Weld vertices", "eSnapWeldVerts" ) )
	mgtk_append_menu( submenu2, mgtk_menu_item( "Cleanup vertices", "eCleanupVertices" ) )
	mgtk_append_menu( submenu2, mgtk_menu_item( "Repack", "eMeshRepack" ) )
	mgtk_append_menu( submenu2, mgtk_menu_item( "Keyframe to mesh", "eMeshKeyframeCopy" ) )
	submenu2 = mgtk_submenu( "_Bone" )
	mgtk_append_menu( submenu, submenu2 )
	mgtk_append_menu( submenu2, mgtk_menu_item( "_New", "eBoneNew" ) )
	mgtk_append_menu( submenu2, mgtk_menu_item( "_Delete", "eBoneDelete" ) )
	mgtk_append_menu( submenu2, mgtk_menu_item( "_Select", "eBoneSelect" ) )
	mgtk_append_menu( submenu2, mgtk_menu_item( "_Parent", "eBoneLinkChild" ) )
	mgtk_append_menu( submenu2, mgtk_menu_item( "_Unparent", "eBoneUnLinkChild" ) )
	mgtk_append_menu( submenu2, mgtk_menu_item( "_Metadata", "eBoneMetaData" ) )
	mgtk_append_menu( submenu2, mgtk_menu_item( "_Update", "eBoneRefreshBindPose" ) )
	mgtk_append_menu( submenu2, mgtk_menu_item( "_Key Metadata", "eBoneKeyFrameMetaData" ) )

	-- Plugins
	submenu = mgtk_submenu( "_Plugins...", "ePluginMenu" )
	mgtk_append_menu( menubar, submenu )
	mgtk_append_menu( submenu, mgtk_submenu( "_Lua", "eRecentLua", "recent_lua_bind" ) )
	mgtk_append_menu( submenu, mgtk_submenu( "_Python", "eRecentPython", "recent_python_bind" ) )
	mgtk_append_menu( submenu, mgtk_submenu( "Material", "eRecentMaterial", "recent_material_bind" ) )
	mgtk_append_menu( submenu, mgtk_submenu( "_Test" ) )

	-- Snippets
	submenu = mgtk_submenu( "_Snippets" )
	mgtk_append_menu( menubar, submenu )
	mgtk_append_menu( submenu, mgtk_submenu( "Import..." ) )
	mgtk_append_menu( submenu, mgtk_submenu( "Export..." ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "Export Model...", "eNone", "gtk-save-as" ) )
	mgtk_append_menu( submenu, mgtk_menu_separator() )
	mgtk_append_menu( submenu, mgtk_menu_item( "Export Weights...", "eNone", "gtk-save-as" ) )
	mgtk_append_menu( submenu, mgtk_menu_separator() )
	mgtk_append_menu( submenu, mgtk_menu_item( "Export Track...", "eNone", "gtk-save-as" ) )
	mgtk_append_menu( submenu, mgtk_menu_separator() )
	mgtk_append_menu( submenu, mgtk_menu_separator() )
	mgtk_append_menu( submenu, mgtk_submenu( "Recent _Mesh...", "eRecentMeshXML" ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "Import Mesh...", "eXMLToMesh", "gtk-open" ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "Export Mesh...", "eMeshToXML", "gtk-save-as" ) )
	mgtk_append_menu( submenu, mgtk_menu_separator() )
	mgtk_append_menu( submenu, mgtk_submenu( "Recent Meta_data...", "eRecentMetadataXML" ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "Import Metadata...", "eXMLToMeta", "gtk-open" ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "Export Metadata...", "eMetaToXML", "gtk-save-as" ) )
	mgtk_append_menu( submenu, mgtk_menu_separator() )
	mgtk_append_menu( submenu, mgtk_submenu( "Recent _Skeleton...", "eRecentSkeletonXML" ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "Import Skeleton...", "eXMLToBones", "gtk-open" ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "Export Skeleton...", "eBonesToXML", "gtk-save-as" ) )
	mgtk_append_menu( submenu, mgtk_menu_separator() )
	mgtk_append_menu( submenu, mgtk_submenu( "Recent _Keyframe...", "eRecentKeyframe" ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "Import Keyframe...", "eNone", "gtk-open" ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "Export Keyframe...", "eNone", "gtk-save-as" ) )
	
	-- Separator
	mgtk_append_menu( menubar, mgtk_menu_separator() )

	-- Help
	submenu = mgtk_submenu( "_Help" )
	mgtk_append_menu( menubar, submenu )
	mgtk_append_menu( submenu, mgtk_menu_item( "_Contents", "eHelpDialog", "F1", "gtk-help" ) )
	mgtk_append_menu( submenu, mgtk_menu_separator() )
	mgtk_append_menu( submenu, mgtk_menu_item( "Get help online...", "eLaunchOnlineHelp", "gtk-network" ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "Report bugs...", "eLaunchBugTracker", "gtk-edit" ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "Check for updates...", "eLaunchUpdate", "gtk-refresh" ) )
	mgtk_append_menu( submenu, mgtk_menu_separator() )
	mgtk_append_menu( submenu, mgtk_menu_item( "_About", "eAboutDialog", "gtk-about" ) )
end


function freyja3d_ui_shelf_file( shelf )

	tab_file = mgtk_tab(shelf, "File", -1)
	toolbar1 = mgtk_toolbar( tab_file )
	mgtk_toolbar_button(toolbar1, "New", -1, "gtk-new", "New Model.")
	menu_recent_model = 
	mgtk_toolbar_menubutton(toolbar1, "Open", -1, "gtk-open", "Open Model...")
	mgtk_toolbar_button(toolbar1, "Save", -1, "gtk-save", "Save Model.")
	mgtk_toolbar_button(toolbar1, "Save As", -1, "gtk-save-as", "Save Model As...")
	mgtk_toolbar_separator(toolbar1)
	menu_recent_lua = 
	mgtk_toolbar_menubutton(toolbar1, "Open", -1, "icons/24x24/lua.png", "Open Lua Script...")
	menu_recent_python = 
	mgtk_toolbar_menubutton(toolbar1, "Open", -1, "icons/24x24/python.png", "Open Python Script...")
end


function freyja3d_ui_shelf_view( shelf )
	tab_view = mgtk_tab(shelf, "View", -1)
	toolbar2 = mgtk_toolbar( tab_view )

	optmenu = mgtk_optionmenu("Viewport", -1)
	mgtk_append_menu( optmenu, mgtk_menu_item("Orbit   ", -1) )
	mgtk_append_menu( optmenu, mgtk_menu_item("Front   ", -1) )
	mgtk_append_menu( optmenu, mgtk_menu_item("Back    ", -1) )
	mgtk_append_menu( optmenu, mgtk_menu_item("Right   ", -1) )
	mgtk_append_menu( optmenu, mgtk_menu_item("Left    ", -1) )
	mgtk_append_menu( optmenu, mgtk_menu_item("Top     ", -1) )
	mgtk_append_menu( optmenu, mgtk_menu_item("Bottom  ", -1) )
	mgtk_append_menu( optmenu, mgtk_menu_item("Material", -1) )
	mgtk_append_menu( optmenu, mgtk_menu_item("UV      ", -1) )
	mgtk_append_menu( optmenu, mgtk_menu_item("Curve   ", -1) )
	mgtk_append_menu( optmenu, mgtk_menu_item("Camera  ", -1) )
	mgtk_box_pack( mgtk_toolbar_box( toolbar2 ), optmenu )

	mgtk_toolbar_togglebutton(toolbar2, "Four Window", -1, 0, "icons/24x24/fourwin.png", "Four window view.")
	mgtk_toolbar_togglebutton(toolbar2, "Grid", -1, 1, "icons/24x24/grid.png", "Grid lines.")
end


function freyja3d_ev_test( )
	print("-- Main screen turn on.")
end


function freyja3d_ui_sidebar_model( sidebar )

	tab = mgtk_tab( sidebar, "Model", 
					mgtk_event("sidebar_model", "freyja3d_ev_test()") )
	handlebox = mgtk_handlebox( 1 )
	vbox = mgtk_vbox( 1, 1 )
	mgtk_box_pack( tab, vbox, 0, 1, 1 )
	mgtk_box_pack( vbox, handlebox )

	-- Transform box
	expander = mgtk_expander( handlebox, "Transform Box", true )
	hbox = mgtk_hbox( 1, 1 )
	mgtk_box_pack( expander, hbox, 0, 1, 1 )
	eventid = mgtk_event("move", "freyja3d_ev_test()")
	mgtk_box_pack( hbox, mgtk_button( "Move", eventid ), 1, 1, 1 )
	mgtk_box_pack( hbox, mgtk_spinbutton_float(-1, 0.0, -900.0, 1000.0, 1, 0.1, 0.1 ) )
	mgtk_box_pack( hbox, mgtk_spinbutton_float(-1, 0.0, -900.0, 1000.0, 1, 0.1, 0.1 ) )
	mgtk_box_pack( hbox, mgtk_spinbutton_float(-1, 0.0, -900.0, 1000.0, 1, 0.1, 0.1 ) )
	hbox = mgtk_hbox()
	mgtk_box_pack( expander, hbox, 0, 1, 1 )
	mgtk_box_pack( hbox, mgtk_button( "Rotate", -1 ), 1, 1, 1 )
	mgtk_box_pack( hbox, mgtk_spinbutton_float(-1, 0.0, -180.0, 180.0, 1, 0.1, 0.1 ) )
	mgtk_box_pack( hbox, mgtk_spinbutton_float(-1, 0.0, -180.0, 180.0, 1, 0.1, 0.1 ) )
	mgtk_box_pack( hbox, mgtk_spinbutton_float(-1, 0.0, -180.0, 180.0, 1, 0.1, 0.1 ) )
	hbox = mgtk_hbox()
	mgtk_box_pack( expander, hbox, 0, 1, 1 )
	mgtk_box_pack( hbox, mgtk_button( "Scale", -1 ), 1, 1, 1 )
	mgtk_box_pack( hbox, mgtk_spinbutton_float(-1, 1.0, -1.0, 1000.0, 1, 0.1, 0.1 ) )
	mgtk_box_pack( hbox, mgtk_spinbutton_float(-1, 1.0, -1.0, 1000.0, 1, 0.1, 0.1 ) )
	mgtk_box_pack( hbox, mgtk_spinbutton_float(-1, 1.0, -1.0, 1000.0, 1, 0.1, 0.1 ) )

end


function freyja3d_ui_init()

	-- Application Window	
	window = 
	mgtk_window( "*test.freyja (/tmp) - freyja", "icons/freyja-dev.png", 64, 64 )
	vbox = mgtk_vbox()
	mgtk_box_pack( window, vbox )
	-- FIXME Currently 'window' is actually a vbox embedded in the window.
	--mgtk_window_move( window, 64, 64 )

	-- Menubar
	freyja3d_ui_menubar( vbox )

	-- Shelf
	shelf = mgtk_notebook( -1 )
	mgtk_box_pack( vbox, shelf )
	freyja3d_ui_shelf_file( shelf )
	freyja3d_ui_shelf_view( shelf )
	tab_create = mgtk_tab(shelf, "Create", -1)
	tab_modify = mgtk_tab(shelf, "Modify", -1)
	tab_skinning = mgtk_tab(shelf, "Skinning", -1)
	tab_material = mgtk_tab(shelf, "Material", -1)

	-- Main UI Box
	hbox = mgtk_hbox()
	mgtk_box_pack( vbox, hbox )

	-- OpenGL Canvas
	canvas = mgtk_opengl_canvas(1280-340, 720)
	mgtk_box_pack( hbox, canvas )

	-- Sidebar
	hbox2 = mgtk_hbox()
	mgtk_box_pack( hbox, hbox2 )
	expander = mgtk_expander( hbox2, " ", true )
	sidebar = mgtk_notebook( -1, 340, 720 )
	mgtk_box_pack( expander, sidebar )
	freyja3d_ui_sidebar_model( sidebar )
	tab = mgtk_tab(sidebar, "UV", -1)
	tab = mgtk_tab(sidebar, "Material", -1)
	tab = mgtk_tab(sidebar, "Plugins", -1)
	
	-- Statusbar
	statusbar1 = mgtk_statusbar()
	mgtk_box_pack( vbox, statusbar1 )
end


freyja3d_ui_init()

