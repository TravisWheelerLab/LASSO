// 
// unboundedSubstSum.h
// Runs UnboundedSubsetSum function
// 

#ifndef UNBOUNDEDSUBSETSUM_H
#define UNBOUNDEDSUBSETSUM_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <vector>

#include <boost/unordered_map.hpp>

#include "processInputs.h"
#include "subsetSummer.h"


/**
 * @brief Runs the functions to process input parameters, write a zeroboard to memory, query the zeroboard, and then delete dynamically applied portions of the zeroboard
 * 
 * @param input_set The input set: a pointer to the first item in an array of double type values
 * @param input_set_size The number of items in the input set
 * @param query_value The target value to which combinations must sum
 * @param epsilon The value that the target value can vary by
 * @param print_times Require printing of runtimes for each of the three tasks defined in the synopsis above
 * @param print_comb Require printing of all commbinations summing to the target value 
 * @param print_details Require printing of details about the algorithm run
 * @param print_test_times Require printing of total runtime
 */
void unboundedSubsetSum(
  double* input_set,
  int input_set_size, 
  double query_value, 
  double epsilon, 
  int print_times, 
  int print_comb, 
  int print_details, 
  int print_test_times) 
{
  
  if (print_details) printf("\n *** Running Unbounded Subset Sum Algorithm: ***\n\n");

  // Timing variables
  clock_t start, finish;
  double time_used_write, time_used_query, time_used_delete, total_time_used = 0;
  
  // Assignment of algorithm variables
  int     search_space_comb_len = 0,    // Combination length of the search space
          search_space_min      = 3,    // Minimum combination length of search space 
          search_space_max      = 7,    // User setting for the maximum search space combination length; if 0, then automated (a reasonable max is dependent on the size of the input input_set and number of queries to be performed)
          dp_precision          = 5,    // Total number of decimal places precision
          combination_length    = 0;    // If specified, algorithm searches that combination length only; if 0, all combination lengths are searched
  
  // Error check input values, sort input input_set, and remove duplicates
  process_inputs(input_set, input_set_size, query_value, &search_space_comb_len, search_space_min, search_space_max, dp_precision, combination_length, epsilon, print_details);
  
  // create the zeroboard
  start              = clock();
    Board zeroboard;
    writeZeroBoard(input_set, &zeroboard, input_set_size, search_space_comb_len);
  finish             = clock();
  time_used_write    = ((double) (finish - start)) / CLOCKS_PER_SEC;
  total_time_used   += time_used_write;

  // print the contents of the zeroboard - only a reasonable task for small zeroboard values
  // print_zeroboard(&zeroboard);

  // query the zeroboard
  start         = clock();
    queryZeroBoard(input_set, input_set_size, &zeroboard, search_space_comb_len, search_space_min, dp_precision, query_value, combination_length, epsilon, print_details, print_comb);
  finish        = clock();
  time_used_query = ((double) (finish - start)) / CLOCKS_PER_SEC;
  total_time_used   += time_used_query;

  // free heap memory used by the zeroboard
  start         = clock();
    delete_zeroboard(&zeroboard);
  finish        = clock();
  time_used_delete = ((double) (finish - start)) / CLOCKS_PER_SEC;
  total_time_used   += time_used_delete;

  // print time taken for each function
  if (print_times) {
    printf("%f seconds to create zeroboard\n", time_used_write);
    printf("%f seconds to query zeroboard\n", time_used_query);
    printf("%f seconds to free zeroboard memory\n", time_used_delete);
    printf("%f seconds total\n\n", total_time_used);
  }
  // print time taken overall
  if (print_test_times) {
    printf("%f\n", total_time_used);
  }
  if (print_details) printf(" *** End of Algorithm Run ***\n");

}

#endif /* UNBOUNDEDSUBSETSUM_H */