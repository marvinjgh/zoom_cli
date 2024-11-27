# Zoom shortcut

A cli shortcut, to open Zoom Meeting Room.

## Using

This program read the file `~/.zoom_meeting`, each line has the info of a meetting.

Can also pass a url metting.

## Compile

This proyect use CMake.

### macOX

``` sh
# create folder 'out' and go
mkdir our && cd out
 
# create build config files
cmake ..

# create executed file
make
```

### Windows

On Windows I use Visual Studio 2022.  

## TODO

- [ ] Add linux configuration
- [ ] Add delete room