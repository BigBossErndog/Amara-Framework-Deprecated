# Amara
A C++ game framework built on top of SDL2. Revolves around Actors reciting Scripts, with a flow inspired by the Phaser.io JS framework.

Being used to make the games for Project: Retched Garden.
https://twitter.com/RetchedGarden

Currently being used to make Pry Into The Void:
https://store.steampowered.com/app/1234990/Pry_Into_The_Void/

## State of the project

Enough features to make a full game, still working on more features to match the capabilities of other frameworks.

This will be updated as the framework becomes more feature complete, documentation will also be in the works eventually.
MinGW is required for building on Windows.

Please reach out to me if you would be interested in learning how to use it.
www.twitter.com/bigbosserndog
DMs are open.


## Why does this exist?

Alternative work flow based on Actors reciting Scripts, making it easier to program event based behaviors into the game. An easy to use state machine is built into Scripts which makes it easy to give sequential instructions to an Actor for things like enemy AI and moving characters in cutscenes. Multiple Scripts can run on the same Actor in parallel, and they can be chained together too. This makes decoupling entities from their behaviors very easy.

The design of Amara is heavily inspired by the Phaser.io Javascript framework, with the use of Scenes and Entities. I just found it very intuitive and easy to use, so I wanted to use something similar in native development. 

## No GUI?
The focus right now is to make a full game with it, meaning making a GUI for this engine is a job for another day.
Keeping it as a library framework is sufficient enough for me right now.

## Features
 - Actors which can 'recite' Scripts, which can be started and stopped arbitrarily with multiple Scripts running parallel on the same Actor, chained together and more. Allows for easy decoupling of code and their entities and reuse of code.
 - Data based, nearly every Entity can be converted to and from JSON data with configure() and toData(), with lots of support for manipulating JSON data thanks to nlohhmann::json (https://github.com/nlohmann/json)
 - All your typical stuff for game development, sprites, tilemaps, animations, audio, window management and more.
 - Support for Tiled tilemaps in the JSON format.
 - Similar flow to the Phaser.io framework (with Scenes, Entities and Actors), which I have used for most of my game development life.
 - Manages memory for you. Just call the destroy() method on your entities and Amara will handle it for you, deleting the object, its children and removing references from its parent entities. Amara will also handle the deletion of all your entities when you stop a scene.

## Setting Up
Clone the repository then follow the instructions for setting up Amara on your development platform of choice.
```
git clone https://github.com/BigBossErndog/Amara-Framework.git
cd amara-framework
```
A Makefile with many useful commands for setting up Amara, compilation and playing the game are available.

### Windows
You will find that the project is already set up for Windows, complete with all the necessary headers and dlls :)
You just need to have MinGW set up with make and the g++ compiler.

#### Linux-like Environment Setup

To set up a linux-like environment I am using GNUWin.

Download and install coreutils-5.3.0.exe from the following:  
https://sourceforge.net/projects/gnuwin32/files/coreutils/5.3.0/  
Once installed, add the following include paths to the system environment variables:  
````
C:\Program Files (x86)\GnuWin32\bin
````
You may need to alter the path if you've installed it somewhere else.  

For the make command download and install make-3.81.exe from the following:  
http://sourceforge.net/projects/gnuwin32/files/make/3.81  
It will install to the same folder as coreutils so no more steps are needed.  


#### GCC Compiler Setup

I am using MinGW-w64's version of the gcc compiler to compile for windows.  
Download the latest UCRT Win64 build found by scrolling down on the following page:  
https://winlibs.com  
Extract and place the inner folder mingw64 wherever you'd like.  
Add the following include path to the system environment variables:  
````
C:\(Path to mingw64 folder)\bin
````

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

### Console
I currently do not have access to any console SDKs. I hope to work on plugins for console support in the future as I gain more experience in commercial game development.
With this being an open source project abd the legal nature of console SDKs, I don't think I can include any future console plugin in this repository. If console support will be supported in the future, you will have to contact me directly on Twitter (@bigbosserndog) for access to it.

## Compilation
Once you have all your files in the src folder with a main.cpp, you can compile the code. Unfortunately compiling all your code is the only option available right now, there is no way to compile a debug test build.
### Windows
```
make win64
```
### Linux
```
make linux
```

## Playing your game
You can either run your newly compiled executable from the build directory, or you can do the following.
### Windows
```
make playwin
```
### Linux
```
make playlinux
```
