#!/bin/sh
# Setup basic freyja rc/data directory - Mongoose

echo "Installing Freyja user directories and custom data"

mkdir -p ~/.freyja
mkdir -p ~/.freyja/models/
mkdir -p ~/.freyja/models/python/
mkdir -p ~/.freyja/palettes/
mkdir -p ~/.freyja/prefabs/
mkdir -p ~/.freyja/textures/
mkdir -p ~/.freyja/particles/
mkdir -p ~/.freyja/materials/
mkdir -p ~/.freyja/icons/
mkdir -p ~/.freyja/icons/24x24/
mkdir -p ~/.freyja/shaders/
mkdir -p ~/.freyja/plugins/
mkdir -p ~/.freyja/plugins/image/
mkdir -p ~/.freyja/plugins/model/
mkdir -p ~/.freyja/plugins/python/

# Plugins 
#cp ../bin/freyja/debug/plugins/ui/*.so ~/.freyja/plugins/
#cp ../bin/freyja/debug/plugins/ui/*.mlisp ~/.freyja/plugins/
#cp ../bin/plugins/model/debug/*.so ~/.freyja/plugins/model/
#cp ../bin/plugins/image/debug/*.so ~/.freyja/plugins/image/

# Main data
cp ./data/freyja-dev.mlisp ~/.freyja/
cp ./data/shaders/*.frag ~/.freyja/shaders/
cp ./data/shaders/*.vert ~/.freyja/shaders/
cp ./data/palettes/*.pal ~/.freyja/palettes/
cp ./data/materials/*.mat ~/.freyja/materials/
cp ./data/icons/*.png ~/.freyja/icons/
cp ./data/icons/24x24/*.png ~/.freyja/icons/24x24/

# GNOME Setup
#cp ./data/freyja.desktop /usr/share/applications/
#cp ./data/freyja.desktop /usr/share/applnk/Graphics/
#cp ./data/freyja.desktop ~/.gnome2/vfolders/applications/
cp ./data/freyja.desktop ~/.local/share/applications/
cp ./data/icons/freyja.png ~/.icons/

#cp ./data/freyja.desktop ~/Desktop

#printf "Would you like to put Freyja icon on the desktop? [y/N]"
#read DESKTOP_ICON
#
#case "DESKTOP_ICON" in
#	y)
#		cp ./data/freyja.desktop ~/Desktop
#		;;
#	*)
#		;;
#esac
