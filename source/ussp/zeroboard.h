// 
// zeroboard.h
// Contains structures and functions defining the zeroboard data structure which allows for fast querying of combinations.
// Used by functions in UnboundedSubsetSum and subsetSummer.
// 

#ifndef ZEROBOARD_H
#define ZEROBOARD_H


/**
 * @brief A struct that holds an array of indexes and is part of a linked list in a hash-table bucket. When those indexes are read as values of the input dataset, combinations stored in this struct will sum to the key of the bucket.
 * 
 * @param combination Pointer to an array of indexes that are associated with specific values in the input dataset
 * @param combination_len Number of indexes stored in combination (i.e. length of the permutation or no. of values making up the query value)
 * @param next Pointer to the next combination set in the list
 */
struct combination_set {
  int* combination;
  int combination_len; 
  combination_set* next; 
};


// An unordered map which draws functions from the Boost library.
// The map uses hash functionality to store lists of combinations (defined by 'struct combination_set') by key type double.
typedef boost::unordered_map< double, combination_set* > Board;


/**
 * @brief Inserts a combination set into the zeroboard, associated with a specific key
 * 
 * @param zeroboard The zeroboard to insert the key-value pair into
 * @param key The key for the bucket which is the sum of the combination being inserted
 * @param combination The combination of input set indexes summing to the key
 * @param combination_len 
 */
void board_insert(Board* zeroboard, double key, int* combination, int combination_len) {
  
  // If key does not exist, create new bucket for this key
  // unordered_map->count() is expected constant time, with worst case linear in size of the container 
  if (zeroboard->count(key) == 0) {
    // Allocate memory for the combination
    combination_set* new_set = (combination_set*)malloc(sizeof(combination_set));
    new_set->combination = combination;
    new_set->combination_len = combination_len;
    // Next is NULL as this is the head of the list
    new_set->next = NULL;

    (*zeroboard).emplace( key,new_set );

  // If the key already exists, add the new combination to the head of the list in the existing bucket
  } else {
    // Allocate memory for the combination
    combination_set* new_set = (combination_set*)malloc(sizeof(combination_set));
    new_set->combination = combination;
    new_set->combination_len = combination_len;

    // Make this combination the head of the list
    new_set->next = (*zeroboard)[key];
    (*zeroboard)[key] = new_set;
  }

}


/**
 * @brief Cleans up any dynamically allocated memory used in a zeroboard.
 * 
 * @param zeroboard Pointer to the zeroboard to be deleted.
 */
void delete_zeroboard(Board* zeroboard) {}


/**
 * @brief Prints all keys stored in the zeroboard along with all combinations associated with each key.
 * 
 * @param zeroboard The zeroboard to print contents of.
 */
void print_zeroboard(Board* zeroboard) {
  
  // Iterate over each key in the zeroboard
  for (auto bucket : (*zeroboard) ) {
    printf("%f:\n", bucket.first); // print the key of the bucket
    
    // Iterate over combination sets in bucket
    combination_set* set = bucket.second;
    while (set != NULL) {

      // print combination set
      printf("  ");
      for (int i=0; i<set->combination_len; i++)
        printf("%d ", set->combination[i]);
      printf("\n");
      set = set->next;  // go to next combination set
    
    } // end while(set != NULL)
  } // end for loop
  
}

#endif /* ZEROBOARD_H */