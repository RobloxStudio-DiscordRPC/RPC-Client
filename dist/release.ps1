param (
    [string] $ReleaseVersion,

    [switch] $Redeploy,
    [switch] $Pre,
    [switch] $NoRedirect
)

Set-PSDebug -Off

@"
release.ps1 [-ReleaseVersion] 0.0.0.0 [-Redeploy] [-Pre] [-NoRedirect]
Creates new draft release on GitHub.
When finished uploading draft, redirect to GitHub

Dependencies:
- GitHub CLI (gh)

Switches:
 -Redeploy   : Rerun the deploy script
 -Pre        : Mark new release as pre release
 -NoRedirect : Do not redirect to GitHub when finished uploading draft release to edit

"@

function Inform([string] $msg) {
    "i - $msg"
}

function Warn([string] $msg) {
    Write-Host ("! - $msg") -ForegroundColor Yellow
}

function Ask([string] $question) {
    Write-Host ("? - $($question)?") -ForegroundColor Blue
}

function Error([string] $msg) {
    Write-Host ("X - $msg. Aborting.") -ForegroundColor Red
    exit # exit program 
}

function ArrayListString([System.Collections.ArrayList] $list) {
    $out = ""
    foreach ($str in $list) {
        $out += $str + " "
    }
    return $out
}

function CurrDir {
    [OutputType([string])] param()

    return (Get-Location).ToString()
}

$TITLE_TEMPLATE = "RSDRPC version {}"

Push-Location $PSScriptRoot
    if ($Redeploy) {
        Inform("Redeploy parameter specified, running deploy script.")
        . ".\\deploy.ps1"
        Inform("Finished redeploying.")
    }

    Inform("Preparing to create draft release")
    $projDir  = [System.IO.DirectoryInfo] ((CurrDir) + "\..")
    $projName = $projDir.BaseName

    $ghArgs = [System.Collections.ArrayList] @(
        "release",
        "create",
        
        "v$ReleaseVersion",
        "`"$(CurrDir)\$projName-Win64.zip#RSDRPC for 64-bit Windows`"",
        "-t $($TITLE_TEMPLATE.Replace("{}", $ReleaseVersion))",
        "-d"
    )
    if ($Pre) {$ghArgs.Add("-p") | Out-Null}

    Inform("Creating draft release")
    ">   gh $ghArgs"
    $gh = Start-Process -FilePath "gh" -ArgumentList $ghArgs -WindowStyle Hidden -PassThru
    $gh.WaitForExit()

    if ($gh.ExitCode -ne 0) {
        Error("Could not publish release. Error: " + $gh.ExitCode.ToString())
    }

    if (!$NoRedirect) {
        Start-Process "https://github.com/RobloxStudio-DiscordRPC/RPC-Client/releases/edit/v$ReleaseVersion"
    }
Pop-Location