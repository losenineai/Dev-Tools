#!/bin/sh
set -ex

OUT="~/Library/Application\ Support/Code/User"

# eval 让$OUT里的\转移字符生效
eval cp keybindings.json $OUT
eval cp settings.json $OUT



