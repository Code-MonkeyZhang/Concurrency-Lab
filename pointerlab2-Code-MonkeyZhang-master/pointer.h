#ifndef POINTER_H
#define POINTER_H

#include <stdio.h>
#include <stdbool.h>
#include <math.h>

//
// Structure definitions and function pointer typedefs
//

typedef double (*area_fn)(void* shape); // Function pointer type for area functions
typedef double (*perimeter_fn)(void* shape); // Function pointer type for perimeter functions

typedef struct {
    struct {
        area_fn area; // Area function for shape
        perimeter_fn perimeter; // Perimeter function for shape
    } virtual_func_table; // Table of function pointers for the emulated virtual functions
    const char* name; // Name of shape
    unsigned int edges; // Number of edges
} shape_t; // Generic base class for a shape

typedef struct {
    shape_t shape; // Include the base class at the start of a specific type of shape
    double width;
    double length;
} rectangle_t;

typedef struct {
    shape_t shape; // Include the base class at the start of a specific type of shape
    double length; // Length of equilateral triangle edge
} triangle_t;

// Function pointer type for shape comparison functions
// Returns -1 if shape1 is less than shape2
// Returns 1 if shape1 is greater than shape2
// Returns 0 if shape1 is equal to shape2
typedef int (*compare_fn)(shape_t* shape1, shape_t* shape2);

typedef struct linked_list_node {
    shape_t* shape; // Shape associated with the node
    struct linked_list_node* next;
} linked_list_node_t;

typedef struct {
    compare_fn compare; // Shape comparison function
    linked_list_node_t* head;
} linked_list_t;

typedef struct tree_node {
    shape_t* shape; // Shape associated with the node
    struct tree_node* left; // Left subtree
    struct tree_node* right; // Right subtree
} tree_node_t;

#define MAX_DEPTH 30

typedef struct {
    tree_node_t* curr; // Pointer to the current node
    unsigned int depth; // Current depth in the tree (root is at depth 0)
    tree_node_t* parents[MAX_DEPTH]; // Pointer to parent nodes
} tree_iterator_t; // Helper struct for iterating through a tree -- represents a current location within the tree

typedef double (*foreach_fn)(shape_t* shape, double data); // Function pointer type for functions being run foreach shape in a linked list

//
// Shape functions
//

// Returns the area of a shape
static inline double shape_area(shape_t* shape)
{
    return shape->virtual_func_table.area(shape);
}

// Returns the perimeter of a shape
static inline double shape_perimeter(shape_t* shape)
{
    return shape->virtual_func_table.perimeter(shape);
}

// Returns the name of a shape
static inline const char* shape_name(shape_t* shape)
{
    return shape->name;
}

// Returns the number of edges in a shape
static inline unsigned int shape_edges(shape_t* shape)
{
    return shape->edges;
}

// Prints info about a shape
static inline void shape_print(shape_t* shape)
{
    printf("%s: area = %.2f, perimeter = %.2f, edges = %d\n", shape_name(shape), shape_area(shape), shape_perimeter(shape), shape_edges(shape));
}

double rectangle_area(void* shape);

double triangle_area(void* shape);

double rectangle_perimeter(void* shape);

double triangle_perimeter(void* shape);

void rectangle_construct(rectangle_t* shape, const char* name, double width, double length);

void triangle_construct(triangle_t* shape, const char* name, double length);

int compare_by_area(shape_t* shape1, shape_t* shape2);

int compare_by_perimeter(shape_t* shape1, shape_t* shape2);

//
// Linked list functions
//

void linked_list_init(linked_list_t* list, compare_fn compare);

void linked_list_insert(linked_list_t* list, linked_list_node_t* node);

void linked_list_remove(linked_list_t* list, shape_t* shape);

//
// Tree iterator functions
//

void tree_iterator_begin(tree_iterator_t* iter, tree_node_t* root);

void tree_iterator_next(tree_iterator_t* iter);

bool tree_iterator_at_end(tree_iterator_t* iter);

tree_node_t* tree_iterator_get_node(tree_iterator_t* iter);

shape_t* tree_iterator_get_shape(tree_iterator_t* iter);

//
// Tree analysis functions
//

void max_min_avg_area(tree_node_t* root, double* max, double* min, double* avg);

double foreach(tree_node_t* root, foreach_fn func, double data);

#endif // POINTER_H
