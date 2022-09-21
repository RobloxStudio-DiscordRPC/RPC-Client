# BUILDING FROM SOURCE

I'm not sure about Linux or MacOS, this tutorial is preferably for Windows machines.

### Chapters

1. [Get project files](#Get-project-files)
    1. [Clone the repository](#Clone-the-repository)
2. [Download libraries](#Download-libraries)
    1. [Install Qt](#Install-Qt)
    2. [Download other libraries](#Download-other-libraries)
3. [Building](#Building)
    1. [Building](#Building)
    1. [Running](#Running)

## Get project files

First, get the source.

### Clone the repository

Clone this repository to your machine by running the following command in the command line:
```
git clone https://github.com/RobloxStudio-DiscordRPC/RPC-Client
```
You should know how it works.

## Download libraries

To build, you'll need three libraries:
- Qt **(11-37GB)**
- [cpp-httplib](https://github.com/yhirose/cpp-httplib)
- Discord GameSDK

### Install Qt

The first library you'll have to install is Qt.
And before installing, you've got to [create a Qt account](https://login.qt.io/register)

Install Qt using the [online installer](https://www.qt.io/cs/c/?cta_guid=074ddad0-fdef-4e53-8aa8-5e8a876d6ab4&signature=AAH58kE2RVgnpCWyGBENtpRF_GNAz4tZWg&pageId=12602948080&placement_guid=99d9dd4f-5681-48d2-b096-470725510d34&click=58c3e682-aab0-49c2-8c16-c40c999aadee&hsutk=&canon=https%3A%2F%2Fwww.qt.io%2Fdownload-open-source&portal_id=149513&redirect_url=APefjpHS2kXoqiJps2TfaUJaYgNFG2lfQKsTc6Wub8tvk78-ETZyxJukRgt0jG2nZMccfs6x6VLAJUwnulsqLV0Sw_pikV-AQ7OX7OZ1ARDRA0ESoiC27C2Oy-yE4gGGIMCokwtu4a_X).
The online installer also comes with Qt Creator, the IDE for Qt, which we will use to build the application.

Run the installer, fill in everything up until the part where to install the various components of Qt.
I have a few things checked, but I only want you to focus on those with an arrow pointing to them.

![qt install 1](/doc/assets/qtInstall1.png)

![qt install 2](/doc/assets/qtInstall2.png)

- A - Pick the version of MSVC that matches your machine architecture
- B - Might need to check this aswell

**If this doesn't go as planned, please file an issue**

### Download other libraries

In the root of the cloned repository, there should be a batch file with the name `dwlibs.bat`, run that to install cpp-httplib and Discord GameSDK automatically.

## Building

Finally, open Qt Creator, and with Qt Creator, open the cloned repository.

Press the hammer button at the bottom left corner of the window, or press Ctrl-B

### Running

Press the play button at the bottom left corner of the window, or press Ctrl-R
