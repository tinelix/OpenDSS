#!/bin/bash

su -c "apt install -y libtls-dev"

cd ffmpeg
./configure \
    --prefix=../out/ffmpeg \
    --enable-version3 \
    --disable-nonfree \
    --enable-avcodec \
    --enable-avformat \
    --enable-avutil \
    --disable-programs \
    --disable-ffmpeg \
    --disable-ffplay \
    --disable-ffprobe \
    --disable-doc \
    --disable-htmlpages \
    --disable-dxva2 \
    --disable-vaapi \
    --disable-vdpau \
    --disable-encoders \
    --disable-decoders \
    --disable-demuxers \
    --disable-parsers \
    --disable-muxers \
    --disable-filters \
    --disable-bsfs \
    --disable-iconv \
    --enable-libtls \
    --enable-protocol=file,http,async,tcp,tls \
    --enable-parser=flac,aac,aac_latm,vorbis,ogg,theora \
    --enable-demuxer=mp3,mov,ogg,data \
    --enable-decoder=mp3,aac,aac_latm,flac,vorbis \
    --enable-encoder=libmp3lame,vorbis,aac \
    --enable-muxer=ogg,mp3

make -j8 && make install

cd ..
make

echo
echo "Build completed!"
echo
echo "Run './out/opendss' for testing, 'gdb ./out/opendss' for debug"
cd ./out
