Set-PSDebug -Step

@"
dwlibs.ps1
Run script anywhere and libraries for RobloxStudio-DiscordRPC will install in $($PSScriptRoot)\\libs\\

Dependencies:
- powershell
- git
"@

$httplibRepo = "..\\httplib\\"
$discordDir = ".\\libs\\discord_game_sdk\\"

function Push([string] $directory) {
    Push-Location $directory | Out-Null
}

function Inform([string] $msg) {
    "i - $($msg)"
}

Push("$($PSScriptRoot)\\..\\")

    Inform("Resetting library folders")
    Remove-Item -Path ".\\libs\\" -Force -Recurse

    New-Item -Path $discordDir -ItemType Directory -Force

    Inform("Cloning the cpp-httplib repository")
    & git clone "https://github.com/yhirose/cpp-httplib" $httplibRepo | Out-Null

    Push($httplibRepo)
        Inform("Getting version v0.11.0")
        & git checkout tags/v0.11.0 | Out-Null
    Pop-Location

    New-Item -Path ".\\libs\\" -ItemType Directory -Force

    Copy-Item -Path "$($httplibRepo)httplib.h" libs\

    Inform("Deleting cloned repository")
    Remove-Item -Path $httplibRepo -Force -Recurse

    Inform("Downloading the Discord GameSDK")
    Invoke-WebRequest -Uri "https://dl-game-sdk.discordapp.net/2.5.6/discord_game_sdk.zip" -OutFile ".\\discord_game_sdk.zip"

    Inform("Extracting the SDK")
    Expand-Archive -Path ".\\discord_game_sdk.zip" -DestinationPath ".\\libs\\discord_game_sdk\\" -Force

    Inform("Deleting the compressed SDK")
    Remove-Item -Path ".\\discord_game_sdk.zip"

    Push($discordDir)
        Inform("Deleting unnecessary files & folders")
        Remove-Item -Path ".\\c\\" -Force -Recurse
        Remove-Item -Path ".\\csharp\\" -Force -Recurse
        Remove-Item -Path ".\\examples\\" -Force -Recurse
        Remove-Item -Path ".\\README.md"

        Push(".\\lib\\")
            Inform("Filtering binaries")
            Copy-Item -Path ".\\x86_64\\*" -Destination ".\\" -Force -Recurse
            Remove-Item -Path ".\\x86_64\\" -Force -Recurse
            Remove-Item -Path ".\\x86\\" -Force -Recurse

            Rename-Item -Path ".\\discord_game_sdk.dll.lib" -NewName "discord_game_sdk.lib"
        Pop-Location
    Pop-Location
Pop-Location

"That's it!"

exit
