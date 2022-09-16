# gc22-project-13

## How to checkout

Because the project contains submodules, you should 
clone it with ```--recursive``` flag, like so:
```git clone --recursive git@github.com:IS-CG/cg22-project-13.git```.

## How to configure and compile

Firstly, make sure that Qt6 binaries are installed somewhere in the system. 
To install Qt6, refer to the [official qt's website](https://qt.io).

Secondly, we have to point CMake to those installed Qt6 binaries. 
The ```<qt6 bin path>``` should look similarly to ```<Qt install dir>/6.3.2/gcc_64/```.

#### Linux + VSCode with CMake extension
Set cmake extension setting: ```"cmake.installPrefix": "<qt6 bin path>"```.

#### Linux + CLion
File -> Settings -> Build, Execution, Deployment -> CMake: set ```CMake options``` setting
to ```-DCMAKE_PREFIX_PATH=<qt6 bin path>```.

After pointing cmake to qt6 binaries, the project should configure successfully. 
After configuring, the only step left is to compile everything normally.
