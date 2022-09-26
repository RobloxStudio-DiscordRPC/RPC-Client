QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# QttpServer
#CONFIG += debug libraries/QttpServer/qttpserver.pro

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

## discord game sdk (auto-generated by qt creator)
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libs/discord_game_sdk/lib/ -ldiscord_game_sdk
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libs/discord_game_sdk/lib/ -ldiscord_game_sdk
else:unix: LIBS += -L$$PWD/libs/discord_game_sdk/lib/ -ldiscord_game_sdk

INCLUDEPATH += $$PWD/libs/discord_game_sdk/cpp
DEPENDPATH += $$PWD/libs/discord_game_sdk/cpp

SOURCES += $$files(*.cpp, true)
#SOURCES -= $$files(libs/*, true)

HEADERS += $$files(*.h, true)
HEADERS -= $$files(libs/*, true)

FORMS  = $$files(*.ui, true)
UI_DIR = ./interface

# Program resources/metadata

VERSION   = 1.0.0.4
RESOURCES = assets/assets.qrc
RC_ICONS  = assets/app.ico

QMAKE_TARGET_COMPANY = MatusGuy
QMAKE_TARGET_DESCRIPTION = $$quote(Discord rich presence for Roblox Studio)
QMAKE_TARGET_COPYRIGHT = $$quote(   © 2022-2023 MatusGuy)
QMAKE_TARGET_PRODUCT = $$quote(Roblox Studio Discord RPC)
QMAKE_TARGET_ORIGINAL_FILENAME = RobloxStudio-DiscordRPC.exe
QMAKE_TARGET_INTERNALNAME = RSDRPC

# Ability to access version in source code
DEFINES += __VERSION__=$$quote(\\\"$${VERSION}\\\")

# libraries

## OPENSSL

win32:OPENSSL = $$[QT_INSTALL_PREFIX]/../../Tools/OpenSSL/Win_x64/

LIBS += -L$$OPENSSL/lib -llibcrypto
LIBS += -L$$OPENSSL/lib -llibssl

INCLUDEPATH += $$OPENSSL/include/openssl

## httplib and possibly others
INCLUDEPATH += libs/

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


