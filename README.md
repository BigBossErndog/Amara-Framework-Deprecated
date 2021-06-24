# Amara
A C++ game framework built on top of SDL2. Based on Actors and Script recitals, with a scene architecture inspired by the Phaser.io JS framework.

## State of the project

Enough features to make a full game, still working on more features to match the capabilities of other frameworks.

This will be updated as the framework becomes more feature complete, documentation will also be in the works eventually.
For building on Windows, either CMake or MinGW will be necessary.

Please reach out to me if you would be interested in learning how to use it.
www.twitter.com/bigbosserndog
DMs are open.


## Why does this exist?

Alternative work flow based on Actors reciting Scripts, making it easier to program event based behaviors into the game. An easy to use State Machine is built into Scripts which makes it easy to give sequential instructions to an Actor for things like enemy AI and moving characters in cutscenes. Multiple Scripts can run on the same Actor in parallel, and they can be chained together too.

Some design decisions are also borrowed from the Phaser.io Javascript framework, which I like and became accustomed to. I wanted to emulate the architecture of some of my web games in C++, built on top of SDL2 meaning easy to build native cross-platform games for many devices such as Nintendo Switch which does not currently support web games. I just want to be comfortable making games, and so should you.

## No GUI?
The focus right now is to make a commercial game with it, meaning making a GUI for this engine is a job for another day.
I'm not personally a fan of game engine GUIs, so keeping it as a library framework is suitable for me right now.

## Features
 - Actors which can 'recite' Scripts, which can be started and stopped arbitrarily with multiple Scripts running parallel on the same Actor, chained together and more. Allows for easy decoupling of code and their entities and reuse of code.
 - Data based, nearly every Entity can be converted to and from JSON data with configure() and toData().
 - All your typical stuff for game development, tilemaps, 
 - Support for Tiled tilemaps in the JSON format.
 - Similar flow to the Phaser.io framework (with Scenes, Entities and Actors), which I have used for most of my game development life.

## Setting Up
Clone the repository then follow the instructions for setting up Amara on your development platform of choice.
```
git clone https://github.com/BigBossErndog/Amara-Framework.git
cd amara-framework
```

### Windows
You will find that the project is already set up for Windows, complete with all the necessary headers and dlls :)
You just need to have MinGW set up with the g++ compiler.
https://sourceforge.net/projects/mingw/

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
#### Pacman (Manjaro / Arch Linux) 64-bit
```
make setup-pacman64
```


### MacOS
As I don't have access to a Mac, I cannot provide a way to build for MacOS right now.

