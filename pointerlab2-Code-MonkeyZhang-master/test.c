#include <stdio.h>
#include "pointer.h"
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define mu_str_(text) #text
#define mu_str(text) mu_str_(text)
#define mu_assert(message, test) do { if (!(test)) return "See " __FILE__ " Line " mu_str(__LINE__) ": " message; } while (0)

static int string_equal(const char* str1, const char* str2)
{
    if ((str1 == NULL) && (str2 == NULL)) {
        return 1;
    }
    if ((str1 == NULL) || (str2 == NULL)) {
        return 0;
    }
    return (strcmp(str1, str2) == 0);
}

static int approx_equal(double val1, double val2)
{
    double epsilon = 1e-9;
    return fabs(val1 - val2) < epsilon;
}

static char* test_rectangle_area()
{
    rectangle_t rectangle;
    rectangle_construct(&rectangle, "test_rectangle", 3, 5);
    mu_assert("Testing rectangle area is calculated correctly",
              approx_equal(rectangle_area((shape_t*)&rectangle), 15));
    rectangle_construct(&rectangle, "test_rectangle", 2.7, 4.6);
    mu_assert("Testing rectangle area is calculated correctly",
              approx_equal(rectangle_area((shape_t*)&rectangle), 12.42));
    rectangle_construct(&rectangle, "test_rectangle", 1, 1);
    mu_assert("Testing rectangle area is calculated correctly",
              approx_equal(rectangle_area((shape_t*)&rectangle), 1));
    rectangle_construct(&rectangle, "test_rectangle", 100, 1);
    mu_assert("Testing rectangle area is calculated correctly",
              approx_equal(rectangle_area((shape_t*)&rectangle), 100));
    return NULL;
}

static char* test_triangle_area()
{
    triangle_t triangle;
    triangle_construct(&triangle, "triangle", sqrt(12/sqrt(3)));
    mu_assert("Testing triangle area is calculated correctly",
              approx_equal(triangle_area((shape_t*)&triangle), 3));
    triangle_construct(&triangle, "test_triangle", sqrt(20/sqrt(3)));
    mu_assert("Testing triangle area is calculated correctly",
              approx_equal(triangle_area((shape_t*)&triangle), 5));
    triangle_construct(&triangle, "test_triangle", sqrt(22/sqrt(3)));
    mu_assert("Testing triangle area is calculated correctly",
              approx_equal(triangle_area((shape_t*)&triangle), 5.5));
    triangle_construct(&triangle, "test_triangle", sqrt(800/sqrt(3)));
    mu_assert("Testing triangle area is calculated correctly",
              approx_equal(triangle_area((shape_t*)&triangle), 200));
    triangle_construct(&triangle, "test_triangle", 1);
    mu_assert("Testing triangle area is calculated correctly",
              approx_equal(triangle_area((shape_t*)&triangle), sqrt(3)/4));
    return NULL;
}

static char* test_rectangle_perimeter()
{
    rectangle_t rectangle;
    rectangle_construct(&rectangle, "test_rectangle", 3, 5);
    mu_assert("Testing rectangle perimeter is calculated correctly",
              approx_equal(rectangle_perimeter((shape_t*)&rectangle), 16));
    rectangle_construct(&rectangle, "test_rectangle", 5.9, 1.2);
    mu_assert("Testing rectangle perimeter is calculated correctly",
              approx_equal(rectangle_perimeter((shape_t*)&rectangle), 14.2));
    rectangle_construct(&rectangle, "test_rectangle", 1, 1);
    mu_assert("Testing rectangle perimeter is calculated correctly",
              approx_equal(rectangle_perimeter((shape_t*)&rectangle), 4));
    rectangle_construct(&rectangle, "test_rectangle", 100, 1);
    mu_assert("Testing rectangle perimeter is calculated correctly",
              approx_equal(rectangle_perimeter((shape_t*)&rectangle), 202));
    return NULL;
}

static char* test_triangle_perimeter()
{
    triangle_t triangle;
    triangle_construct(&triangle, "triangle", 3);
    mu_assert("Testing triangle perimeter is calculated correctly",
              approx_equal(triangle_perimeter((shape_t*)&triangle), 9));
    triangle_construct(&triangle, "triangle", 7.7);
    mu_assert("Testing triangle perimeter is calculated correctly",
              approx_equal(triangle_perimeter((shape_t*)&triangle), 23.1));
    triangle_construct(&triangle, "triangle", 1);
    mu_assert("Testing triangle perimeter is calculated correctly",
              approx_equal(triangle_perimeter((shape_t*)&triangle), 3));
    triangle_construct(&triangle, "triangle", 100);
    mu_assert("Testing triangle perimeter is calculated correctly",
              approx_equal(triangle_perimeter((shape_t*)&triangle), 300));
    return NULL;
}

static char* test_rectangle_construct()
{
    rectangle_t rectangle;
    rectangle_construct(&rectangle, "test_rectangle1", 3, 5);
    mu_assert("Testing name is set appropriately",
              string_equal(shape_name((shape_t*)&rectangle), "test_rectangle1"));
    mu_assert("Testing width is set appropriately",
              rectangle.width == 3);
    mu_assert("Testing length is set appropriately",
              rectangle.length == 5);
    mu_assert("Testing edges set appropriately",
              shape_edges((shape_t*)&rectangle) == 4);
    mu_assert("Testing area is calling the right function",
              shape_area((shape_t*)&rectangle) == rectangle_area((shape_t*)&rectangle));
    mu_assert("Testing perimeter is calling the right function",
              shape_perimeter((shape_t*)&rectangle) == rectangle_perimeter((shape_t*)&rectangle));
    rectangle_construct(&rectangle, "test_rectangle2", 2.7, 4.9);
    mu_assert("Testing name is set appropriately",
              string_equal(shape_name((shape_t*)&rectangle), "test_rectangle2"));
    mu_assert("Testing width is set appropriately",
              rectangle.width == 2.7);
    mu_assert("Testing length is set appropriately",
              rectangle.length == 4.9);
    mu_assert("Testing edges set appropriately",
              shape_edges((shape_t*)&rectangle) == 4);
    mu_assert("Testing area is calling the right function",
              shape_area((shape_t*)&rectangle) == rectangle_area((shape_t*)&rectangle));
    mu_assert("Testing perimeter is calling the right function",
              shape_perimeter((shape_t*)&rectangle) == rectangle_perimeter((shape_t*)&rectangle));
    rectangle_construct(&rectangle, "test_rectangle3", 1, 1);
    mu_assert("Testing name is set appropriately",
              string_equal(shape_name((shape_t*)&rectangle), "test_rectangle3"));
    mu_assert("Testing width is set appropriately",
              rectangle.width == 1);
    mu_assert("Testing length is set appropriately",
              rectangle.length == 1);
    mu_assert("Testing edges set appropriately",
              shape_edges((shape_t*)&rectangle) == 4);
    mu_assert("Testing area is calling the right function",
              shape_area((shape_t*)&rectangle) == rectangle_area((shape_t*)&rectangle));
    mu_assert("Testing perimeter is calling the right function",
              shape_perimeter((shape_t*)&rectangle) == rectangle_perimeter((shape_t*)&rectangle));
    rectangle_construct(&rectangle, "test_rectangle4", 100, 200);
    mu_assert("Testing name is set appropriately",
              string_equal(shape_name((shape_t*)&rectangle), "test_rectangle4"));
    mu_assert("Testing width is set appropriately",
              rectangle.width == 100);
    mu_assert("Testing length is set appropriately",
              rectangle.length == 200);
    mu_assert("Testing edges set appropriately",
              shape_edges((shape_t*)&rectangle) == 4);
    mu_assert("Testing area is calling the right function",
              shape_area((shape_t*)&rectangle) == rectangle_area((shape_t*)&rectangle));
    mu_assert("Testing perimeter is calling the right function",
              shape_perimeter((shape_t*)&rectangle) == rectangle_perimeter((shape_t*)&rectangle));
    return NULL;
}

static char* test_triangle_construct()
{
    triangle_t triangle;
    triangle_construct(&triangle, "test_triangle1", 5);
    mu_assert("Testing name is set appropriately",
              string_equal(shape_name((shape_t*)&triangle), "test_triangle1"));
    mu_assert("Testing length is set appropriately",
              triangle.length == 5);
    mu_assert("Testing edges set appropriately",
              shape_edges((shape_t*)&triangle) == 3);
    mu_assert("Testing area is calling the right function",
              shape_area((shape_t*)&triangle) == triangle_area((shape_t*)&triangle));
    mu_assert("Testing perimeter is calling the right function",
              shape_perimeter((shape_t*)&triangle) == triangle_perimeter((shape_t*)&triangle));
    triangle_construct(&triangle, "test_triangle2", 3.7);
    mu_assert("Testing name is set appropriately",
              string_equal(shape_name((shape_t*)&triangle), "test_triangle2"));
    mu_assert("Testing length is set appropriately",
              triangle.length == 3.7);
    mu_assert("Testing edges set appropriately",
              shape_edges((shape_t*)&triangle) == 3);
    mu_assert("Testing area is calling the right function",
              shape_area((shape_t*)&triangle) == triangle_area((shape_t*)&triangle));
    mu_assert("Testing perimeter is calling the right function",
              shape_perimeter((shape_t*)&triangle) == triangle_perimeter((shape_t*)&triangle));
    triangle_construct(&triangle, "test_triangle1", sqrt(4/sqrt(3)));
    mu_assert("Testing name is set appropriately",
              string_equal(shape_name((shape_t*)&triangle), "test_triangle1"));
    mu_assert("Testing length is set appropriately",
              triangle.length == sqrt(4/sqrt(3)));
    mu_assert("Testing edges set appropriately",
              shape_edges((shape_t*)&triangle) == 3);
    mu_assert("Testing area is calling the right function",
              shape_area((shape_t*)&triangle) == triangle_area((shape_t*)&triangle));
    mu_assert("Testing perimeter is calling the right function",
              shape_perimeter((shape_t*)&triangle) == triangle_perimeter((shape_t*)&triangle));
    triangle_construct(&triangle, "test_triangle1", 25);
    mu_assert("Testing name is set appropriately",
              string_equal(shape_name((shape_t*)&triangle), "test_triangle1"));
    mu_assert("Testing length is set appropriately",
              triangle.length == 25);
    mu_assert("Testing edges set appropriately",
              shape_edges((shape_t*)&triangle) == 3);
    mu_assert("Testing area is calling the right function",
              shape_area((shape_t*)&triangle) == triangle_area((shape_t*)&triangle));
    mu_assert("Testing perimeter is calling the right function",
              shape_perimeter((shape_t*)&triangle) == triangle_perimeter((shape_t*)&triangle));
    return NULL;
}

static char* test_compare_by_area()
{
    rectangle_t rectangle;
    triangle_t triangle;
    rectangle_construct(&rectangle, "shape1", 3, 5);
    triangle_construct(&triangle, "shape2", sqrt(60/sqrt(3)));
    mu_assert("Testing area comparison is returning the correct comparison values",
              compare_by_area((shape_t*)&rectangle, (shape_t*)&triangle) == 0);
    mu_assert("Testing area comparison is returning the correct comparison values",
              compare_by_area((shape_t*)&triangle, (shape_t*)&rectangle) == 0);
    rectangle_construct(&rectangle, "shape1", 5, 5);
    triangle_construct(&triangle, "shape2", sqrt(60/sqrt(3)));
    mu_assert("Testing area comparison is returning the correct comparison values",
              compare_by_area((shape_t*)&rectangle, (shape_t*)&triangle) > 0);
    mu_assert("Testing area comparison is returning the correct comparison values",
              compare_by_area((shape_t*)&triangle, (shape_t*)&rectangle) < 0);
    rectangle_construct(&rectangle, "shape1", 2, 5);
    triangle_construct(&triangle, "shape2", sqrt(60/sqrt(3)));
    mu_assert("Testing area comparison is returning the correct comparison values",
              compare_by_area((shape_t*)&rectangle, (shape_t*)&triangle) < 0);
    mu_assert("Testing area comparison is returning the correct comparison values",
              compare_by_area((shape_t*)&triangle, (shape_t*)&rectangle) > 0);
    return NULL;
}

static char* test_compare_by_perimeter()
{
    rectangle_t rectangle;
    triangle_t triangle;
    rectangle_construct(&rectangle, "shape1", 3, 3);
    triangle_construct(&triangle, "shape2", 4);
    mu_assert("Testing area comparison is returning the correct comparison values",
              compare_by_perimeter((shape_t*)&rectangle, (shape_t*)&triangle) == 0);
    mu_assert("Testing area comparison is returning the correct comparison values",
              compare_by_perimeter((shape_t*)&triangle, (shape_t*)&rectangle) == 0);
    rectangle_construct(&rectangle, "shape1", 3, 4);
    triangle_construct(&triangle, "shape2", 4);
    mu_assert("Testing area comparison is returning the correct comparison values",
              compare_by_perimeter((shape_t*)&rectangle, (shape_t*)&triangle) > 0);
    mu_assert("Testing area comparison is returning the correct comparison values",
              compare_by_perimeter((shape_t*)&triangle, (shape_t*)&rectangle) < 0);
    rectangle_construct(&rectangle, "shape1", 3, 2);
    triangle_construct(&triangle, "shape2", 5);
    mu_assert("Testing area comparison is returning the correct comparison values",
              compare_by_perimeter((shape_t*)&rectangle, (shape_t*)&triangle) < 0);
    mu_assert("Testing area comparison is returning the correct comparison values",
              compare_by_perimeter((shape_t*)&triangle, (shape_t*)&rectangle) > 0);
    return NULL;
}

static char* test_list_insert_single_unsorted_empty_start()
{
    shape_t shape1;
    linked_list_node_t node1 = {&shape1, &node1};
    linked_list_t list;
    linked_list_init(&list, NULL);
    linked_list_insert(&list, &node1);
    mu_assert("Testing list insert empty list",
              list.head == &node1);
    mu_assert("Testing list insert empty list",
              node1.next == NULL);
    return NULL;
}

static char* test_list_insert_single_unsorted_nonempty_start()
{
    shape_t shape1;
    shape_t shape2;
    shape_t shape3;
    linked_list_node_t node2 = {&shape2, NULL};
    linked_list_node_t node1 = {&shape1, &node2};
    linked_list_node_t node3 = {&shape3, NULL};
    linked_list_t list = {NULL, &node1};
    linked_list_insert(&list, &node3);
    mu_assert("Testing list insert non-empty list",
              list.head == &node3);
    mu_assert("Testing list insert non-empty list",
              node3.next == &node1);
    mu_assert("Testing list insert non-empty list",
              node1.next == &node2);
    mu_assert("Testing list insert non-empty list",
              node2.next == NULL);
    return NULL;
}

static char* test_list_insert_multiple_unsorted()
{
    shape_t shape1;
    shape_t shape2;
    shape_t shape3;
    shape_t shape4;
    shape_t shape5;
    linked_list_node_t node1 = {&shape1, NULL};
    linked_list_node_t node2 = {&shape2, NULL};
    linked_list_node_t node3 = {&shape3, NULL};
    linked_list_node_t node4 = {&shape4, NULL};
    linked_list_node_t node5 = {&shape5, NULL};
    linked_list_t list;
    linked_list_init(&list, NULL);
    linked_list_insert(&list, &node1);
    mu_assert("Testing list insert empty list",
              list.head == &node1);
    mu_assert("Testing list insert empty list",
              node1.next == NULL);
    linked_list_insert(&list, &node2);
    mu_assert("Testing list insert non-empty list",
              list.head == &node2);
    mu_assert("Testing list insert non-empty list",
              node2.next == &node1);
    linked_list_insert(&list, &node3);
    mu_assert("Testing list insert non-empty list",
              list.head == &node3);
    mu_assert("Testing list insert non-empty list",
              node3.next == &node2);
    linked_list_insert(&list, &node4);
    mu_assert("Testing list insert non-empty list",
              list.head == &node4);
    mu_assert("Testing list insert non-empty list",
              node4.next == &node3);
    linked_list_insert(&list, &node5);
    mu_assert("Testing list insert non-empty list",
              list.head == &node5);
    mu_assert("Testing list insert non-empty list",
              node5.next == &node4);
    return NULL;
}

static int list_compare(shape_t* shape1, shape_t* shape2)
{
    if (shape1 < shape2) {
        return -1;
    } else if (shape1 > shape2) {
        return 1;
    } else {
        return 0;
    }
}

static char* test_list_insert_sorted_order_sorted_by_shape_value()
{
    linked_list_node_t node1 = {(shape_t*)1, NULL};
    linked_list_node_t node2 = {(shape_t*)2, NULL};
    linked_list_node_t node3 = {(shape_t*)3, NULL};
    linked_list_node_t node4 = {(shape_t*)4, NULL};
    linked_list_node_t node5 = {(shape_t*)5, NULL};
    linked_list_t list;
    linked_list_init(&list, list_compare);
    linked_list_insert(&list, &node1);
    mu_assert("Testing list insert",
              list.head == &node1);
    mu_assert("Testing list insert",
              node1.next == NULL);
    linked_list_insert(&list, &node2);
    mu_assert("Testing list insert",
              node1.next == &node2);
    mu_assert("Testing list insert",
              node2.next == NULL);
    linked_list_insert(&list, &node3);
    mu_assert("Testing list insert",
              node2.next == &node3);
    mu_assert("Testing list insert",
              node3.next == NULL);
    linked_list_insert(&list, &node4);
    mu_assert("Testing list insert",
              node3.next == &node4);
    mu_assert("Testing list insert",
              node4.next == NULL);
    linked_list_insert(&list, &node5);
    mu_assert("Testing list insert",
              node4.next == &node5);
    mu_assert("Testing list insert",
              node5.next == NULL);
    return NULL;
}

static char* test_list_insert_reverse_order_sorted_by_shape_value()
{
    linked_list_node_t node1 = {(shape_t*)1, NULL};
    linked_list_node_t node2 = {(shape_t*)2, NULL};
    linked_list_node_t node3 = {(shape_t*)3, NULL};
    linked_list_node_t node4 = {(shape_t*)4, NULL};
    linked_list_node_t node5 = {(shape_t*)5, NULL};
    linked_list_t list;
    linked_list_init(&list, list_compare);
    linked_list_insert(&list, &node5);
    mu_assert("Testing list insert",
              list.head == &node5);
    mu_assert("Testing list insert",
              node5.next == NULL);
    linked_list_insert(&list, &node4);
    mu_assert("Testing list insert",
              list.head == &node4);
    mu_assert("Testing list insert",
              node4.next == &node5);
    linked_list_insert(&list, &node3);
    mu_assert("Testing list insert",
              list.head == &node3);
    mu_assert("Testing list insert",
              node3.next == &node4);
    linked_list_insert(&list, &node2);
    mu_assert("Testing list insert",
              list.head == &node2);
    mu_assert("Testing list insert",
              node2.next == &node3);
    linked_list_insert(&list, &node1);
    mu_assert("Testing list insert",
              list.head == &node1);
    mu_assert("Testing list insert",
              node1.next == &node2);
    return NULL;
}

static char* test_list_insert_alternating_order_sorted_by_shape_value()
{
    linked_list_node_t node1 = {(shape_t*)1, NULL};
    linked_list_node_t node2 = {(shape_t*)2, NULL};
    linked_list_node_t node3 = {(shape_t*)3, NULL};
    linked_list_node_t node4 = {(shape_t*)4, NULL};
    linked_list_node_t node5 = {(shape_t*)5, NULL};
    linked_list_t list;
    linked_list_init(&list, list_compare);
    linked_list_insert(&list, &node3);
    mu_assert("Testing list insert",
              list.head == &node3);
    mu_assert("Testing list insert",
              node3.next == NULL);
    linked_list_insert(&list, &node2);
    mu_assert("Testing list insert",
              list.head == &node2);
    mu_assert("Testing list insert",
              node2.next == &node3);
    linked_list_insert(&list, &node4);
    mu_assert("Testing list insert",
              node3.next == &node4);
    mu_assert("Testing list insert",
              node4.next == NULL);
    linked_list_insert(&list, &node1);
    mu_assert("Testing list insert",
              list.head == &node1);
    mu_assert("Testing list insert",
              node1.next == &node2);
    linked_list_insert(&list, &node5);
    mu_assert("Testing list insert",
              node4.next == &node5);
    mu_assert("Testing list insert",
              node5.next == NULL);
    return NULL;
}

static char* test_list_insert_random_order_sorted_by_shape_value()
{
    linked_list_node_t node1 = {(shape_t*)1, NULL};
    linked_list_node_t node2 = {(shape_t*)2, NULL};
    linked_list_node_t node3 = {(shape_t*)3, NULL};
    linked_list_node_t node4 = {(shape_t*)4, NULL};
    linked_list_node_t node5 = {(shape_t*)5, NULL};
    linked_list_t list;
    linked_list_init(&list, list_compare);
    linked_list_insert(&list, &node3);
    mu_assert("Testing list insert empty list",
              list.head == &node3);
    mu_assert("Testing list insert empty list",
              node3.next == NULL);
    linked_list_insert(&list, &node1);
    mu_assert("Testing list insert head",
              list.head == &node1);
    mu_assert("Testing list insert head",
              node1.next == &node3);
    linked_list_insert(&list, &node4);
    mu_assert("Testing list insert tail",
              node3.next == &node4);
    mu_assert("Testing list insert tail",
              node4.next == NULL);
    linked_list_insert(&list, &node2);
    mu_assert("Testing list insert middle",
              node1.next == &node2);
    mu_assert("Testing list insert middle",
              node2.next == &node3);
    linked_list_insert(&list, &node5);
    mu_assert("Testing list insert tail",
              node4.next == &node5);
    mu_assert("Testing list insert tail",
              node5.next == NULL);
    return NULL;
}

static char* test_list_insert_duplicate_sorted_by_shape_value()
{
    linked_list_node_t node1 = {(shape_t*)1, NULL};
    linked_list_node_t node2 = {(shape_t*)2, NULL};
    linked_list_node_t node3 = {(shape_t*)2, NULL};
    linked_list_node_t node4 = {(shape_t*)4, NULL};
    linked_list_node_t node5 = {(shape_t*)5, NULL};
    linked_list_t list;
    linked_list_init(&list, list_compare);
    linked_list_insert(&list, &node1);
    mu_assert("Testing list insert",
              list.head == &node1);
    mu_assert("Testing list insert",
              node1.next == NULL);
    linked_list_insert(&list, &node2);
    mu_assert("Testing list insert",
              node1.next == &node2);
    mu_assert("Testing list insert",
              node2.next == NULL);
    linked_list_insert(&list, &node3);
    mu_assert("Testing list insert (duplicate)",
              node1.next->shape == (shape_t*)2);
    mu_assert("Testing list insert (duplicate)",
              node1.next->next->shape == (shape_t*)2);
    mu_assert("Testing list insert (duplicate)",
              node1.next->next->next == NULL);
    linked_list_insert(&list, &node4);
    mu_assert("Testing list insert",
              node1.next->next->next == &node4);
    mu_assert("Testing list insert",
              node4.next == NULL);
    linked_list_insert(&list, &node5);
    mu_assert("Testing list insert",
              node4.next == &node5);
    mu_assert("Testing list insert",
              node5.next == NULL);
    return NULL;
}

static char* test_list_insert_sorted_order_sorted_by_perimeter()
{
    rectangle_t rectangle1;
    rectangle_t rectangle2;
    triangle_t triangle3;
    rectangle_t rectangle4;
    rectangle_t rectangle5;
    rectangle_construct(&rectangle1, "rectangle1", 0.25, 0.25);
    rectangle_construct(&rectangle2, "rectangle2", 0.5, 0.5);
    triangle_construct(&triangle3, "triangle3", 1);
    rectangle_construct(&rectangle4, "rectangle4", 1, 1);
    rectangle_construct(&rectangle5, "rectangle5", 1, 1.5);
    linked_list_node_t node1 = {(shape_t*)&rectangle1, NULL};
    linked_list_node_t node2 = {(shape_t*)&rectangle2, NULL};
    linked_list_node_t node3 = {(shape_t*)&triangle3, NULL};
    linked_list_node_t node4 = {(shape_t*)&rectangle4, NULL};
    linked_list_node_t node5 = {(shape_t*)&rectangle5, NULL};
    linked_list_t list;
    linked_list_init(&list, compare_by_perimeter);
    linked_list_insert(&list, &node1);
    mu_assert("Testing list insert",
              list.head == &node1);
    mu_assert("Testing list insert",
              node1.next == NULL);
    linked_list_insert(&list, &node2);
    mu_assert("Testing list insert",
              node1.next == &node2);
    mu_assert("Testing list insert",
              node2.next == NULL);
    linked_list_insert(&list, &node3);
    mu_assert("Testing list insert",
              node2.next == &node3);
    mu_assert("Testing list insert",
              node3.next == NULL);
    linked_list_insert(&list, &node4);
    mu_assert("Testing list insert",
              node3.next == &node4);
    mu_assert("Testing list insert",
              node4.next == NULL);
    linked_list_insert(&list, &node5);
    mu_assert("Testing list insert",
              node4.next == &node5);
    mu_assert("Testing list insert",
              node5.next == NULL);
    return NULL;
}

static char* test_list_insert_reverse_order_sorted_by_perimeter()
{
    rectangle_t rectangle1;
    rectangle_t rectangle2;
    triangle_t triangle3;
    rectangle_t rectangle4;
    rectangle_t rectangle5;
    rectangle_construct(&rectangle1, "rectangle1", 0.25, 0.25);
    rectangle_construct(&rectangle2, "rectangle2", 0.5, 0.5);
    triangle_construct(&triangle3, "triangle3", 1);
    rectangle_construct(&rectangle4, "rectangle4", 1, 1);
    rectangle_construct(&rectangle5, "rectangle5", 1, 1.5);
    linked_list_node_t node1 = {(shape_t*)&rectangle1, NULL};
    linked_list_node_t node2 = {(shape_t*)&rectangle2, NULL};
    linked_list_node_t node3 = {(shape_t*)&triangle3, NULL};
    linked_list_node_t node4 = {(shape_t*)&rectangle4, NULL};
    linked_list_node_t node5 = {(shape_t*)&rectangle5, NULL};
    linked_list_t list;
    linked_list_init(&list, compare_by_perimeter);
    linked_list_insert(&list, &node5);
    mu_assert("Testing list insert",
              list.head == &node5);
    mu_assert("Testing list insert",
              node5.next == NULL);
    linked_list_insert(&list, &node4);
    mu_assert("Testing list insert",
              list.head == &node4);
    mu_assert("Testing list insert",
              node4.next == &node5);
    linked_list_insert(&list, &node3);
    mu_assert("Testing list insert",
              list.head == &node3);
    mu_assert("Testing list insert",
              node3.next == &node4);
    linked_list_insert(&list, &node2);
    mu_assert("Testing list insert",
              list.head == &node2);
    mu_assert("Testing list insert",
              node2.next == &node3);
    linked_list_insert(&list, &node1);
    mu_assert("Testing list insert",
              list.head == &node1);
    mu_assert("Testing list insert",
              node1.next == &node2);
    return NULL;
}

static char* test_list_insert_alternating_order_sorted_by_perimeter()
{
    rectangle_t rectangle1;
    rectangle_t rectangle2;
    triangle_t triangle3;
    rectangle_t rectangle4;
    rectangle_t rectangle5;
    rectangle_construct(&rectangle1, "rectangle1", 0.25, 0.25);
    rectangle_construct(&rectangle2, "rectangle2", 0.5, 0.5);
    triangle_construct(&triangle3, "triangle3", 1);
    rectangle_construct(&rectangle4, "rectangle4", 1, 1);
    rectangle_construct(&rectangle5, "rectangle5", 1, 1.5);
    linked_list_node_t node1 = {(shape_t*)&rectangle1, NULL};
    linked_list_node_t node2 = {(shape_t*)&rectangle2, NULL};
    linked_list_node_t node3 = {(shape_t*)&triangle3, NULL};
    linked_list_node_t node4 = {(shape_t*)&rectangle4, NULL};
    linked_list_node_t node5 = {(shape_t*)&rectangle5, NULL};
    linked_list_t list;
    linked_list_init(&list, compare_by_perimeter);
    linked_list_insert(&list, &node3);
    mu_assert("Testing list insert",
              list.head == &node3);
    mu_assert("Testing list insert",
              node3.next == NULL);
    linked_list_insert(&list, &node2);
    mu_assert("Testing list insert",
              list.head == &node2);
    mu_assert("Testing list insert",
              node2.next == &node3);
    linked_list_insert(&list, &node4);
    mu_assert("Testing list insert",
              node3.next == &node4);
    mu_assert("Testing list insert",
              node4.next == NULL);
    linked_list_insert(&list, &node1);
    mu_assert("Testing list insert",
              list.head == &node1);
    mu_assert("Testing list insert",
              node1.next == &node2);
    linked_list_insert(&list, &node5);
    mu_assert("Testing list insert",
              node4.next == &node5);
    mu_assert("Testing list insert",
              node5.next == NULL);
    return NULL;
}

static char* test_list_insert_random_order_sorted_by_perimeter()
{
    rectangle_t rectangle1;
    rectangle_t rectangle2;
    triangle_t triangle3;
    rectangle_t rectangle4;
    rectangle_t rectangle5;
    rectangle_construct(&rectangle1, "rectangle1", 0.25, 0.25);
    rectangle_construct(&rectangle2, "rectangle2", 0.5, 0.5);
    triangle_construct(&triangle3, "triangle3", 1);
    rectangle_construct(&rectangle4, "rectangle4", 1, 1);
    rectangle_construct(&rectangle5, "rectangle5", 1, 1.5);
    linked_list_node_t node1 = {(shape_t*)&rectangle1, NULL};
    linked_list_node_t node2 = {(shape_t*)&rectangle2, NULL};
    linked_list_node_t node3 = {(shape_t*)&triangle3, NULL};
    linked_list_node_t node4 = {(shape_t*)&rectangle4, NULL};
    linked_list_node_t node5 = {(shape_t*)&rectangle5, NULL};
    linked_list_t list;
    linked_list_init(&list, compare_by_perimeter);
    linked_list_insert(&list, &node3);
    mu_assert("Testing list insert empty list",
              list.head == &node3);
    mu_assert("Testing list insert empty list",
              node3.next == NULL);
    linked_list_insert(&list, &node1);
    mu_assert("Testing list insert head",
              list.head == &node1);
    mu_assert("Testing list insert head",
              node1.next == &node3);
    linked_list_insert(&list, &node4);
    mu_assert("Testing list insert tail",
              node3.next == &node4);
    mu_assert("Testing list insert tail",
              node4.next == NULL);
    linked_list_insert(&list, &node2);
    mu_assert("Testing list insert middle",
              node1.next == &node2);
    mu_assert("Testing list insert middle",
              node2.next == &node3);
    linked_list_insert(&list, &node5);
    mu_assert("Testing list insert tail",
              node4.next == &node5);
    mu_assert("Testing list insert tail",
              node5.next == NULL);
    return NULL;
}

static char* test_list_insert_duplicate_sorted_by_perimeter()
{
    rectangle_t rectangle1;
    rectangle_t rectangle3;
    triangle_t triangle3;
    rectangle_t rectangle4;
    rectangle_t rectangle5;
    rectangle_construct(&rectangle1, "rectangle1", 0.25, 0.25);
    rectangle_construct(&rectangle3, "rectangle3", 0.5, 1);
    triangle_construct(&triangle3, "triangle3", 1);
    rectangle_construct(&rectangle4, "rectangle4", 1, 1);
    rectangle_construct(&rectangle5, "rectangle5", 1, 1.5);
    linked_list_node_t node1 = {(shape_t*)&rectangle1, NULL};
    linked_list_node_t node2 = {(shape_t*)&rectangle3, NULL};
    linked_list_node_t node3 = {(shape_t*)&triangle3, NULL};
    linked_list_node_t node4 = {(shape_t*)&rectangle4, NULL};
    linked_list_node_t node5 = {(shape_t*)&rectangle5, NULL};
    linked_list_t list;
    linked_list_init(&list, compare_by_perimeter);
    linked_list_insert(&list, &node1);
    mu_assert("Testing list insert",
              list.head == &node1);
    mu_assert("Testing list insert",
              node1.next == NULL);
    linked_list_insert(&list, &node2);
    mu_assert("Testing list insert",
              node1.next == &node2);
    mu_assert("Testing list insert",
              node2.next == NULL);
    linked_list_insert(&list, &node3);
    mu_assert("Testing list insert (duplicate)",
              shape_perimeter(node1.next->shape) == 3);
    mu_assert("Testing list insert (duplicate)",
              shape_perimeter(node1.next->next->shape) == 3);
    mu_assert("Testing list insert (duplicate)",
              node1.next->next->next == NULL);
    linked_list_insert(&list, &node4);
    mu_assert("Testing list insert",
              node1.next->next->next == &node4);
    mu_assert("Testing list insert",
              node4.next == NULL);
    linked_list_insert(&list, &node5);
    mu_assert("Testing list insert",
              node4.next == &node5);
    mu_assert("Testing list insert",
              node5.next == NULL);
    return NULL;
}

static char* test_list_remove_single_head()
{
    linked_list_node_t node5 = {(shape_t*)5, NULL};
    linked_list_node_t node4 = {(shape_t*)4, &node5};
    linked_list_node_t node3 = {(shape_t*)3, &node4};
    linked_list_node_t node2 = {(shape_t*)2, &node3};
    linked_list_node_t node1 = {(shape_t*)1, &node2};
    linked_list_t list = {NULL, &node1};
    linked_list_remove(&list, (shape_t*)1);
    mu_assert("Testing list remove head",
              list.head == &node2);
    return NULL;
}

static char* test_list_remove_single_tail()
{
    linked_list_node_t node5 = {(shape_t*)5, NULL};
    linked_list_node_t node4 = {(shape_t*)4, &node5};
    linked_list_node_t node3 = {(shape_t*)3, &node4};
    linked_list_node_t node2 = {(shape_t*)2, &node3};
    linked_list_node_t node1 = {(shape_t*)1, &node2};
    linked_list_t list = {NULL, &node1};
    linked_list_remove(&list, (shape_t*)5);
    mu_assert("Testing list remove tail",
              node4.next == NULL);
    return NULL;
}

static char* test_list_remove_single_middle()
{
    linked_list_node_t node5 = {(shape_t*)5, NULL};
    linked_list_node_t node4 = {(shape_t*)4, &node5};
    linked_list_node_t node3 = {(shape_t*)3, &node4};
    linked_list_node_t node2 = {(shape_t*)2, &node3};
    linked_list_node_t node1 = {(shape_t*)1, &node2};
    linked_list_t list = {NULL, &node1};
    linked_list_remove(&list, (shape_t*)3);
    mu_assert("Testing list remove tail",
              node2.next == &node4);
    return NULL;
}

static char* test_list_remove_multiple_alternating()
{
    linked_list_node_t node7 = {(shape_t*)7, NULL};
    linked_list_node_t node6 = {(shape_t*)6, &node7};
    linked_list_node_t node5 = {(shape_t*)7, &node6};
    linked_list_node_t node4 = {(shape_t*)4, &node5};
    linked_list_node_t node3 = {(shape_t*)7, &node4};
    linked_list_node_t node2 = {(shape_t*)2, &node3};
    linked_list_node_t node1 = {(shape_t*)7, &node2};
    linked_list_t listA = {NULL, &node1};
    linked_list_remove(&listA, (shape_t*)7);
    mu_assert("Testing list remove multiple: head, middle, and tail",
              listA.head == &node2);
    mu_assert("Testing list remove multiple: head, middle, and tail",
              node2.next == &node4);
    mu_assert("Testing list remove multiple: head, middle, and tail",
              node4.next == &node6);
    mu_assert("Testing list remove multiple: head, middle, and tail",
              node6.next == NULL);
    linked_list_node_t node11 = {(shape_t*)7, NULL};
    linked_list_node_t node10 = {(shape_t*)10, &node11};
    linked_list_node_t node9 = {(shape_t*)7, &node10};
    linked_list_node_t node8 = {(shape_t*)8, &node9};
    linked_list_t listB = {NULL, &node8};
    linked_list_remove(&listB, (shape_t*)7);
    mu_assert("Testing list remove multiple: middle and tail",
              listB.head == &node8);
    mu_assert("Testing list remove multiple: middle and tail",
              node8.next == &node10);
    mu_assert("Testing list remove multiple: middle and tail",
              node10.next == NULL);
    linked_list_node_t node15 = {(shape_t*)15, NULL};
    linked_list_node_t node14 = {(shape_t*)7, &node15};
    linked_list_node_t node13 = {(shape_t*)13, &node14};
    linked_list_node_t node12 = {(shape_t*)7, &node13};
    linked_list_t listC = {NULL, &node12};
    linked_list_remove(&listC, (shape_t*)7);
    mu_assert("Testing list remove multiple: head and middle",
              listC.head == &node13);
    mu_assert("Testing list remove multiple: head and middle",
              node13.next == &node15);
    mu_assert("Testing list remove multiple: head and middle",
              node15.next == NULL);
    return NULL;
}

static char* test_list_remove_multiple_adjacent()
{
    linked_list_node_t node8 = {(shape_t*)7, NULL};
    linked_list_node_t node7 = {(shape_t*)7, &node8};
    linked_list_node_t node6 = {(shape_t*)6, &node7};
    linked_list_node_t node5 = {(shape_t*)7, &node6};
    linked_list_node_t node4 = {(shape_t*)7, &node5};
    linked_list_node_t node3 = {(shape_t*)3, &node4};
    linked_list_node_t node2 = {(shape_t*)7, &node3};
    linked_list_node_t node1 = {(shape_t*)7, &node2};
    linked_list_t listA = {NULL, &node1};
    linked_list_remove(&listA, (shape_t*)7);
    mu_assert("Testing list remove multiple: head, middle, and tail",
              listA.head == &node3);
    mu_assert("Testing list remove multiple: head, middle, and tail",
              node3.next == &node6);
    mu_assert("Testing list remove multiple: head, middle, and tail",
              node6.next == NULL);
    linked_list_node_t node12 = {(shape_t*)7, NULL};
    linked_list_node_t node11 = {(shape_t*)7, &node12};
    linked_list_node_t node10 = {(shape_t*)7, &node11};
    linked_list_node_t node9 = {(shape_t*)9, &node10};
    linked_list_t listB = {NULL, &node9};
    linked_list_remove(&listB, (shape_t*)7);
    mu_assert("Testing list remove multiple: tail",
              listB.head == &node9);
    mu_assert("Testing list remove multiple: tail",
              node9.next == NULL);
    linked_list_node_t node16 = {(shape_t*)16, NULL};
    linked_list_node_t node15 = {(shape_t*)7, &node16};
    linked_list_node_t node14 = {(shape_t*)7, &node15};
    linked_list_node_t node13 = {(shape_t*)7, &node14};
    linked_list_t listC = {NULL, &node13};
    linked_list_remove(&listC, (shape_t*)7);
    mu_assert("Testing list remove multiple: head",
              listC.head == &node16);
    mu_assert("Testing list remove multiple: head",
              node16.next == NULL);
    linked_list_node_t node20 = {(shape_t*)20, NULL};
    linked_list_node_t node19 = {(shape_t*)7, &node20};
    linked_list_node_t node18 = {(shape_t*)7, &node19};
    linked_list_node_t node17 = {(shape_t*)17, &node18};
    linked_list_t listD = {NULL, &node17};
    linked_list_remove(&listD, (shape_t*)7);
    mu_assert("Testing list remove multiple: middle",
              listD.head == &node17);
    mu_assert("Testing list remove multiple: middle",
              node17.next == &node20);
    mu_assert("Testing list remove multiple: middle",
              node20.next == NULL);
    return NULL;
}

static bool tree_less_than(tree_node_t* node1, tree_node_t* node2)
{
    return (node1->shape < node2->shape) || ((node1->shape == node2->shape) && (node1 < node2));
}

static void tree_insert(tree_node_t* root, tree_node_t* node)
{
    if (tree_less_than(node, root)) {
        if (root->left) {
            tree_insert(root->left, node);
        } else {
            root->left = node;
        }
    } else {
        if (root->right) {
            tree_insert(root->right, node);
        } else {
            root->right = node;
        }
    }
}

static shape_t* tree_build(tree_node_t* nodes, int num_nodes)
{
    tree_node_t* root = &nodes[0];
    shape_t* min_shape = root->shape;
    for (int i = 1; i < num_nodes; i++) {
        if (nodes[i].shape < min_shape) {
            min_shape = nodes[i].shape;
        }
        tree_insert(root, &nodes[i]);
    }
    return min_shape;
}

static void tree_node_init(tree_node_t* node, int val)
{
    node->shape = (shape_t*)(unsigned long)val;
    node->left = NULL;
    node->right = NULL;
}

static void tree_cut(tree_node_t* root, tree_node_t* node_to_cut, tree_node_t** cut_node, tree_node_t** cut_parent)
{
    tree_node_t* left1 = NULL;
    tree_node_t* left2 = NULL;
    tree_node_t* curr = root;
    while (curr != node_to_cut) {
        if (tree_less_than(node_to_cut, curr)) {
            left2 = left1;
            left1 = curr;
            curr = curr->left;
        } else {
            curr = curr->right;
        }
    }
    if (left2) {
        *cut_parent = left2;
        *cut_node = left2->left;
        left2->left = NULL;
    } else {
        *cut_parent = NULL;
        *cut_node = NULL;
    }
}

static void tree_restore(tree_node_t* cut_node, tree_node_t* cut_parent)
{
    if (cut_parent) {
        cut_parent->left = cut_node;
    }
}

static char* tree_verify(tree_node_t* root, shape_t* min_shape, int num_nodes, bool cut)
{
    tree_node_t* cut_node = NULL;
    tree_node_t* cut_parent = NULL;
    int node_count = 0;
    shape_t* prev = NULL;
    tree_iterator_t iter;
    tree_iterator_begin(&iter, root);
    mu_assert("Testing iteration begins at the right node",
              min_shape == tree_iterator_get_shape(&iter));
    while (!tree_iterator_at_end(&iter)) {
        mu_assert("Testing tree iteration in correct order",
                  prev <= tree_iterator_get_shape(&iter));
        mu_assert("Testing node points to correct shape",
                  tree_iterator_get_node(&iter)->shape == tree_iterator_get_shape(&iter));
        node_count++;
        if (cut) {
            tree_cut(root, tree_iterator_get_node(&iter), &cut_node, &cut_parent);
        }
        prev = tree_iterator_get_shape(&iter);
        tree_iterator_next(&iter);
        if (cut) {
            tree_restore(cut_node, cut_parent);
        }
    }
    mu_assert("Testing at end NULL shape",
              tree_iterator_get_shape(&iter) == NULL);
    mu_assert("Testing at end NULL node",
              tree_iterator_get_node(&iter) == NULL);
    mu_assert("Testing correct number of nodes - did not iterate through all the nodes",
              node_count == num_nodes);
    return NULL;
}

static char* tree_build_verify(tree_node_t* nodes, int num_nodes, bool cut)
{
    shape_t* min_shape = tree_build(nodes, num_nodes);
    tree_node_t* root = &nodes[0];
    return tree_verify(root, min_shape, num_nodes, cut);
}

static char* test_tree_iterator_empty_tree()
{
    tree_iterator_t iter;
    tree_iterator_begin(&iter, NULL);
    mu_assert("test_tree_iterator_empty_tree: Testing at end",
              tree_iterator_at_end(&iter));
    mu_assert("test_tree_iterator_empty_tree: Testing at end NULL shape",
              tree_iterator_get_shape(&iter) == NULL);
    mu_assert("test_tree_iterator_empty_tree: Testing at end NULL node",
              tree_iterator_get_node(&iter) == NULL);
    tree_iterator_next(&iter);
    mu_assert("test_tree_iterator_empty_tree: Testing still at end",
              tree_iterator_at_end(&iter));
    mu_assert("test_tree_iterator_empty_tree: Testing still at end NULL shape",
              tree_iterator_get_shape(&iter) == NULL);
    mu_assert("test_tree_iterator_empty_tree: Testing still at end NULL node",
              tree_iterator_get_node(&iter) == NULL);
    return NULL;
}

static char* test_tree_iterator_single_node()
{
    tree_iterator_t iter;
    tree_node_t node;
    tree_node_init(&node, 12345);
    tree_iterator_begin(&iter, &node);
    mu_assert("test_tree_iterator_single_node: Testing not at end",
              !tree_iterator_at_end(&iter));
    mu_assert("test_tree_iterator_single_node: Testing correct shape",
              tree_iterator_get_shape(&iter) == (shape_t*)(long)12345);
    mu_assert("test_tree_iterator_single_node: Testing correct node",
              tree_iterator_get_node(&iter) == &node);
    tree_iterator_next(&iter);
    mu_assert("test_tree_iterator_single_node: Testing at end",
              tree_iterator_at_end(&iter));
    mu_assert("test_tree_iterator_single_node: Testing at end NULL shape",
              tree_iterator_get_shape(&iter) == NULL);
    mu_assert("test_tree_iterator_single_node: Testing at end NULL node",
              tree_iterator_get_node(&iter) == NULL);
    return NULL;
}

static char* test_tree_iterator_small()
{
    tree_node_t nodes[3];
    tree_node_init(&nodes[0], 2);
    tree_node_init(&nodes[1], 1);
    tree_node_init(&nodes[2], 3);
    return tree_build_verify(nodes, 3, false);
}

static char* test_tree_iterator_medium()
{
    tree_node_t nodes[7];
    tree_node_init(&nodes[0], 4);
    tree_node_init(&nodes[1], 2);
    tree_node_init(&nodes[2], 6);
    tree_node_init(&nodes[3], 1);
    tree_node_init(&nodes[4], 3);
    tree_node_init(&nodes[5], 5);
    tree_node_init(&nodes[6], 7);
    return tree_build_verify(nodes, 7, false);
}

static char* test_tree_iterator_medium_cut_edges()
{
    tree_node_t nodes[7];
    tree_node_init(&nodes[0], 4);
    tree_node_init(&nodes[1], 2);
    tree_node_init(&nodes[2], 6);
    tree_node_init(&nodes[3], 1);
    tree_node_init(&nodes[4], 3);
    tree_node_init(&nodes[5], 5);
    tree_node_init(&nodes[6], 7);
    return tree_build_verify(nodes, 7, true);
}

static char* test_tree_iterator_large()
{
    tree_node_t nodes[31];
    tree_node_init(&nodes[0], 16);
    tree_node_init(&nodes[1], 8);
    tree_node_init(&nodes[2], 24);
    tree_node_init(&nodes[3], 4);
    tree_node_init(&nodes[4], 12);
    tree_node_init(&nodes[5], 20);
    tree_node_init(&nodes[6], 28);
    tree_node_init(&nodes[7], 2);
    tree_node_init(&nodes[8], 6);
    tree_node_init(&nodes[9], 10);
    tree_node_init(&nodes[10], 14);
    tree_node_init(&nodes[11], 18);
    tree_node_init(&nodes[12], 22);
    tree_node_init(&nodes[13], 26);
    tree_node_init(&nodes[14], 30);
    tree_node_init(&nodes[15], 1);
    tree_node_init(&nodes[16], 3);
    tree_node_init(&nodes[17], 5);
    tree_node_init(&nodes[18], 7);
    tree_node_init(&nodes[19], 9);
    tree_node_init(&nodes[20], 11);
    tree_node_init(&nodes[21], 13);
    tree_node_init(&nodes[22], 15);
    tree_node_init(&nodes[23], 17);
    tree_node_init(&nodes[24], 19);
    tree_node_init(&nodes[25], 21);
    tree_node_init(&nodes[26], 23);
    tree_node_init(&nodes[27], 25);
    tree_node_init(&nodes[28], 27);
    tree_node_init(&nodes[29], 29);
    tree_node_init(&nodes[30], 31);
    return tree_build_verify(nodes, 31, false);
}

static char* test_tree_iterator_large_cut_edges()
{
    tree_node_t nodes[31];
    tree_node_init(&nodes[0], 16);
    tree_node_init(&nodes[1], 8);
    tree_node_init(&nodes[2], 24);
    tree_node_init(&nodes[3], 4);
    tree_node_init(&nodes[4], 12);
    tree_node_init(&nodes[5], 20);
    tree_node_init(&nodes[6], 28);
    tree_node_init(&nodes[7], 2);
    tree_node_init(&nodes[8], 6);
    tree_node_init(&nodes[9], 10);
    tree_node_init(&nodes[10], 14);
    tree_node_init(&nodes[11], 18);
    tree_node_init(&nodes[12], 22);
    tree_node_init(&nodes[13], 26);
    tree_node_init(&nodes[14], 30);
    tree_node_init(&nodes[15], 1);
    tree_node_init(&nodes[16], 3);
    tree_node_init(&nodes[17], 5);
    tree_node_init(&nodes[18], 7);
    tree_node_init(&nodes[19], 9);
    tree_node_init(&nodes[20], 11);
    tree_node_init(&nodes[21], 13);
    tree_node_init(&nodes[22], 15);
    tree_node_init(&nodes[23], 17);
    tree_node_init(&nodes[24], 19);
    tree_node_init(&nodes[25], 21);
    tree_node_init(&nodes[26], 23);
    tree_node_init(&nodes[27], 25);
    tree_node_init(&nodes[28], 27);
    tree_node_init(&nodes[29], 29);
    tree_node_init(&nodes[30], 31);
    return tree_build_verify(nodes, 31, true);
}

static char* test_tree_iterator_chain()
{
    tree_node_t nodes[MAX_DEPTH];
    for (int i = 0; i < MAX_DEPTH; i++) {
        tree_node_init(&nodes[i], i);
    }
    return tree_build_verify(nodes, MAX_DEPTH, false);
}

static char* test_tree_iterator_reverse_chain()
{
    tree_node_t nodes[MAX_DEPTH];
    for (int i = 0; i < MAX_DEPTH; i++) {
        tree_node_init(&nodes[i], MAX_DEPTH - i);
    }
    return tree_build_verify(nodes, MAX_DEPTH, false);
}

static char* test_tree_iterator_reverse_chain_cut_edges()
{
    tree_node_t nodes[MAX_DEPTH];
    for (int i = 0; i < MAX_DEPTH; i++) {
        tree_node_init(&nodes[i], MAX_DEPTH - i);
    }
    return tree_build_verify(nodes, MAX_DEPTH, true);
}

static char* test_tree_iterator_zigzag()
{
    tree_node_t nodes[MAX_DEPTH];
    for (int i = 0; i < (MAX_DEPTH/2); i++) {
        tree_node_init(&nodes[(2*i)], MAX_DEPTH - i);
        tree_node_init(&nodes[(2*i)+1], i);
    }
    return tree_build_verify(nodes, 2*(MAX_DEPTH/2), false);
}

static char* test_tree_iterator_zigzag_cut_edges()
{
    tree_node_t nodes[MAX_DEPTH];
    for (int i = 0; i < (MAX_DEPTH/2); i++) {
        tree_node_init(&nodes[(2*i)], MAX_DEPTH - i);
        tree_node_init(&nodes[(2*i)+1], i);
    }
    return tree_build_verify(nodes, 2*(MAX_DEPTH/2), true);
}

static char* test_tree_iterator_zagzig()
{
    tree_node_t nodes[MAX_DEPTH];
    for (int i = 0; i < (MAX_DEPTH/2); i++) {
        tree_node_init(&nodes[(2*i)], i);
        tree_node_init(&nodes[(2*i)+1], MAX_DEPTH - i);
    }
    return tree_build_verify(nodes, 2*(MAX_DEPTH/2), false);
}

static char* test_tree_iterator_zagzig_cut_edges()
{
    tree_node_t nodes[MAX_DEPTH];
    for (int i = 0; i < (MAX_DEPTH/2); i++) {
        tree_node_init(&nodes[(2*i)], i);
        tree_node_init(&nodes[(2*i)+1], MAX_DEPTH - i);
    }
    return tree_build_verify(nodes, 2*(MAX_DEPTH/2), true);
}

// May need to update MAX_DEPTH in pointer.h if adjusting these parameters
#define STRESS_SEED1 12345
#define STRESS_SEED2 23456
#define STRESS_SEED3 34567
#define STRESS_SEED4 45678
#define STRESS_SEED5 56789
#define STRESS_NUM_NODES 1000

static char* tree_iterator_stress(unsigned int seed, bool cut)
{
    srand(seed);
    tree_node_t nodes[STRESS_NUM_NODES];
    for (int i = 0; i < STRESS_NUM_NODES; i++) {
        tree_node_init(&nodes[i], rand());
    }
    return tree_build_verify(nodes, STRESS_NUM_NODES, cut);
}

static char* test_tree_iterator_stress1()
{
    return tree_iterator_stress(STRESS_SEED1, false);
}

static char* test_tree_iterator_stress1_cut_edges()
{
    return tree_iterator_stress(STRESS_SEED1, true);
}

static char* test_tree_iterator_stress2()
{
    return tree_iterator_stress(STRESS_SEED2, false);
}

static char* test_tree_iterator_stress2_cut_edges()
{
    return tree_iterator_stress(STRESS_SEED2, true);
}

static char* test_tree_iterator_stress3()
{
    return tree_iterator_stress(STRESS_SEED3, false);
}

static char* test_tree_iterator_stress3_cut_edges()
{
    return tree_iterator_stress(STRESS_SEED3, true);
}

static char* test_tree_iterator_stress4()
{
    return tree_iterator_stress(STRESS_SEED4, false);
}

static char* test_tree_iterator_stress4_cut_edges()
{
    return tree_iterator_stress(STRESS_SEED4, true);
}

static char* test_tree_iterator_stress5()
{
    return tree_iterator_stress(STRESS_SEED5, false);
}

static char* test_tree_iterator_stress5_cut_edges()
{
    return tree_iterator_stress(STRESS_SEED5, true);
}

static char* test_max_min_avg_area_NULL_tree()
{
    tree_node_t* root = NULL;
    double max = -1;
    double min = -1;
    double avg = -1;
    max_min_avg_area(root, &max, &min, &avg);
    mu_assert("Testing max not modified",
              max == -1);
    mu_assert("Testing min not modified",
              min == -1);
    mu_assert("Testing avg not modified",
              avg == -1);
    return NULL;
}

static char* test_max_min_avg_area_single_node_tree()
{
    rectangle_t rectangle1;
    rectangle_construct(&rectangle1, "rectangle1", 3, 5);
    tree_node_t node1 = {(shape_t*)&rectangle1, NULL, NULL};
    tree_node_t* root = &node1;
    double max = 0;
    double min = 0;
    double avg = 0;
    max_min_avg_area(root, &max, &min, &avg);
    mu_assert("Testing max is correct",
              approx_equal(max, 15));
    mu_assert("Testing min is correct",
              approx_equal(min, 15));
    mu_assert("Testing avg is correct",
              approx_equal(avg, 15));
    return NULL;
}

static char* test_max_min_avg_area_balanced_tree()
{
    rectangle_t rectangle1;
    rectangle_t rectangle2;
    rectangle_t rectangle3;
    rectangle_t rectangle4;
    triangle_t triangle5;
    triangle_t triangle6;
    triangle_t triangle7;
    rectangle_construct(&rectangle1, "rectangle1", 3, 5);
    rectangle_construct(&rectangle2, "rectangle2", 1, 1);
    rectangle_construct(&rectangle3, "rectangle3", 13, 15);
    rectangle_construct(&rectangle4, "rectangle4", 2.3, 6.8);
    triangle_construct(&triangle5, "triangle5", sqrt(28/sqrt(3)));
    triangle_construct(&triangle6, "triangle6", sqrt(4000/sqrt(3)));
    triangle_construct(&triangle7, "triangle7", sqrt(8/sqrt(3)));
    tree_node_t node1 = {(shape_t*)&rectangle1, NULL, NULL};
    tree_node_t node3 = {(shape_t*)&rectangle3, NULL, NULL};
    tree_node_t node5 = {(shape_t*)&triangle5, NULL, NULL};
    tree_node_t node7 = {(shape_t*)&triangle7, NULL, NULL};
    tree_node_t node2 = {(shape_t*)&rectangle2, &node1, &node3};
    tree_node_t node6 = {(shape_t*)&triangle6, &node5, &node7};
    tree_node_t node4 = {(shape_t*)&rectangle4, &node2, &node6};
    tree_node_t* root = &node4;
    double max = 0;
    double min = 0;
    double avg = 0;
    max_min_avg_area(root, &max, &min, &avg);
    mu_assert("Testing max is correct",
              approx_equal(max, 1000));
    mu_assert("Testing min is correct",
              approx_equal(min, 1));
    mu_assert("Testing avg is correct",
              approx_equal(avg, 176.52));
    return NULL;
}

static char* test_max_min_avg_area_chain()
{
    rectangle_t rectangle1;
    rectangle_t rectangle2;
    rectangle_t rectangle3;
    rectangle_t rectangle4;
    triangle_t triangle5;
    triangle_t triangle6;
    triangle_t triangle7;
    rectangle_construct(&rectangle1, "rectangle1", 3, 5);
    rectangle_construct(&rectangle2, "rectangle2", 1, 1);
    rectangle_construct(&rectangle3, "rectangle3", 13, 15);
    rectangle_construct(&rectangle4, "rectangle4", 2.3, 6.8);
    triangle_construct(&triangle5, "triangle5", sqrt(28/sqrt(3)));
    triangle_construct(&triangle6, "triangle6", sqrt(4000/sqrt(3)));
    triangle_construct(&triangle7, "triangle7", sqrt(8/sqrt(3)));
    tree_node_t node7 = {(shape_t*)&triangle7, NULL, NULL};
    tree_node_t node6 = {(shape_t*)&triangle6, NULL, &node7};
    tree_node_t node5 = {(shape_t*)&triangle5, NULL, &node6};
    tree_node_t node4 = {(shape_t*)&rectangle4, NULL, &node5};
    tree_node_t node3 = {(shape_t*)&rectangle3, NULL, &node4};
    tree_node_t node2 = {(shape_t*)&rectangle2, NULL, &node3};
    tree_node_t node1 = {(shape_t*)&rectangle1, NULL, &node2};
    tree_node_t* root = &node1;
    double max = 0;
    double min = 0;
    double avg = 0;
    max_min_avg_area(root, &max, &min, &avg);
    mu_assert("Testing max is correct",
              approx_equal(max, 1000));
    mu_assert("Testing min is correct",
              approx_equal(min, 1));
    mu_assert("Testing avg is correct",
              approx_equal(avg, 176.52));
    return NULL;
}

static char* test_max_min_avg_area_reverse_chain()
{
    rectangle_t rectangle1;
    rectangle_t rectangle2;
    rectangle_t rectangle3;
    rectangle_t rectangle4;
    triangle_t triangle5;
    triangle_t triangle6;
    triangle_t triangle7;
    rectangle_construct(&rectangle1, "rectangle1", 3, 5);
    rectangle_construct(&rectangle2, "rectangle2", 1, 1);
    rectangle_construct(&rectangle3, "rectangle3", 13, 15);
    rectangle_construct(&rectangle4, "rectangle4", 2.3, 6.8);
    triangle_construct(&triangle5, "triangle5", sqrt(28/sqrt(3)));
    triangle_construct(&triangle6, "triangle6", sqrt(4000/sqrt(3)));
    triangle_construct(&triangle7, "triangle7", sqrt(8/sqrt(3)));
    tree_node_t node1 = {(shape_t*)&rectangle1, NULL, NULL};
    tree_node_t node2 = {(shape_t*)&rectangle2, &node1, NULL};
    tree_node_t node3 = {(shape_t*)&rectangle3, &node2, NULL};
    tree_node_t node4 = {(shape_t*)&rectangle4, &node3, NULL};
    tree_node_t node5 = {(shape_t*)&triangle5, &node4, NULL};
    tree_node_t node6 = {(shape_t*)&triangle6, &node5, NULL};
    tree_node_t node7 = {(shape_t*)&triangle7, &node6, NULL};
    tree_node_t* root = &node7;
    double max = 0;
    double min = 0;
    double avg = 0;
    max_min_avg_area(root, &max, &min, &avg);
    mu_assert("Testing max is correct",
              approx_equal(max, 1000));
    mu_assert("Testing min is correct",
              approx_equal(min, 1));
    mu_assert("Testing avg is correct",
              approx_equal(avg, 176.52));
    return NULL;
}

static double tens(shape_t* shape, double data) {
    data *= 10;
    data += shape_area(shape);
    return data;
}

static char* test_foreach_NULL_tree()
{
    tree_node_t* root = NULL;
    mu_assert("Testing foreach did not modify data",
              foreach(root, tens, 1) == 1);
    return NULL;
}

static char* test_foreach_single_node_tree()
{
    rectangle_t rectangle1;
    rectangle_construct(&rectangle1, "rectangle1", 1, 1);
    tree_node_t node1 = {(shape_t*)&rectangle1, NULL, NULL};
    tree_node_t* root = &node1;
    mu_assert("Testing foreach tens, single value",
              approx_equal(foreach(root, tens, 1), 11));
    return NULL;
}

static char* test_foreach_balanced_tree()
{
    rectangle_t rectangle1;
    rectangle_t rectangle2;
    rectangle_t rectangle3;
    rectangle_t rectangle4;
    triangle_t triangle5;
    triangle_t triangle6;
    triangle_t triangle7;
    rectangle_construct(&rectangle1, "rectangle1", 1, 1);
    rectangle_construct(&rectangle2, "rectangle2", 1, 2);
    rectangle_construct(&rectangle3, "rectangle3", 1, 3);
    rectangle_construct(&rectangle4, "rectangle4", 1, 4);
    triangle_construct(&triangle5, "triangle5", sqrt(20/sqrt(3)));
    triangle_construct(&triangle6, "triangle6", sqrt(24/sqrt(3)));
    triangle_construct(&triangle7, "triangle7", sqrt(28/sqrt(3)));
    tree_node_t node1 = {(shape_t*)&rectangle1, NULL, NULL};
    tree_node_t node3 = {(shape_t*)&rectangle3, NULL, NULL};
    tree_node_t node5 = {(shape_t*)&triangle5, NULL, NULL};
    tree_node_t node7 = {(shape_t*)&triangle7, NULL, NULL};
    tree_node_t node2 = {(shape_t*)&rectangle2, &node1, &node3};
    tree_node_t node6 = {(shape_t*)&triangle6, &node5, &node7};
    tree_node_t node4 = {(shape_t*)&rectangle4, &node2, &node6};
    tree_node_t* root = &node4;
    mu_assert("Testing foreach tens",
              approx_equal(foreach(root, tens, 1), 11234567));
    return NULL;
}

static char* test_foreach_chain()
{
    rectangle_t rectangle1;
    rectangle_t rectangle2;
    rectangle_t rectangle3;
    rectangle_t rectangle4;
    triangle_t triangle5;
    triangle_t triangle6;
    triangle_t triangle7;
    rectangle_construct(&rectangle1, "rectangle1", 1, 1);
    rectangle_construct(&rectangle2, "rectangle2", 1, 2);
    rectangle_construct(&rectangle3, "rectangle3", 1, 3);
    rectangle_construct(&rectangle4, "rectangle4", 1, 4);
    triangle_construct(&triangle5, "triangle5", sqrt(20/sqrt(3)));
    triangle_construct(&triangle6, "triangle6", sqrt(24/sqrt(3)));
    triangle_construct(&triangle7, "triangle7", sqrt(28/sqrt(3)));
    tree_node_t node7 = {(shape_t*)&triangle7, NULL, NULL};
    tree_node_t node6 = {(shape_t*)&triangle6, NULL, &node7};
    tree_node_t node5 = {(shape_t*)&triangle5, NULL, &node6};
    tree_node_t node4 = {(shape_t*)&rectangle4, NULL, &node5};
    tree_node_t node3 = {(shape_t*)&rectangle3, NULL, &node4};
    tree_node_t node2 = {(shape_t*)&rectangle2, NULL, &node3};
    tree_node_t node1 = {(shape_t*)&rectangle1, NULL, &node2};
    tree_node_t* root = &node1;
    mu_assert("Testing foreach tens",
              approx_equal(foreach(root, tens, 1), 11234567));
    return NULL;
}

static char* test_foreach_reverse_chain()
{
    rectangle_t rectangle1;
    rectangle_t rectangle2;
    rectangle_t rectangle3;
    rectangle_t rectangle4;
    triangle_t triangle5;
    triangle_t triangle6;
    triangle_t triangle7;
    rectangle_construct(&rectangle1, "rectangle1", 1, 1);
    rectangle_construct(&rectangle2, "rectangle2", 1, 2);
    rectangle_construct(&rectangle3, "rectangle3", 1, 3);
    rectangle_construct(&rectangle4, "rectangle4", 1, 4);
    triangle_construct(&triangle5, "triangle5", sqrt(20/sqrt(3)));
    triangle_construct(&triangle6, "triangle6", sqrt(24/sqrt(3)));
    triangle_construct(&triangle7, "triangle7", sqrt(28/sqrt(3)));
    tree_node_t node1 = {(shape_t*)&rectangle1, NULL, NULL};
    tree_node_t node2 = {(shape_t*)&rectangle2, &node1, NULL};
    tree_node_t node3 = {(shape_t*)&rectangle3, &node2, NULL};
    tree_node_t node4 = {(shape_t*)&rectangle4, &node3, NULL};
    tree_node_t node5 = {(shape_t*)&triangle5, &node4, NULL};
    tree_node_t node6 = {(shape_t*)&triangle6, &node5, NULL};
    tree_node_t node7 = {(shape_t*)&triangle7, &node6, NULL};
    tree_node_t* root = &node7;
    mu_assert("Testing foreach tens",
              approx_equal(foreach(root, tens, 1), 11234567));
    return NULL;
}

typedef char* (*test_fn)();
typedef struct {
    char* name;
    test_fn test;
} test_t;

#define TEST_CASE(test_name) {#test_name, test_name}

static test_t tests[] = {TEST_CASE(test_rectangle_area),
                         TEST_CASE(test_triangle_area),
                         TEST_CASE(test_rectangle_perimeter),
                         TEST_CASE(test_triangle_perimeter),
                         TEST_CASE(test_rectangle_construct),
                         TEST_CASE(test_triangle_construct),
                         TEST_CASE(test_compare_by_area),
                         TEST_CASE(test_compare_by_perimeter),
                         TEST_CASE(test_list_insert_single_unsorted_empty_start),
                         TEST_CASE(test_list_insert_single_unsorted_nonempty_start),
                         TEST_CASE(test_list_insert_multiple_unsorted),
                         TEST_CASE(test_list_insert_sorted_order_sorted_by_shape_value),
                         TEST_CASE(test_list_insert_reverse_order_sorted_by_shape_value),
                         TEST_CASE(test_list_insert_alternating_order_sorted_by_shape_value),
                         TEST_CASE(test_list_insert_random_order_sorted_by_shape_value),
                         TEST_CASE(test_list_insert_duplicate_sorted_by_shape_value),
                         TEST_CASE(test_list_insert_sorted_order_sorted_by_perimeter),
                         TEST_CASE(test_list_insert_reverse_order_sorted_by_perimeter),
                         TEST_CASE(test_list_insert_alternating_order_sorted_by_perimeter),
                         TEST_CASE(test_list_insert_random_order_sorted_by_perimeter),
                         TEST_CASE(test_list_insert_duplicate_sorted_by_perimeter),
                         TEST_CASE(test_list_remove_single_head),
                         TEST_CASE(test_list_remove_single_tail),
                         TEST_CASE(test_list_remove_single_middle),
                         TEST_CASE(test_list_remove_multiple_alternating),
                         TEST_CASE(test_list_remove_multiple_adjacent),
                         TEST_CASE(test_tree_iterator_empty_tree),
                         TEST_CASE(test_tree_iterator_single_node),
                         TEST_CASE(test_tree_iterator_small),
                         TEST_CASE(test_tree_iterator_medium),
                         TEST_CASE(test_tree_iterator_medium_cut_edges),
                         TEST_CASE(test_tree_iterator_large),
                         TEST_CASE(test_tree_iterator_large_cut_edges),
                         TEST_CASE(test_tree_iterator_chain),
                         TEST_CASE(test_tree_iterator_reverse_chain),
                         TEST_CASE(test_tree_iterator_reverse_chain_cut_edges),
                         TEST_CASE(test_tree_iterator_zigzag),
                         TEST_CASE(test_tree_iterator_zigzag_cut_edges),
                         TEST_CASE(test_tree_iterator_zagzig),
                         TEST_CASE(test_tree_iterator_zagzig_cut_edges),
                         TEST_CASE(test_tree_iterator_stress1),
                         TEST_CASE(test_tree_iterator_stress1_cut_edges),
                         TEST_CASE(test_tree_iterator_stress2),
                         TEST_CASE(test_tree_iterator_stress2_cut_edges),
                         TEST_CASE(test_tree_iterator_stress3),
                         TEST_CASE(test_tree_iterator_stress3_cut_edges),
                         TEST_CASE(test_tree_iterator_stress4),
                         TEST_CASE(test_tree_iterator_stress4_cut_edges),
                         TEST_CASE(test_tree_iterator_stress5),
                         TEST_CASE(test_tree_iterator_stress5_cut_edges),
                         TEST_CASE(test_max_min_avg_area_NULL_tree),
                         TEST_CASE(test_max_min_avg_area_single_node_tree),
                         TEST_CASE(test_max_min_avg_area_balanced_tree),
                         TEST_CASE(test_max_min_avg_area_chain),
                         TEST_CASE(test_max_min_avg_area_reverse_chain),
                         TEST_CASE(test_foreach_NULL_tree),
                         TEST_CASE(test_foreach_single_node_tree),
                         TEST_CASE(test_foreach_balanced_tree),
                         TEST_CASE(test_foreach_chain),
                         TEST_CASE(test_foreach_reverse_chain)};
static size_t num_tests = sizeof(tests)/sizeof(tests[0]);

#define MAX_MESSAGE_LEN 4096
static char error_message[MAX_MESSAGE_LEN];
static int tests_run = 0;
static char* single_test(test_t* test, size_t iters)
{
    for (size_t i = 0; i < iters; i++) {
        char* message = test->test();
        tests_run++;
        if (message) {
            snprintf(error_message, MAX_MESSAGE_LEN, "FAILURE: %s: %s", test->name, message);
            return error_message;
        }
    }
    return NULL;
}

static char* all_tests(size_t iters)
{
    for (size_t i = 0; i < num_tests; i++) {
        char* result = single_test(&tests[i], iters);
        if (result) {
            return result;
        }
    }
    return NULL;
}

int main(int argc, char** argv)
{
    char* result = NULL;
    size_t iters = 1;
    if (argc == 1) {
        result = all_tests(iters);
        if (result) {
            printf("%s\n", result);
        } else {
            printf("ALL TESTS PASSED\n");
        }
        printf("Tests run: %d\n", tests_run);
        return result != NULL;
    } else if (argc == 3) {
        iters = (size_t)atoi(argv[2]);
    } else if (argc > 3) {
        printf("Wrong number of arguments, only one test is accepted at time");
    }
    result = "Did not find test";
    for (size_t i = 0; i < num_tests; i++) {
        if (string_equal(argv[1], tests[i].name)) {
            result = single_test(&tests[i], iters);
            break;
        }
    }
    if (result) {
        printf("%s\n", result);
    }
    else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);
    return result != NULL;
}
