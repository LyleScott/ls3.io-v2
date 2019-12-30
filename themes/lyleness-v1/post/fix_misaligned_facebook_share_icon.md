---
title: "Fix Misaligned Facebook Share Icon"
description: ""
date: "2017-12-05"
tags:
  - "misc"
meta_kw:
  - "misaligned facebook icon"
  - "misaligned facebook share icon"
  - "facebook"
  - "share"
  - "icon"
---

I had the following situation happen:

{{<figure src="/imgs/misaligned_fb_share_icon.jpg" alt="Misaligned Facebook Share Icon" title="Misaligned Facebook Share Icon">}}

I took a closer look when Chrome debug tools and found that Facebook was including an element with a `vertical-alignment` of `bottom`. This turned out to be the culprit and it is easily fixed with a basic CSS override.

```css
div.fb_iframe_widget > span {
    vertical-align: unset !important;
}
```

All this does is unset whatever custom value was set for `vertical-align`.
