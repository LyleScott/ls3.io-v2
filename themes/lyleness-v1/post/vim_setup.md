---
title: "My vim Setup"
description: ""
date: 2019-01-01T19:56:32-05:00
draft: true
tags:
  - "misc"
  - "programming"
meta_kw:
  - "vim"
  - "ide"
  - "python"
  - "bash"
  - "rust"
  - "rust-lang"
  - "go"
  - "go-lang"
  - "syntax highlight"
  - "syntax highlighting"
  - "YouCompleteMe"
  - "Syntastic"
  - "lint"
---

I've used [vim](https://www.vim.org) for the better part of my life and the following are my
current (it'll change, but everything is cached these days!) settings that keep me productive.

> I really don't have anything exotic but I _have_ done some leg work for some settings out

```vim
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" General
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

filetype off
syntax on
set nocompatible
set expandtab
set tabstop=4
set shiftwidth=4
set colorcolumn=100
filetype plugin indent on

# Associate specific file extensions with a language.
au BufNewFile,BufRead Jenkinsfile setf groovy

" Increase buffer size of yank.
set viminfo='50,<1000,s100,h

" Enable mouse support.
set mouse=a

" Check file change every 4 seconds ('CursorHold') and reload the buffer upon detecting change
set autoread
au CursorHold * checktime

" Better yaml indenting.
autocmd FileType yaml setlocal ts=2 sts=2 sw=2 expandtab
 
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" Vundle
" vim package manager
" https://github.com/VundleVim/Vundle.Vim
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

set rtp+=~/.vim/bundle/Vundle.vim
call vundle#begin()
Plugin 'ekalinin/Dockerfile.vim'
Plugin 'elzr/vim-json'
Plugin 'farmergreg/vim-lastplace'
Plugin 'jeffkreeftmeijer/vim-numbertoggle'
Plugin 'majutsushi/tagbar'
Plugin 'ntpeters/vim-better-whitespace'
Plugin 'racer-rust/vim-racer'
Plugin 'rust-lang/rust.vim'
Plugin 'scrooloose/nerdtree'
Plugin 'Valloric/YouCompleteMe'
Plugin 'vim-airline/vim-airline'
Plugin 'vim-airline/vim-airline-themes'
Plugin 'vim-syntastic/syntastic'
Plugin 'VundleVim/Vundle.vim'
Plugin 'mattn/webapi-vim'
call vundle#end()

""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" NERD Tree
" a file system explorer
" https://github.com/scrooloose/nerdtree
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

" Close vim if the only window left open is a NERDTree
autocmd bufenter * if (winnr("$") == 1 && exists("b:NERDTree") && b:NERDTree.isTabTree()) | q | endif

" Map CTRL+n to open NERDTree
map <C-n> :NERDTreeToggle<CR>

" Open a NERDTree automatically when vim starts up if no files were specified
autocmd StdinReadPre * let s:std_in=1
autocmd VimEnter * if argc() == 0 && !exists("s:std_in") | NERDTree | endif

""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" Rust
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

let g:racer_cmd = '~/.cargo/bin/racer'
let g:racer_experimental_completer = 1
let g:rust_clip_command = 'pbcopy'
let g:rustfmt_autosave = 1
let g:rustfmt_command = 'rustup run nightly rustfmt'

" Associate more file extensions with Rust.
au FileType rust nmap gd <Plug>(rust-def)
au FileType rust nmap gs <Plug>(rust-def-split)
au FileType rust nmap gx <Plug>(rust-def-vertical)
au FileType rust nmap gr <Plug>(rust-doc)

""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" Syntastic
" syntax checking
" https://github.com/vim-syntastic/syntastic
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

set statusline+=%#warningmsg#
set statusline+=%{SyntasticStatuslineFlag()}
set statusline+=%*

let g:syntastic_always_populate_loc_list = 1
let g:syntastic_auto_loc_list = 1
let g:syntastic_check_on_open = 1
let g:syntastic_check_on_wq = 0
let g:syntastic_python_checkers = ['pyflake8']

""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" Tagbar
" browse tags of the current file and get an overview of its structure
" b.io/tagbar/
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

" Map Ctrl+t to open Tagbar
nmap <C-t> :TagbarToggle<CR>
let g:tagbar_type_rust = {
    \ 'ctagstype' : 'rust',
    \ 'kinds' : [
        \'T:types,type definitions',
        \'f:functions,function definitions',
        \'g:enum,enumeration names',
        \'s:structure names',
        \'m:modules,module names',
        \'c:consts,static constants',
        \'t:traits',
        \'i:impls,trait implementations',
    \]
    \}

""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" YouCompleteMe
" autocomplete engine
" http://valloric.github.io/YouCompleteMe/
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

let g:ycm_rust_src_path = '/Users/lyle/.rustup/toolchains/nightly-x86_64-apple-darwin/lib/rustlib/src/rust/src'
let g:ycm_server_python_interpreter="/usr/local/bin/python3"
```
