
---
title: "Rotate a 2D Coordinate Around a Point in Python"
date: 2017-11-19T13:20:09-05:00
tags:
  - "python"
meta_kw:
  - "python"
  - "rotate coordinates"
  - "around point"
  - "translate"
  - "transform"
---

I was doing some research on how to rotate coordinates around a point or origin for some silly
animations I was doing. After some time kicking stuff around, I came up with a few possibilities.

{{< gist LyleScott e36e08bfb23b1f87af68c9051f985302 >}}

And of course, I atleast did a quick timer check on the functions to look for one that is obviously
faster. I was surprised just how slow the numpy operation was... but I guess in the end it isn't
surprising since it's executing more Python code in general.

{{< gist LyleScott d17e9d314fbe6fc29767d8c5c029c362 >}}


