# CS 3340 - Analysis of Algorithms - Western University
## Assignment 2
Design and implement an algorithm to find the connected components in a binary
image using Disjoint-Set data structure.

### Functions
- uandf(n): (constructor) constructs a disjoint-set data type with n elements, 1, 2, . . . , n.
- make_set(i): creates a new set whose only member (and thus representative) is i.
- find_set(i): returns the representative of the set containing i.
- union_sets(i, j): unites the dynamic sets that contains i and j, respectively, into a
new set that is the union of these two sets.
- final_sets(): returns the total number of current sets, no sets, and finalizes the
- current_sets: (i) make set() and union sets() will have no effect after this operation
and (ii) resets the representatives of the sets so that integers from 1 to no sets will
be used as representatives.
