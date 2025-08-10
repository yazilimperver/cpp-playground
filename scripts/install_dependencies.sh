#!/bin/sh

echo Linux icin kontroller ve kurulumlar başlatılıyor

# Verilen komut var mi kontrol eder yok ise kurar
# Eger paket, komut ismindan farkli ise ikinci arguman olarak geciriliyor
CheckExistance() {
    if ! type "$1" > /dev/null; then
        # Bazen komutla paket ismi farklı olabiliyor. Onun için
        if [ $# -eq 2 ]; then
            echo "$2 kurulu değil, öncelikle onu kuralım"
            sudo apt install $2
        else
            echo "$1 kurulu değil, öncelikle onu kuralım"
            sudo apt install $1
        fi
    else
        echo "$1 kurulu, bu adım atlanıyor."
    fi
}

# Verilen SDL3 kutuphanesini indirir
# 1. arguman kutuphane ismi
# 2. arguman kutuphane dosya ismi
# 3. arguman kutuphane adresi
DownloadAndInstallSDL3Item(){
    echo "#################  $1 - START  ######################"

    if test -f $2.tar.gz; then
        echo $2.tar.gz dosyasi zaten var!
    else
        echo $2.tar.gz dosyasi yok. İndirilecek!
    fi
    
    wget $3
    tar -xvf $2.tar.gz
    rm $2.tar.gz*
    cd $2
    cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DSDL_UNIX_CONSOLE_BUILD=ON
    cmake --build build --config Release --parallel
    sudo cmake --install build --config Release --prefix /usr/local

    cd ..
    echo "#################  $1 - END  ######################"
}

DownloadAndInstallSDL3Libraries(){
    echo "#################  SDL - START  ######################"    
    mkdir sdl3
    cd sdl3

    DownloadAndInstallSDL3Item SDL3 SDL3-3.2.20 https://github.com/libsdl-org/SDL/releases/download/release-3.2.20/SDL3-3.2.20.tar.gz
    # DownloadAndInstallSDL3Item SDL3-Image SDL3_image-3.2.4 https://github.com/libsdl-org/SDL_image/releases/download/release-3.2.4/SDL3_image-3.2.4.tar.gz
    # DownloadAndInstallSDL3Item SDL3-TTF SDL3_ttf-3.2.2 https://github.com/libsdl-org/SDL_ttf/releases/download/release-3.2.2/SDL3_ttf-3.2.2.tar.gz

    echo "#################  SDL - END  ######################"    
}

# Gerekli araçlar var mı kontrol edelim
CheckExistance g++ build-essential
CheckExistance git
CheckExistance cmake
CheckExistance ninja ninja-build

sudo apt-get install build-essential git make \
    pkg-config cmake ninja-build gnome-desktop-testing libasound2-dev libpulse-dev \
    libaudio-dev libjack-dev libsndio-dev libx11-dev libxext-dev \
    libxrandr-dev libxcursor-dev libxfixes-dev libxi-dev libxss-dev libxtst-dev \
    libxkbcommon-dev libdrm-dev libgbm-dev libgl1-mesa-dev libgles2-mesa-dev \
    libegl1-mesa-dev libdbus-1-dev libibus-1.0-dev libudev-dev libpipewire-0.3-dev libwayland-dev libdecor-0-dev liburing-dev

DownloadAndInstallSDL3Libraries

echo Linux için kontroller ve kurulumlar tamamlandı
