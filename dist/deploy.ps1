Set-PSDebug -Off

@"
deployment.ps1
Creates folder with DLL files
in order to get app ready to release
"@

function MakeDir([string] $path) {
    New-Item -Path $path -ItemType Directory | Out-Null
}

function Inform([string] $msg) {
    "i - $($msg)"
}

function Error([string] $msg) {
    Write-Host ("X - $($msg). Aborting.") -ForegroundColor Red
    exit # exit program 
}

function CurrDir {
    [OutputType([string])] param()

    return (Get-Location).ToString()
}

Push-Location $PSScriptRoot
    Inform("")
    $projDir  = [System.IO.DirectoryInfo] ((CurrDir) + "\..")
    $projName = $projDir.BaseName
    $releaseDirName  = $projName + "-Win64"
    $releaseDirPath  = (CurrDir) + "\\$($releaseDirName)\\"
    $releaseDirItems = $releaseDirItems + "*"
    $releaseZip = (CurrDir) + "\\$($releaseDirName).zip"

    Inform("Getting build folder")
    $buildDir = "$($projDir.ToString())\\..\\build-$($projName)-Desktop_Qt_6_3_1_MSVC2019_64bit-Release"
    $exe = ($buildDir + "\\release\\$($projName).exe")

    if (!(Test-Path -Path $exe)) {
        Error("Could not find final executable")
    }

    Inform("Resetting the deploy folder")

    if (Test-Path -Path $releaseDirPath) {
        Remove-Item -Path $releaseDirItems -Force -Recurse
    } else {
        MakeDir($releaseDirPath)
    }

    Push-Location $releaseDirPath
        Inform("Getting paths for Qt libraries")

        $Qt = $ENV:QT_HOME

        $buildtools = "msvc2019_64"
        $version = "6.3.1"

        $bt = "$($Qt)\\$($version)\\$($buildtools)"
        $btBin = $bt + "\\bin"
        $btPlugins = $bt + "\\plugins"

        Inform("Copying Qt libraries to distribution folder")
        Copy-Item -Path ($btBin + "\Qt6Core.dll") -Destination .
        Copy-Item -Path ($btBin + "\Qt6Gui.dll") -Destination .
        Copy-Item -Path ($btBin + "\Qt6Widgets.dll") -Destination .
        
        Inform("Copying Qt plugins to distribution folder")
        MakeDir(".\\platforms\\")
        Push-Location ".\\platforms\\"
            Copy-Item -Path ($btPlugins + "\\platforms\\qwindows.dll") -Destination .
        Pop-Location

        Inform("Copying the Discord GameSDK library to distribution folder")
        Copy-Item -Path "..\\..\\libs\\discord_game_sdk\\lib\\discord_game_sdk.dll" -Destination .

        Inform("Copying the final executable to distribution folder")
        Copy-Item -Path $exe -Destination .
    Pop-Location

    Inform("Compressing distribution folder into zip file")
    Compress-Archive -Path $releaseDirItems -DestinationPath ".\\$($releaseDirName).zip" -Force

Pop-Location

"That's it!"

exit