## Compile
g++ --std=c++11 -O2 -O OptimalPartition OptimalPartition.cpp

## Usage
command CacheSize Prog1 Prog2 ...
CacheSize : Cache sizes in MB
ProgX: footprint file of *x-th* program

## Example
./a.out 128 ./data/astar.dat ./data/lbm.dat

## Important Detail
Cache is partitioned at the granularity of **strip** cache blocks, which is defined in *aux.h* and is 128 by default.