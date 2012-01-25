" show cursor position
set ruler
" preserve current indent on new lines
set ai
" make backspaces delete sensibly
set backspace=indent,eol,start
" indentation levels every four columns
set tabstop=4
" convert all tabs to space
"set expandtab
" indent/outdent by four columns
set sw=4
" indent/outdent to nearest tabstops
set shiftround
syntax on
au BufReadPost *.thtml,*.ctp set syntax=php
au BufReadPost *.html set syntax=htmldjango

" Paste Mode On/Off
map <F12> :call Paste_on_off()<CR>
set pastetoggle=<F12>

let paste_mode = 0 " 0 = normal, 1 = paste

func! Paste_on_off()
        if g:paste_mode == 0
                set paste
                let g:paste_mode = 1
        else
                set nopaste
                let g:paste_mode = 0
        endif  
        return
endfunc 
