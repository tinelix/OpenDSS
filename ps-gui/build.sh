#!/bin/bash

echo "Building Tinelix OpenDSS..."

if [[ $OSTYPE == "linux-gnu" || $OSTYPE == "linux" ]]; then
    echo "Your OS: GNU/Linux"
    echo
    if [ -x "$(command -v apt)" ]; then         # <-- for Ubuntu/Debian/derivatives
        su -c "apt install -y libncurses-dev"
    elif [ -x "$(command -v yum)" ]; then       # <-- for Fedora/CentOS/AltLinux/derivatives
        su -c "yum install ncurses-devel"
    elif [ -x "$(command -v pacman)" ]; then    # <-- for Arch/Artix/Manjaro/derivatives
        su -c "pacman -S ncurses"
    elif [ -x "$(command -v zypper)" ]; then
	su -c "zypper in ncurses-devel"		# <-- for openSUSE/derivatives
    else
        echo "ERROR: Your package manager is not supported"
        echo
        exit 1
    fi
elif [ $OSTYPE == "cygwin" ]; then
    echo "Your OS: Cygwin/Windows"
    echo
    if [ -x "$(command -v apt-cyg)" ]; then
        apt-cyg install libncursesw-devel
    else
        echo "ERROR: 'apt-cyg' not found"
        echo
        echo "Possible solutions to the problem:"
        echo "* Install the 'gnutls-devel' and 'libncursesw-devel' packages" \
        "in the Cygwin installer"
        echo "* Download 'apt-cyg' package manager from" \
        "https://github.com/transcode-open/apt-cyg"
        echo "* Enter 'make' if all required packages have already been" \
        "installed."
        exit 1
    fi
elif [[ $OSTYPE == "msys" ]]; then
    echo "Your OS: MSYS2/Windows"
    echo
    pacman -S mingw-w64-{i686,x86_64}-ncurses
else
    echo "ERROR: Your OS is not supported"
    echo
    exit 1
fi


cd libs/ffmpeg

FFMPEG_FLAGS+="--prefix=../../out/ffmpeg \
    --enable-version3 \
    --disable-nonfree \
    --enable-avcodec \
    --enable-avformat \
    --enable-avutil \
    --enable-network \
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
