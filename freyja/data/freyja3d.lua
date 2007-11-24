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

	-- FIXME Break up into functions, and move menubar into its own file.
	menubar = mgtk_menubar()
	handlebox = mgtk_handlebox( 1 )
	mgtk_box_pack( vbox, handlebox )
	mgtk_box_pack( handlebox, menubar )

	-- File
	submenu = mgtk_submenu("_File")
	mgtk_append_menu( menubar, submenu )

	mgtk_append_menu( submenu, mgtk_menu_item( "_New", "eNewFile", "C-n", "gtk-new" ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "Ap_pend Model...", "eAppendFile", "gtk-paste" ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "_Open Model...", "eOpenFile", "C-o", "gtk-open" ) )
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
	mgtk_append_menu( submenu, mgtk_menu_item_check( "_Viewports", "eViewports", 0 ) )
	mgtk_append_menu( submenu, mgtk_menu_separator() )
	mgtk_append_menu( submenu, mgtk_menu_item_check( "_Grid", "eRenderGrid", 1 ) )
	submenu2 = mgtk_submenu( "_Mesh" )
	mgtk_append_menu( submenu, submenu2 )
	mgtk_append_menu( submenu2, mgtk_menu_item_check( "_Points", "eRenderVertex", 0 ) )
	mgtk_append_menu( submenu2, mgtk_menu_item_check( "_Wireframe", "eRenderWireframe", 0 ) )
	mgtk_append_menu( submenu2, mgtk_menu_item_check( "_Normals", "eRenderNormals", 0 ) )
	mgtk_append_menu( submenu2, mgtk_menu_item_check( "_Faces", "eRenderFace", 1 ) )
	mgtk_append_menu( submenu2, mgtk_menu_item_check( "_Lighting", "eRenderLighting", 1 ) )
	mgtk_append_menu( submenu2, mgtk_menu_item_check( "_Material", "eRenderMaterial", 1 ) )
	mgtk_append_menu( submenu2, mgtk_menu_item_check( "_Bounding Volume", "eRenderBbox", 1 ) )
	mgtk_append_menu( submenu2, mgtk_menu_item_check( "_Skeleton", "eRenderSkeleton", 1 ) )
	mgtk_append_menu( submenu2, mgtk_menu_item_check( "_Pick Ray", "eRenderPickRay", 1 ) )
	submenu2 = mgtk_submenu( "_Joint" )
	mgtk_append_menu( submenu, submenu2 )
	mgtk_append_menu( submenu2, mgtk_menu_item_check( "_Point", "ePointJoint", 1 ) )
	mgtk_append_menu( submenu2, mgtk_menu_item_check( "_Sphere", "eSphereJoint", 0 ) )
	mgtk_append_menu( submenu2, mgtk_menu_item_check( "_Axes", "eAxisJoint", 0 ) )
	submenu2 = mgtk_submenu( "_Bone" )
	mgtk_append_menu( submenu, submenu2 )
	mgtk_append_menu( submenu2, mgtk_menu_item_check( "_ZBuffer", "eRenderBoneZClear", 0 ) )
	mgtk_append_menu( submenu2, mgtk_menu_item_check( "_Line", "eLineBone", 0 ) )
	mgtk_append_menu( submenu2, mgtk_menu_item_check( "_PolyMesh", "ePolyMeshBone", 1 ) )


	-- Create
	submenu = mgtk_submenu( "_Create" )
	mgtk_append_menu( menubar, submenu )
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
	submenu2 = mgtk_submenu( "Export..." )
	mgtk_append_menu( submenu, submenu2 )
	mgtk_append_menu( submenu2, mgtk_menu_item( "Export Model...", "eNone", "gtk-save-as" ) )
	mgtk_append_menu( submenu2, mgtk_menu_separator() )
	mgtk_append_menu( submenu2, mgtk_menu_item( "Export Weights...", "eNone", "gtk-save-as" ) )
	mgtk_append_menu( submenu2, mgtk_menu_separator() )
	mgtk_append_menu( submenu2, mgtk_menu_item( "Export Track...", "eNone", "gtk-save-as" ) )
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

	tab = mgtk_tab(shelf, "File", -1)
	toolbar = mgtk_toolbar( tab )

	mgtk_toolbar_button(toolbar, "New", "eNewFile", "gtk-new", "New Model.")
	menu_recent_model = 
	mgtk_toolbar_menubutton(toolbar, "Open", "eOpenFile", "gtk-open", "Open Model...", "recent_models_bind" )
	mgtk_toolbar_button(toolbar, "Save", "eSaveFileModel", "gtk-save", "Save Model.")
	mgtk_toolbar_button(toolbar, "Save As", "eSaveModel", "gtk-save-as", "Save Model As...")
	mgtk_toolbar_separator(toolbar)
	menu_recent_lua = 
	mgtk_toolbar_menubutton(toolbar, "Open", "eLoadLuaScript", "icons/24x24/lua.png", "Open Lua Script...", "recent_lua_bind")
	menu_recent_python = 
	mgtk_toolbar_menubutton(toolbar, "Open", "eLoadPythonScript", "icons/24x24/python.png", "Open Python Script...", "recent_python_bind")

	mgtk_toolbar_separator(toolbar)
	eventid = mgtk_event("move", "freyja3d_ev_test()")
	mgtk_toolbar_button(toolbar, "lua_test", eventid, "icons/24x24/lua.png", "Embedded lua script test." )

end


function freyja3d_ui_shelf_view( shelf )
	tab = mgtk_tab(shelf, "View", -1)
	toolbar = mgtk_toolbar( tab )

	-- View mode selection
	box = mgtk_toolbar_box( toolbar )
	optmenu = mgtk_optionmenu( box, "Viewport", "eViewportModeMenu" )
	mgtk_append_menu( optmenu, mgtk_menu_item("Orbit   ", "eViewportOrbit") )
	mgtk_append_menu( optmenu, mgtk_menu_item("Front   ", "eViewportFront") )
	mgtk_append_menu( optmenu, mgtk_menu_item("Back    ", "eViewportBack") )
	mgtk_append_menu( optmenu, mgtk_menu_item("Right   ", "eViewportRight") )
	mgtk_append_menu( optmenu, mgtk_menu_item("Left    ", "eViewportLeft") )
	mgtk_append_menu( optmenu, mgtk_menu_item("Top     ", "eViewportTop") )
	mgtk_append_menu( optmenu, mgtk_menu_item("Bottom  ", "eViewportBottom") )
	mgtk_append_menu( optmenu, mgtk_menu_item("Material", "eViewportMaterial") )
	mgtk_append_menu( optmenu, mgtk_menu_item("UV      ", "eViewportUV") )
	mgtk_append_menu( optmenu, mgtk_menu_item("Curve   ", "eViewportCurve") )
	mgtk_append_menu( optmenu, mgtk_menu_item("Camera  ", "eViewportCamera") )
	mgtk_optionmenu_set_by_id( "eViewportModeMenu", 1 )

	-- Render options
	mgtk_toolbar_togglebutton(toolbar, "Four Window", "eViewports", false, "icons/24x24/fourwin.png", "Four window view" )
	mgtk_toolbar_togglebutton(toolbar, "Grid", "eRenderGrid", false, "icons/24x24/grid.png", "Plane grid rendering" )
	mgtk_toolbar_togglebutton(toolbar, "Ground", "eRenderSolidGround", false, "icons/24x24/grid.png", "Plane solid rendering" )
	mgtk_toolbar_togglebutton(toolbar, "Pick Ray", "eRenderPickRay", false, "icons/24x24/pickray.png", "Pick ray rendering" )
	mgtk_toolbar_separator( toolbar )
	mgtk_toolbar_togglebutton(toolbar, "Points", "eRenderVertex", false, "icons/24x24/point.png", "Point render mode" )
	mgtk_toolbar_togglebutton(toolbar, "Normals", "eRenderNormals", false, "icons/24x24/normals.png", "Normals render mode" )
	mgtk_toolbar_togglebutton(toolbar, "Wireframe", "eRenderWireframe", false, "icons/24x24/wireframe.png", "Wireframe render mode" )
	mgtk_toolbar_togglebutton(toolbar, "Faces", "eRenderFace", false, "icons/24x24/solid.png", "Solid face render mode" )
	mgtk_toolbar_togglebutton(toolbar, "Material", "eRenderMaterial", false, "icons/24x24/texture.png", "Material toggle" )
	mgtk_toolbar_togglebutton(toolbar, "Color", "eGroupColors", false, "icons/24x24/groupcolors.png", "Group color toggle" )
	mgtk_toolbar_togglebutton(toolbar, "Light", "eRenderLighting", false, "icons/24x24/light.png", "Lighting toggle" )
	mgtk_toolbar_togglebutton(toolbar, "BoundingBox", "eRenderBbox", false, "icons/24x24/bvol.png", "Render Bounding boxes" )
	mgtk_toolbar_separator( toolbar )
	mgtk_toolbar_togglebutton(toolbar, "Bones", "eRenderSkeleton", false, "icons/24x24/bone-tag.png", "Show legacy skeleton." )
	mgtk_toolbar_togglebutton(toolbar, "Bones2", "eRenderSkeleton2", false, "icons/24x24/bone-tag.png", "Show bind pose skeleton." )
	mgtk_toolbar_togglebutton(toolbar, "Bones3", "eRenderSkeleton3", false, "icons/24x24/bone-tag.png", "Show transformed skeleton." )
	mgtk_toolbar_togglebutton(toolbar, "BoneName", "eRenderBoneName", false, "icons/24x24/bone-name.png", "Show bind pose bone names." )
	mgtk_toolbar_separator( toolbar )
	mgtk_toolbar_togglebutton(toolbar, "Shadow", "eShadowVolume", false, "icons/24x24/shadow.png", "Render shadows" )
	mgtk_toolbar_togglebutton(toolbar, "Blend", "eSkeletalDeform", false, "icons/24x24/deform.png", "Skeletal vertex blending" )

	-- Zoom
	mgtk_toolbar_separator( toolbar )
	box = mgtk_toolbar_box( toolbar )
	mgtk_box_pack( box, mgtk_label( "Zoom" ) )
	spnbtn = mgtk_spinbutton_float( "eZoom", 1.0, 0.0001, 1000.0, 4, 0.0001, 1.0, 1.0 )
	mgtk_box_pack( box, spnbtn )
end


function freyja3d_ui_shelf_create( shelf )
	tab = mgtk_tab(shelf, "Create", -1)
	toolbar = mgtk_toolbar( tab )
	mgtk_toolbar_button(toolbar, " ", "eGeneratePlane", "icons/24x24/sheet.png", "Polygon plane" )
	mgtk_toolbar_button(toolbar, " ", "eGenerateRing", "icons/24x24/ring.png", "Polygon ring" )
	mgtk_toolbar_button(toolbar, " ", "eGenerateCircle", "icons/24x24/circle.png", "Polygon circle" )
	mgtk_toolbar_button(toolbar, " ", "eGenerateCube", "icons/24x24/cube.png", "Polygon cube" )
	mgtk_toolbar_button(toolbar, " ", "eGenerateTube", "icons/24x24/tube.png", "Polygon tube" )
	mgtk_toolbar_button(toolbar, " ", "eGenerateCone", "icons/24x24/cone.png", "Polygon cone" )
	mgtk_toolbar_button(toolbar, " ", "eGenerateCylinder", "icons/24x24/cylinder.png", "Polygon cylinder" )
	mgtk_toolbar_button(toolbar, " ", "eGenerateSphere", "icons/24x24/sphere.png", "Polygon sphere" )
	mgtk_toolbar_button(toolbar, " ", "eBezierPolygonPatch", "icons/24x24/patch.png", "Patch" )
	mgtk_toolbar_separator( toolbar )
	mgtk_toolbar_button(toolbar, " ", "eExtrude", "icons/24x24/extrude.png", "Extrude face" )
	mgtk_toolbar_separator( toolbar )
	mgtk_toolbar_button(toolbar, "UVMap", "eMeshTexcoordPlaneProj", "icons/24x24/plane.png", "Planar UVMap selected faces"  )
	mgtk_toolbar_button(toolbar, "UVMap", "eMeshTexcoordSpherical", "icons/24x24/texgen-sphere.png",  "Spherical UVMap selected faces"  )
	mgtk_toolbar_button(toolbar, "UVMap", "eMeshTexcoordCylindrical", "icons/24x24/cylinder.png",  "Cylindrical UVMap selected faces" )
end


function freyja3d_ui_shelf_modify( shelf )
	tab = mgtk_tab(shelf, "Modify", -1)
	toolbar = mgtk_toolbar( tab )	

	-- Edit mode selection
	box = mgtk_toolbar_box( toolbar )
	optmenu = mgtk_optionmenu( box, "Edit Mode", "eTransformMenu" )
	mgtk_append_menu( optmenu, mgtk_menu_item("Model      ", "eTransformModel") )
	mgtk_append_menu( optmenu, mgtk_menu_item("Mesh       ", "eTransformMesh") )
	mgtk_append_menu( optmenu, mgtk_menu_item("Meshes...  ", "eTransformMeshes") )
	mgtk_append_menu( optmenu, mgtk_menu_item("Face       ", "eTransformFace") )
	mgtk_append_menu( optmenu, mgtk_menu_item("Faces...   ", "eTransformFaces") )
	mgtk_append_menu( optmenu, mgtk_menu_item("Vertex     ", "eTransformVertex") )
	mgtk_append_menu( optmenu, mgtk_menu_item("Vertices...", "eTransformVertices") )
	mgtk_append_menu( optmenu, mgtk_menu_item("Skeleton   ", "eTransformSkeleton") )
	mgtk_append_menu( optmenu, mgtk_menu_item("Bone       ", "eTransformBone") )
	mgtk_append_menu( optmenu, mgtk_menu_item("Metadata   ", "eTransformMetadata") )
	mgtk_append_menu( optmenu, mgtk_menu_item("Light      ", "eTransformLight") )
	mgtk_append_menu( optmenu, mgtk_menu_item("Camera     ", "eTransformCamera") )

	mgtk_toolbar_togglebutton(toolbar, "SelectB", "eSelectionByBox", false, "icons/24x24/bbox-select.png",  "Select by bounding box, Ctrl+RMouse ends selection" )
	mgtk_toolbar_togglebutton(toolbar, "Info", "eInfoObject", false, "gtk-info", "Info on selected object" )
	mgtk_toolbar_togglebutton(toolbar, "Select", "eSelect", true, "icons/24x24/cursor-select.png", "Select object by cursor" )
	mgtk_toolbar_togglebutton(toolbar, "Move", "eMoveObject", false, "icons/24x24/move.png", "Move object" )
	mgtk_toolbar_togglebutton(toolbar, "Rotate", "eRotateObject", false, "icons/24x24/rotate.png", "Rotate object" )
	mgtk_toolbar_togglebutton(toolbar, "Scale", "eScaleObject", false, "icons/24x24/scale.png", "Scale object" )
	mgtk_toolbar_togglebutton(toolbar, "Paint", "ePaintObject", false, "icons/24x24/paint.png", "Paint object" )

	-- Paint mode selection
	box = mgtk_toolbar_box( toolbar )
	optmenu = mgtk_optionmenu( box, "Paint Mode", -1 )
	mgtk_append_menu( optmenu, mgtk_menu_item( "Weight", "ePaintWeight" ) )
	mgtk_append_menu( optmenu, mgtk_menu_item( "Unweight", "ePaintUnweight" ) )
	mgtk_append_menu( optmenu, mgtk_menu_item( "Select", "ePaintSelect" ) )
	mgtk_append_menu( optmenu, mgtk_menu_item( "Unselect", "ePaintUnselect" ) )
	mgtk_append_menu( optmenu, mgtk_menu_item( "Material", "ePaintMaterial" ) )
	mgtk_append_menu( optmenu, mgtk_menu_item( "Height", "ePaintHeight" ) )
	mgtk_append_menu( optmenu, mgtk_menu_item( "Dmap", "ePaintDmap" ) )

	mgtk_toolbar_separator( toolbar )

	mgtk_toolbar_button(toolbar, "Undo", "eUndo", "gtk-undo", "Undo" )
	mgtk_toolbar_button(toolbar, "Redo", "eRedo", "gtk-redo", "Redo" )
	mgtk_toolbar_separator( toolbar )
	mgtk_toolbar_button(toolbar, "Cut", "eCut", "gtk-cut", "Cut object" )
	mgtk_toolbar_button(toolbar, "Copy", "eCopy", "gtk-copy", "Copy object" )
	mgtk_toolbar_button(toolbar, "Paste", "ePaste", "gtk-paste", "Paste object" )
	mgtk_toolbar_separator( toolbar )
	mgtk_toolbar_button(toolbar, "Create", "eCreate", "gtk-new", "Create new object" )
	mgtk_toolbar_button(toolbar, "Delete", "eDelete", "gtk-delete", "Delete selected object" )
	mgtk_toolbar_separator( toolbar )
	mgtk_toolbar_button(toolbar, "Duplicate", "eDupeObject", "icons/24x24/mdupe.png", "Duplicate object" )
	mgtk_toolbar_button(toolbar, "Split", "eSplitObject", "icons/24x24/msplit.png", "Split object" )
	mgtk_toolbar_button(toolbar, "Merge", "eMergeObject", "icons/24x24/mmerge.png", "Merge objects" )

	mgtk_toolbar_separator( toolbar )

	mgtk_toolbar_button(toolbar, "SubDiv", "eMeshSubDivLoop", "gtk-add", "SubDiv Mesh" )
	--(toolbar_menu_button "icons/24x24/subdiv.png" "Subdiv"	"Subdivide mesh..." eEvent eMeshSubDivLoop
	--mgtk_append_menu( submenu, mgtk_menu_item_check( "Flat", "eNone", 1 ) )
end


function freyja3d_ui_shelf_skinning( shelf )
	tab = mgtk_tab(shelf, "Skinning", -1)
	toolbar = mgtk_toolbar( tab )	

	mgtk_toolbar_button(toolbar, " ", "eAssignWeight", "gtk-add", "Assign vertices to joint " )
	mgtk_toolbar_button(toolbar, " ", "eClearWeight", "gtk-remove", "Remove vertices weighting " )	

	-- Bone Iterator
	mgtk_toolbar_separator( toolbar )
	box = mgtk_toolbar_box( toolbar )
	mgtk_box_pack( box, mgtk_label( "Bone: " ) )
	spnbtn = mgtk_spinbutton_uint( "eBoneIterator", 0, 0, 512 )
	mgtk_box_pack( box, spnbtn )

	-- Weight value
	mgtk_toolbar_separator( toolbar )
	box = mgtk_toolbar_box( toolbar )
	mgtk_box_pack( box, mgtk_label( "Weight: " ) )
	spnbtn = mgtk_spinbutton_float( "eWeight", 1.0, 0.0, 1.0, 3, 0.01, 0.1, 0.1 )
	mgtk_box_pack( box, spnbtn )
end


function freyja3d_ui_shelf_material( shelf )
	tab = mgtk_tab(shelf, "Material", -1)
	toolbar = mgtk_toolbar( tab )	
	mgtk_toolbar_button(toolbar, " ", "eSetMeshTexture", "gtk-add", "Set Mesh Material" )
	mgtk_toolbar_button(toolbar, " ", "eSetFacesMaterial", "gtk-add", "Set Selected Faces Material" )

	-- Material selection by id.
	mgtk_toolbar_separator( toolbar )
	box = mgtk_toolbar_box( toolbar )
	mgtk_box_pack( box, mgtk_label( "Material index: " ) )
	spnbtn = mgtk_spinbutton_uint( "eSetMaterial", 0, 0, 128 )
	mgtk_box_pack( box, spnbtn )
	--;(textbox eSetMaterialName)
end


function freyja3d_ev_test( )
	print("-- Main screen turn on.")
end


function freyja3d_ui_sidebar_model( sidebar )

	tab = mgtk_tab( sidebar, "Model", "eModeModel" )

	-- Transform box
	handlebox = mgtk_handlebox( 1 )
	mgtk_box_pack( tab, handlebox )
	expander = mgtk_expander( handlebox, "Transform Box", true )
	hbox = mgtk_hbox()
	mgtk_box_pack( expander, hbox )
	mgtk_box_pack( hbox, mgtk_button( "Move", "eMove" ), 1, 1, 0 )
	mgtk_box_pack( hbox, mgtk_spinbutton_float("eMove_X", 0.0, -900.0, 1000.0, 1, 0.1, 1, 1 ) )
	mgtk_box_pack( hbox, mgtk_spinbutton_float("eMove_Y", 0.0, -900.0, 1000.0, 1, 0.1, 1, 1 ) )
	mgtk_box_pack( hbox, mgtk_spinbutton_float("eMove_Z", 0.0, -900.0, 1000.0, 1, 0.1, 1, 1 ) )
	hbox = mgtk_hbox()
	mgtk_box_pack( expander, hbox )
	mgtk_box_pack( hbox, mgtk_button( "Rotate", "eRotate" ), 1, 1, 0 )
	mgtk_box_pack( hbox, mgtk_spinbutton_float("eRotate_X", 0.0, -180.0, 180.0, 1, 0.1, 1, 1 ) )
	mgtk_box_pack( hbox, mgtk_spinbutton_float("eRotate_Y", 0.0, -180.0, 180.0, 1, 0.1, 1, 1 ) )
	mgtk_box_pack( hbox, mgtk_spinbutton_float("eRotate_Z", 0.0, -180.0, 180.0, 1, 0.1, 1, 1 ) )
	hbox = mgtk_hbox()
	mgtk_box_pack( expander, hbox )
	mgtk_box_pack( hbox, mgtk_button( "Scale", "eScale" ), 1, 1, 0 )
	mgtk_box_pack( hbox, mgtk_spinbutton_float("eScale_X", 1.0, -1.0, 1000.0, 1, 0.1, 1, 1 ) )
	mgtk_box_pack( hbox, mgtk_spinbutton_float("eScale_Y", 1.0, -1.0, 1000.0, 1, 0.1, 1, 1 ) )
	mgtk_box_pack( hbox, mgtk_spinbutton_float("eScale_Z", 1.0, -1.0, 1000.0, 1, 0.1, 1, 1 ) )

	-- Groups box
	handlebox = mgtk_handlebox( 1 )
	mgtk_box_pack( tab, handlebox )
	expander = mgtk_expander( handlebox, "Groups Box", true )
	mgtk_box_pack( handlebox, expander )

	hbox = mgtk_hbox( 1, 0 )
	mgtk_box_pack( expander, hbox )
	mgtk_box_pack( hbox, mgtk_togglebutton( " 1", "eSmooth", 0 ), 1, 1, 0 )
	mgtk_box_pack( hbox, mgtk_togglebutton( " 2", "eSmooth", 1 ), 1, 1, 0 )
	mgtk_box_pack( hbox, mgtk_togglebutton( " 3", "eSmooth", 2 ), 1, 1, 0 )
	mgtk_box_pack( hbox, mgtk_togglebutton( " 4", "eSmooth", 3 ), 1, 1, 0 )
	mgtk_box_pack( hbox, mgtk_togglebutton( " 5", "eSmooth", 4 ), 1, 1, 0 )
	mgtk_box_pack( hbox, mgtk_togglebutton( " 6", "eSmooth", 5 ), 1, 1, 0 )
	mgtk_box_pack( hbox, mgtk_togglebutton( " 7", "eSmooth", 6 ), 1, 1, 0 )
	mgtk_box_pack( hbox, mgtk_togglebutton( " 8", "eSmooth", 7 ), 1, 1, 0 )

	hbox = mgtk_hbox( 1, 0 )
	mgtk_box_pack( expander, hbox )
	mgtk_box_pack( hbox, mgtk_togglebutton( " 9 ", "eSmooth", 8 ), 1, 0, 0 )
	mgtk_box_pack( hbox, mgtk_togglebutton( "10", "eSmooth", 9 ), 1, 0, 0 )
	mgtk_box_pack( hbox, mgtk_togglebutton( "11", "eSmooth", 10 ), 1, 0, 0 )
	mgtk_box_pack( hbox, mgtk_togglebutton( "12", "eSmooth", 11 ), 1, 0, 0 )
	mgtk_box_pack( hbox, mgtk_togglebutton( "13", "eSmooth", 12 ), 1, 0, 0 )
	mgtk_box_pack( hbox, mgtk_togglebutton( "14", "eSmooth", 13 ), 1, 0, 0 )
	mgtk_box_pack( hbox, mgtk_togglebutton( "15", "eSmooth", 14 ), 1, 0, 0 )
	mgtk_box_pack( hbox, mgtk_togglebutton( "16", "eSmooth", 15 ), 1, 0, 0 )

	hbox = mgtk_hbox( 1, 0 )
	mgtk_box_pack( expander, hbox )
	mgtk_box_pack( hbox, mgtk_togglebutton( "17", "eSmooth", 16 ), 1, 1, 0 )
	mgtk_box_pack( hbox, mgtk_togglebutton( "18", "eSmooth", 17 ), 1, 1, 0 )
	mgtk_box_pack( hbox, mgtk_togglebutton( "19", "eSmooth", 18 ), 1, 1, 0 )
	mgtk_box_pack( hbox, mgtk_togglebutton( "20", "eSmooth", 19 ), 1, 1, 0 )
	mgtk_box_pack( hbox, mgtk_togglebutton( "21", "eSmooth", 20 ), 1, 1, 0 )
	mgtk_box_pack( hbox, mgtk_togglebutton( "22", "eSmooth", 21 ), 1, 1, 0 )
	mgtk_box_pack( hbox, mgtk_togglebutton( "23", "eSmooth", 22 ), 1, 1, 0 )
	mgtk_box_pack( hbox, mgtk_togglebutton( "24", "eSmooth", 23 ), 1, 1, 0 )

	hbox = mgtk_hbox( 1, 1 )
	mgtk_box_pack( expander, hbox )
	mgtk_box_pack( hbox, mgtk_button( "Assign", "eGroupAssign" ), 1, 1, 0 )
	mgtk_box_pack( hbox, mgtk_button( "Clear ", "eGroupClear" ), 1, 1, 0 )

	hbox = mgtk_hbox( 1, 1 )
	mgtk_box_pack( expander, hbox )
	mgtk_box_pack( hbox, mgtk_button( "Smooth", "eSelectedFacesGenerateNormals" ), 1, 1, 0 )
	mgtk_box_pack( hbox, mgtk_button( "Flip    ", "eSelectedFacesFlipNormals" ), 1, 1, 0 )

	-- Scenegraph box
	handlebox = mgtk_handlebox( 1 )
	mgtk_box_pack( tab, handlebox, 1, 1, 0 )
	expander = mgtk_expander( handlebox, "Scenegraph", true )
	mgtk_box_pack( handlebox, expander, 1, 1, 0 )
	mgtk_box_pack( expander, mgtk_tree( "Scenegraph", "eBoneIterator", "eSetBoneName" ), 1, 1, 1 )

	-- Animation control
	hbox = mgtk_hbox( )
	mgtk_box_pack( tab, hbox )
	toolbar = mgtk_toolbar( hbox )
	mgtk_toolbar_togglebutton( toolbar, "Auto", "eModeAnim", false, "gtk-media-record", "Animation mode (Auto Keyframe)" ) 
	mgtk_toolbar_button( toolbar, "Set", "eSetKeyFrame", "icons/24x24/key.png", "Set Keyframe" )
	mgtk_toolbar_button( toolbar, "Prev", "eAnimationPrev", "gtk-media-previous", "Previous" )
	mgtk_toolbar_button( toolbar, "", -1, "gtk-media-rewind", "" )
	mgtk_toolbar_button( toolbar, "Play", "eAnimationPlay", "gtk-media-play", "Play" )
	mgtk_toolbar_button( toolbar, "Stop", "eAnimationStop", "gtk-media-stop", "Stop" )
	mgtk_toolbar_button( toolbar, "", -1, "gtk-media-forward", "" )
	mgtk_toolbar_button( toolbar, "Next", "eAnimationNext", "gtk-media-next", "Next Track" )

end


function freyja3d_ui_sidebar_uv( sidebar )
	tab = mgtk_tab( sidebar, "UV", "eModeUV" )

	-- Material selection by id.
	hbox = mgtk_hbox( )
	mgtk_box_pack( tab, hbox )
	mgtk_box_pack( hbox, mgtk_label( "Material index: " ) )
	spnbtn = mgtk_spinbutton_uint( "eSetMaterial", 0, 0, 128 )
	mgtk_box_pack( hbox, spnbtn )

	-- FIXME (hsep)	
	hbox = mgtk_hbox()
	mgtk_box_pack( tab, hbox )
	mgtk_box_pack( hbox, mgtk_label( " " ) )

	hbox = mgtk_hbox()
	mgtk_box_pack( tab, hbox )
	mgtk_box_pack( hbox, mgtk_label( "Assign material to:" ) )

	hbox = mgtk_hbox()
	mgtk_box_pack( tab, hbox )	
	vbox2 = mgtk_vbox()
	mgtk_box_pack( hbox, vbox2 )
	mgtk_box_pack( vbox2, mgtk_button( "Mesh Index", "eSetMeshTexture" ) )
	mgtk_box_pack( vbox2, mgtk_button( "Face Index", "eSetPolygonTexture" ) )
	mgtk_box_pack( vbox2, mgtk_button( "Selected Faces...", "eSetFacesMaterial" ) )
	vbox2 = mgtk_vbox()
	mgtk_box_pack( hbox, vbox2 )		
	mgtk_box_pack( vbox2, mgtk_spinbutton_uint( "eMeshIterator", 0, 0, 100000 ) )
	mgtk_box_pack( vbox2, mgtk_spinbutton_uint( "ePolygonIterator", 0, 0, 100000 ) )
	mgtk_box_pack( vbox2, mgtk_label( " " ) )

	-- FIXME (hsep)
	hbox = mgtk_hbox()
	mgtk_box_pack( tab, hbox )
	mgtk_box_pack( hbox, mgtk_label( " " ) )
		
	hbox = mgtk_hbox()
	mgtk_box_pack( tab, hbox )
	mgtk_box_pack( hbox, mgtk_label( "Texcoord operations" ) )

	vbox2 = mgtk_vbox()
	mgtk_box_pack( tab, vbox2 )
	mgtk_box_pack( vbox2, mgtk_button( "Texcoord weld mode", "eTexcoordCombine" ) )
	mgtk_box_pack( vbox2, mgtk_button( "Generate UVMap", "eUVMapCreate" ) )
	mgtk_box_pack( vbox2, mgtk_button( "Clear UVMap", "eUVMapDelete" ) )
	mgtk_box_pack( vbox2, mgtk_button( "PolyMap", "ePolyMapTexturePolygon" ) )
	mgtk_box_pack( vbox2, mgtk_button( "Mirror X", "eMirrorUV_X" ) )
	mgtk_box_pack( vbox2, mgtk_button( "Mirror Y", "eMirrorUV_Y" ) )
	mgtk_box_pack( vbox2, mgtk_button( "Set Alpha", "eSetSelectedFacesAlpha" ) )
	mgtk_box_pack( vbox2, mgtk_button( "Clear Alpha", "eClearSelectedFacesAlpha" ) )
end


function freyja3d_ui_sidebar_material_color( box, name )
	hbox = mgtk_hbox()
	mgtk_box_pack( box, hbox )
	mgtk_box_pack( hbox, mgtk_label( name ) )
	hbox = mgtk_hbox()
	mgtk_box_pack( tab, hbox )
	spnbtn = 
	mgtk_spinbutton_float( "eMaterial" .. name .. "0", 0.0, 0.0, 1.0, 3, 0.001, 0.1, 0.1 )
	mgtk_box_pack( hbox, spnbtn )
	spnbtn =
	mgtk_spinbutton_float( "eMaterial" .. name .. "1", 0.0, 0.0, 1.0, 3, 0.001, 0.1, 0.1 )
	mgtk_box_pack( hbox, spnbtn )
	spnbtn =
	mgtk_spinbutton_float( "eMaterial" .. name .. "2", 0.0, 0.0, 1.0, 3, 0.001, 0.1, 0.1 )
	mgtk_box_pack( hbox, spnbtn )
	spnbtn = 
	mgtk_spinbutton_float( "eMaterial" .. name .. "3", 0.0, 0.0, 1.0, 3, 0.001, 0.1, 0.1 )
	mgtk_box_pack( hbox, spnbtn )
	mgtk_box_pack( hbox, mgtk_colorbutton( "eColorMaterial" .. name ) )
end

function freyja3d_ui_sidebar_material( sidebar )
	tab = mgtk_tab( sidebar, "Material", "eModeMaterial" )

	hbox = mgtk_hbox()
	mgtk_box_pack( tab, hbox )
	toolbar = mgtk_toolbar( hbox )
	mgtk_toolbar_button(toolbar, "New", "eNewMaterial", "gtk-new", "New material" )
	mgtk_toolbar_menubutton(toolbar, "Open Material", "eOpenMaterial", "gtk-open", "Open Material...", "recent_material_bind" )
	mgtk_toolbar_togglebutton(toolbar, "Lock", "eMaterialSlotLoad", false, "gtk-apply", "Overwrite material on load" )
	mgtk_toolbar_button(toolbar, "Save as...", "eSaveMaterial", "gtk-save-as", "Save Material..." )

	-- Material selection by id.
	hbox = mgtk_hbox( )
	mgtk_box_pack( tab, hbox )
	mgtk_box_pack( hbox, mgtk_label( "Material Select: " ) )
	hbox = mgtk_hbox( )
	mgtk_box_pack( tab, hbox )
	spnbtn = mgtk_spinbutton_uint( "eSetMaterial", 0, 0, 128 )
	mgtk_box_pack( hbox, spnbtn )
	mgtk_box_pack( hbox, mgtk_textbox( "eSetMaterialName" ), 1, 1, 0 )

	hbox = mgtk_hbox()
	mgtk_box_pack( tab, hbox )
	toolbar = mgtk_toolbar( hbox )
	mgtk_toolbar_togglebutton(toolbar, "Texture", "eEnableMaterialTexture", false, "gtk-add", "Enable texture mapping" )
	mgtk_toolbar_togglebutton(toolbar, "Blend", "eEnableMaterialBlending", false, "gtk-add", "Enable blending" )
	mgtk_toolbar_togglebutton(toolbar, "Fragment", "eEnableMaterialFragment", false, "gtk-add", "Enable fragment" )
	mgtk_toolbar_separator( toolbar )
	mgtk_toolbar_togglebutton(toolbar, "GLSL Fragment", "eGLSLFragmentMode", false, "gtk-add", "GLSL fragment mode" )
	mgtk_toolbar_togglebutton(toolbar, "ARB Fragment", "eARBFragmentMode", false, "gtk-add", "ARB fragment mode" )
	mgtk_toolbar_separator( toolbar )
	mgtk_toolbar_togglebutton(toolbar, "Detail", "eEnableDetailTexture", false, "gtk-add", "Enable detail texture" )
	mgtk_toolbar_togglebutton(toolbar, "Normalize", "eEnableNormalize", false, "gtk-add", "Enable normalization for render scaling lighting" )

	-- FIXME (hbox 1 0 0 1 0 (hsep))

	hbox = mgtk_hbox()
	mgtk_box_pack( tab, hbox )
	mgtk_box_pack( hbox, mgtk_label( "Texture" ) )

	hbox = mgtk_hbox()
	mgtk_box_pack( tab, hbox )
	mgtk_box_pack( hbox, mgtk_spinbutton_uint( "eSetMaterialTexture", 0, 0, 128 ) )
	mgtk_box_pack( hbox, mgtk_textbox( "eSetTextureNameA" ), 1, 1, 0 )
	mgtk_box_pack( hbox, mgtk_togglebutton( "Slot", "eTextureSlotLoad" ) )
	mgtk_box_pack( hbox, mgtk_filechooserbutton( "...", "Open Texture...", "eOpenTexture", "texture_pattern" ) )
				
	hbox = mgtk_hbox()
	mgtk_box_pack( tab, hbox )
	mgtk_box_pack( hbox, mgtk_label( "Fragment" ) )

	hbox = mgtk_hbox()
	mgtk_box_pack( tab, hbox )
	mgtk_box_pack( hbox, mgtk_spinbutton_uint( "eSetMaterialShader", 0, 0, 128 ) )
	mgtk_box_pack( hbox, mgtk_textbox( "eSetMaterialShaderFilename" ), 1, 1, 0 )
	mgtk_box_pack( hbox, mgtk_togglebutton( "Slot", "eShaderSlotLoad" ) )
	mgtk_box_pack( hbox, mgtk_filechooserbutton( "...", "Open Fragment program...", "eOpenShader", "shader_pattern") )
	

	-- FIXME (hbox 1 0 0 1 0 (hsep))

	-- Material colors
	freyja3d_ui_sidebar_material_color( tab, "Ambient" )
	freyja3d_ui_sidebar_material_color( tab, "Diffuse" )
	freyja3d_ui_sidebar_material_color( tab, "Specular" )
	freyja3d_ui_sidebar_material_color( tab, "Emissive" )

	hbox = mgtk_hbox()
	mgtk_box_pack( tab, hbox )
	mgtk_box_pack( hbox, mgtk_label( "Shininess" ) )
	spnbtn = 
	mgtk_spinbutton_float( "eMaterialShine", 1.0, 0.0, 100.0, 3, 0.01, 0.1, 0.1 )
	mgtk_box_pack( hbox, spnbtn )

	hbox = mgtk_hbox()
	mgtk_box_pack( tab, hbox )
	mgtk_box_pack( hbox, mgtk_label( "Blend Source" ) )
	submenu = mgtk_optionmenu( hbox, "Blend Source", "eBlendSrcMenu" )
	mgtk_append_menu( submenu, mgtk_menu_item( "GL__ZERO", "eBlendSrc", 0 ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "GL__ONE", "eBlendSrc", 1 ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "GL__SRC__COLOR", "eBlendSrc", 2 ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "GL__ONE__MINUS__SRC__COLOR", "eBlendSrc", 3 ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "GL__DST__COLOR", "eBlendSrc", 4 ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "GL__ONE__MINUS__DST__COLOR", "eBlendSrc", 5 ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "GL__SRC__ALPHA", "eBlendSrc", 6 ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "GL__ONE__MINUS__SRC__ALPHA", "eBlendSrc", 7 ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "GL__DST__ALPHA", "eBlendSrc", 8 ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "GL__ONE__MINUS__DST__ALPHA", "eBlendSrc", 9 ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "GL__SRC__ALPHA__SATURATE", "eBlendSrc", 10 ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "GL__CONSTANT__COLOR", "eBlendSrc", 11 ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "GL__ONE__MINUS__CONSTANT__COLOR", "eBlendSrc", 12 ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "GL__CONSTANT__ALPHA", "eBlendSrc", 13 ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "GL__ONE__MINUS__CONSTANT__ALPHA", "eBlendSrc", 14 ) )

	hbox = mgtk_hbox()
	mgtk_box_pack( tab, hbox )
	mgtk_box_pack( hbox, mgtk_label( "Blend Dest  " ) )
	submenu = mgtk_optionmenu( hbox, "Blend Dest", "eBlendDestMenu" )
	mgtk_append_menu( submenu, mgtk_menu_item( "GL__ZERO", "eBlendDest", 0 ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "GL__ONE", "eBlendDest", 1 ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "GL__SRC__COLOR", "eBlendDest", 2 ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "GL__ONE__MINUS__SRC__COLOR", "eBlendDest", 3 ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "GL__DST__COLOR", "eBlendDest", 4 ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "GL__ONE__MINUS__DST__COLOR", "eBlendDest", 5 ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "GL__SRC__ALPHA", "eBlendDest", 6 ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "GL__ONE__MINUS__SRC__ALPHA", "eBlendDest", 7 ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "GL__DST__ALPHA", "eBlendDest", 8 ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "GL__ONE__MINUS__DST__ALPHA", "eBlendDest", 9 ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "GL__SRC__ALPHA__SATURATE", "eBlendDest", 10 ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "GL__CONSTANT__COLOR", "eBlendDest", 11 ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "GL__ONE__MINUS__CONSTANT__COLOR", "eBlendDest", 12 ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "GL__CONSTANT__ALPHA", "eBlendDest", 13 ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "GL__ONE__MINUS__CONSTANT__ALPHA", "eBlendDest", 14 ) )
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
	freyja3d_ui_shelf_create( shelf )
	freyja3d_ui_shelf_modify( shelf )
	freyja3d_ui_shelf_skinning( shelf )
	freyja3d_ui_shelf_material( shelf )

	-- Main UI Box
	hbox = mgtk_hbox( )
	mgtk_box_pack( vbox, hbox )

	-- OpenGL Canvas
	canvas = mgtk_opengl_canvas( 1280 - 340, 720 )
	mgtk_box_pack( hbox, canvas )

	-- Sidebar
	hbox2 = mgtk_hbox()
	mgtk_box_pack( hbox, hbox2, 0, 0, 0 )
	--mgtk_box_pack( hbox2, mgtk_button( ">", -1 ) )	
	expander = mgtk_expander( hbox2, " ", true )
	sidebar = mgtk_notebook( -1, 340, 720 )
	mgtk_box_pack( expander, sidebar )
	freyja3d_ui_sidebar_model( sidebar )
	freyja3d_ui_sidebar_uv( sidebar )
	freyja3d_ui_sidebar_material( sidebar )
	tab = mgtk_tab( sidebar, "Plugins", -1 )
	mgtk_expander( tab, "Plugin Dock", true, "FirstPartyPluginSlot" )
	--mgtk_expander( tab, "Community Plugins", true, "ThirdPartyPluginSlot" )

	-- Animation scrubber
	mgtk_box_pack( vbox, mgtk_hslider( "eAnimationSlider", 0, 500 ) )

	-- Statusbar
	statusbar1 = mgtk_statusbar()
	mgtk_box_pack( vbox, statusbar1 )
end


function freyja3d_ui_dialogs()

	-- Confirmation dialogs
	mgtk_confirmation_dialog( "CloseNewFileDialog",
		"gtk-dialog-question",
		"Creating a new model will close the current model.",
		"Would you like to close the model and possibly lose unsaved changes?",
		"gtk-cancel", "_Cancel", "gtk-close", "C_lose" )

	mgtk_confirmation_dialog( "CloseToOpenFileDialog",
		"gtk-dialog-question",
		"You must close the current model to open a new one.",
		"Open the new model and lose unsaved changes?",
		"gtk-cancel", "_Cancel", "gtk-open", "_Open")

	mgtk_confirmation_dialog( "RevertFileDialog",
		"gtk-dialog-question",
		"You will lose all changes you made by reverting.",
		"Would you like to revert the model and lose unsaved changes?",
		"gtk-cancel",	"_Cancel",	"gtk-revert",	"_Revert")

	mgtk_confirmation_dialog( "OverwriteFileDialog",
		"gtk-dialog-question",
		"You are about to overwrite a file.",
		"Are you sure you want to overwrite the file?",
		"gtk-cancel",	"_Cancel",	"gtk-ok",	"_Overwrite")

	mgtk_confirmation_dialog( "PolyMapDialog",
		"gtk-dialog-question",
		"You just generated a UV map using vertex UVs." ,
		-- "Some model formats perfer polygon-mapped UVs, and may not export without being polymapped.",
		"Would you like to promote it to ploymapped texcoords instead?",
		"gtk-cancel",	"_Cancel",	"gtk-ok",	"_Promote")

	mgtk_confirmation_dialog( "DupeMeshDialog",
		"gtk-dialog-question",
		"You are about to duplicate the selected mesh.",
		"Would you like to continue?",
		"gtk-cancel",	"_Cancel",	"gtk-ok",	"_Duplicate")

	mgtk_confirmation_dialog( "DupeObjectDialog",
		"gtk-dialog-question",
		"You are about to duplicate the selected object.",
		"Would you like to continue?",
		"gtk-cancel",	"_Cancel",	"gtk-ok",	"_Duplicate")

	mgtk_confirmation_dialog( "SplitMeshDialog",
		"gtk-dialog-question",
		"You are about to split the selected faces \n from the currently selected mesh into a new mesh.\n",
		"Would you like to continue?",
		"gtk-cancel",	"_Cancel",	"gtk-ok",	"_Split")

	mgtk_confirmation_dialog( "MergeObjDialog",
		"gtk-dialog-question",
		"You are about to merge two objects.\n",
		"Would you like to continue the merge?",
		"gtk-cancel",	"_Cancel",	"gtk-ok",	"_Merge")

	mgtk_confirmation_dialog( "MergeDelObjDialog",
		"gtk-dialog-question",
		"You have the option to delete the mesh being merged.\n",
		"Would you like to delete or keep the merged mesh?",
		"gtk-cancel",	"_Keep",	"gtk-ok",	"_Delete")

	mgtk_confirmation_dialog( "SplitMeshCullDialog",
		"gtk-dialog-question",
		"You have the option to cull or duplicate the selected faces\n from the current mesh.\n",
		"Would you like to duplicate or cull the selected faces?",
		"gtk-cancel",	"_Cull",	"gtk-ok",	"_Duplicate")

	mgtk_confirmation_dialog( "DelVertDialog",
		"gtk-dialog-question",
		"You are about to delete the selected vertices.",
		"Are you sure you want to delete these vertices?",
		"gtk-cancel",	"_Cancel",	"gtk-ok",	"_Delete")

	mgtk_confirmation_dialog( "DelBoneDialog",
		"gtk-dialog-question",
		"You are about to delete the selected joint/bone.",
		"Are you sure you want to delete this bone?",
		"gtk-cancel",	"_Cancel",	"gtk-ok",	"_Delete")

	mgtk_confirmation_dialog( "CloseNewFileDialog",
		"gtk-dialog-question",
		"Creating a new model will close the current model.",
		"Would you like to close the model and possibly lose unsaved changes?",
		"gtk-cancel",	"_Cancel",	"gtk-close",	"C_lose")

	mgtk_confirmation_dialog( "ExitWarningDialog",
		"gtk-dialog-question",
		"If you exit you will lose any unsaved data.",
		"Would you still like to exit?",
		"gtk-cancel",	"_Cancel",	"gtk-quit",	"_Exit")


	-- Query Dialogs 
	mgtk_query_dialog( "MetadataPropertyDialog",
		"gtk-dialog-question",
		"Metadata property editor.",
		"textentry",	"name",		"Name    ",		" ",
		"textentry",	"type",		"Type    ",		" ",
		"textarea",		"metadata",	"Metadata",		" ",
		"textentry",	"model",	"Model   ",		" ",
		"textentry",	"material",	"Material",		" ",
		"gtk-cancel", "_Cancel", "gtk-ok", "_Update")

	mgtk_query_dialog( "GenerateSheetDialog",
		"gtk-dialog-question",
		"Sheet mesh generation.",
		"int", "rows", "How many rows?   ",	1, 1, 64,  -- default min max
		"int", "cols", "How many columns?",	1, 1, 64,  -- default min max
		"float", "size", "How large?       ", 8, 1, 128, -- default min max
		"gtk-cancel", "_Cancel", "gtk-ok", "_Create")

	mgtk_query_dialog( "GenerateCubeDialog",
		"gtk-dialog-question",
		"Cube mesh generation.",
		"int", "rows", "How many rows?   ",	1, 1, 64,  -- default min max
		"int", "cols", "How many columns?",	1, 1, 64,  -- default min max
		"float", "size", "How large?       ", 8, 1, 64,  -- default min max
		"gtk-cancel", "_Cancel", "gtk-ok", "_Create")

	mgtk_query_dialog( "GenerateRingDialog",
		"gtk-dialog-question",
		"Ring mesh generation.",
		"int",	"rings",	"How many rings?   ",	1, 1, 64,  -- default min max
		"int",	"count",	"How many sections?",	16, 4, 64,  -- default min max
		"float",	"radius",	"Radius?",	8, 1, 64,  -- default min max
		"gtk-cancel",	"_Cancel",	"gtk-ok",	"_Create")

	mgtk_query_dialog( "GenerateCircleDialog",
		"gtk-dialog-question",
		"Circle mesh generation.",
		"int",	"count",	"How many sections?",	16, 4, 64,  -- default min max
		"float",	"radius",	"Radius?",	8, 0.5, 64,  -- default min max
		"gtk-cancel",	"_Cancel",	"gtk-ok",	"_Create")

	mgtk_query_dialog( "GenerateTubeDialog",
		"gtk-dialog-question",
		"Tube mesh generation.",
		"int",	"segments",	"How many segments?",	4, 1, 64,  -- default min max
		"int",	"count",	"Vertex count per ring?",	16, 4, 64,  -- default min max
		"float", "radius",	"Radius?",	8, 0.1, 128,  -- default min max
		"float",	"height",	"Height?",	16, 0.1, 128,  -- default min max
		"gtk-cancel",	"_Cancel",	"gtk-ok",	"_Create")

	mgtk_query_dialog( "GenerateSphereDialog",
		"gtk-dialog-question",
		"Sphere mesh generation.",
		"int",	"count",	"Vertex count per ring?",	16, 4, 64,  -- default min max
		"float",	"radius",	"Radius?",	8, 0.1, 128,  -- default min max
		"gtk-cancel",	"_Cancel",	"gtk-ok",	"_Create")

	mgtk_query_dialog( "DuplicateBoneDialog",
		"gtk-dialog-question",
		"Would you like to duplicate the selected bone?",
		"int", "recurse", "Recursively duplicate children?", 0, 0, 1,  -- default min max
		"gtk-cancel", "_Cancel", "gtk-ok", "_Duplicate")

	-- Macro editor dialog
	dialog = mgtk_dialog( "Macro Editor freyja", "eMacroDialog", 0 )
	vbox = mgtk_vbox( )
	mgtk_box_pack( dialog, vbox )
	menubar = mgtk_menubar()
	mgtk_box_pack( vbox, menubar )
	submenu = mgtk_submenu("_File")
	mgtk_append_menu( menubar, submenu )
	mgtk_append_menu( submenu, mgtk_menu_item( "_New", "eMacroNew", "gtk-new" ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "_Open", "eMacroOpen", "gtk-open" ) )
	mgtk_append_menu( submenu, mgtk_menu_separator() )
	mgtk_append_menu( submenu, mgtk_menu_item( "_Save", "eMacroSave", "gtk-save" ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "Save _As...", "eMacroSaveAs", "gtk-save-as" ) )
	mgtk_append_menu( submenu, mgtk_menu_item( "_Revert", "eMacroRevert", "gtk-revert-to-saved" ) )
	mgtk_append_menu( submenu, mgtk_menu_separator() )
	mgtk_append_menu( submenu, mgtk_menu_item( "_Close", "eMacroClose", "C-w", "gtk-close" ) )


	-- Help dialog
	dialog = mgtk_dialog( "Help freyja", "eHelpDialog", 0 )
	vbox = mgtk_vbox( )
	mgtk_box_pack( dialog, vbox )
	mgtk_box_pack( vbox, mgtk_label( "If you'd like to work on documentation email me.\nHelp, bug reports, and feature requests:\n  · http://icculus.org/freyja\n  · http://icculus.org/freyja/forum.html\n  · irc://irc.freenode.net/#freyja\n  · mongooseichiban@gmail.com" ) )
	mgtk_box_pack( vbox, mgtk_label( "  <u>Common keys</u>\n\n\t· F4 - Extrude\t· F5 - Move\t· Ctrl  (held) - tSelect\n\t· F6 - Rotate\t· Shift (held) - tUnSelect\n\t· F7 - Scale\n" ) )
	mgtk_box_pack( vbox, mgtk_label( "The last viewport you click in becomes your main window" ) ) 

	-- About dialog
	dialog = mgtk_dialog( "About freyja", "eAboutDialog", 0 )
	vbox = mgtk_vbox( )
	mgtk_box_pack( dialog, vbox )
	hbox = mgtk_hbox( )
	mgtk_box_pack( vbox, hbox )
	mgtk_box_pack( hbox, mgtk_icon( "freyja.png", "IconSize_Dialog" ) )
	mgtk_box_pack( hbox, mgtk_label(" <big>Freyja 0.9.5 Development Branch</big>\nFreyja is an Open Source 3d modeling system.\nThis build is from the development branch.\n\n\t· mongooseichiban@gmail.com ·\n<small>Freyja Copyright © 2002-2006 by Terry 'Mongoose' Hendrix II </small>" ) )

	-- Preferences dialog
	-- dialog = mgtk_dialog( "趣味 freyja", "ePreferencesDialog", 0 )
	dialog = mgtk_dialog( "Preferences freyja", "ePreferencesDialog", 0 )
	vbox = mgtk_vbox( )
	mgtk_box_pack( dialog, vbox )
	notebook = mgtk_notebook( -1 )
	mgtk_box_pack( vbox, notebook )

	-- General
	tab = mgtk_tab( notebook, "General", -1 )

	-- (spinbutton2 20.0 20.0 60.0 20.0 20.0 20.0 3 eSetNearHeight)
	--hbox = mgtk_box()
	--mgtk_box_pack( tab, hbox )
	--mgtk_box_pack( hbox, mgtk_label("Near Height ") )
	--mgtk_box_pack( hbox, mgtk_spinbutton_float("eSetNearHeight", 20.0, 20.0, 60.0 ) )

	-- (spinbutton2 0.1 0.0001 1000.0 0.0001 0.001 0.001 4 eZoom)
	hbox = mgtk_hbox()
	mgtk_box_pack( tab, hbox )
	mgtk_box_pack( hbox, mgtk_label("Render scaling") )
	mgtk_box_pack( hbox, mgtk_spinbutton_float( "eZoom", 1.0, 0.0001, 1000.0, 4 ) )

	hbox = mgtk_hbox()
	mgtk_box_pack( tab, hbox )
	mgtk_box_pack( hbox, mgtk_label("Polygon # of sides") )
	mgtk_box_pack( hbox, mgtk_spinbutton_uint( "ePolygonSize", 3, 3, 6 ) )

	hbox = mgtk_hbox()
	mgtk_box_pack( tab, hbox )
	mgtk_box_pack( hbox, mgtk_label("UV texcoord pick radius") )
	mgtk_box_pack( hbox, mgtk_spinbutton_float( "eUVPickRadius", 0.1, 0.0001, 1000.0, 4 ) )

	hbox = mgtk_hbox()
	mgtk_box_pack( tab, hbox )
	mgtk_box_pack( hbox, mgtk_label("Vertex pick radius") )
	mgtk_box_pack( hbox, mgtk_spinbutton_float( "eVertexPickRadius", 0.1, 0.0001, 1000.0, 4 ) )

	hbox = mgtk_hbox()
	mgtk_box_pack( tab, hbox )
	mgtk_box_pack( hbox, mgtk_label("Vertex snap-weld dist") )
	mgtk_box_pack( hbox, mgtk_spinbutton_float( "eSnapWeldVertsDist", 0.1, 0.0001, 1000.0, 4 ) )

	-- Colors
	tab = mgtk_tab( notebook, "Colors", -1 )

	hbox = mgtk_hbox()
	mgtk_box_pack( tab, hbox )
	mgtk_box_pack( hbox, mgtk_colorbutton( "eColorBackground" ) )
	mgtk_box_pack( hbox, mgtk_label( "Background color" ) ) -- 遠景の呈色

	hbox = mgtk_hbox()
	mgtk_box_pack( tab, hbox )
	mgtk_box_pack( hbox, mgtk_colorbutton( "eColorGrid" ) )
	mgtk_box_pack( hbox, mgtk_label( "Grid color" ) ) -- 基底の呈色

	hbox = mgtk_hbox()
	mgtk_box_pack( tab, hbox )
	mgtk_box_pack( hbox, mgtk_colorbutton( "eColorVertex" ) )
	mgtk_box_pack( hbox, mgtk_label( "Vertex color" ) ) -- 天頂の呈色

	hbox = mgtk_hbox()
	mgtk_box_pack( tab, hbox )
	mgtk_box_pack( hbox, mgtk_colorbutton( "eColorVertexHighlight" ) )
	mgtk_box_pack( hbox, mgtk_label( "Vertex highlight" ) ) -- 現行天頂の呈色

	hbox = mgtk_hbox()
	mgtk_box_pack( tab, hbox )
	mgtk_box_pack( hbox, mgtk_colorbutton( "eColorMesh" ) )
	mgtk_box_pack( hbox, mgtk_label( "Wireframe color" ) ) -- Meshの呈色

	hbox = mgtk_hbox()
	mgtk_box_pack( tab, hbox )
	mgtk_box_pack( hbox, mgtk_colorbutton( "eColorMeshHighlight" ) )
	mgtk_box_pack( hbox, mgtk_label( "Wireframe highlight" ) ) -- 現行Meshの呈色

	hbox = mgtk_hbox()
	mgtk_box_pack( tab, hbox )
	mgtk_box_pack( hbox, mgtk_colorbutton( "eColorBone" ) )
	mgtk_box_pack( hbox, mgtk_label( "Bone color" ) ) -- 骨の呈色

	hbox = mgtk_hbox()
	mgtk_box_pack( tab, hbox )
	mgtk_box_pack( hbox, mgtk_colorbutton( "eColorBoneHighlight" ) )
	mgtk_box_pack( hbox, mgtk_label( "Bone highlight" ) ) -- 現行骨の呈色

	hbox = mgtk_hbox()
	mgtk_box_pack( tab, hbox )
	mgtk_box_pack( hbox, mgtk_colorbutton( "eColorJoint" ) )
	mgtk_box_pack( hbox, mgtk_label( "Joint color" ) ) -- Jointの呈色

	hbox = mgtk_hbox()
	mgtk_box_pack( tab, hbox )
	mgtk_box_pack( hbox, mgtk_colorbutton( "eColorJointHighlight" ) )
	mgtk_box_pack( hbox, mgtk_label( "Joint highlight" ) ) -- 現行Jointの呈色

	mgtk_box_pack( tab, mgtk_button( "Reset to defaults", "eResetColorsToDefault" ) )

	-- Lights
	tab = mgtk_tab( notebook, "Lights", -1 )	
	hbox = mgtk_hbox()
	mgtk_box_pack( tab, hbox )
	mgtk_box_pack( hbox, mgtk_label( "Light" ) )
	mgtk_box_pack( hbox, mgtk_label( "Ambient" ) )
	mgtk_box_pack( hbox, mgtk_label( "Diffuse" ) )
	mgtk_box_pack( hbox, mgtk_label( "Specular" ) )
	hbox = mgtk_hbox()
	mgtk_box_pack( tab, hbox )
	mgtk_box_pack( hbox, mgtk_spinbutton_uint( "eSelectLight", 0, 0, 100 ) )
	mgtk_box_pack( hbox, mgtk_colorbutton( "eColorLightAmbient" ) )
	mgtk_box_pack( hbox, mgtk_colorbutton( "eColorLightDiffuse" ) )
	mgtk_box_pack( hbox, mgtk_colorbutton( "eColorLightSpecular" ) )
	hbox = mgtk_hbox()
	mgtk_box_pack( tab, hbox )
	mgtk_box_pack( hbox, mgtk_spinbutton_float( "eLightPosX", 0.0, 10000.0, 10000.0 ) )
	mgtk_box_pack( hbox, mgtk_spinbutton_float( "eLightPosY", 0.0, 10000.0, 10000.0 ) )
	mgtk_box_pack( hbox, mgtk_spinbutton_float( "eLightPosZ", 0.0, 10000.0, 10000.0 ) )

	-- Cameras
	tab = mgtk_tab( notebook, "Cameras", -1 )
end


function freyja3d_ui_defaults()
	--Custom integer values.
	--mgtk_integer_set( "ASSERT_BREAKPOINT", 1 )

	-- Custom string values.
	mgtk_string_set( "FONT", "vera.ttf" )

	-- Custom color values.
	mgtk_color_set( "eColorBackground", 0.5, 0.5, 0.5, 1.0 ) 
	mgtk_color_set( "eColorGrid", 0.4, 0.4, 0.4, 0.9 )
	mgtk_color_set( "eColorMesh", 0.0, 0.0, 0.0, 1.0 )
	mgtk_color_set( "eColorMeshHighlight", 0.2, 0.2, 1.0, 1.0 )
	mgtk_color_set( "eColorVertex", 0.2, 1.0, 1.0, 1.0 )
	mgtk_color_set( "eColorVertexHighlight", 1.0, 0.0, 0.86, 1.0 )
	mgtk_color_set( "eColorBone", 0.882352941, 0.654901961, 0.219607843, 1.0 )
	mgtk_color_set( "eColorBoneHighlight", 0.956862745, 0.415686275, 0.2, 1.0 )
	mgtk_color_set( "eColorJoint", 1.0, 1.0, 0.0, 1.0 )
	mgtk_color_set( "eColorJointHighlight",	1.0, 0.75, 0.75, 1.0 )
	mgtk_color_set( "eColorLightAmbient", 0.8, 0.8, 0.8, 1.0 )
	mgtk_color_set( "eColorLightDiffuse", 0.8, 0.8, 0.8, 1.0 )
	mgtk_color_set( "eColorLightSpecular", 0.8, 0.8, 0.8, 1.0 )

	-- Custom boolean values.
	mgtk_boolean_set( "eSelect", 1 )
	mgtk_boolean_set( "eRenderBoneZClear", 1 )
	mgtk_boolean_set( "eRenderGrid", 1 )
	mgtk_boolean_set( "eRenderFace", 1 )
	mgtk_boolean_set( "eRenderMaterial", 1 )
	mgtk_boolean_set( "eRenderLighting", 1 )
	mgtk_boolean_set( "eRenderMaterial", 1 )
	mgtk_boolean_set( "eRenderBbox", 1 )
	mgtk_boolean_set( "eRenderSkeleton", 1 )
	mgtk_boolean_set( "eRenderPickRay", 0 )
	mgtk_boolean_set( "ePointJoint", 1 )
	mgtk_boolean_set( "ePolyMeshBone", 1 )
	mgtk_boolean_set( "eGLSLFragmentMode", 1 )
	mgtk_boolean_set( "eGroupColors", 1 )
	mgtk_boolean_set( "eFPSCap", 1 )
end



-- Generate the user interface.
freyja3d_ui_init()
freyja3d_ui_dialogs()

-- Call this after widgets are loaded so widgets can be updated. 
freyja3d_ui_defaults()



