#!/bin/bash

# Define paths
BINARY="../../bin/Release/3DViewer"
APPDIR="3DViewer.AppDir"
ICON="../../res/icon.png"
RESOURCES="../../res"
IMGUI_CONFIG="../../res/imgui.ini"

# Check if the binary exists
if [ ! -f "$BINARY" ]; then
    echo "Error: File '$BINARY' does not exist."
    exit 1
fi

# Create necessary directories
mkdir -p "$APPDIR/usr/bin"

# Copy the binary to the target directory
cp "$BINARY" "$APPDIR/usr/bin/"
cp "$IMGUI_CONFIG" "$APPDIR/usr/bin/"
# Copy the icon to the target directory
if [ -f "$ICON" ]; then
    cp "$ICON" "$APPDIR/"
else
    echo "Warning: Icon file '$ICON' does not exist. Skipping."
fi

# Copy the resources folder to the target directory
if [ -d "$RESOURCES" ]; then
    cp -r "$RESOURCES" "$APPDIR/usr/bin/"
else
    echo "Warning: Resources folder '$RESOURCES' does not exist. Skipping."
fi

# Build the AppImage
ARCH=x86_64 appimagetool "$APPDIR"