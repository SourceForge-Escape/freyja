;; MDLModelUnitTest.el
(menu 2 0
      (submenu "Model"                  ; Key codes passed to handleKey()
	       (menu_item "Reset Mesh" 114) ; 'r'
	       (menu_item "Debug"      50)  ; '2'
	       (menu_item "Screenshot" EVENT_SCREENSHOT)
	       (menu_item "Quit"       EVENT_QUIT))

      (submenu "Rendering"                
	       (menu_item "Show Bones" EVENT_SHOWBONES)
	       (menu_item "Wireframe"  EVENT_WIREFRAME)
	       (menu_item "No Faces"   49)  ; '1'
	       (menu_item "Texture"    51)  ; '3' 
	       (menu_item "Points"     52)  ; '4'
	       (menu_item "Rotation"   55)  ; '7'
	       (menu_item "Blending"   56)  ; '8'
	       (menu_item "Lighting"   57)) ; '9'

      (submenu "Resolution"
	       (menu_item "Fullscreen" EVENT_FULLSCREEN)
	       (menu_item "640x480"    EVENT_RES640x480)
	       (menu_item "800x600"    EVENT_RES800x600)
	       (menu_item "1024x768"   EVENT_RES1024x768)
	       (menu_item "1280x1024"  EVENT_RES1280x1024))

      ;(submenu "Light"
	  ;     (menu_item "Bone Snap"  47)  ; '/'
	  ;     (menu_item "Scale up"   61)  ; '='
	  ;     (menu_item "Scale down" 45)) ; '-'

      (submenu "Misc"
	       (menu_item "Bone Snap"  47)  ; '/'
	       (menu_item "Scale up"   61)  ; '='
	       (menu_item "Scale down" 45)) ; '-'
)
