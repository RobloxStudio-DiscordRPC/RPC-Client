@echo on

echo dwlibs.bat
echo run script anywhere and libs for RobloxStudio-DiscordRPC will install in %~dp0\libs\
echo
echo dependencies (in PATH):
echo - curl
echo - powershell
echo - git

set httplibRepo=..\httplib\
set discordDir=libs\discord_game_sdk\

pushd %~dp0
    :: reset libs folder
    call :delDir libs

    if not exist %discordDir% (
        mkdir %discordDir%
    )

    :: git clone httplib
    git clone "https://github.com/yhirose/cpp-httplib" %httplibRepo%

    :: get v0.10.8 release
    pushd %httplibRepo%
        git checkout tags/v0.11.0
    popd

    if not exist libs\ (
        mkdir libs
    )

    :: copy the header to libs folder
    copy %httplibRepo%httplib.h libs\

    :: no longer needing repo
    call :delDir %httplibRepo%

    :: download discord game sdk
    curl "https://dl-game-sdk.discordapp.net/2.5.6/discord_game_sdk.zip" --output discord_game_sdk.zip
    powershell -command "Expand-Archive discord_game_sdk.zip .\\libs\\discord_game_sdk\\"

    del discord_game_sdk.zip

    pushd %discordDir%
        :: delete unused directories
        call :delDir c
        call :delDir csharp
        call :delDir examples
        del README.md

        pushd lib
            xcopy /s /y x86_64 .
            call :delDir x86_64
            call :delDir x86

            ren discord_game_sdk.dll.lib discord_game_sdk.lib
        popd
    popd
popd

exit /b 0

:: call delDir <directory>
:delDir
    del /s /q %~1
    rmdir /s /q %~1
    exit /b 0
