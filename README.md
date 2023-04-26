# CloverSim

## requirements
 - yaml-cpp (https://github.com/jbeder/yaml-cpp)
 - Geant4.10 (It doesn't work with 4.11.)
 
## installation
```
git clone https://github.com/rin-yokoyama/cloversim.git
cd cloversim
mkdir build
cd build
cmake .. (-DCMAKE_INSTALL_PREFIX=[path_to_yaml-cpp_if_not_found])
make; make install
```

## usage
```
cloversim -c config.yaml
```
