
Debian
====================
This directory contains files used to package taonad/taona-qt
for Debian-based Linux systems. If you compile taonad/taona-qt yourself, there are some useful files here.

## taona: URI support ##


taona-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install taona-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your taona-qt binary to `/usr/bin`
and the `../../share/pixmaps/taona128.png` to `/usr/share/pixmaps`

taona-qt.protocol (KDE)

