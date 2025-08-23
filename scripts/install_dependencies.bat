::@echo off
:: Bu betik ile SDL3 icin ihtiyac duyulan ucuncu parti bagimliliklar kontrol edilecek/kurulacaktir
echo Windows icin kontroller ve kurulumlar baslatiliyor& echo.

:: Turkce karakterler icin
chcp 65001 > NUL

:: oncelikle CMake kontrolu
call :CheckExistance cmake

:: ninja kontrolu
call :CheckExistance ninja

:: Sonra SDL3 kutuphanelerinin kurulum
call :DownloadSDL3

echo Windows icin kontroller ve kurulumlar tamamlandi

goto :EOF

:DownloadSDL3
echo #################  SDL - START  ######################& echo.

mkdir sdl3
cd sdl3

call :DownloadAndInstallSDL3Item SDL3 SDL3-3.2.20 https://github.com/libsdl-org/SDL/releases/download/release-3.2.20/SDL3-3.2.20.tar.gz
call :DownloadAndInstallSDL3Item SDL3-Image SDL3_image-3.2.4 https://github.com/libsdl-org/SDL_image/releases/download/release-3.2.4/SDL3_image-3.2.4.tar.gz
call :DownloadAndInstallSDL3Item SDL3-TTF SDL3_ttf-3.2.2 https://github.com/libsdl-org/SDL_ttf/releases/download/release-3.2.2/SDL3_ttf-3.2.2.tar.gz

cd ..

:: Kurulumlar tamamlnadi
goto :EOF

:: Ilgili komut varligi kontrolu
:CheckExistance

echo #################  %~1 - START ######################
where /q %~1.exe

if ERRORLEVEL 1 (
    echo %~1 kurulu degil. Lutfen kurunuz ve PATH ortam degiskenine eklendiginden emin olunuz.
    exit /B
) ELSE (
    %~1 --version
    echo %~1 kurulu. Bir sonraki adima gecelim!
)
echo ###################  %~1 -END ####################& echo.
goto :EOF


:: Tek tek SDL kalemlerini indirmek icin 
:DownloadAndInstallSDL3Item
echo #################  %~1 - START  ######################& echo.
if not exist %~2 (
    curl.exe -L --ssl-revoke-best-effort %~3 --output %~2.tar.gz
) else (
    echo %~2 dosyasi zaten var!
)
tar -xvf %~2.tar.gz
::rmdir /s /q %~2
cd %~2
cd
echo %CD%\..\..\sdl3-install
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DSDL_STATIC=ON -G Ninja -DSDL_TESTS=OFF -DCMAKE_INSTALL_PREFIX=%CD%\..\..\sdl3-install -DCMAKE_PREFIX_PATH=%CD%\..\..\sdl3-install
cmake --build build --config Release  --target install
cd ..

echo #################  %~1 - END  ######################& echo.
goto :EOF
