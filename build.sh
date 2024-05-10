#!/bin/bash

echo "Building OpenDSS..."

FFMPEG_FLAGS=""

if [[ $OSTYPE == "linux-gnu" ]]; then
    echo "Your OS: GNU/Linux"
    echo
    FFMPEG_FLAGS+="--enable-libtls"
    if [ -x "apt" ]; then                                      # <-- for Ubuntu/Debian/derivatives
        su -c "apt install -y libtls-dev libncurses-dev"
    elif [ -x "yum" ]; then                                    # <-- for Fedora/CentOS/AltLinux/derivatives
        su -c "yum install libressl-devel ncurses-devel"
    elif [ -x "pacman" ]; then                                 # <-- for Arch/Artix/Manjaro/derivatives
        su -c "pacman -S libressl ncurses"
    else
        echo "ERROR: Your package manager is not supported"
        echo
        exit 1
    fi
elif [ $OSTYPE == "cygwin" ]; then
    echo "Your OS: Cygwin/Windows"
    echo
    FFMPEG_FLAGS+="--enable-gnutls"
    if [ -x apt-cyg ]; then
        apt-cyg install gnutls-devel libncursesw-devel
    else
        echo "ERROR: 'apt-cyg' not found"
        echo
        echo "Possible solutions to the problem:"
        echo "* Install the 'gnutls-devel' and 'libncursesw-devel' packages" \
        "in the Cygwin installer"
        echo "* Download 'apt-cyg' package manager from" \
        "https://github.com/transcode-open/apt-cyg"
        echo
        exit 1
    fi
elif [[ $OSTYPE == "msys" ]]; then
    echo "Your OS: MSYS2/Windows"
    echo
    FFMPEG_FLAGS+="--enable-gnutls"
    pacman -S mingw-w64-{i686,x86_64}-gnutls mingw-w64-{i686,x86_64}-ncurses
else
    echo "ERROR: Your OS is not supported"
    echo
    exit 1
fi

cd libs/ffmpeg

FFMPEG_FLAGS+="--prefix=../out/ffmpeg \
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
    --enable-protocol=file,http,async,tcp,tls \
    --enable-parser=flac,aac,aac_latm,vorbis,ogg,theora \
    --enable-demuxer=mp3,mov,ogg,data \
    --enable-decoder=mp3,aac,aac_latm,flac,vorbis \
    --enable-encoder=libmp3lame,vorbis,aac \
    --enable-muxer=ogg,mp3"

./configure $FFMPEG_FLAGS

make -j8 && make install

cd ../..
make

echo
echo "Build completed!"
echo
echo "Run './out/opendss' for testing, 'gdb ./out/opendss' + 'r' for debug"
cd ./out
