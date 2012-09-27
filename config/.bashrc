# Use 256 colors term
export TERM=xterm-256color
TERM=xterm-256color

# SSH agent
if [ -z "$(ps -U $USER | grep ssh-agent)" ]; then
    exec ssh-agent bash
    pushd $HOME/.ssh
    ssh-add
    popd
   #test -f $HOME/.ssh-agent && rm -f $HOME/.ssh-agent
   #ssh-agent > $HOME/.ssh-agent && chmod 600 $HOME/.ssh-agent
   #source $HOME/.ssh-agent
   #ssh-add
   echo ssh-agent started
else
   #source $HOME/.ssh-agent
    :
fi

export EDITOR=vim
