:: Creates folder with DLL files
:: in order to get app ready to release
@echo off

pushd %1
    set buildDir=%CD%
popd

pushd %~dp0

    if exist deploy\ (
        del /S /Q deploy\*
    ) else (
        mkdir deploy
    )

    pushd deploy

        set Qt=C:\Qt

        set buildtools=msvc2019_64
        set version=6.3.1

        set bt=%Qt%\%version%\%buildtools%
        set btBin=%bt%\bin
        set btPlugins=%bt%\plugins

        copy %btBin%\Qt6Core.dll .
        copy %btBin%\Qt6Gui.dll .
        copy %btBin%\Qt6Widgets.dll .

        mkdir platforms
        pushd platforms
            copy %btPlugins%\platforms\qwindows.dll .
        popd

        copy ..\..\libs\discord_game_sdk\libs\discord_game_sdk.dll .

        copy %buildDir%\release\RobloxStudio-DiscordRPC.exe .
    popd

    powershell -command "Compress-Archive .\\deploy\\* .\\deploy.zip -Force"

popd
