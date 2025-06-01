# cpp-playground
www.yazilimperver.com üzerinden yayınlayacağım yazılara eşlik edecek C++ ve benzeri kaynakları saklayacağımız repo.

# Linux Üzerinde SDL ve İlgili Kütüphanelerin Kurulumu
## SDL3 Kurulumu
``` bash
wget https://github.com/libsdl-org/SDL/releases/download/release-3.2.14/SDL3-3.2.14.tar.gz
tar xvf SDL3-3.2.14.tar.gz
cd SDL3-3.2.14
cmake -S . -B build
cmake --build build
sudo cmake --install build --prefix /usr/local
```

## SDL3 Image Kurulumu

``` bash
wget https://github.com/libsdl-org/SDL_image/releases/download/release-3.2.4/SDL3_image-3.2.4.tar.gz
tar xvf SDL3_image-3.2.4.tar.gz
cd SDL3_image-3.2.4
cmake -S . -B build
cmake --build build
sudo cmake --install build --prefix /usr/local
```

## SDL3 TTF Kurulumu

``` bash
wget https://github.com/libsdl-org/SDL_ttf/releases/download/release-3.2.2/SDL3_ttf-3.2.2.tar.gz
tar xvf SDL3_ttf-3.2.2.tar.gz
cd SDL3_ttf-3.2.2
cmake -S . -B build
cmake --build build
sudo cmake --install build --prefix /usr/local
```