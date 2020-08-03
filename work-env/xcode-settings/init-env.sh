#!/bin/sh
set -ex

OUT=~/Library/Developer/Xcode/UserData/KeyBindings

cp xcode-white.idekeybindings $OUT

cp IDETextKeyBindingSet.plist /Applications/Xcode.app/Contents/Frameworks/IDEKit.framework/Resources/



