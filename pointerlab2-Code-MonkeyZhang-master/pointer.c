// DO NOT INCLUDE ANY OTHER LIBRARIES/FILES
#include "pointer.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// In this assignment, you can assume that function parameters are valid and the memory is managed by the caller //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Returns the area of a rectangle
// The shape is guaranteed to be a valid rectangle
double rectangle_area(void *shape)
{
    rectangle_t *rectangle = (rectangle_t *)shape;
    // IMPLEMENT THIS
    return rectangle->width * rectangle->length;
}

// Returns the area of an equilateral triangle
// The shape is guaranteed to be a valid triangle
// The area of an equilateral triangle is sqrt(3)/4 times length squared
double triangle_area(void *shape)
{
    triangle_t *triangle = (triangle_t *)shape;
    // IMPLEMENT THIS
    return triangle->length * triangle->length * sqrt(3) / 4;
}

// Returns the perimeter of a rectangle
// The shape is guaranteed to be a valid rectangle
double rectangle_perimeter(void *shape)
{
    rectangle_t *rectangle = (rectangle_t *)shape;
    // IMPLEMENT THIS
    return (rectangle->width + rectangle->length) * 2;
}

// Returns the perimeter of an equilateral triangle
// The shape is guaranteed to be a valid triangle
double triangle_perimeter(void *shape)
{
    triangle_t *triangle = (triangle_t *)shape;
    // IMPLEMENT THIS
    return triangle->length * 3;
}

// Initializes a rectangle shape
void rectangle_construct(rectangle_t *shape, const char *name, double width, double length)
{
    // IMPLEMENT THIS
    shape->shape.name = name;
    shape->shape.edges = 4;
    shape->width = width;
    shape->length = length;
    shape->shape.virtual_func_table.area = rectangle_area;
    shape->shape.virtual_func_table.perimeter = rectangle_perimeter;
}

// Initializes a triangle shape
void triangle_construct(triangle_t *shape, const char *name, double length)
{
    // IMPLEMENT THIS
    shape->shape.name = name;
    shape->shape.edges = 3;
    shape->length = length;
    shape->shape.virtual_func_table.area = triangle_area;
    shape->shape.virtual_func_table.perimeter = triangle_perimeter;
}

// Compares the area of shape1 with shape2
// Returns -1 if the area of shape1 is less than the area of shape2
// Returns 1 if the area of shape1 is greater than the area of shape2
// Returns 0 if the area of shape1 is equal to the area of shape2
int compare_by_area(shape_t *shape1, shape_t *shape2)
{
    // IMPLEMENT THIS
    double area1 = shape1->virtual_func_table.area(shape1);
    double area2 = shape2->virtual_func_table.area(shape2);
    if (area1 < area2)
        return -1;
    else if (area1 > area2)
        return 1;
    else
        return 0;
}

// Compares the perimeter of shape1 with shape2
// Returns -1 if the perimeter of shape1 is less than the perimeter of shape2
// Returns 1 if the perimeter of shape1 is greater than the perimeter of shape2
// Returns 0 if the perimeter of shape1 is equal to the perimeter of shape2
int compare_by_perimeter(shape_t *shape1, shape_t *shape2)
{
    // IMPLEMENT THIS
    double perimeter1 = shape1->virtual_func_table.perimeter(shape1);
    double perimeter2 = shape2->virtual_func_table.perimeter(shape2);
    if (perimeter1 < perimeter2)
        return -1;
    else if (perimeter1 > perimeter2)
        return 1;
    else
        return 0;
    return 0;
}

//
// Linked list functions
//

// Initializes a singly linked list
// If compare is NULL, the list is unsorted and new nodes are inserted at the head of the list
// If compare is not NULL, the list is sorted in increasing order based on the comparison function
void linked_list_init(linked_list_t *list, compare_fn compare)
{
    // IMPLEMENT THIS
    list->head = NULL;
    list->compare = compare;
}

void insert_head_helper(linked_list_t *list, linked_list_node_t *node)
{
    node->next = list->head;
    list->head = node;
}

// Inserts a node into the linked list based on the list comparison function
void linked_list_insert(linked_list_t *list, linked_list_node_t *node)
{
    // IMPLEMENT THIS
    // Unsorted case or head is Null， insert at head
    if (list->compare == NULL || list->head == NULL)
    {
        insert_head_helper(list, node);
    }
    // Sorted case
    else
    {
        // iterate from head
        linked_list_node_t *current = list->head;
        linked_list_node_t *prev = NULL;

        //  -1 shape1 is less than shape2
        //  1 shape1 is greater than shape2
        //  0 shape1 is equal to shape2
        while (true)
        {
            // if current >= node
            if (current == NULL || list->compare(current->shape, node->shape) >= 0)
            {
                if (list->head == current)
                {
                    insert_head_helper(list, node);
                    break;
                }
                else
                {
                    // insert ahead of current
                    if (prev != NULL)
                    {
                        prev->next = node;
                    }

                    node->next = current;
                    break;
                }
            }
            // if current is less than node
            else if (list->compare(current->shape, node->shape) == -1)
            {
                // goto next
                prev = current;
                current = current->next;
            }
        }
    }
}

// Removes all nodes from the linked list containing the given shape
void linked_list_remove(linked_list_t *list, shape_t *shape)
{
    // IMPLEMENT THIS
    linked_list_node_t *current = list->head;
    linked_list_node_t *prev = NULL;

    while (current != NULL)
    {
        if (current->shape == shape)
        {
            if (current == list->head)
            {
                list->head = current->next;
                prev = NULL;
                current = current->next;
            }
            else
            {
                prev->next = current->next;
                current = current->next;
            }
        }
        else
        {
            prev = current;
            current = current->next;
        }
    }
}

//
// Tree iterator functions
//

// Initializes an iterator to the beginning of a tree (i.e., first in-order node)
void tree_iterator_begin(tree_iterator_t *iter, tree_node_t *root)
{
    // IMPLEMENT THIS
    // incase root is null
    if (root == NULL)
    {
        iter->curr = NULL;
        iter->depth = 0;
        for (int i = 0; i < MAX_DEPTH; i++)
        {
            iter->parents[i] = NULL;
        }
        return;
    }

    iter->curr = root;
    iter->depth = 0;
    // Traverse to the left-most node
    while (iter->curr->left != NULL)
    {
        iter->parents[iter->depth] = iter->curr;
        iter->curr = iter->curr->left;
        iter->depth++;
    }
}

// Updates an iterator to move to the next in-order node in the tree if possible
// Moving past the last in-order node in the tree results in the iterator reaching a NULL state representing the end of the tree
void tree_iterator_next(tree_iterator_t *iter)
{
    // IMPLEMENT THIS
    if (iter->curr == NULL)
    {
        return;
    }

    if (iter->curr->right != NULL)
    {
        // If the current node has a right child, move to the left-most node of the right subtree
        iter->parents[iter->depth++] = iter->curr;
        iter->curr = iter->curr->right;
        // Traverse to the left-most node
        while (iter->curr->left != NULL)
        {
            iter->parents[iter->depth++] = iter->curr;
            iter->curr = iter->curr->left;
        }
    }
    else
    {
        // If the current node does not have a right child, move to the parent node
        while (iter->depth != 0 && iter->curr == iter->parents[iter->depth - 1]->right)
        {
            iter->curr = iter->parents[--iter->depth];
        }
        // If the current node is not the root node, move to the parent node
        if (iter->depth > 0)
        {
            iter->curr = iter->parents[--iter->depth];
        }
        // If the current node is the root node, move to the NULL state
        else if (iter->depth == 0)
        {
            iter->curr = NULL;
        }
    }
}

// Returns true if iterator is at the end of the tree or false otherwise
// The end of the tree is the position after the last in-order node in the tree (i.e., NULL state from tree_iterator_next)
bool tree_iterator_at_end(tree_iterator_t *iter)
{
    // IMPLEMENT THIS
    return iter->curr == NULL;
}

// Returns the current node that the iterator references or NULL if the iterator is at the end of the list
tree_node_t *tree_iterator_get_node(tree_iterator_t *iter)
{
    // IMPLEMENT THIS
    return iter->curr;
}

// Returns the current shape that the iterator references or NULL if the iterator is at the end of the list
shape_t *tree_iterator_get_shape(tree_iterator_t *iter)
{
    // IMPLEMENT THIS
    if (iter->curr != NULL)
    {
        return iter->curr->shape;
    }
    else
    {
        return NULL;
    }
}

//
// Tree analysis functions
//

// Returns the maximum, minimum, and average area of shapes in the tree
// An empty tree should not modify the maximum, minimum, or average
void max_min_avg_area(tree_node_t *root, double *max, double *min, double *avg)
{
    // IMPLEMENT THIS
    if (root == NULL)
        return;

    int count = 0;
    double sum = 0;
    // Initialize the iterator
    tree_iterator_t iter;
    tree_iterator_begin(&iter, root);

    // Iterate through the tree
    while (!tree_iterator_at_end(&iter))
    {
        // Get the area of the current shape
        shape_t *shape = tree_iterator_get_shape(&iter);
        double area = shape->virtual_func_table.area(shape);

        // Update the max and min
        if (count == 0)
        {
            // If this is the first node, set the max and min to the area
            *max = area;
            *min = area;
        }
        else
        {
            // Otherwise, update the max and min if necessary
            if (area > *max)
                *max = area;
            if (area < *min)
                *min = area;
        }

        sum += area;
        count++;
        tree_iterator_next(&iter);
    }
    // Calculate the average
    *avg = sum / count;
}

// Executes the func function for each node in the tree in-order
// The function takes in an input data and returns an output data, which is used as input to the next call to the function
// The initial input data is provided as a parameter to foreach, and foreach returns the final output data
// For example, if there are three nodes, foreach should behave like: return func(node3, func(node2, func(node1, data)))
double foreach (tree_node_t *root, foreach_fn func, double data)
{
    // incase root is null
    if (root == NULL)
        return data;

    tree_iterator_t iter;
    // Initialize the iterator to the beginning of the tree
    tree_iterator_begin(&iter, root);
    // Traverse the tree in-order
    while (!tree_iterator_at_end(&iter))
    {
        shape_t *shape = tree_iterator_get_shape(&iter);
        data = func(shape, data);
        // Move to the next node
        tree_iterator_next(&iter);
    }

    return data;
}