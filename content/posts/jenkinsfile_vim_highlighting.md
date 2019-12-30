---
title: "Jenkinsfile VIM syntax highlighting"
description: ""
date: "2017-04-18T15:22:34-04:00"
tags:
  - "misc"
meta_kw:
  - "Jenkins"
  - "Jenkinsfile"
  - "VIM"
  - "syntax highlight"
---

VIM is actually set up to syntax highlight [Groovy](http://www.groovy-lang.org/) source files
(the programming language that a `Jenkinsfile` is written in) by default! What it doesn't know
is that the `Jenkinsfile` is indeed a [Groovy](http://www.groovy-lang.org/) source file.

In VIM config land, you can associate a syntax for a file name that matches a pattern. You'd
normally use it like the following if you wanted to associate the file extension `.foobar` with the
Groovy syntax.

```bash
au BufNewFile,BufRead *.foobar setf groovy
```

Using this, we can associate a `Jenkinsfile` with the Groovy syntax highlighter.

```bash
echo 'au BufNewFile,BufRead Jenkinsfile setf groovy' >> ~/.vimrc
```

Restart VIM and open up a `Jenkinsfile`. You should see syntax highlighting, now!
