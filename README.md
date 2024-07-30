# FRED FIT

## Version
* 1.0 - Initialization


## Build & Run

With MAPI:
```
git submodule add https://gitlab.cern.ch/alialfred/FREDServer.git core
cd core
cmake3 . -DMAPI=1
make
cd ..
bin/FREDServer
```

Without MAPI:
```
git submodule add https://gitlab.cern.ch/alialfred/FREDServer.git core
cd core
cmake3 .
make
cd ..
bin/FREDServer
```
