// Implements a list of numbers with an array of dynamic size using realloc

#include <stdio.h>   // Standard Input/Output - gives us printf()
#include <stdlib.h>  // Standard Library - gives us malloc(), realloc(), free()

int main(void)
{
    // List of size 3
    // malloc = "memory allocate" - asks the OS for memory on the heap
    // We're asking for space for 3 integers
    // sizeof(int) = usually 4 bytes, so we're getting 3 * 4 = 12 bytes total
    // malloc returns a POINTER (memory address) to the first byte of that space
    int *list = malloc(3 * sizeof(int));
    
    // ALWAYS check if malloc succeeded!
    // If the computer is out of memory, malloc returns NULL (address 0x0)
    // Using NULL would cause a segmentation fault (program crash)
    if (list == NULL)
    {
        return 1;  // Return 1 = error code, something went wrong
    }

    // Initialize list of size 3 with numbers
    // We use array syntax even though list is a pointer
    // list[0] is actually shorthand for *(list + 0) - "dereference the pointer"
    // This stores the value 1 at the first memory location
    list[0] = 1;
    list[1] = 2;  // Stores 2 in the second spot (4 bytes after list[0])
    list[2] = 3;  // Stores 3 in the third spot (4 more bytes after list[1])

    // Resize list to be of size 4
    // 🎯 REALLOC = "reallocate" - the MVP of this code!
    // What realloc does:
    //   1. Takes your OLD memory block (list) and the NEW size you want (4 * sizeof(int))
    //   2. Tries to EXPAND the existing memory block in place if possible
    //   3. If it can't expand in place, it:
    //      - Finds a NEW spot in memory big enough for 4 ints
    //      - COPIES all your old data (1, 2, 3) to the new location
    //      - FREES the old memory automatically
    //   4. Returns a pointer to the memory (either the old spot or the new spot)
    //
    // ⚠️ CRITICAL: We use a TEMPORARY pointer (tmp) here, NOT list directly!
    // Why? If realloc fails and returns NULL, we'd lose our original pointer
    // and create a MEMORY LEAK (memory we can't access or free)
    int *tmp = realloc(list, 4 * sizeof(int));
    
    // Check if realloc succeeded
    // If it failed (returned NULL), we still have our original list pointer
    if (tmp == NULL)
    {
        free(list);  // Free the ORIGINAL memory before exiting (no memory leak!)
        return 1;    // Exit with error code
    }
    
    // SUCCESS! Now we can safely update our list pointer
    // list now points to memory that holds 4 integers (old 3 + space for 1 new)
    // The old values (1, 2, 3) are preserved in the first 3 spots
    list = tmp;

    // Add number to list
    // Now we can use the 4th spot that realloc gave us
    list[3] = 4;  // Store 4 in the newly allocated space

    // Print list
    // Standard for loop - i starts at 0, goes while i < 4, increments by 1 each time
    for (int i = 0; i < 4; i++)
    {
        printf("%i\n", list[i]);  // %i = integer format, \n = newline
    }
    // Output will be:
    // 1
    // 2
    // 3
    // 4

    // Free list
    // SUPER IMPORTANT: Always free() what you malloc()/realloc()!
    // This returns the memory to the OS so other programs can use it
    // Not freeing = memory leak = your program is a bad roommate hogging resources
    free(list);
    
    return 0;  // Return 0 = success! Everything worked!
}
