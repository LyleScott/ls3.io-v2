---
title: "A handy script to update brew packages"
description: "...again."
date: "2016-12-04"
tags:
  - "misc"
meta_kw:
  - "brew"
  - "update"
  - "upgrade"
  - "osx"
  - "mac"
  - "terminal"
---

I'm not one to roll with outdated packages. Much like the continuous deployment philosophy, I like
to update often so I don't end up updating half of my installed packages at one time and be left
with a broken mess that I can't easily track down the culprit for.

To facilitate this, I just wrote a quick `bash` script. Run it when it's convenient or throw it in
`cron.`

```bash
#!/bin/bash

# -x = Display the commands that are being executed.
# -e = Exit the script on the first error. 
set -ex

# Update the local index of known packages.
brew update

# Upgrade installed packages that are out of date.
brew upgrade

# A brew upgrade leaves the old packages installed and it can take up a lot of space.
# This removes old versions.
brew cleanup

# Let brew inspect your setup and warn you of any weirdness.
brew doctor
```

As you can see, nothing fancy here. I've never had a reason to so I hope you shouldn't either.
Having a script handy make upgrading a no-brainer and I can save my fingers!
