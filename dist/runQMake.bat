:: keep me entertained until i go to a place with better internet
:: to reinstall qt

set buildDir=C:\Users\Mateus\Documents\Programming\Qt\build-RobloxStudio-DiscordRPC-Desktop_Qt_6_3_1_MSVC2019_64bit-Debug
set msvc=C:\Qt\6.3.1\msvc2019_64

:: "Qt\6.3.1\msvc2019_64\mkspecs" for full list of makespecs
set mkspec=win32-msvc

pushd %buildDir%
    %msvc%\bin\qmake.exe C:\Users\Mateus\Documents\Programming\Qt\RobloxStudio-DiscordRPC\RobloxStudio-DiscordRPC.pro -spec %mkspec% "CONFIG+=debug" "CONFIG+=qml_debug" "QMAKE_CFLAGS+=--sysroot="\^""%msvc%"\^""" "QMAKE_CXXFLAGS+=--sysroot="\^""%msvc%"\^""" "QMAKE_LFLAGS+=--sysroot="\^""%msvc%"\^""" "QMAKE_MSC_VER="
    :: "QMAKE_CFLAGS+=--target=x86_64-w64-mingw32" "QMAKE_CXXFLAGS+=--target=x86_64-w64-mingw32" "QMAKE_LFLAGS+=--target=x86_64-w64-mingw32"
    C:/Qt/Tools/QtCreator/bin/jom/jom.exe qmake_all
popd

exit /b 0
