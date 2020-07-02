#!/bin/sh
set -ex

sh -c "$(curl -fsSL https://raw.github.com/robbyrussell/oh-my-zsh/master/tools/install.sh)"
cp .zshrc ~/
cp .vimrc ~/
cp -R bin ~/


