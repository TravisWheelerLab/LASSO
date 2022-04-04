// 
// processInputs.h
// Used by UnboundedSubsetSum
// 

#ifndef PROCESSINPUTS_H
#define PROCESSINPUTS_H

/**
 * @brief Runs a quicksort algorithm where the pivot is the first element in the array portion, sorting the input values because the algorithm requires a sorted input set
 * 
 * @param input_set pointer to first value in input set
 * @param first index of first value in array portion
 * @param last index of last value in array portion
 */
void Sort(
  double *input_set, 
  int first, 
  int last ) 
{
  int pivot, i, j;
  if (first < last) {
    pivot = first;
    i = first;
    j = last;
    while (i < j) {
      while(input_set[i] <= input_set[pivot] && i < last)
        i++;
      while(input_set[j] > input_set[pivot])
        j--;
      if(i < j) {
        double temp;
        temp = input_set[i];
        input_set[i] = input_set[j];
        input_set[j] = temp;
      }
    }
    double temp;
    temp = input_set[pivot];
    input_set[pivot] = input_set[j];
    input_set[j] = temp;
    Sort(input_set, first, j - 1);
    Sort(input_set, j + 1, last);
  }
}

/**
 * @brief Process the input parameters before running the algorithm.
 * Error checks the parameters and checks for sorted input. If not sorted, runs Sort()
 * 
 * @param input_set A pointer to the first item in an array of double type values
 * @param input_set_size The number of items in the input_set
 * @param query_value The target value to which combinations must sum
 * @param search_space_comb_len Combination length of the search space
 * @param search_space_min Minimum combination length of search space 
 * @param search_space_max User setting for the maximum search space combination length; if 0, then automated (a reasonable max is dependent on the size of the input input_set and number of queries to be performed)
 * @param dp_precision Total number of decimal places precision
 * @param combin_len If specified, algorithm searches that combination length only; if 0, all combination lengths are searched
 * @param epsilon The value that the target value can vary by
 * @param print_details Require printing of details about the algorithm run
 * 
 * @throws Exits on failure. If print_details==0 no error is printed.
 */
void process_inputs(
  double *input_set,
  int input_set_size,
  double query_value,
  int *search_space_comb_len,
  int search_space_min,
  int search_space_max,
  int dp_precision,
  int combin_len,
  double epsilon,
  int print_details)
{

  // Part 1: Error check input values

    // Calculate search space combination length:
    // If the size is not specified by the user, calculate it
    if (*search_space_comb_len == 0) {
      *search_space_comb_len = (int)(query_value/input_set[input_set_size-1]);
      // If the size is less than the min, set it to the min size
      if (*search_space_comb_len < search_space_min)
          *search_space_comb_len = search_space_min;
      // If the max size is specified and the calculated size is larger, set it to the max size
      if (search_space_max != 0 && *search_space_comb_len > search_space_max)
        *search_space_comb_len = search_space_max;
      // Print if required
      if (print_details)
        printf("Search Space Combination Length: %d\n", *search_space_comb_len);
      
    // If the size is specified by the user, error check it
    } else {
      // Require that the size set by the user is larger than the min...
      if (*search_space_comb_len < search_space_min) {
        if (print_details) 
          printf("\nERROR: Search space combination length cannot be smaller than the minimum specified\n"
          "\tSearch space combination length: %d\n"
          "\tSearch space min length        : %d\n",
          *search_space_comb_len, search_space_min);
        exit(EXIT_FAILURE);
      // ...and smaller than the max IF the max is specified
      } else if (search_space_max != 0 && *search_space_comb_len > search_space_max) {
        if (print_details) 
          printf("\nERROR: Search space combination length cannot be greater than the maximum specified\n"
          "\tSearch space combination length: %d\n"
          "\tSearch space max length        : %d\n",
          *search_space_comb_len, search_space_max);
        exit(EXIT_FAILURE);
      }
    }

  // Error check decimal place precision, combination length, and epsilon
    if (dp_precision < 0 || combin_len < 0 || epsilon < 0.0) {
      if (print_details) 
        printf("\nERROR: Input values cannot be negative\n"
        "\tdp_precision\t\t: %d\n"
        "\tCombination Len\t: %d \t\t(if 0, all valid lengths are checked)\n"
        "\tEpsilon\t\t: %f \t(if 0, only precise solutions are included)\n\n",
        dp_precision, combin_len, epsilon);
      exit(EXIT_FAILURE);
    }
    // Error check user defined search space combination length and user defined search space maximum length
    if (*search_space_comb_len < 0 || search_space_max < 0) {
      if (print_details) {
        printf("ERROR: Zeroboard size and zeroboard max must be >= 0\n");
        printf("\tzeroboard size: %d\n\tzeroboard max : %d\n\n", *search_space_comb_len, search_space_max);
      }
      exit(EXIT_FAILURE);
    }
  // Error check query value
  if (query_value < input_set[0]) {
      if (print_details) printf("\nERROR: Query value cannot be less than input_set minimum\n\tQuery value: %f\n\tinput_set min: %f\n\n", query_value, input_set[0]);
      exit(EXIT_FAILURE);
    } else
      if (print_details) printf("Query Value: %.5f\n", query_value);


  // PART 2: If input is not sorted, sort it

    for (int i=1; i<input_set_size; ++i)
      if (input_set[i-1] > input_set[i])
        Sort(input_set, 0, input_set_size);

    // PART 3: use sorted characteristic to remove duplicates in linear time
      bool duplicates = false;
      // A. check for duplicates
      for (int i=0; i<(input_set_size)-1; ++i) {
        // if value is duplicated, set variable to true and break from iteration
        if (input_set[i] == input_set[i+1]) {
          duplicates == true;
          break;
        }
      }
      // B. if duplicates exist, remove them
      if (duplicates) {
        // create new input_set
        double *remove_duplicates = (double*)malloc(sizeof(double)*input_set_size);
        int counter = 0;
        // copy unqiue values to new input_set 
        for (int i=0; i<input_set_size; ) {
          remove_duplicates[i] = input_set[i];
          counter = i+1;
          while (input_set[counter] == input_set[i])
            ++counter;
          i = counter;
        }
        // C. redirect pointer and reset input_set size
        free(input_set);
        input_set = remove_duplicates;
        input_set_size = counter;
      }

} // end process_inputs()


#endif /* PROCESSINPUTS_H */