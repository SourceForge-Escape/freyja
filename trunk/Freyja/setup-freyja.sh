#!/bin/sh
# Setup basic freyja rc/data directory - Mongoose

echo "Installing Freyja user directories and custom data"

mkdir -p ~/.freyja
mkdir -p ~/.freyja/models/
mkdir -p ~/.freyja/palettes/
mkdir -p ~/.freyja/prefabs/
mkdir -p ~/.freyja/textures/
mkdir -p ~/.freyja/particles/
mkdir -p ~/.freyja/materials/
mkdir -p ~/.freyja/icons/

cp ./data/freyja8.mlisp ~/.freyja/
cp ./data/palettes/*.pal ~/.freyja/palettes/
cp ./data/materials/*.mat ~/.freyja/materials/
cp ./data/icons/* ~/.freyja/icons/

# GNOME Setup
#cp ./data/freyja.desktop /usr/share/applications/
cp ./data/freyja.desktop /usr/share/applnk/Graphics/
cp ./data/freyja.desktop ~/.gnome2/vfolders/applications/
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
