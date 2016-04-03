#!/bin/sh

/bin/ffmpeg/ffmpeg \
-s 640x480 -f video4linux2 -i /dev/video0 -f mpeg1video -b 1000k -r 30 http://127.0.0.1:8082
# -s 320x240 image.jpg



