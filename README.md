## id3v2 library

Library was created to support some personal applications to organize my music across different machines, locations and storage devices.     
Supports v3 & v4 of the id3v2 spec. Supports text, comment and attached-picture frames.   
Does not support footers (future update will have option to strip footers).   

## special features / tags
PRIV tag (owner mark: `0x99`) contains sha256 hash of the music data 'minus' the id3tag data.

## Build
Library uses `cmake` to generate build assets.   
```shell
# debug build by default
make config build

# release build
make config build BUILD_TYPE=Release
```

## Generate packages
Requires `docker` and copies to `dist/` dir.

```shell
# alma9
make package-alma9
```
