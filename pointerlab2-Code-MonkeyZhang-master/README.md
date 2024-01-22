# pointerlab: a review of C and pointers

## Introduction
This assignment serves as a review of C and pointers. You will be implementing all the functions in pointer.c. The comments provide details on what each function should do. Any relevant structure definitions, constants, and inline functions are provided for you in pointer.h. **You should NOT make any changes in any file besides pointer.c.**

The assignment consists of 4 parts that build on top of each other:
1. Shape functions (20 pts) [Difficulty: Easy]
    These functions serve to introduce the concept of polymorphism and object-oriented programming. You will be programming two types of objects that represent shapes. The rectangle_t type represents a rectangle shape with a width and length. The triangle_t type represents an equilateral triangle with a length. Both types are based on a base shape_t type that corresponds to the base class in an object-oriented language.

    Polymorphism is the concept that a pointer to a base object can refer to objects of different sub-types. For example, a shape_t pointer can refer to a rectangle_t or a triangle_t. Object-oriented languages handle this automatically, but in a non-object-oriented language like C, we must emulate this behavior ourselves. This is done by having the shape_t struct located at the beginning of the rectangle_t and triangle_t structs (as shown in pointer.h). Thus, casting a rectangle_t pointer to a shape_t pointer is valid since the beginning of rectangle_t is a shape_t struct (note this is an actual struct, not a pointer to the struct).

    To support virtual functions (i.e., shape functions that can either refer to rectangle functions or triangle functions), we use what's known as a virtual function pointer table that contains function pointers that refer to the correct functions. For example, rectangle_t will have function pointers within the shape_t portion that point to rectangle functions, and triangle_t will have function pointers within the shape_t portion that point to triangle functions. Thus, a shape can call the appropriate function by using the virtual function pointer table. For example, the provided shape_area function uses the table to call the appropriate area function for the shape.

    Your role in this part of the assignment is to 1) understand how the shapes are manipulated, and 2) implement a set of functions that emulate object-oriented programming in C. The first four functions implement the area and perimeter calculations for rectangles and triangles. The next two functions illustrate the initialization of each shape. The last two functions illustrate how to compare generic shapes, which may either be rectangles, triangles, or some other shape.

2. Linked list functions (30 pts) [Difficulty: Medium]
    In this part of the assignment, you will implement a basic singly linked list that is optionally sorted. The init function simply initializes the linked list and configures it based on a provided comparison function pointer. If the comparison function pointer is NULL, this indicates the list should be unsorted, and insertions occur at the head of the list. If the comparison function pointer is non-NULL, it provides the desired comparison function for sorting the list in increasing order. The insert function is responsible for inserting a new node into the list based on the comparison function. The remove function handles removing all nodes that contain the specified shape pointer.

3. Tree iterator functions (30 pts) [Difficulty: Hard]
    An iterator is a structure that refers to the current position in some data structure. It is used to iterate through a data structure without needing to know the underlying details of the data structure. In other words, it encapsulates the process of iterating through a data structure.

    In this part of the assignment, you will be implementing a tree iterator for a binary tree of shapes. The tree structure and iterator structure can be found in pointer.h. Each tree node will have a pointer to a shape as well as a pointer to its left and right subtrees. The iterator will contain a pointer to the current node as well as the current depth of the node in the tree (i.e., distance from the root node) and an array of parent pointers for iterating through the tree. The first begin function starts the iteration at the first in-order node. The following next function transitions the iterator to the next in-order node. The following at_end function indicates whether the iterator has reached the end of the tree, which is represented by a NULL state beyond the last node in the tree. The last two functions are accessor functions that return the node and shape corresponding to the current iterator position.

    The purpose of the parents array is to help you traverse the tree as you are iterating through it, so you'll need to update and/or use the array in the begin and next functions. The idea is that the array represents a stack of parent pointers similar to how recursively iterating through a tree tracks the nodes via variables/parameters stored within the function stack. The parents array essentially allows you to store that partial stack state as you iterate through the tree. Figuring out how this works is the challenge in this part of the assignment, and it is your responsibility to come up with a solution entirely on your own. Looking up solutions and/or hints is an academic integrity violation. This is meant to be the hardest part of the assignment where you have to think critically to develop a solution. Drawing pictures of trees and working through small examples can be very helpful. Lastly, note that you are NOT allowed to search for the next node starting from the root of the tree. We have explicitly added tests to break this approach. The purpose of the iterator is to store enough information to be able to effectively traverse through the tree from the current node.

4. Tree analysis functions (20 pts) [Difficulty: Easy-Medium]
    The last part of the assignment looks at iterating through the tree data structure and performing some computations. There are two general approaches for implementing the functions. If you've correctly implemented the tree iterator functions, the easy approach is to simply utilize the iterator functions rather than reimplementing tree iteration. The memory for the iterator structure can be allocated as a local variable in your functions. An alternative approach is to use recursion to iterate through the tree data structure, and this would work even if your iterator code is incorrect. The first function computes the max, min, and average area across a tree of shapes. The second function iterates through the tree in-order and calls a user-provided function on each shape in the tree.

## Programming rules
You are not allowed to use any libraries besides the math.h library already included in pointer.h. Do not include any other libraries/files in pointer.c. If you really think you need some library function, please contact the course instructor to determine eligibility. You are allowed to use any of the inline functions, constants, etc. within the pointer.h file.

## Evaluation and testing your code
You will receive zero points if:
- You violate the academic integrity policy (sanctions can be greater than just a 0 for the assignment)
- You don't show your partial work by periodically adding, committing, and pushing your code to GitHub
- You break any of the programming rules
- Your code does not compile/build
- Your code crashes the grading script

Your code will be evaluated for correctness. We have provided some tests, though we reserve the right to add additional tests during the final grading, so you are responsible for ensuring your code is correct. In terms of a grade point breakdown, we will assign:
- (20 pts) Shape functions
- (30 pts) Linked list functions
- (30 pts) Tree iterator functions
- (20 pts) Tree analysis functions

To run the grading script, simply run the following command in the assignment folder:
```make test```

## Handin
We will be using GitHub for managing submissions, and you must show your partial work by periodically adding, committing, and pushing your code to GitHub. This helps us see your code if you ask any questions on Canvas (please include your GitHub username) and also helps deter academic integrity violations.

Additionally, please input the desired commit number that you would like us to grade in Canvas. You can get the commit number from github.com. In your repository, click on the commits link to the right above your files. Find the commit from the appropriate day/time that you want graded. Click on the clipboard icon to copy the commit number. Note that this is a much longer number (40 characters) than the displayed number. Paste your very long commit number and only your commit number in this assignment submission textbox. Do NOT include any other text besides your commit number.

## Hints
To compile your code in debug mode (to make it easier to debug with gdb), you can simply run:
```make debug```
You can launch gdb on your program by running:
```gdb pointer```
Within gdb, you can type:
```r```
to run all of the tests, or you can type:
```r test_name```
with test_name replaced by whatever test you want to specifically run. You can find the tests in test.c. When debugging test failures, you should read the relevant test code in test.c to understand the failure. If you want to stop when the failure occurs, conditional breakpoints within gdb are very helpful.
