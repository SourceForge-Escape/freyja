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

-- Ideas
-- Event system should allow for lua func / C/C++ callbacks / event ids.
-- Try out vertical tabs for OpenGL view.
-- Widget for icon view selection for textures...
-- Try to get WIN32 mgtk_create_glarea() using gdk_gl_config_new_by_mode(..)
-- Allow setting display() callbacks on each opengl_canvas widget.
-- Abstract and hook up the animation timers for opengl_canvas.
-- Abstract widget size function.
-- Write something to translate pieces of mlisp to lua for testing.


function freyja3d_ui_menubar( vbox )

	menubar = mgtk_menubar()
	mgtk_box_pack( vbox, menubar )

	--handlebox = mgtk_handlebox( 1 )
	--mgtk_box_pack( vbox, handlebox )
	--mgtk_box_pack( handlebox, menubar )

	submenu = mgtk_submenu("_File", -1)
	mgtk_append_menu( menubar, submenu )
	mgtk_append_menu( submenu, mgtk_menu_item("_New", -1, "C-n", "gtk-new") )
	mgtk_append_menu( submenu, mgtk_menu_item("_Open...", -1, "C-o", "gtk-open") )
	mgtk_append_menu( submenu, mgtk_menu_item("Save _As...", -1, "S-C-s", "gtk-save-as") )
	mgtk_append_menu( submenu, mgtk_menu_separator() )
	mgtk_append_menu( submenu, mgtk_menu_item("_Quit", -1, "C-q", "gtk-quit") )
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
	vbox = mgtk_vbox( 1, 1, 0, 1, 1)
	mgtk_box_pack( tab, vbox )
	mgtk_box_pack( vbox, handlebox )

	-- Transform box
	expander = mgtk_expander( handlebox, "Transform Box", true )
	hbox = mgtk_hbox(1, 1, 0, 1, 1)
	mgtk_box_pack( expander, hbox )
	eventid = mgtk_event("move", "freyja3d_ev_test()")
	mgtk_box_pack( hbox, mgtk_button( "Move", eventid ) )
	mgtk_box_pack( hbox, mgtk_spinbutton_float(-1, 0.0, -10000.0, 10000.0, 1) )
	mgtk_box_pack( hbox, mgtk_spinbutton_float(-1, 0.0, -10000.0, 10000.0, 1) )
	mgtk_box_pack( hbox, mgtk_spinbutton_float(-1, 0.0, -10000.0, 10000.0, 1) )
	hbox = mgtk_hbox(1, 1, 0, 1, 1)
	mgtk_box_pack( expander, hbox )
	mgtk_box_pack( hbox, mgtk_button( "Rotate", 1 ) )
	mgtk_box_pack( hbox, mgtk_spinbutton_float(-1, 0.0, -180.0, 180.0, 2) )
	mgtk_box_pack( hbox, mgtk_spinbutton_float(-1, 0.0, -180.0, 180.0, 2) )
	mgtk_box_pack( hbox, mgtk_spinbutton_float(-1, 0.0, -180.0, 180.0, 2) )
	hbox = mgtk_hbox(1, 1, 0, 1, 1)
	mgtk_box_pack( expander, hbox )
	mgtk_box_pack( hbox, mgtk_button( "Scale", -1 ) )
	mgtk_box_pack( hbox, mgtk_spinbutton_float(-1, 1.0, -1.0, 1000.0, 2) )
	mgtk_box_pack( hbox, mgtk_spinbutton_float(-1, 1.0, -1.0, 1000.0, 2) )
	mgtk_box_pack( hbox, mgtk_spinbutton_float(-1, 1.0, -1.0, 1000.0, 2) )

end


function freyja3d_ui_init()

	-- Application Window	
	window = 
	mgtk_window( "*test.freyja (/tmp) - freyja", "icons/freyja-dev.png", 64, 64 )
	vbox = mgtk_vbox( 0, 0, 0, 1, 0 )
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
	hbox = mgtk_hbox( 0, 1, 1, 1, 1 )
	mgtk_box_pack( vbox, hbox )

	-- OpenGL Canvas
	canvas = mgtk_opengl_canvas(1280-340, 720)
	mgtk_box_pack( hbox, canvas )

	-- Sidebar
	hbox2 = mgtk_hbox( 0, 0, 0, 0, 0 )
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

