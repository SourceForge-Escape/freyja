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
mkdir -p ~/.freyja/plugins/

# Plugins ( UI plugins )
cp ../bin/freyja/debug/plugins/*.so  ~/.freyja/plugins/
cp ../bin/freyja/debug/plugins/*.dll ~/.freyja/plugins/
cp plugins/*.mlisp ~/.freyja/plugins/

# Main data
cp ./data/freyja-dev.mlisp ~/.freyja/
cp ./data/palettes/*.pal ~/.freyja/palettes/
cp ./data/materials/*.mat ~/.freyja/materials/
cp ./data/icons/*.png ~/.freyja/icons/
mkdir -p ~/.freyja/icons/24x24/
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
