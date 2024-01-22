#!/usr/bin/env python3

import os
import shutil
import subprocess
import signal

# Location of sandbox
sandbox_dir = "sandbox"

# Location of original files and the files to copy
original_dir = "."
files_to_copy = ["Makefile", "pointer.h", "test.c"]

# Handin file
handin_file = "pointer.c"

# Timeouts (seconds)
timeout_default = 2
timeout_make = 30

# Test cases
test_cases = {
    "test_rectangle_area": {"args": ["./pointer", "test_rectangle_area"], "timeout": timeout_default},
    "test_triangle_area": {"args": ["./pointer", "test_triangle_area"], "timeout": timeout_default},
    "test_rectangle_perimeter": {"args": ["./pointer", "test_rectangle_perimeter"], "timeout": timeout_default},
    "test_triangle_perimeter": {"args": ["./pointer", "test_triangle_perimeter"], "timeout": timeout_default},
    "test_rectangle_construct": {"args": ["./pointer", "test_rectangle_construct"], "timeout": timeout_default},
    "test_triangle_construct": {"args": ["./pointer", "test_triangle_construct"], "timeout": timeout_default},
    "test_compare_by_area": {"args": ["./pointer", "test_compare_by_area"], "timeout": timeout_default},
    "test_compare_by_perimeter": {"args": ["./pointer", "test_compare_by_perimeter"], "timeout": timeout_default},
    "test_list_insert_single_unsorted_empty_start": {"args": ["./pointer", "test_list_insert_single_unsorted_empty_start"], "timeout": timeout_default},
    "test_list_insert_single_unsorted_nonempty_start": {"args": ["./pointer", "test_list_insert_single_unsorted_nonempty_start"], "timeout": timeout_default},
    "test_list_insert_multiple_unsorted": {"args": ["./pointer", "test_list_insert_multiple_unsorted"], "timeout": timeout_default},
    "test_list_insert_sorted_order_sorted_by_shape_value": {"args": ["./pointer", "test_list_insert_sorted_order_sorted_by_shape_value"], "timeout": timeout_default},
    "test_list_insert_reverse_order_sorted_by_shape_value": {"args": ["./pointer", "test_list_insert_reverse_order_sorted_by_shape_value"], "timeout": timeout_default},
    "test_list_insert_alternating_order_sorted_by_shape_value": {"args": ["./pointer", "test_list_insert_alternating_order_sorted_by_shape_value"], "timeout": timeout_default},
    "test_list_insert_random_order_sorted_by_shape_value": {"args": ["./pointer", "test_list_insert_random_order_sorted_by_shape_value"], "timeout": timeout_default},
    "test_list_insert_duplicate_sorted_by_shape_value": {"args": ["./pointer", "test_list_insert_duplicate_sorted_by_shape_value"], "timeout": timeout_default},
    "test_list_insert_sorted_order_sorted_by_perimeter": {"args": ["./pointer", "test_list_insert_sorted_order_sorted_by_perimeter"], "timeout": timeout_default},
    "test_list_insert_reverse_order_sorted_by_perimeter": {"args": ["./pointer", "test_list_insert_reverse_order_sorted_by_perimeter"], "timeout": timeout_default},
    "test_list_insert_alternating_order_sorted_by_perimeter": {"args": ["./pointer", "test_list_insert_alternating_order_sorted_by_perimeter"], "timeout": timeout_default},
    "test_list_insert_random_order_sorted_by_perimeter": {"args": ["./pointer", "test_list_insert_random_order_sorted_by_perimeter"], "timeout": timeout_default},
    "test_list_insert_duplicate_sorted_by_perimeter": {"args": ["./pointer", "test_list_insert_duplicate_sorted_by_perimeter"], "timeout": timeout_default},
    "test_list_remove_single_head": {"args": ["./pointer", "test_list_remove_single_head"], "timeout": timeout_default},
    "test_list_remove_single_tail": {"args": ["./pointer", "test_list_remove_single_tail"], "timeout": timeout_default},
    "test_list_remove_single_middle": {"args": ["./pointer", "test_list_remove_single_middle"], "timeout": timeout_default},
    "test_list_remove_multiple_alternating": {"args": ["./pointer", "test_list_remove_multiple_alternating"], "timeout": timeout_default},
    "test_list_remove_multiple_adjacent": {"args": ["./pointer", "test_list_remove_multiple_adjacent"], "timeout": timeout_default},
    "test_tree_iterator_empty_tree": {"args": ["./pointer", "test_tree_iterator_empty_tree"], "timeout": timeout_default},
    "test_tree_iterator_single_node": {"args": ["./pointer", "test_tree_iterator_single_node"], "timeout": timeout_default},
    "test_tree_iterator_small": {"args": ["./pointer", "test_tree_iterator_small"], "timeout": timeout_default},
    "test_tree_iterator_medium": {"args": ["./pointer", "test_tree_iterator_medium"], "timeout": timeout_default},
    "test_tree_iterator_medium_cut_edges": {"args": ["./pointer", "test_tree_iterator_medium_cut_edges"], "timeout": timeout_default},
    "test_tree_iterator_large": {"args": ["./pointer", "test_tree_iterator_large"], "timeout": timeout_default},
    "test_tree_iterator_large_cut_edges": {"args": ["./pointer", "test_tree_iterator_large_cut_edges"], "timeout": timeout_default},
    "test_tree_iterator_chain": {"args": ["./pointer", "test_tree_iterator_chain"], "timeout": timeout_default},
    "test_tree_iterator_reverse_chain": {"args": ["./pointer", "test_tree_iterator_reverse_chain"], "timeout": timeout_default},
    "test_tree_iterator_reverse_chain_cut_edges": {"args": ["./pointer", "test_tree_iterator_reverse_chain_cut_edges"], "timeout": timeout_default},
    "test_tree_iterator_zigzag": {"args": ["./pointer", "test_tree_iterator_zigzag"], "timeout": timeout_default},
    "test_tree_iterator_zigzag_cut_edges": {"args": ["./pointer", "test_tree_iterator_zigzag_cut_edges"], "timeout": timeout_default},
    "test_tree_iterator_zagzig": {"args": ["./pointer", "test_tree_iterator_zagzig"], "timeout": timeout_default},
    "test_tree_iterator_zagzig_cut_edges": {"args": ["./pointer", "test_tree_iterator_zagzig_cut_edges"], "timeout": timeout_default},
    "test_tree_iterator_stress1": {"args": ["./pointer", "test_tree_iterator_stress1"], "timeout": timeout_default},
    "test_tree_iterator_stress1_cut_edges": {"args": ["./pointer", "test_tree_iterator_stress1_cut_edges"], "timeout": timeout_default},
    "test_tree_iterator_stress2": {"args": ["./pointer", "test_tree_iterator_stress2"], "timeout": timeout_default},
    "test_tree_iterator_stress2_cut_edges": {"args": ["./pointer", "test_tree_iterator_stress2_cut_edges"], "timeout": timeout_default},
    "test_tree_iterator_stress3": {"args": ["./pointer", "test_tree_iterator_stress3"], "timeout": timeout_default},
    "test_tree_iterator_stress3_cut_edges": {"args": ["./pointer", "test_tree_iterator_stress3_cut_edges"], "timeout": timeout_default},
    "test_tree_iterator_stress4": {"args": ["./pointer", "test_tree_iterator_stress4"], "timeout": timeout_default},
    "test_tree_iterator_stress4_cut_edges": {"args": ["./pointer", "test_tree_iterator_stress4_cut_edges"], "timeout": timeout_default},
    "test_tree_iterator_stress5": {"args": ["./pointer", "test_tree_iterator_stress5"], "timeout": timeout_default},
    "test_tree_iterator_stress5_cut_edges": {"args": ["./pointer", "test_tree_iterator_stress5_cut_edges"], "timeout": timeout_default},
    "test_max_min_avg_area_NULL_tree": {"args": ["./pointer", "test_max_min_avg_area_NULL_tree"], "timeout": timeout_default},
    "test_max_min_avg_area_single_node_tree": {"args": ["./pointer", "test_max_min_avg_area_single_node_tree"], "timeout": timeout_default},
    "test_max_min_avg_area_balanced_tree": {"args": ["./pointer", "test_max_min_avg_area_balanced_tree"], "timeout": timeout_default},
    "test_max_min_avg_area_chain": {"args": ["./pointer", "test_max_min_avg_area_chain"], "timeout": timeout_default},
    "test_max_min_avg_area_reverse_chain": {"args": ["./pointer", "test_max_min_avg_area_reverse_chain"], "timeout": timeout_default},
    "test_foreach_NULL_tree": {"args": ["./pointer", "test_foreach_NULL_tree"], "timeout": timeout_default},
    "test_foreach_single_node_tree": {"args": ["./pointer", "test_foreach_single_node_tree"], "timeout": timeout_default},
    "test_foreach_balanced_tree": {"args": ["./pointer", "test_foreach_balanced_tree"], "timeout": timeout_default},
    "test_foreach_chain": {"args": ["./pointer", "test_foreach_chain"], "timeout": timeout_default},
    "test_foreach_reverse_chain": {"args": ["./pointer", "test_foreach_reverse_chain"], "timeout": timeout_default},
}

# Score breakdown by points (points, list of tests required to get points)
point_breakdown = [
    # Shape (20 pts)
    (2, ["test_rectangle_area"]),
    (2, ["test_triangle_area"]),
    (2, ["test_rectangle_perimeter"]),
    (2, ["test_triangle_perimeter"]),
    (3, ["test_rectangle_construct"]),
    (3, ["test_triangle_construct"]),
    (3, ["test_compare_by_area"]),
    (3, ["test_compare_by_perimeter"]),
    # Linked list (30 pts)
    (3, ["test_list_insert_single_unsorted_empty_start"]),
    (3, ["test_list_insert_single_unsorted_nonempty_start"]),
    (4, ["test_list_insert_multiple_unsorted"]),
    (1, ["test_list_insert_sorted_order_sorted_by_shape_value"]),
    (1, ["test_list_insert_reverse_order_sorted_by_shape_value"]),
    (1, ["test_list_insert_alternating_order_sorted_by_shape_value"]),
    (1, ["test_list_insert_random_order_sorted_by_shape_value"]),
    (1, ["test_list_insert_duplicate_sorted_by_shape_value"]),
    (1, ["test_list_insert_sorted_order_sorted_by_perimeter"]),
    (1, ["test_list_insert_reverse_order_sorted_by_perimeter"]),
    (1, ["test_list_insert_alternating_order_sorted_by_perimeter"]),
    (1, ["test_list_insert_random_order_sorted_by_perimeter"]),
    (1, ["test_list_insert_duplicate_sorted_by_perimeter"]),
    (2, ["test_list_remove_single_head"]),
    (2, ["test_list_remove_single_tail"]),
    (2, ["test_list_remove_single_middle"]),
    (2, ["test_list_remove_multiple_alternating"]),
    (2, ["test_list_remove_multiple_adjacent"]),
    # Tree iterator (30pts)
    (1, ["test_tree_iterator_empty_tree"]),
    (1, ["test_tree_iterator_single_node"]),
    (1, ["test_tree_iterator_small"]),
    (3, ["test_tree_iterator_medium", "test_tree_iterator_medium_cut_edges"]),
    (4, ["test_tree_iterator_large", "test_tree_iterator_large_cut_edges"]),
    (2, ["test_tree_iterator_chain"]),
    (2, ["test_tree_iterator_reverse_chain", "test_tree_iterator_reverse_chain_cut_edges"]),
    (3, ["test_tree_iterator_zigzag", "test_tree_iterator_zigzag_cut_edges"]),
    (3, ["test_tree_iterator_zagzig", "test_tree_iterator_zagzig_cut_edges"]),
    (2, ["test_tree_iterator_stress1", "test_tree_iterator_stress1_cut_edges"]),
    (2, ["test_tree_iterator_stress2", "test_tree_iterator_stress2_cut_edges"]),
    (2, ["test_tree_iterator_stress3", "test_tree_iterator_stress3_cut_edges"]),
    (2, ["test_tree_iterator_stress4", "test_tree_iterator_stress4_cut_edges"]),
    (2, ["test_tree_iterator_stress5", "test_tree_iterator_stress5_cut_edges"]),
    # Tree analysis (20 pts)
    (2, ["test_max_min_avg_area_NULL_tree"]),
    (2, ["test_max_min_avg_area_single_node_tree"]),
    (2, ["test_max_min_avg_area_balanced_tree"]),
    (2, ["test_max_min_avg_area_chain"]),
    (2, ["test_max_min_avg_area_reverse_chain"]),
    (2, ["test_foreach_NULL_tree"]),
    (2, ["test_foreach_single_node_tree"]),
    (2, ["test_foreach_balanced_tree"]),
    (2, ["test_foreach_chain"]),
    (2, ["test_foreach_reverse_chain"]),
]

def print_success(test):
    print("****SUCCESS: {}****".format(test))

def print_failed(test):
    print("****FAILED: {}****".format(test))

def check_includes():
    with open(handin_file, "r") as f:
        for line in f:
            if "#include" in line:
                if line.rstrip() != "#include \"pointer.h\"":
                    print_failed(line.rstrip() + " is not allowed to be included")
                    return False
    return True

def make_assignment():
    args = ["make", "clean", "all"]
    try:
        subprocess.check_output(args, stderr=subprocess.STDOUT, timeout=timeout_make)
        while not os.access("pointer", os.X_OK | os.R_OK): # VirtualBox SharedFolders workaround
            pass
        return True
    except subprocess.CalledProcessError as e:
        print_failed("make")
        print(e.output.decode())
    except subprocess.TimeoutExpired as e:
        print_failed("make")
        print("Failed to compile within {} seconds".format(e.timeout))
    except KeyboardInterrupt:
        print_failed("make")
        print("User interrupted compilation")
    except:
        print_failed("make")
        print("Unknown error occurred")
    return False

def grade(submission_dir, submission_file):
    result = {}

    # Check file type
    if submission_file.endswith(".c"):
        # Clear previous sandbox
        if os.path.exists(sandbox_dir):
            subprocess.check_output(["rm", "-rf", sandbox_dir], stderr=subprocess.STDOUT)
        os.makedirs(sandbox_dir)

        # Copy handin file
        shutil.copy2(os.path.join(submission_dir, submission_file), os.path.join(sandbox_dir, handin_file))

        # Copy original files
        for f in files_to_copy:
            if os.path.exists(os.path.join(sandbox_dir, f)):
                os.remove(os.path.join(sandbox_dir, f))
            shutil.copy2(os.path.join(original_dir, f), os.path.join(sandbox_dir, f))

        # Switch to sandbox
        cwd = os.getcwd()
        os.chdir(sandbox_dir)

        # Check includes and run make on the assignment
        if check_includes() and make_assignment():
            # Run test cases
            for test, config in test_cases.items():
                try:
                    output = subprocess.check_output(config["args"], stderr=subprocess.STDOUT, timeout=config["timeout"]).decode()
                    if "ALL TESTS PASSED" in output:
                        result[test] = True
                        print_success(test)
                    else:
                        result[test] = False
                        print_failed(test)
                        print(output)
                except subprocess.CalledProcessError as e:
                    result[test] = False
                    print_failed(test)
                    print(e.output.decode())
                    if e.returncode < 0:
                        if -e.returncode == signal.SIGSEGV:
                            print("Segmentation fault (core dumped)")
                        else:
                            print("Died with signal {}".format(-e.returncode))
                except subprocess.TimeoutExpired as e:
                    result[test] = False
                    print_failed(test)
                    print("Failed to complete within {} seconds".format(e.timeout))
                except KeyboardInterrupt:
                    result[test] = False
                    print_failed(test)
                    print("User interrupted test")
                except:
                    result[test] = False
                    print_failed(test)
                    print("Unknown error occurred")

        # Switch back to original directory
        os.chdir(cwd)
    else:
        print_failed("invalid file {}".format(submission_file))

    # Calculate score
    score = 0
    total_possible = 0
    for points, tests in point_breakdown:
        if all(map(lambda test : test in result and result[test], tests)):
            score += points
        total_possible += points
    return (score, total_possible)

if __name__ == "__main__":
    submission_dir = "."
    score, total_possible = grade(submission_dir, handin_file)
    print("Final score: {}/{}".format(score, total_possible))
