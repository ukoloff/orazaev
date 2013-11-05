" set numbers
set number

" set incremental highlight search
set is
set hlsearch

" replase tabs with spaces
set tabstop=4 shiftwidth=4 expandtab

" UI options
syntax on
colors ora
set guifont=monaco

" Recover position and folding in file
if has("autocmd")
    " set viewoptions=cursor,folds
    set viewoptions=cursor
    au BufWinLeave ?* mkview
    au BufWinEnter ?* silent loadview
endif


function HighlightTrailingSpaces()
    let b:mtrailingws=matchadd('ErrorMsg', '\s\+$', -1)
endfunction


function HighlightLongString()
    let w:m1=matchadd('Search', '\%<101v.\%>79v', -1)
    let w:m2=matchadd('ErrorMsg', '\%>100v.\+', -1)
endfunction


function HighlightMixedTabsAndSpaces()
    let b:mtabbeforesp=matchadd('ErrorMsg', '\v(\t+)\ze( +)', -1)
    let b:mtabaftersp=matchadd('ErrorMsg', '\v( +)\zs(\t+)', -1)
endfunction


function MakeMatches()
    call HighlightLongString()
    call HighlightTrailingSpaces()
    call HighlightMixedTabsAndSpaces()
endfunction

au BufNewFile,BufRead *.cpp,*.h,*.java,*.sh,*.py,*.sml call MakeMatches()


" Don't screw up folds when inserting text that might affect them, until
" leaving insert mode. Foldmethod is local to the window. Protect against
" screwing up folding when switching between windows.
" autocmd InsertEnter * if !exists('w:last_fdm') | let w:last_fdm=&foldmethod | setlocal foldmethod=manual | endif
" autocmd InsertLeave,WinLeave * if exists('w:last_fdm') | let &l:foldmethod=w:last_fdm | unlet w:last_fdm | endif

" For xterm
" set background=dark

map <F4> [I:let nr = input("Which one: ")<Bar>exe "normal " . nr ."[\t"<CR>

set autoindent
au FileType cpp set cindent

set nocompatible
filetype off

set rtp+=~/.vim/bundle/vundle/
call vundle#rc()


" let Vundle manage Vundle
" required!
"
Bundle 'gmarik/vundle'

" My Bundles here:
"
" original repos on github:

Bundle 'terryma/vim-multiple-cursors'
Bundle 'vim-scripts/taglist.vim'
Bundle 'scrooloose/nerdtree'
Bundle 'tpope/vim-abolish'

Bundle 'bling/vim-airline'
set laststatus=2
let g:airline_left_sep = ''
let g:airline_right_sep = ''
let g:airline_powerline_fonts = 0
let g:airline_theme = 'understated'

" REPLs:
" Bundle 'khorser/vim-repl' -- it sucks
" Bundle 'jpalardy/vim-slime.git' -- it sucks
Bundle 'pthrasher/conqueterm-vim'

" need if_lua
" Bundle 'Shougo/neocomplete.vim'

" Colors:
Bundle 'junegunn/seoul256.vim'
Bundle 'altercation/vim-colors-solarized'
let g:solarized_termcolors=256

filetype plugin indent on
