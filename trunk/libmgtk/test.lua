--======================================================================
-- 
-- Project : libmgtk
-- Author  : Terry 'Mongoose' Hendrix II
-- Website : http://icculus.org/~mongoose
-- Email   : mongooseichiban@gmail.com
-- Object  : 
-- Comments: This is a unit test script for the libmgtk lua binding.
-- 
---- History ---------------------------------------------------------- 
--
-- 2007.11.14:
-- Mongoose, Created
--======================================================================

print("-- test.lua --")

-- Event system should allow for lua func / C/C++ callbacks / event ids.

function init_ui()
	window1 = mgtk_window( "(Lua unit test) - mgtk", "unit/freyja.png" )
	print("window1 = " .. mgtk_is_null( window1 ) )

	--vbox = mgtk_vbox( true, 1 )
	--mgtk_box_pack( window, vbox )

	menubar1 = mgtk_menubar()
	mgtk_box_pack( window1, menubar1 )

	print("--menubar test--")
	submenu1 = mgtk_submenu("_File", -1)
	--print("submenu1 = " .. mgtk_is_null( submenu1 ) )
	mgtk_append_menu( menubar1, submenu1 )
	item1 = mgtk_menu_item("_New", -1, "C-n", "gtk-new")
	mgtk_append_menu( submenu1, item1 )
	item2 = mgtk_menu_item("_Open...", -1, "C-o", "gtk-open")
	mgtk_append_menu( submenu1, item2 )
	item3 = mgtk_menu_item("Save _As...", -1, "S-C-s", "gtk-save-as")
	mgtk_append_menu( submenu1, item3 )
	mgtk_append_menu( submenu1, mgtk_menu_sep() )
	item4 = mgtk_menu_item("_Quit", -1, "C-q", "gtk-quit")
	mgtk_append_menu( submenu1, item4 )

	print("--toolbar test--")
	toolbar1 = mgtk_toolbar( window1 )
	mgtk_toolbar_button(toolbar1, "New", -1, "gtk-new", "This is a button.")
	mgtk_toolbar_togglebutton(toolbar1, "Save", -1, 0, "gtk-save", "This is a togglebutton.")
	menubtn1 = mgtk_toolbar_menubutton(toolbar1, "Open", -1, "gtk-open", "This is a menubutton.")

	print("-- notebook test --")
	notebook1 = mgtk_notebook(-1)
	mgtk_box_pack( window1, notebook1 )
	tab1 = mgtk_tab(notebook1, "Modify", -1)
	tab2 = mgtk_tab(notebook1, "Create", -1)
	tab3 = mgtk_tab(notebook1, "View", -1)

	print("--tree test--")
	tree1 = mgtk_tree("Name", -1, -1)
	mgtk_box_pack( tab3, tree1 )

	print("--textview test--")
	textview1 = mgtk_textview( "textview1" )
	--print("textview1 = " .. mgtk_is_null( textview1 ) )
	mgtk_box_pack( tab1, textview1 )

	print("--spinbutton test--")
	spnbtnf1 = mgtk_spinbutton_float(-1, 0.0, 0.0, 100.0, 2)
	--print("spnbtnf1 = " .. mgtk_is_null( spnbtnf1 ) )
	mgtk_box_pack( window1, spnbtnf1 )

	print("--label test--")
	label1 = mgtk_label( "label1", 0, 0 )
	--print("label1 = " .. mgtk_is_null( label1 ) )
	mgtk_box_pack( window1, label1 )

	print("--button test--")
	hbox1 = mgtk_hbox()
	mgtk_box_pack( tab2, hbox1 )
	btn1 = mgtk_button( "btn1", -1 )
	mgtk_box_pack( hbox1, btn1 )
	btn2 = mgtk_button( "btn2", -1 )
	mgtk_box_pack( hbox1, btn2 )
	btn3 = mgtk_button( "btn3", -1 )
	mgtk_box_pack( hbox1, btn3 )

	print("--statusbar test--")
	statusbar1 = mgtk_statusbar()
	mgtk_box_pack( window1, statusbar1 )
end


init_ui()

