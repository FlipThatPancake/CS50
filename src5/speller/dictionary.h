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