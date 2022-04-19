// 
// zeroboard.h
// Contains structures and functions defining the zeroboard data structure which allows for fast querying of combinations.
// Used by functions in UnboundedSubsetSum and subsetSummer.
// 

#ifndef ZEROBOARD_H
#define ZEROBOARD_H

#define PRECISION 1e15 

/**
 * @brief A struct that holds an array of indexes and is part of a linked list in a hash-table bucket. When those indexes are read as values of the input dataset, combinations stored in this struct will sum to the key of the bucket.
 * 
 * @param combination Pointer to an array of indexes that are associated with specific values in the input dataset
 * @param combination_len Number of indexes stored in combination (i.e. length of the permutation or no. of values making up the query value)
 * @param prev Pointer to the previous combination set in the list
 * @param next Pointer to the next combination set in the list
 */
struct combination_set {
  int* combination;
  int combination_len;
  combination_set* next;
};

/**
 * @brief 
 * 
 * @param prev
 * @param key
 * @param head
 * @param next
 */
struct combination_set_item {
  combination_set_item* prev;
  double key;
  combination_set* head;
  combination_set_item* next;
};

/**
 * @brief A struct that holds an array of indexes and is part of a linked list in a hash-table bucket. When those indexes are read as values of the input dataset, combinations stored in this struct will sum to the key of the bucket.
 * 
 * @param head Pointer to the first combination set in the list
 * @param tail Pointer to the last combination set in the list
 */
struct combination_set_list {
  combination_set_item* head;
  combination_set_item* tail;
};


// An unordered map which draws functions from the Boost library.
// The map uses hash functionality to store lists of combinations (defined by 'struct combination_set') by key type double.
typedef boost::unordered_map< double, combination_set_list* > Board;


/**
 * @brief Calculates which hash-table bin a specified key is associated with
 * 
 * @param key The key with which to calculate a bin value
 * @param decimal_places The number of decimal places that are significant to this query
 * @return double: the bin value to associate with a certain key
 */
double bin_value(double key, double decimal_places) {
  return ceil(key * decimal_places) / decimal_places;
}

/**
 * @brief Inserts a combination set into the zeroboard, associated with a specific key
 * 
 * @param zeroboard The zeroboard to insert the key-value pair into
 * @param key The key for the bucket which is the sum of the combination being inserted
 * @param combination The combination of input set indexes summing to the key
 * @param combination_len 
 */
void board_insert(
  Board* zeroboard, 
  double key, 
  double decimal_places, 
  int* combination, 
  int combination_len ) 
{
  // Which bin to put this key/value pair into
  double bin_val = -1.0;
  if (decimal_places)
    bin_val = bin_value(key, decimal_places);
  else
    bin_val = ceil(key*100.0)/100.0;
  // Assert bin key is valid
  if (bin_val==-1.0) {
    printf("ERROR: Bad bin value\n");
    exit(EXIT_FAILURE);
  }

  // If key does not exist, create new bucket for this key
  // unordered_map->count() is expected constant time, with worst case linear in size of the container 
  if (zeroboard->count(bin_val) == 0) {
    // 1. Allocate memory for the list of items in this bin
    combination_set_list* new_list = (combination_set_list*)malloc(sizeof(combination_set_list));
    // 2. Allocate memory for a new list item
    combination_set_item* new_item = (combination_set_item*)malloc(sizeof(combination_set_item));
    new_item->key = key;
    new_item->prev = NULL;
    new_item->next = NULL;
    // 1a. Assign new and only item as head and tail of bin list from step 1
    new_list->head = new_item;
    new_list->tail = new_item;
    // 3. Allocate memory for new combination set
    combination_set* new_set = (combination_set*)malloc(sizeof(combination_set));
    // 3a. Assign values to combination set
    new_set->combination = combination;
    new_set->combination_len = combination_len;
    new_set->next = NULL;
    // 2a. Insert combination set from step 3 into list from step 2 
    new_item->head = new_set;
    // Finally: Insert into hash-table
    zeroboard->emplace(bin_val, new_list);
    return;

  // If the key already exists, add the new combination to the existing bucket
  } else {
    // Note: Combinations are stored in lists within each bin
    //       Keys in a bin list are ordered from smallest at the head to largest at the tail
    combination_set_list* set_list = (*zeroboard)[bin_val];
    long long key_max_precision  = ceil(key*PRECISION);
    long long head_max_precision = ceil(set_list->head->key*PRECISION);
    long long tail_max_precision = ceil(set_list->tail->key*PRECISION);
    
    /* Case 1: 
      Key is smaller than or == key at head of list
      New head if new key < existing key else add commbination
    */
    if (key_max_precision <= head_max_precision) {
      // key is less than head key then assign this new key:value pair as new head
      if (key_max_precision < head_max_precision) {
        // Allocate memory for new combination set
        combination_set* new_set = (combination_set*)malloc(sizeof(combination_set));
        // Assign values to combination set
        new_set->combination = combination;
        new_set->combination_len = combination_len;
        new_set->next = NULL;
        // Allocate memory for new commbination set item
        combination_set_item* new_set_item = (combination_set_item*)malloc(sizeof(combination_set_item));
        new_set_item->key = key;
        new_set_item->head = new_set;
        new_set_item->prev = NULL;
        // Set new item as head of list
        new_set_item->next = set_list->head;
        set_list->head->prev = new_set_item;
        set_list->head = new_set_item;
        return;
      
      // key from new key:value pair is the same as key in head -> add to list
      } else { // else if (key == set_list->head->key)
        // Allocate memory for new combination set
        combination_set* new_set = (combination_set*)malloc(sizeof(combination_set));
        // Assign values to combination set
        new_set->combination = combination;
        new_set->combination_len = combination_len;
        // Add new_set to head of list
        new_set->next = set_list->head->head;
        set_list->head->head = new_set;
        return;

      }

    /* 
    Case 2: 
      Key is larger than or == key at tail of list
      New tail if new key > existing key else add combination
    */
    } else if (key_max_precision >= tail_max_precision) {
      // key is greater than tail key then assign this new key:value pair as new tail
      if (key_max_precision > tail_max_precision) {
        // Allocate memory for new combination set
        combination_set* new_set = (combination_set*)malloc(sizeof(combination_set));
        // Assign values to combination set
        new_set->combination = combination;
        new_set->combination_len = combination_len;
        new_set->next = NULL;
        // Allocate memory for new commbination set item
        combination_set_item* new_set_item = (combination_set_item*)malloc(sizeof(combination_set_item));
        new_set_item->key = key;
        new_set_item->head = new_set;
        new_set_item->next = NULL;
        // Set new item as tail of list
        new_set_item->prev = set_list->tail;
        set_list->tail->next = new_set_item;
        set_list->tail = new_set_item;
        return;
      
      // key from new key:value pair is the same as key in head -> add to list
      } else { // else if (key == set_list->head->key)
        // Allocate memory for new combination set
        combination_set* new_set = (combination_set*)malloc(sizeof(combination_set));
        // Assign values to combination set
        new_set->combination = combination;
        new_set->combination_len = combination_len;
        // Add new_set to head of list
        new_set->next = set_list->head->head;
        set_list->head->head = new_set;
        return;

      }
    
    /* Case 3: 
      Key is between head and tail
      1. Decide where to start, head or tail
      2. Search for set item with same key -> 
          a. If found, add to head of list of existing set item
          b. If not found, insert new set item
    */
    } else {
      // 1.
      double min = set_list->head->key;
      double max = set_list->head->key;
      if (key < (min+max)/2.0) {  // start at head of list
        // 2.
        combination_set_item* item = set_list->head;
        // Logic: if the key < item->key then we need to insert just before this item because the key is not found
        while (key<item->key) {
          // if the key to insert is found, add the combination to this list
          if (key==item->key) {
            // 2a.
            // Allocate memory for new combination set
            combination_set* new_set = (combination_set*)malloc(sizeof(combination_set));
            // Assign values to combination set
            new_set->combination = combination;
            new_set->combination_len = combination_len;
            // Make new combination set head of comb_set_item list
            new_set->next = item->head;
            item->head = new_set;
            return;
          }
          item = item->next;
        }
        // 2b. if the key to insert was not found, create new comb_set_item
        // Allocate memory for new combination set
        combination_set* new_set = (combination_set*)malloc(sizeof(combination_set));
        new_set->combination = combination;
        new_set->combination_len = combination_len;
        new_set->next = NULL;
        // Allocate memory for new combination set item
        combination_set_item* new_set_item = (combination_set_item*)malloc(sizeof(combination_set_item));
        new_set_item->key = key;
        new_set_item->head = new_set;
        new_set_item->next = item;
        new_set_item->prev = item->prev;
        // Insert comb_set_item
        item->prev->next = new_set_item;
        item->prev = new_set_item;
        return;

      } else {  // start at tail of list
        // 2.
        combination_set_item* item = set_list->tail;
        // Logic: if the key > item->key then we need to insert just after this item because the key is not found
        while (key>item->key) {
          // if the key to insert is found, add the combination to this list
          if (key==item->key) {
            // 2a.
            // Allocate memory for new combination set
            combination_set* new_set = (combination_set*)malloc(sizeof(combination_set));
            // Assign values to combination set
            new_set->combination = combination;
            new_set->combination_len = combination_len;
            // Make new combination set head of comb_set_item list
            new_set->next = item->head;
            item->head = new_set;
            return;
          }
          item = item->next;
        }
        // 2b. if the key to insert was not found, create new comb_set_item
        // Allocate memory for new combination set
        combination_set* new_set = (combination_set*)malloc(sizeof(combination_set));
        new_set->combination = combination;
        new_set->combination_len = combination_len;
        new_set->next = NULL;
        // Allocate memory for new combination set item
        combination_set_item* new_set_item = (combination_set_item*)malloc(sizeof(combination_set_item));
        new_set_item->key = key;
        new_set_item->head = new_set;
        new_set_item->prev = item->prev;
        new_set_item->next = item;
        // Insert comb_set_item
        item->prev->next = new_set_item;
        item->prev = new_set_item;
        return;
      }

    }

  } // end if-else(zeroboard->count(bin_val) == 0) checking for existence of bin in hash-table using count

}



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
void get_combinations(
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
        // Iterate over items in bin
        combination_set_item* item = (*zeroboard)[tare_value]->head;
        while (item != NULL) {
          // Iterate over combination sets in item
          combination_set* set = item->head;
          while (set != NULL) {
              for (int i=0; i<set->combination_len; ++i)
                printf("%f ", input_set[set->combination[i]]);
              printf("\n");
              // Increment results counter for this combination set
              ++(*num_results);
              // Move to next combination set
              set = set->next;
          }
          // Move to next item in bin
          item = item->next;
        }

      } else { // else if (combin_len !- -1) which means that we are above the combination length of the zeroboard, so we need to include the 'array' indexes 
        // Iterate over items in bin
        combination_set_item* item = (*zeroboard)[tare_value]->head;
        while (item != NULL) {
          // Iterate over combination sets in item
          combination_set* set = item->head;
          while (set != NULL) {
            if (set->combination[0] >= array[combin_len]) {
              for (int i=0; i<combin_len+1; ++i)
                printf("%f ", input_set[array[i]]);
              for (int i=0; i<set->combination_len; ++i)
                printf("%f ", input_set[set->combination[i]]);
              printf("\n");
              // Increment results counter for this combination set
              ++(*num_results);
              // Move to next combination set
              set = set->next;
            } else break; // if the commbination is not valid, exit the loop
          }
          // Move to next item in bin
          item = item->next;
        }

      } // end if else block (combin_len == -1)

    } // end if (zeroboard->count(tare_value) != 0)

  // else if printing results is not required, repeat the above block without printing all combinations to the terminal
  } else {
    if (zeroboard->count(tare_value) != 0) {
      
      if (combin_len == -1) {
        // Iterate over items in bin
        combination_set_item* item = (*zeroboard)[tare_value]->head;
        while (item != NULL) {
          // Iterate over combination sets in item
          combination_set* set = item->head;
          while (set != NULL) {
              // Increment results counter for this combination set
              ++(*num_results);
              // Move to next combination set
              set = set->next;
          }
          // Move to next item in bin
          item = item->next;
        }

      } else { // else if (combin_len !- -1)
        // Iterate over items in bin
        combination_set_item* item = (*zeroboard)[tare_value]->head;
        while (item != NULL) {
          // Iterate over combination sets in item
          combination_set* set = item->head;
          while (set != NULL) {
            if (set->combination[0] >= array[combin_len]) {
              // Increment results counter for this combination set
              ++(*num_results);
              // Move to next combination set
              set = set->next;
            } else break; // if the commbination is not valid, exit the loop
          }
          // Move to next item in bin
          item = item->next;
        }
        
      } // end if else block (combin_len == -1)

    } // end if (zeroboard->count(tare_value) != 0)

  } // end if else printing results is/is-not required

}


/**
 * @brief Cleans up any dynamically allocated memory used in a zeroboard.
 * 
 * @param zeroboard Pointer to the zeroboard to be deleted.
 */
void delete_zeroboard(Board* zeroboard) {
  // Iterate over each bucket in the zeroboard
  for (auto bucket : (*zeroboard) ) {
    combination_set_item* item = bucket.second->head;
    combination_set_item* prev_item;

    // Iterate over items in bucket
    while (item != NULL) {
      combination_set* set = item->head;
      combination_set* prev_set = item->head;
      
      // Iterate over combination sets in this item
      while (set != NULL) {
        prev_set = set;
        set = set->next;
        // 1. free combination_set
        free(prev_set);
      }
      prev_item = item;
      // go to next item in bucket
      item = item->next;
      // 2. free combination_set_item
      free(prev_item);

    } // end while item!=NULL loop
    
    // 3. free combination_set_list
    free(bucket.second);

  } // end bucket for loop
}


/**
 * @brief Prints all keys stored in the zeroboard along with all combinations associated with each key.
 * 
 * @param zeroboard The zeroboard to print contents of.
 */
void print_zeroboard(Board* zeroboard) {
  
  // Iterate over each bucket in the zeroboard
  for (auto bucket : (*zeroboard) ) {
    combination_set_item* item = bucket.second->head;
    
    // Iterate over items in bucket
    while (item != NULL) {
      printf("%.5f:\n", item->key);
      combination_set* set = item->head;
      
      // Iterate over combination sets in this item
      // All of these combination sets will have the same key
      while (set != NULL) {
        for (int i=0; i<set->combination_len; ++i)
          printf("%d ", set->combination[i]);
        printf("\n");
        set = set->next;
      }

      // go to next item in bucket
      item = item->next;
    } // end while item!=NULL loop

  } // end bucket for loop
  
}

#endif /* ZEROBOARD_H */