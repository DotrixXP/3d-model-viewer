#!/bin/bash
cp ../bin/Release/3DViewer 3DViewer.AppDir/usr/bin/

if [ ! -f ../bin/Release/3DViewer ]; then
    echo "Error: File '../bin/Release/3DViewer' does not exist."
    exit 1
fi

# Copy the binary to the target directory
cp ../bin/Release/3DViewer 3DViewer.AppDir/usr/bin/

ARCH=x86_64 appimagetool 3DViewer.AppDir
