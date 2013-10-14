#!/usr/bin/env bash


##############################################
#               vim configure
##############################################
ln -s $PWD/.vimrc ~/.vimrc
ln -s $PWD/.vim ~/.vim

# get vundle to automatic install other plugins
git clone https://github.com/gmarik/vundle.git ~/.vim/bundle/vundle

echo "export TERM=xterm-256color" >> ~/.bashrc
echo "export EDITOR=vim" >> ~/.bashrc

##############################################
#              python confiture
##############################################
ln -s $PWD/.pythonstartup ~/.pythonstartup

echo "export PYTHONSTARTUP=~/.pythonstartup" >> ~/.bashrc
