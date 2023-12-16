#include <stdio.h>
#include <stdlib.h> // "debugmalloc.h"


// PANCAKES!!
typedef struct pcake {
    int size;   //!< Size of the pancake
    struct pcake *next, *prev; //!< Double linked list previous/next item
} pcake;

/**
 * @brief Let's eat the pancakes so they don't go unused on the heap
 * @param pancakes The list of the pancakes we want to eat (free)
 */
void eat(pcake* pancakes) {
    pcake* next;
    while (pancakes != NULL) {
        next = pancakes->next;
        free(pancakes);
        pancakes = next;
    }
}

/**
 * @brief Cook some yummy 2xlinked pancakes
 * @param number Number of pancakes to make
 * @param sizes  The size of those pancakes
 * @return The still hot pancakes in a list
 */
pcake* cook(int number, const int sizes[]) {
    if (number < 1) return NULL;
    pcake* first = malloc(sizeof(pcake));
    if (first == NULL) return NULL;
    first->prev = NULL;
    first->size = sizes[0];

    pcake* fresh = first;
    for (int i = 1; i < number; ++i) {
        fresh->next = malloc(sizeof(pcake));
        if (fresh->next == NULL) { eat(first); return NULL; }
        fresh->next->prev = fresh;
        fresh = fresh->next;
        fresh->size = sizes[i];
    }

    fresh->next = NULL;
    return first;
}

/**
 * @brief Quickly peek at the pancake sizes
 * @param pancakes PANCAKES! (ptr to first)
 */
void peek(pcake* pancakes) {
    int i = 0;
    while (pancakes != NULL) {
        printf("%d", pancakes->size);
        if (pancakes->next == NULL) break;
        printf(" > ");
        pancakes = pancakes->next;
        if (++i > 12) break;
    }
    printf("\n");
}

/**
 * @brief We just reach in with a spatula between two of the pancakes and flip the whole thing over in one careful move
 * @param middle Where we reach in
 * @returns The pancake now on the bottom
 */
pcake* flip(pcake* middle) {
    pcake* off = middle->prev;
    if (middle->next == NULL) return middle;
    middle->prev = NULL; // No infinite pancake hack - don't loop back at the end

    while (1) {
        pcake* juggle = middle->next;
        middle->next = middle->prev;
        middle->prev = juggle;

        if (middle->prev == NULL) {
            if (off != NULL) {
                off->next = middle;
                middle->prev = off;
            }
            return middle;
        }
        middle = middle->prev;
    }
}

/**
 * @brief Find the most optimal pancake to eat (the biggest one)
 * @param pile The list of the pancakes (searching only forward)
 * @return The most optimal (biggest) pancake
 */
pcake* optimal(pcake* pile) {
    pcake* big = pile;
    while (pile != NULL) {
        if (pile->size > big->size) big = pile;
        pile = pile->next;
    }

    return big;
}

int main() {
    pcake *first = cook(10, (int[]) {5, 7, 6, 4, 9, 2, 1, 10, 3, 8});
    if (first == NULL) {
        printf("Well... I think I burnt some of them... (Failed to cook the pancakes please try again later)\n");
        return -1;
    }
    peek(first);

    // Keeping in mind how sorted the pancakes are
    for (pcake* done = first; done != NULL; done = done->next) {
        pcake *mid = optimal(done);    // Figure out which pancake is the biggest

        // If the biggest pancake is already at its location on the bottom, no need to flip stuff around
        if (mid == done) continue;

        // If the biggest pancake is already at the top, all we can do with it is flip it upside down
        if (mid->next != NULL) {
            flip(mid);
            peek(first);
        }

        // Flip the biggest pancake to the bottom
        done = flip(done);
        if (first->next == NULL) first = done;
        peek(first);
    }

    eat(first);
    return 0;
}
