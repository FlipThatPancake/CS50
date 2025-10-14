// ============================================================================
// FILE: dictionary.h
// PURPOSE: Header file that declares the dictionary's functionality
// This is like a "table of contents" - it tells other files what functions
// are available and what data structures we're using
// ============================================================================

#ifndef DICTIONARY_H  // Include guard - prevents this file from being included twice
#define DICTIONARY_H  // If DICTIONARY_H isn't defined, define it and include everything below

#include <stdbool.h>  // Needed for the 'bool' type (true/false)

// Maximum length for a word
// The longest word in major dictionaries is pneumonoultramicroscopicsilicovolcanoconiosis (45 letters!)
#define LENGTH 45

// Function prototypes - these tell the compiler "these functions exist somewhere"
// The actual implementations are in dictionary.c

// Checks if a word is in the dictionary (case-insensitive)
// Returns true if word is spelled correctly, false if misspelled
bool check(const char *word);

// Hashes a word to get an index in the hash table
// Takes a word and returns a number (which bucket it goes in)
unsigned int hash(const char *word);

// Loads the dictionary file into memory
// Returns true if successful, false if it fails (e.g., file not found)
bool load(const char *dictionary);

// Returns the number of words currently loaded in the dictionary
// Returns 0 if dictionary hasn't been loaded yet
unsigned int size(void);

// Frees all memory used by the dictionary
// Returns true if successful, false if it fails
bool unload(void);

#endif // DICTIONARY_H - End of the include guard


// ============================================================================
// FILE: dictionary.c
// PURPOSE: Implements all the dictionary functions using a hash table
// This is where the "magic" happens - loading words, checking spelling, etc.
// ============================================================================

#include <ctype.h>      // For tolower() function to make words case-insensitive
#include <stdbool.h>    // For bool type (true/false)
#include <stdio.h>      // For FILE operations like fopen, fclose, fscanf
#include <stdlib.h>     // For malloc() and free() - dynamic memory allocation
#include <string.h>     // For strcpy() and strlen() - string manipulation
#include <strings.h>    // For strcasecmp() - case-insensitive string comparison

#include "dictionary.h" // Header file with function prototypes and LENGTH constant

// Represents a node in a hash table
// This is a linked list node that stores one word from the dictionary
typedef struct node
{
    char word[LENGTH + 1];  // Array to store the word (+1 for null terminator '\0')
    struct node *next;       // Pointer to the next node in the linked list (for collision handling)
} node;

// Choose number of buckets in hash table
// Using 26*26 = 676 buckets (one for each two-letter combination like "aa", "ab", etc.)
// More buckets = fewer collisions = faster lookups!
const unsigned int N = 676;

// Hash table - array of pointers to linked lists
// Each index points to the first node of a linked list (or NULL if empty)
node *table[N];

// Global variable to keep track of how many words we've loaded
// We increment this in load() and use it in size()
unsigned int word_count = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Step 1: Hash the word to get an index (which bucket to look in)
    unsigned int index = hash(word);
    
    // Step 2: Create a cursor pointer to traverse the linked list at table[index]
    // Start at the head of the list
    node *cursor = table[index];
    
    // Step 3: Traverse the linked list looking for the word
    while (cursor != NULL)  // Keep going until we reach the end of the list
    {
        // Compare the word we're looking for with the current node's word
        // strcasecmp = case-insensitive comparison (so "The" matches "the")
        // Returns 0 if strings are equal
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;  // Found it! Word is spelled correctly
        }
        
        // Move to the next node in the linked list
        cursor = cursor->next;
    }
    
    // We've checked the entire linked list and didn't find the word
    return false;  // Word is misspelled (not in dictionary)
}

// Hashes word to a number
// Takes a word and returns an index (0 to N-1) to determine which bucket it goes in
unsigned int hash(const char *word)
{
    // Using a simple hash function based on first two letters
    // Convert to uppercase and calculate index
    
    // Get first character, convert to uppercase, get value 0-25
    unsigned int hash_value = toupper(word[0]) - 'A';
    
    // If there's a second character, factor it in too
    // This gives us 26*26 = 676 possible hash values
    if (word[1] != '\0')  // Check if there's a second character
    {
        hash_value = hash_value * 26 + (toupper(word[1]) - 'A');
    }
    
    // Make sure we don't go out of bounds (mod by N to keep it in range)
    return hash_value % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Step 1: Open the dictionary file
    FILE *file = fopen(dictionary, "r");  // "r" means read mode
    if (file == NULL)  // Check if file opening failed
    {
        return false;  // Can't load dictionary if we can't open the file
    }
    
    // Step 2: Initialize the hash table to NULL
    // This makes sure all buckets start empty
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;  // Each bucket starts with no nodes
    }
    
    // Step 3: Read words from dictionary file one at a time
    char word[LENGTH + 1];  // Temporary buffer to hold each word we read
    
    // fscanf reads formatted input from file
    // "%s" means read a string (stops at whitespace)
    // Returns EOF (End Of File) when done reading
    while (fscanf(file, "%s", word) != EOF)
    {
        // Step 4: Create a new node for this word
        node *new_node = malloc(sizeof(node));  // Allocate memory for a new node
        if (new_node == NULL)  // Check if malloc failed (out of memory)
        {
            fclose(file);  // Clean up: close the file
            unload();      // Clean up: free any memory we've allocated so far
            return false;  // Loading failed
        }
        
        // Step 5: Copy the word into the new node
        strcpy(new_node->word, word);  // Copy word into node's word array
        
        // Step 6: Hash the word to find which bucket it belongs in
        unsigned int index = hash(word);
        
        // Step 7: Insert node at the beginning of the linked list (easiest/fastest)
        // This is called "prepending" - adding to the front
        new_node->next = table[index];  // New node points to current first node
        table[index] = new_node;        // New node becomes the new first node
        
        // Step 8: Increment our word counter
        word_count++;
    }
    
    // Step 9: Close the file (important to free up resources!)
    fclose(file);
    
    // Successfully loaded all words!
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // Simply return the count we've been tracking
    // We increment word_count every time we load a word in load()
    return word_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // We need to free ALL memory we allocated with malloc()
    // This means freeing every node in every linked list
    
    // Step 1: Loop through each bucket in the hash table
    for (int i = 0; i < N; i++)
    {
        // Step 2: For each bucket, traverse its linked list and free each node
        node *cursor = table[i];  // Start at the head of this bucket's list
        
        while (cursor != NULL)  // Keep going until we reach the end
        {
            // Important: Save the next pointer BEFORE we free the current node!
            // If we free first, we lose access to cursor->next!
            node *temp = cursor;      // Remember current node
            cursor = cursor->next;    // Move to next node
            free(temp);               // Now free the old node
        }
        
        // After freeing all nodes in this bucket, set it to NULL
        table[i] = NULL;
    }
    
    // Reset word count since dictionary is now empty
    word_count = 0;
    
    // Successfully unloaded everything!
    return true;
}


// ============================================================================
// FILE: speller.c
// PURPOSE: Main program that uses the dictionary to spell-check text files
// This is the "driver" - it ties everything together and provides the UI
// ============================================================================

#include <ctype.h>           // For isalpha(), isdigit(), isalnum() - character checking
#include <stdio.h>           // For printf(), fopen(), fread(), fclose() - I/O operations
#include <sys/resource.h>    // For getrusage() - measuring resource usage (CPU time)
#include <sys/time.h>        // For time structures used by getrusage()

#include "dictionary.h"      // Our dictionary functions (load, check, size, unload)

// Undefine any existing definitions (just in case)
#undef calculate
#undef getrusage

// Default dictionary file to use if user doesn't specify one
#define DICTIONARY "dictionaries/large"

// Function prototype - tells compiler this function exists below
double calculate(const struct rusage *b, const struct rusage *a);

int main(int argc, char *argv[])
{
    // argc = argument count (number of command-line arguments)
    // argv = argument vector (array of strings with the actual arguments)
    // Example: ./speller texts/cat.txt
    //   argc = 2, argv[0] = "./speller", argv[1] = "texts/cat.txt"
    
    // Check for correct number of args
    // Valid usage: ./speller [DICTIONARY] text
    // Can be 2 args (just text file) or 3 args (dictionary + text file)
    if (argc != 2 && argc != 3)
    {
        printf("Usage: ./speller [DICTIONARY] text\n");
        return 1;  // Exit with error code 1 (non-zero = error)
    }

    // Structures for timing data
    // rusage = "resource usage" - tracks CPU time, memory, etc.
    // We'll use these to measure how long each function takes
    struct rusage before, after;

    // Variables to store timing benchmarks (in seconds)
    double time_load = 0.0, time_check = 0.0, time_size = 0.0, time_unload = 0.0;

    // Determine which dictionary to use
    // If 3 args: argv[1] is dictionary, argv[2] is text file
    // If 2 args: use default dictionary, argv[1] is text file
    // The ternary operator (condition ? value_if_true : value_if_false) is like a compact if-else
    char *dictionary = (argc == 3) ? argv[1] : DICTIONARY;

    // Load dictionary into memory and time how long it takes
    getrusage(RUSAGE_SELF, &before);  // Record time/resources BEFORE loading
    bool loaded = load(dictionary);   // Call our load() function from dictionary.c
    getrusage(RUSAGE_SELF, &after);   // Record time/resources AFTER loading

    // Exit if dictionary not loaded (file not found, out of memory, etc.)
    if (!loaded)
    {
        printf("Could not load %s.\n", dictionary);
        return 1;  // Exit with error
    }

    // Calculate time to load dictionary
    // calculate() subtracts the before time from after time
    time_load = calculate(&before, &after);

    // Determine which text file to spell-check
    // If 3 args: argv[2] is the text file
    // If 2 args: argv[1] is the text file
    char *text = (argc == 3) ? argv[2] : argv[1];
    
    // Try to open the text file for reading
    FILE *file = fopen(text, "r");  // "r" = read mode
    if (file == NULL)  // fopen returns NULL if it fails
    {
        printf("Could not open %s.\n", text);
        unload();  // Clean up: free the dictionary from memory
        return 1;  // Exit with error
    }

    // Print header for misspelled words section
    printf("\nMISSPELLED WORDS\n\n");

    // Initialize counters for spell-checking
    int index = 0;         // Current position in the word buffer
    int misspellings = 0;  // Count of misspelled words found
    int words = 0;         // Total count of words checked
    char word[LENGTH + 1]; // Buffer to build each word character-by-character

    // Spell-check each word in text
    // Strategy: Read file one character at a time, build words letter-by-letter
    char c;  // Variable to hold each character we read
    
    // fread() reads data from file
    // Parameters: &c (where to store), sizeof(char) (how much to read),
    //             1 (read 1 item), file (from where)
    // Returns number of items read (0 when end of file)
    while (fread(&c, sizeof(char), 1, file))
    {
        // Allow only alphabetical characters and apostrophes (for contractions like "don't")
        // isalpha(c) returns true if c is a letter (a-z, A-Z)
        if (isalpha(c) || (c == '\'' && index > 0))  // Allow apostrophe only if not at start
        {
            // Append character to word buffer
            word[index] = c;
            index++;  // Move to next position in buffer

            // Ignore alphabetical strings too long to be words
            // If word exceeds LENGTH (45), it's probably not a real word
            if (index > LENGTH)
            {
                // Consume remainder of alphabetical string
                // Keep reading until we hit a non-letter
                while (fread(&c, sizeof(char), 1, file) && isalpha(c));

                // Prepare for new word - reset index
                index = 0;
            }
        }

        // Ignore words with numbers (like MS Word does)
        // isdigit(c) returns true if c is a digit (0-9)
        // Examples to ignore: "CS50", "2fast", "hello123"
        else if (isdigit(c))
        {
            // Consume remainder of alphanumeric string
            // isalnum(c) returns true if c is letter OR digit
            while (fread(&c, sizeof(char), 1, file) && isalnum(c));

            // Prepare for new word - reset index
            index = 0;
        }

        // We must have found a whole word
        // If index > 0, we have letters in the buffer
        // If we're here, we hit a non-letter (space, punctuation, newline, etc.)
        else if (index > 0)
        {
            // Terminate current word with null character
            // Strings in C must end with '\0' to mark the end
            word[index] = '\0';

            // Update counter - we found another word!
            words++;

            // Check word's spelling and time how long it takes
            getrusage(RUSAGE_SELF, &before);       // Time before checking
            bool misspelled = !check(word);        // Call check() - returns false if NOT in dictionary
            getrusage(RUSAGE_SELF, &after);        // Time after checking

            // Update benchmark - add time for this check to running total
            // We accumulate time because we check thousands of words
            time_check += calculate(&before, &after);

            // Print word if misspelled
            if (misspelled)
            {
                printf("%s\n", word);  // Print the misspelled word
                misspellings++;        // Increment misspelling counter
            }

            // Prepare for next word - reset index
            index = 0;
        }
    }

    // Check whether there was an error reading the file
    // ferror() returns non-zero if an error occurred during reading
    if (ferror(file))
    {
        fclose(file);  // Clean up: close the file
        printf("Error reading %s.\n", text);
        unload();  // Clean up: free dictionary memory
        return 1;  // Exit with error
    }

    // Close text file (always close files when done!)
    fclose(file);

    // Determine dictionary's size and time it
    getrusage(RUSAGE_SELF, &before);  // Time before
    unsigned int n = size();           // Call size() to get word count
    getrusage(RUSAGE_SELF, &after);   // Time after

    // Calculate time to determine dictionary's size
    time_size = calculate(&before, &after);

    // Unload dictionary from memory and time it
    getrusage(RUSAGE_SELF, &before);  // Time before
    bool unloaded = unload();          // Call unload() to free all memory
    getrusage(RUSAGE_SELF, &after);   // Time after

    // Abort if dictionary not unloaded (this shouldn't happen)
    if (!unloaded)
    {
        printf("Could not unload %s.\n", dictionary);
        return 1;  // Exit with error
    }

    // Calculate time to unload dictionary
    time_unload = calculate(&before, &after);

    // Report all statistics and benchmarks
    printf("\nWORDS MISSPELLED:     %d\n", misspellings);  // How many typos found
    printf("WORDS IN DICTIONARY:  %d\n", n);               // Size of dictionary
    printf("WORDS IN TEXT:        %d\n", words);           // How many words we checked
    printf("TIME IN load:         %.2f\n", time_load);     // Time to load dictionary
    printf("TIME IN check:        %.2f\n", time_check);    // Time to check all words
    printf("TIME IN size:         %.2f\n", time_size);     // Time to get size (should be fast!)
    printf("TIME IN unload:       %.2f\n", time_unload);   // Time to free memory
    printf("TIME IN TOTAL:        %.2f\n\n",               // Total time
           time_load + time_check + time_size + time_unload);

    // Success! Exit with code 0
    return 0;
}

// Returns number of seconds between b (before) and a (after)
// This function calculates elapsed CPU time using rusage structures
double calculate(const struct rusage *b, const struct rusage *a)
{
    // Check for NULL pointers (defensive programming)
    if (b == NULL || a == NULL)
    {
        return 0.0;
    }
    else
    {
        // Calculate user time (time spent in user mode - running your code)
        // ru_utime.tv_sec = seconds, ru_utime.tv_usec = microseconds
        // Convert everything to microseconds first, then to seconds
        
        // Calculate system time (time spent in kernel mode - system calls, I/O)
        // ru_stime.tv_sec = seconds, ru_stime.tv_usec = microseconds
        
        // Total time = (user time after - user time before) + (system time after - system time before)
        // Divide by 1,000,000 to convert microseconds to seconds
        return ((((a->ru_utime.tv_sec * 1000000 + a->ru_utime.tv_usec) -
                  (b->ru_utime.tv_sec * 1000000 + b->ru_utime.tv_usec)) +
                 ((a->ru_stime.tv_sec * 1000000 + a->ru_stime.tv_usec) -
                  (b->ru_stime.tv_sec * 1000000 + b->ru_stime.tv_usec)))
                / 1000000.0);
    }
}