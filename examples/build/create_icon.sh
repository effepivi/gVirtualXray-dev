rm -rf DragonDemo.iconset
mkdir DragonDemo.iconset
sips -z 16 16     DragonDemo1024.png --out DragonDemo.iconset/icon_16x16.png
sips -z 32 32     DragonDemo1024.png --out DragonDemo.iconset/icon_16x16@2x.png
sips -z 32 32     DragonDemo1024.png --out DragonDemo.iconset/icon_32x32.png
sips -z 64 64     DragonDemo1024.png --out DragonDemo.iconset/icon_32x32@2x.png
sips -z 128 128   DragonDemo1024.png --out DragonDemo.iconset/icon_128x128.png
sips -z 256 256   DragonDemo1024.png --out DragonDemo.iconset/icon_128x128@2x.png
sips -z 256 256   DragonDemo1024.png --out DragonDemo.iconset/icon_256x256.png
sips -z 512 512   DragonDemo1024.png --out DragonDemo.iconset/icon_256x256@2x.png
sips -z 512 512   DragonDemo1024.png --out DragonDemo.iconset/icon_512x512.png
cp DragonDemo1024.png DragonDemo.iconset/icon_512x512@2x.png
iconutil -c icns DragonDemo.iconset
rm -rf DragonDemo.iconset
