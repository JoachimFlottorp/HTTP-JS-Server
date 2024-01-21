Fun little project learn about embedding scripting languages.

This is a very basic HTTP server that can delegate incoming requests to JavaScript functions. It uses duktape as the
JavaScript engine.

This is __Windows Only__ because this directly uses the WinSock2 API.

## Building

__VCPKG__ is preferred.

```powershell
# Download

git clone https://github.com/JoachimFlottorp/HTTP-JS-Server --recursive
cd HTTP-JS-Server

# Build the project

cmake -GNinja -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_ROOT\scripts\buildsystems\vcpkg.cmake"

ninja -C build -v
```

## Running

```powershell
.\build\src\HTTP_JS_Server.exe
```
