# Construct

lib installation:
```shell
vcpkg install glfw3
vcpkg install glm
vcpkg install glad
vcpkg install freetype
vcpkg install imgui
```

or using vcpkg.json: 

```shell
vcpkg install .
```


if you are using vcpkg, make sure you added this into your build command.
```
-DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake
```

if you want to specify the toolchain
```
-DCMAKE_TOOLCHAIN_FILE=...\vcpkg\scripts\buildsystems\vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-mingw-dynamic
```


loc count:
```shell
 git ls-files | cloc --list-file=- --exclude-dir=assets,deps
```


**Important**

Vcpkg doesn't have imgui@1.92.3, it only has 1.91.9. 
I'm currently using vcpkg overlay to solve the problem.

If you are using clion, make sure you have VCPKG_OVERLAY_PORTS in your cmake environment.
eg: `VCPKG_OVERLAY_PORTS=/home/<username>/vcpkg/vcpkg-overlays`

