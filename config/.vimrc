" set numbers
set number

" set incremental highlight search
set is
set hlsearch

" replase tabs with spaces
set tabstop=4 shiftwidth=4 expandtab

" UI options
syntax on
colors hickop
set guifont=monaco

" highlight lines with length > 80 characters
:au BufWinEnter * let w:m1=matchadd('Search', '\%<81v.\%>77v', -1)
:au BufWinEnter * let w:m2=matchadd('ErrorMsg', '\%>80v.\+', -1)

" highlight trailing spaces
au BufNewFile,BufRead * let b:mtrailingws=matchadd('ErrorMsg', '\s\+$', -1)

" highlight tabs between spaces
au BufNewFile,BufRead * let b:mtabbeforesp=matchadd('ErrorMsg', '\v(\t+)\ze( +)', -1)
au BufNewFile,BufRead * let b:mtabaftersp=matchadd('ErrorMsg', '\v( +)\zs(\t+)', -1)

" disable matches in help and .vimrc buffers
au BufEnter,FileType help call clearmatches()
au BufEnter,FileType .vimrc call clearmatches()

" Recover position and folding in file
if has("autocmd")
    " set viewoptions=cursor,folds
    set viewoptions=cursor
    au BufWinLeave * mkview
    au BufWinEnter * silent loadview
endif

" For xterm
" set background=dark

map <F4> [I:let nr = input("Which one: ")<Bar>exe "normal " . nr ."[\t"<CR>

set autoindent
au FileType cpp set cindent
