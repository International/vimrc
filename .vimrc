set nocompatible

set rtp+=~/.vim/bundle/vundle/
call vundle#rc()

Bundle "digitaltoad/vim-jade"
Bundle 'fatih/vim-go'
" Bundle 'gmarik/vundle'
Bundle 'Lokaltog/vim-easymotion'
Bundle "majutsushi/tagbar"
Bundle "wting/rust"
Bundle "L9"
Bundle "FuzzyFinder"
Bundle 'ack.vim'
Bundle 'douglasjacobsen/vim-bufexplorer'
Bundle 'kien/ctrlp.vim'
Bundle 'godlygeek/tabular'
Bundle 'vim-scripts/ruby-matchit'
Bundle 'tomtom/tcomment_vim'
Bundle 'tpope/vim-bundler'
Bundle 'kchmck/vim-coffee-script'
Bundle 'flazz/vim-colorschemes'
Bundle 'tpope/vim-endwise'
Bundle 'tpope/vim-haml'
Bundle 'tpope/vim-rails'
Bundle 'tpope/vim-rvm'
Bundle "pangloss/vim-javascript"
Bundle "elzr/vim-json"
" Bundle "tsaleh/vim-matchit"
Bundle "sickill/vim-pasta"
Bundle "vim-ruby/vim-ruby"
Bundle "MarcWeber/vim-addon-mw-utils"
Bundle "tomtom/tlib_vim"
Bundle "kana/vim-textobj-user"
Bundle "nelstrom/vim-textobj-rubyblock"
Bundle 'thoughtbot/vim-rspec'
Bundle "tpope/vim-fugitive"
Bundle "jgdavey/tslime.vim"
Bundle "jgdavey/vim-turbux"
Bundle "scrooloose/nerdtree"
Bundle "molokai"
Bundle "jnwhiteh/vim-golang"
Bundle "tomtom/quickfixsigns_vim"
Bundle "ervandew/screen"
Bundle "jmcantrell/vim-diffchanges"

set noet
set number
syntax on
filetype on
filetype indent on
filetype plugin on
set tabstop=2
set shiftwidth=2
set expandtab
set background=dark

set backspace=indent,eol,start
set hlsearch
set showmatch
set nowrap
set wildmenu
set wildmode=list:longest,full
set hidden
set autochdir
let g:EasyMotion_leader_key = '<Leader>'
set laststatus=2
" noremap / /\v
set listchars=tab:>-,trail:-
set list
set colorcolumn=80
set diffopt=filler,horizontal

map <F2> :NERDTreeToggle<CR>

" nmap <c-a> ^
" nmap <c-e> $

imap <C-e> <esc>$a
imap <C-a> <esc>$I

map <Leader>l :FufLine<CR>
map <Leader>r :CtrlPMRUFiles<CR>
map <Leader>b :FufBuffer<CR>
map <Leader>t :FufTag<CR>
map <Leader>m :TagbarToggle<CR>

map <Leader>d :Gdiff<CR>
map <Leader>s :Gstatus<CR>

map <Leader>z :call RunNearestSpec()<CR>
map <Leader>x :call RunCurrentSpecFile()<CR>
map <Leader>c :call RunAllSpecs()<CR>

nmap // :BufExplorer<CR>

set encoding=utf-8
" set statusline ="%4*\ %<%F%* %{rvm#statusline()}"           "full path
set ruler
set rulerformat=%l,%v
set incsearch

map <S-s> :write<CR>
imap \\ <Esc>

color railscasts
" color molokai
syntax on
set cursorline
set number
set list!                       " Display unprintable characters
set listchars=tab:▸\ ,trail:•,extends:»,precedes:«

" colorscheme molokai

if $TERM =~ '256color'
  set t_Co=256
  colorscheme molokai
elseif $TERM =~ '^xterm$'
  set t_Co=256
  colorscheme molokai
endif

" if has("gui_running")
"   colorscheme eclipse
" endif

let g:Powerline_symbols = 'unicode'

let g:ScreenImpl = 'Tmux'
let g:ScreenShellTmuxInitArgs = '-2'
let g:ScreenShellInitialFocus = 'shell'
let g:ScreenShellQuitOnVimExit = 0

map <C-\> :ScreenShellVertical<CR>
command -nargs=? -complete=shellcmd W  :w | :call ScreenShellSend("load '".@%."';")

set nobackup       
set nowritebackup
set noswapfile     

set t_ut=
if has("autocmd")
  autocmd FileType ruby set omnifunc=rubycomplete#Complete
  autocmd FileType ruby let g:rubycomplete_buffer_loading=1
  autocmd FileType ruby let g:rubycomplete_classes_in_global=1
  autocmd FileType ruby let g:rubycomplete_rails = 1
endif

autocmd BufRead,BufNewFile *.go setlocal filetype=go
autocmd BufWinEnter * if line2byte(line("$") + 1) > 1000000 | syntax clear | endif

nnoremap <leader>. :CtrlPTag<cr>
map <silent> <Leader>rt :!bundle list --paths=true \| xargs ctags-exuberant --extra=+f --exclude='*.js' --exclude=.git --exclude=log -R *<CR><CR> 
