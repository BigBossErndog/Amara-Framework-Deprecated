# Amara
A C++ game framework built on top of SDL2. Based on Actors and Recitals.

## State of the project


Enough features to make a full game, still working on more features to match the capabilities of other frameworks.

This will be updated as the framework becomes more feature complete, documentation will also be in the works eventually.
For building on Windows, either CMake or MinGW will be necessary.

Please reach out to me if you would be interested in learning how to use it.
www.twitter.com/bigbosserndog
DMs are open.

## Setting Up
Clone the repository then follow the instructions for setting up Amara on your development platform of choice.
```
git clone https://github.com/BigBossErndog/Amara-Framework.git
cd amara-framework
```

### Windows
You will find that the project is already set up for Windows, complete with all the necessary headers and dlls :)

### Linux
The following `make` commands will set up Amara depending on which package management system you use.
#### Apt (Ubuntu / Debian) 64-bit
```
make setup-apt64
```
#### Yum (Fedora) 64-bit
```
make setup-yum64
```
#### Pacman (Majaro / Arch Linux) 64-bit
```
make setup-pacman64
```


### MacOS
As I don't have access to a Mac, I cannot provide a way to build for MacOS right now.

## Why does this exist?

Alternative work flow based on Actors and Scripts which the Actors recite, making it easier to program event based behaviors into the game. And easy to use State Machine is built into Scripts which makes it easy to give sequential instructions to an Actor for things like enemy AI and moving characters in cutscenes.

Some design decisions are also borrowed from the Phaser.io Javascript framework, which I like and became accustomed to. I wanted to emulate the architecture of some of my web games in C++, as my eventual aim will be to build native cross-platform games for many devices such as Nintendo Switch.
