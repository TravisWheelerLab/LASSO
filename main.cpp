#include <iostream>

#include "unboundedSubsetSum.h"

int main() {

  double input_set[] = {
    100.0, 120.0, 140.0, 160.0, 180.0, 200.00, 220.0, 240.0, 260.0, 280.0, 300.00, 320.0, 340.0, 360.0, 380.0, 400.00, 420.0, 440.0, 460.0, 480.0
  };
  double query_vals[] = {
    200.0, 400.0, 600.0
  };
  // Calculate query set size for iteration
  int query_set_size   = sizeof(query_vals)/sizeof(query_vals[0]);

  // Iterate over values in query set
  for (int i=0; i<query_set_size; ++i) {
    
    // Parameters
    double query_value   = query_vals[i], 
           epsilon       = 0.0;
    int dataset_size     = sizeof(input_set)/sizeof(input_set[0]),
        print_times      = 1,   // Print all times separately
        print_details    = 1,   // Print details about the algorithm run
        print_comb       = 0,   // Print all combinations of the input set summing to the query value
        print_test_times = 0;   // Print only the total time

    unboundedSubsetSum(&input_set[0], dataset_size, query_value, epsilon, print_times, print_comb, print_details, print_test_times);
  
  } // end for loop

  return 0;
}