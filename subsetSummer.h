// 
// subsetSummer.h
// Used by UnboundedSubsetSum to write and query a zeroboard 
// 

#ifndef SUBSETSUMMER_H
#define SUBSETSUMMER_H

#include "zeroboard.h"

/**
 * @brief A function to directly query the zeroboard hash-table for combinations summing to a target value. If reuired, prints all combinations summing to target
 * 
 * @param input_set The input dataset
 * @param zeroboard The zeroboard to query for combinations summing to the tare_value
 * @param tare_value Rectified value to query for existence of in the zeroboard
 * @param num_results The counter maintaining the number of combinations summing to the target query value
 * @param array The array maintaining the combination being tracked in query_zeroboard()
 * @param array_size Size of the array maintaining the combination in query_zeroboard()
 * @param combin_len Length of combination to include from the 'array'
 * @param print_comb Require printing of all combinations summing to target value
 */
void get_results(
  double* input_set,
  Board* zeroboard,
  double tare_value,
  unsigned long* num_results,
  int* array,
  int array_size,
  int combin_len,
  int print_comb )
{
  
  if (print_comb) {
    // Using count has the same runtime complexity as the contains function from the C++20 std, except that count is included in C++11
    // Runtime complexity is constant on average with worst case being linear in the size of the container
    if (zeroboard->count(tare_value) != 0) {
      
      if (combin_len == -1) { // this means that the 'array' indexes should not be included
        // Iterate over all combinations summing to tare_value.
        combination_set* set = (*zeroboard)[tare_value];
        while (set != NULL) {
          // Print the combination to the terminal
          for (int i=0; i<set->combination_len; ++i)
            printf("%f ", input_set[set->combination[i]]);
          printf("\n");
          // increment counter of number of combinations summing to the target query value
          ++(*num_results);
          // Move to the next combination set
          set = set->next;
        }

      } else { // else if (combin_len !- -1) which means that we are above the combination length of the zeroboard, so we need to include the 'array' indexes 
        
        combination_set* set = (*zeroboard)[tare_value];
        while (set != NULL) {
          // Check if the combination is valid by comparing its first value to the last included value of the 'array' combination:
          //    Count and print all combinations with (first index < array[combin_len]) as results.
          //    This avoid counting combinations more than once.
          if (set->combination[0] >= array[combin_len]) { // As mentioned above, check if the combination is valid
            for (int i=0; i<combin_len+1; ++i)
              printf("%f ", input_set[array[i]]);
            for (int i=0; i<set->combination_len; ++i)
              printf("%f ", input_set[set->combination[i]]);
            printf("\n");
            ++(*num_results);
            set = set->next;
          } else break; // if the combination is not valid, exit the loop
        }

      } // end if else block (combin_len == -1)

    } // end if (zeroboard->count(tare_value) != 0)

  // else if printing results is not required, repeat the above block without printing all combinations to the terminal
  } else {
    if (zeroboard->count(tare_value) != 0) {
      
      if (combin_len == -1) {
        combination_set* set = (*zeroboard)[tare_value];
        while (set != NULL) {
          ++(*num_results);
          set = set->next;
        }

      } else { // else if (combin_len !- -1) 
        combination_set* set = (*zeroboard)[tare_value];
        while (set != NULL) {
          if (set->combination[0] >= array[combin_len]) { // Check if the combination is valid
            ++(*num_results);
            set = set->next;
          } else break; // if the combination is not valid, exit the loop
        }

      } // end if else block (combin_len == -1)

    } // end if (zeroboard->count(tare_value) != 0)

  } // end if else printing results is/is-not required

}


/**
 * @brief A function to methodically query the zeroboard hash-table using a method that excludes significant portions of the search space 
 * 
 * @param input_set The input dataset
 * @param n The number of values in the input datatset
 * @param zeroboard The hash-table data structure that stores combinations summing to a target value
 * @param search_space_comb_len The length of the generalized solution space contained in the zeroboard
 * @param search_space_min The user-defined minimum combination length to be searched
 * @param dp The number of decimal places precision for the target query value
 * @param query_val The target query value
 * @param combination_length The user-defined value that determines a specific combination length to search; 0 by default searches all lengths
 * @param epsilon The amount by which the target query value can vary
 * @param print_details Reuirement to print details about the algorithm run
 * @param print_comb Requirement to print all combinations summing to the target value
 */
void queryZeroBoard(
  double *input_set,
  int n,
  Board* zeroboard,
  int search_space_comb_len,
  int search_space_min,
  int dp,
  double query_val,
  int combination_length,
  double epsilon,
  int print_details,
  int print_comb )
{

  // ** Function Variables **
    // if the user specifies the zb size, set it to that size
    if (search_space_comb_len != 0)
            search_space_comb_len = search_space_comb_len;
    int     curr_comb_len   = (int)(query_val/input_set[0]), 
            n_zeroBased     = n-1,
            end_length      = search_space_comb_len;
    unsigned long 
            resultsCounter  = 0,
            totalResults    = 0;
    double  tare_value      = 0.0,
            dec_places      = pow(10,dp),
            input_set_max           = input_set[n_zeroBased],
            query_val_dp    = query_val*dec_places;
    // check for minimum length combination
    if (curr_comb_len < search_space_min)
      curr_comb_len = search_space_min; 
    // if combination length set, only search that length
    if (combination_length != 0) {
      curr_comb_len           = combination_length;
      end_length            = combination_length-1;
    }
    // tracking arrays
    int     array_size      = curr_comb_len-search_space_comb_len,
            array[array_size];
    double  mins[array_size],
            maxs[array_size],
    // set key sequences
            keySeq[n],
            keySeqSum[n],
            keySeqSumFromEnd[n];
    for (int i=0; i<n; ++i) {
      if (i == 0) {
        keySeq[i]     = 0.0;
        keySeqSum[i]  = 0.0;
      } else {
        keySeq[i]     = input_set[i] - input_set[i-1];
        keySeqSum[i]  = keySeqSum[i-1] + keySeq[i];
      }
    }
    for (int i=0; i<n; ++i) {
      if (i == 0)
        keySeqSumFromEnd[i] = 0.0;
      else
        keySeqSumFromEnd[i] = keySeqSumFromEnd[i-1] + keySeq[n-i];
    }
  // *** End Function Variables ***

  if (print_details) printf("Combination length : Num Results\n");

  // *** Begin Iterating Through Search Space ***

  // iterate through valid combination lengths above minimum
  while (curr_comb_len > end_length && curr_comb_len*input_set[n_zeroBased] >= query_val) {
    int     dim      = 0;
    double  comb_max = curr_comb_len*input_set[n_zeroBased],
            comb_min = curr_comb_len*input_set[0],
         comb_max_dp = comb_max*dec_places;
    
    // If the maximum value for this combination length == the query value then report/count the combination and move to next combination length
    if ((long long)comb_max_dp == (long long)query_val_dp) {
      if (print_comb) {
        for (int i=0; i<curr_comb_len; ++i)
          printf("%f ", input_set[n_zeroBased]);
        printf("\n");
      }
      ++resultsCounter;

    // Else if the minimum value for this combination length == the query value then report/count the combination and move to next combination length
    } else if ((long long)(comb_min*dec_places) == (long long)query_val_dp) {
      if (print_comb) {
        for (int i=0; i<curr_comb_len; ++i)
          printf("%f ", input_set[0]);
        printf("\n");
      }
      ++resultsCounter;

    // Else if the query value lies somewhere between the minimum and maximum values for the current combination length, search for valid combinations
    } else {
      // set tracking arrays
      for (int i=0; i<array_size; ++i) {
        mins[i] = input_set[0]*curr_comb_len;
        maxs[i] = input_set[0]*(i+1) + input_set[n-1]*(curr_comb_len-(i+1));
        array[i] = 0;
      } 

      // Logic: As long as the minimum combination length for this section of search space is less than the query value, continue searching
      while (mins[0] <= query_val && dim < curr_comb_len-search_space_comb_len) {
        // Max finding
        while (dim <= curr_comb_len-search_space_comb_len && array[dim] < n_zeroBased) {
          // Move up along the max values till the max value is greater than the query value
          // Logic: As long as the max value of the search space is less than the query value those sections of search space cannot contain valid combinations.
          //        If it is equal to teh query value, there could be a valid combination there so check the zeroboard and move on.
          while (maxs[dim] <= query_val && array[dim] < n_zeroBased) {
            // Check zeroboard if the maxs value is the query value
            if ((long long)maxs[dim]*(long long)dec_places == (long long)query_val*(long long)dec_places) {
              int u = 0;
              double tare_value = 0.0;
              while (u < curr_comb_len - search_space_comb_len) {
                tare_value = tare_value - ((input_set_max)-input_set[array[u]]);   // keySeqSumFromEnd[n_zeroBased-array[u]];
                ++u;
              }
              tare_value = (tare_value + (comb_max - query_val));
              
              // Check the zeroboard using calculated tare mass
              get_results(input_set, zeroboard, tare_value, &resultsCounter, &array[0], array_size, curr_comb_len-search_space_comb_len-1, print_comb);
            
            } // end if, ie. if its not equal, move on to the next combination

            ++array[dim];
            maxs[dim] = maxs[dim] + (input_set[array[dim]]-input_set[array[dim]-1]);   //keySeq[array[dim]];
          }

          // Reset the maxs array before moving to the next section of the search space
          if (dim < curr_comb_len - search_space_comb_len - 1 && array[dim] < n_zeroBased) {
            ++dim;
            array[dim] = array[dim-1];
            maxs[dim] = maxs[dim-1] - (input_set[n_zeroBased]-input_set[array[dim-1]]);    // keySeqSumFromEnd[n_zeroBased - array[dim-1]];
          } else
            break;
        }
        // Min finding
        if (array[dim] <= n_zeroBased) {
          // Reset the mins array for current position in the combination
          mins[dim] = maxs[dim] - (input_set[n_zeroBased]-input_set[array[dim]])*search_space_comb_len;
          // Logic: As long as the minimum combination sum for this part of the search space is less than the query value, 
          //        the search space could contain valid combinations so we must calculate new tare values and check the zeroboard
          while (mins[dim] <= query_val && array[dim] < n_zeroBased) {
            // Check zeroboard
            int u = 0;
            double tare_value = 0.0;
            while (u < curr_comb_len - search_space_comb_len) {
              tare_value -= (input_set_max-input_set[array[u]]);   // keySeqSumFromEnd[n_zeroBased-array[u]];
              ++u;
            }
            tare_value += (comb_max - query_val);

            get_results(input_set, zeroboard, tare_value, &resultsCounter, &array[0], array_size, curr_comb_len-search_space_comb_len-1, print_comb);
            
            ++array[dim];
            mins[dim] = mins[dim] + (input_set[array[dim]]-input_set[array[dim]-1])*(search_space_comb_len+1);
          }
        }
        // Maintain the combination tracking array
        while (mins[dim] > query_val && dim > 0) {
          if (dim > 0) {
            --dim;
            ++array[dim];
            double temp = 0;
            int u = 0;
            while (u < dim) {
              temp = temp + input_set[array[u]];
              ++u;
            }
            mins[dim] = temp + input_set[array[dim]]*(curr_comb_len-dim);
          } else if (dim == 0)
            mins[dim] = comb_min + keySeqSum[array[dim]]*curr_comb_len;
        }
        // Reset dimensions of combination tracking array that are less than the current combination length
        int r = dim+1;
        while (r < curr_comb_len-search_space_comb_len) {
          array[r] = array[r-1];
          ++r;
        }
        ++dim;
        // Recalculate the maximum combination length for this section of search space
        maxs[dim] = mins[dim-1] + (input_set[n_zeroBased]-input_set[array[dim]]) * (curr_comb_len-(dim+1));
      
      } // end  while (mins[0] <= query_val ...
    } // end if else else block
    
    if (print_details) printf("\t%d\t\t%lu\n", curr_comb_len, resultsCounter);
    totalResults = totalResults + resultsCounter;
    resultsCounter = 0;
    --curr_comb_len;
  }

  // *** END Iterating Through Search Space ***

  // Check minimum combination length (the length of combination stored in the zeroboard)
  if (combination_length == 0) {
    // Calculate tare_value
    tare_value = (curr_comb_len*input_set[n_zeroBased] - query_val);
    // Check if there are combinations summing to the target
    get_results(input_set, zeroboard, tare_value, &resultsCounter, &array[0], array_size, curr_comb_len-search_space_comb_len-1, print_comb);
    // Print number of combinations summing to target if required
    if (print_details) printf("\t%d\t\t%lu\n", curr_comb_len, resultsCounter);
    // Add number of combinations summing to target for final combination length to the total count
    totalResults += resultsCounter;
  }

  // If included in minimum combination size, check combination length of 2
  resultsCounter = 0;
  if (combination_length==0 && search_space_min==3 || combination_length==2) {
    for (int i=0; i<n; ++i) // doubly nexted for loop to check for combinations of length 2 summing to the target query value
      for (int j=i; j<n; ++j)
        if (input_set[i]+input_set[j]==query_val) {
          resultsCounter++;
          if (print_comb) printf("%f %f\n",input_set[i],input_set[j]);
        }
    // end doubly nested for loop
    if (print_details) printf("\t2\t\t%lu\n", resultsCounter);
    totalResults += resultsCounter;
  }
  
  // If required, end by printing total number of combinations summing to target
  if (print_details) printf("\nTotal results: %lu\n\n", totalResults);
}


/**
 * @brief A function to write the contents of a zeroboard, a hash-table based data structure which stores combinations from an input set that sum to the key value of each bin
 * 
 * @param input_set The input set
 * @param zeroboard The zeroboard to write combinations and sums into
 * @param n The number of items in the input set
 * @param search_space_comb_len The length of the generalized solution space contained in the zeroboard
 */
void writeZeroBoard (
  double *input_set,
  Board* zeroboard,
  int n,
  int search_space_comb_len )
{

  // Function Variables: 
  //  Input set size in zero-based counting
  int n_zerobased    = n-1,
  //  Counter used in creating combinations
      counter        = 0,
  //  Size of combination index tracking array
  //  The -2 accounts for the fixed column/row iteration that occurs in this function
      combination_tracker_len = search_space_comb_len - 2;
  //  Maximum value in input set
  double input_set_max = input_set[n_zerobased];
  
  // *** Begin Writing Zeroboard Hash-Table ***
  
  // Logic for this if gate: if the combination_tracker array has a length greater than 0, the combination length being stored in the zeroboard is more than two so use the array.
  // Using the array requires more complexity than a doubly nexted for loop as below in the else block attached to this if gate.
  if (combination_tracker_len > 0) {
    
    //  A tracking array used to create all correct combinations allowing for repetition of input set elements
    int combination_tracker[combination_tracker_len];
    // Initialise combination tracker array values
    for (int i=0; i<combination_tracker_len; ++i)
        combination_tracker[i] = 0;

    // Iterate through tracking array (which maintains tracking of current zeroboard triangle)
    // Logic: When the value at the last index of combination_tracker is == input set length, every commbination has been generated
    while (combination_tracker[combination_tracker_len-1] <= n_zerobased) {
      // Logic: When the value at the first index of combination_tracker is == input set length, increment the combination_tracker array
      while (combination_tracker[0] <= n_zerobased) {

        // iterate through columns of current zeroboard triangle
        int colCounter = combination_tracker[0];
        // Logic: when the value of the column counter gets to the length of the input set, stop making combinations for this part of the search space
        while (colCounter <= n_zerobased) {

          // iterate through columns and rows of current zeroboard triangle
          int rowCounter = colCounter;
          // Logic: when the value of the row counter gets to the length of the input set, stop making combinations for this part of the search space
          while (rowCounter <= n_zerobased) {
            
            // Calculate new value for insertion according to position in zeroboard (by tracker array and row/col counters)
            double combination_sum = (input_set_max - input_set[colCounter]) + (input_set_max - input_set[rowCounter]);
            counter = 0;
            // Create new combination set
            int comb_set_size = combination_tracker_len + 2,
                counter       = 0,
                *combination  = (int*)malloc(sizeof(int)*comb_set_size);
            combination[comb_set_size-1] = rowCounter;
            combination[comb_set_size-2] = colCounter;
            // Iterate through the length of the combination and calculate the tare value as well as set the values 
            // in the combination array that will be inserted into the zeroboard hash-table bin with key 'combination_sum'
            while (counter < combination_tracker_len) {
              combination_sum += (input_set_max - input_set[combination_tracker[counter]]);
              combination[comb_set_size - counter - 3] = combination_tracker[counter];
              ++counter;
            }
            
            // Insert new tare value into zeroboard with associated combination set
            board_insert(zeroboard, combination_sum, combination, comb_set_size);

            // Increment counters
            ++rowCounter;
          }
          ++colCounter;
        }
        ++combination_tracker[0];
      
      } // end while (combination_tracker[0] <= n_zerobased)
      
      // Maintain tracking array
      int decrementor = 0;
      while (combination_tracker[decrementor] >= n-1 && decrementor < combination_tracker_len-1)
        ++decrementor;
      ++combination_tracker[decrementor];
      while (decrementor > 0) {
        --decrementor;
        combination_tracker[decrementor] = combination_tracker[decrementor+1];
      }
      // end Maintenance of tracking array

    } // end while (combination_tracker[combination_tracker_len-1] <= n_zerobased)
  
  } else { // else if combination_tracker_len <= 0 (it will only ever be 0 in this case)
  // This block means the algorithm only requires summing and storing combinations of length 2. 

    // Iterate through columns of current zeroboard triangle
    int colCounter = 0;
    // Logic: when the value of the column counter gets to the length of the input set, stop making combinations for this part of the search space
    while (colCounter <= n_zerobased) {

      // Iterate through columns and rows of current zeroboard triangle
      int rowCounter = colCounter;
      // Logic: when the value of the row counter gets to the length of the input set, stop making combinations for this part of the search space
      while (rowCounter <= n_zerobased) {
        
        // Calculate new value for insertion according to position in zeroboard (by tracker array and row/col counters)
        double combination_sum = (input_set_max - input_set[colCounter]) + (input_set_max - input_set[rowCounter]);
        counter = 0;
        // Create new combination set
        // Create new combination set
        int comb_set_size = combination_tracker_len + 2,
           *combination   = (int*)malloc(sizeof(int)*comb_set_size),
            counter       = 0;
        combination[comb_set_size-1] = rowCounter;
        combination[comb_set_size-2] = colCounter;
        
        // Insert new combination sum into zeroboard with associated combination set
        board_insert(zeroboard, combination_sum, combination, comb_set_size);

        // Increment combination counters
        ++rowCounter;
      }
      ++colCounter;
    }
  } // end if else combination_tracker_len>0

  // *** End Writing Zeroboard Hash-Table ***

} // end function writeZeroBoard()


#endif /* SUBSETSUMMER_H */