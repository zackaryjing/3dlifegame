# Construct

lib installation:
```shell
vcpkg install glfw3
vcpkg install glm
vcpkg install glad
```


if you are using vcpkg, make sure you added this into your build command.
```
-DCMAKE_TOOLCHAIN_FILE=~/vcpkg/scripts/buildsystems/vcpkg.cmake
```

if you want to specify the toolchain
```
-DCMAKE_TOOLCHAIN_FILE=...\vcpkg\scripts\buildsystems\vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-mingw-dynamic
```