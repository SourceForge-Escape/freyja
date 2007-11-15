-- test.lua
--
-- Mongoose:
-- Created, 2007.11.14

print("-- test.lua --")

function init_ui()
	print("-- start init_ui()")

	window1 = mgtk_window( "(Lua unit test) - mgtk", "../freyja/data/icons/freyja.png" )
	print("window1 = " .. mgtk_is_null( window1 ) )

	--vbox = mgtk_vbox( true, 1 )
	--mgtk_box_pack( window, vbox )
	--
	--menubar = mgtk_menubar()
	--mgtk_box_pack( vbox, menubar )
	--mgtk_append_menu( menubar, mgtk_submenu("_Tools", -1) )

	--textview = mgtk_textview( "Hello World." )
	--mgtk_lua_rc_box_pack( window, textview )

	print("--spinbutton test--")
	spnbtnf1 = mgtk_spinbutton_float(-1, 0.0, 0.0, 100.0, 2)
	print("spnbtnf1 = " .. mgtk_is_null( spnbtnf1 ) )
	mgtk_box_pack( window1, spnbtnf1 )

	print("--label test--")
	label1 = mgtk_label( "label: Hello World.", 0, 0 )
	print("label1 = " .. mgtk_is_null( label1 ) )
	mgtk_box_pack( window1, label1 )


	print("-- end init_ui()")
end


init_ui()

