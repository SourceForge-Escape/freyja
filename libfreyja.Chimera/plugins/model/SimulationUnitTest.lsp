;; SimulationUnitTest.el
(menu 2 0
      (submenu "File"              ; Key codes passed to handleKey()
	       (menu_item "Screenshot" EVENT_SCREENSHOT)
	       (menu_item "Quit"       EVENT_QUIT))

      (submenu "Resolution"
	       (menu_item "Fullscreen" EVENT_FULLSCREEN)
	       (menu_item "640x480"    EVENT_RES640x480)
	       (menu_item "800x600"    EVENT_RES800x600)
	       (menu_item "1024x768"   EVENT_RES1024x768)
	       (menu_item "1280x1024"  EVENT_RES1280x1024))

      (submenu "Simulation"              
	       (menu_item "Spring"     115)  ; 's'
	       (menu_item "Gravity"    103)  ; 'g'
	       (menu_item "Start/Stop" 116)  ; 't'
	       (menu_item "Reset"      114)) ; 'r'

      (submenu "Constants"
	       (menu_item "Gravity -"  120)  ; 'x'
	       (menu_item "Gravity +"  121)  ; 'y'
	       (menu_item "Spring C -"  91)  ; '['
	       (menu_item "Spring C +"  93)  ; ']'
	       (menu_item "TimeStep -"  44)  ; ','
	       (menu_item "TimeStep +"  46)) ; '.'
)
