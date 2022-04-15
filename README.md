# Unbounded Subset Sum
With this algorithm I present a solution to the Unbounded Subset Sum Problem (USSP). The algorithm makes use of repeating commbinatorial patterns to leverage repetition in reducing the runtime that is required to retrieve all possible combinations summing to a query/target value. For application of this algorithm to a real-world problem in analytical chemistry, [see this repository](https://github.com/koos-burgoyne/mass-decomposition). Publications resulting from these works are forthcoming.

## Algorithm Synopsis
The algorithm operates in two parts:
* The first part writes all combinations of a specified combination length to local memory, making it relatively fast to retrieve this information later. This is a generalizable solution space because these combinations are repeated many times within the search space making it possible to only calculate them once.
* The second part uses a branch and bound technique to query that generalizable solution space for combinations summing to the query/target value.

## Boost
Note that this algorithm relies on the Boost::unordered_map header to operate. In some Linux-based operating systems, Boost comes with the installation, e.g. `/usr/include/boost`. The Gnu compiler will typically find the Boost library in a Unix type operating system but if you are using Windows, you might need to use the `-I <boost source directory>` flag when compiling with the Gnu compiler.  
  
If your operating system does not come with Boost, you can use it by simply [downloading](https://www.boost.org/users/download/) Boost and unzipping the archive to the desired directory.

## Build
This algorithm was originally written in C but makes use of the Boost hash table library so it is compiled in C++.  
You can build from the source code using CMake and the CMakeLists.txt file included in this repository. In a Unix type operating system, the following steps can be applied from the root directory of this repository:
```
cmake .
```
```
./build/uss
```

## Run
This algorithm does not take command line arguments. As such, you can simply run the algorithm using the format provided in the example `main.cpp`.  
  
If running in Visual Studio Code in Windows, you can use the .vscode directory stored in this repository to run the algorithm in debugging mode as written up in the example. Note that this algorithm uses the Boost library so you will need to have a Boost installation for the CMake builder to find.

## Usage
When using the algorithm in your own project, import the main header file, e.g. `#include "<source directory>/unboundedSubsetSum.h"`, and then call the `unboundedSubsetSum()` function with the following parameters:
```
input_set         : An array filled with the input set, using double type
input_set_size    : The number of values in the input set 
query_value       : The query/target value to which combinations must sum
epsilon           : The amount by which the query value can vary, double type
print_times       : Print time taken by functions called during the algorithm (0 or 1)
print_details     : Print details relating to an algorithm rum (0 or 1)
print_comb        : Print all combinations summing to the query value (0 or 1)
print_test_times  : Print the overall time taken by an algorithm run (0 or 1)
```

## Example
Using the algorithm is fairly straightforward. You can see an example of usage in the `source/main.cpp` file found in this repository.