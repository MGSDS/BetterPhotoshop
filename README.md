# gc22-project-13

## How to checkout

Because the project contains submodules, you should 
clone it with ```--recursive``` flag, like so:
```
git clone --recursive git@github.com:IS-CG/cg22-project-13.git
```

## How to configure and compile

Firstly, make sure that Qt6 binaries are installed somewhere in the system.

### How to install qt6
There are 2 general strategies
1. Installer from [official qt's website](https://www.qt.io/download-qt-installer).
   - installs all needed dependencies
   - installs some other stuff (5+ Gb)
2. Local qt source building based on [official guide](https://wiki.qt.io/Building_Qt_6_from_Git)
   - deals with required modules only - faster and takes less disk space\
     On init-stage add `--module-subset=qtbase`, so
     ```sh
     perl init-repository --module-subset=qtbase
     ```
     On configure stage it is not needed to make examples, add `-nomake examples` to safe your time.\
     We don't recommend to apply `-nomake tests` since there are some which help to detect unsatisfied
     dependencies (e.g., OpenGL implementation).
   - you have to load required dependencies (listed in [the guide](https://wiki.qt.io/Building_Qt_6_from_Git)) before building.

Secondly, we have to point CMake to those Qt6 binaries. 
The ```<qt6 bin path>``` should look similarly to ```<Qt install dir>/6.3.2/gcc_64/```.

#### Linux + VSCode with CMake extension
Set cmake extension setting: ```"cmake.installPrefix": "<qt6 bin path>"```.

#### Linux + CLion
File -> Settings -> Build, Execution, Deployment -> CMake: set ```CMake options``` setting
to ```-DCMAKE_PREFIX_PATH=<qt6 bin path>```.

After pointing cmake to qt6 binaries, the project should configure successfully. 
After configuring, the only step left is to compile everything normally.
