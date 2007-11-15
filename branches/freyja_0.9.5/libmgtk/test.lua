-- test.lua
--
-- Mongoose:
-- Created, 2007.11.14

print("-- test.lua --")


-- Event system should allow for lua func / C/C++ callbacks / event ids.

function init_ui()
	print("-- start init_ui()")

	window1 = mgtk_window( "(Lua unit test) - mgtk", "../freyja/data/icons/freyja.png" )
	print("window1 = " .. mgtk_is_null( window1 ) )

	--vbox = mgtk_vbox( true, 1 )
	--mgtk_box_pack( window, vbox )

	menubar1 = mgtk_menubar()
	mgtk_box_pack( window1, menubar1 )

	print("--menubar test--")
	submenu1 = mgtk_submenu(-1, "_File")
	--print("submenu1 = " .. mgtk_is_null( submenu1 ) )
	mgtk_append_menu( menubar1, submenu1 )
	item1 = mgtk_menu_item("_New", -1, "C-n", "gtk-new")
	mgtk_append_menu( submenu1, item1 )
	item2 = mgtk_menu_item("_Open...", -1, "C-o", "gtk-open")
	mgtk_append_menu( submenu1, item2 )
	item3 = mgtk_menu_item("Save _As...", -1, "gtk-save-as")
	mgtk_append_menu( submenu1, item3 )

	mgtk_append_menu( submenu1, mgtk_menu_sep() )

	item4 = mgtk_menu_item("_Quit", -1, "C-q", "gtk-quit")
	mgtk_append_menu( submenu1, item4 )

	print("--textview test--")
	textview1 = mgtk_textview( "textview1" )
	--print("textview1 = " .. mgtk_is_null( textview1 ) )
	mgtk_box_pack( window1, textview1 )

	print("--spinbutton test--")
	spnbtnf1 = mgtk_spinbutton_float(-1, 0.0, 0.0, 100.0, 2)
	--print("spnbtnf1 = " .. mgtk_is_null( spnbtnf1 ) )
	mgtk_box_pack( window1, spnbtnf1 )

	print("--label test--")
	label1 = mgtk_label( "label1", 0, 0 )
	--print("label1 = " .. mgtk_is_null( label1 ) )
	mgtk_box_pack( window1, label1 )

	print("--button test--")
	btn1 = mgtk_button( "btn1", -1 )
	mgtk_box_pack( window1, btn1 )

	print("--statusbar test--")
	statusbar1 = mgtk_statusbar()
	mgtk_box_pack( window1, statusbar1 )

	print("-- end init_ui()")
end


init_ui()

