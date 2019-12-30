---
title: "Pi Camera on Raspberry Pi Zero"
description: ""
date: "2017-09-20"
tags:
  - "iot"
  - "raspberrypi"
meta_kw:
  - "Raspberry"
  - "raspberry pi"
  - "pi camera"
  - "pi camera2"
  - "pi zero"
  - "tutorial"
---

## Installing the Pi Camera

Just plug in the ribbon cable as shown and you are all set.

> You can purchase these cables in several sizes at
> [Adafruit](https://www.adafruit.com/?q=camera%20flex%20cable) if you need longer lengths.

Take a peek at mine if you need a reference. Take it easy with the black peice that slides out to
pinch the ribbon cable... I broke mine pretty easily and now have a Pi that won't hold a ribbon
cable reliably.

{{<figure src="/imgs/pi0_camera_installed.jpg" alt="Pi Camera 2 installed" title="Pi Camera 2 Installed">}}

## Interacting with Pi Camera

Since we're using Python, we can use an awesome Python library called
[picamera](https://picamera.readthedocs.io). It provides a Python API to do all sorts of things
with the camera. Check out [the docs](https://picamera.readthedocs.io) for great info and examples.

Note, I use Python 3.x for development these days. Python 2.7 should be left out to the pasture.
That being said, I install `python3-picamera` here instead of `python-picamera` (which would also
work if you need Python 2.x).

```bash
$ sudo apt-get install -y python3-picamera
```

### Taking a Picture

Create a simple script.

```python
import picamera

camera = picamera.PiCamera()

# I had my camera oriented in such a way that this made sense.
# ... change if if your picture is upside down.
camera.rotation = 180

camera.capture('/tmp/image.jpg')
```

After you run the script, download the image from the Pi to your computer.

```bash
# Run this on the computer you want to view the image on.
# Substitute 1.2.3.4 in for your Raspberry Pi's IP.
scp pi@1.2.3.4:/tmp/image.jpg .
```

Open `image.jpg` with your favorite image viewer.

### Taking a Video

Create a simple script.

```python
from time import sleep
import picamera

camera = picamera.PiCamera()
camera.rotation = 180
camera.start_recording('/tmp/video.h264')
# take a 20 second video
sleep(20)
camera.stop_recording()
```

After you run the script, download the video from the Pi to your computer.

```bash
# Run this on the computer you want to view the video on.
# Substitute 1.2.3.4 in for your Raspberry Pi's IP.
scp pi@1.2.3.4:/tmp/video.h264 .
```

This is raw video. You can open it with [VLC](http://www.videolan.org/vlc/) or (most likely) your
favorite media player. If all else fails, [VLC](http://www.videolan.org/vlc/) does the trick!
