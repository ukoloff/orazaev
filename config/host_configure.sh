#!/usr/bin/env bash

# vim configure
ln -s $PWD/.vimrc ~/.vimrc
ln -s $PWD/.vim ~/.vim

git clone https://github.com/gmarik/vundle.git ~/.vim/bundle/vundle

# python configure
ln -s $PWD/.pythonstartup ~/.pythonstartup


echo "export EDITOR=vim" >> ${HOME}/.bashrc
echo "export PYTHONSTARTUP=~/.pythonstartup" >> ${HOME}/.bashrc
echo "export TERM=xterm-256color" >> ${HOME}/.bashrc
