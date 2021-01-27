/* Lab 2 TA solutions */
#include "lab2.h"
#include "math.h"

void print_tree(float arr[], int n) {
    int levels = log2(n);
    int num_nodes = 1;
    int index = 0;
    for (int i = 0; i <= levels; i++) {
        int spaces = (n * 3 / num_nodes);
        for (int j = 0; j < num_nodes; j++) {
            if (j == 0) for (int k = 0; k < spaces/2 + levels - i; k++) printf(" ");
            else for (int k = 0; k < spaces; k++) printf(" ");
            printf("%d", (int)arr[index]);
            index++;
        }
        printf("\n");
        num_nodes *= 2;
    }
}

float get_parent_value(float arr[], int n, int index) {
    if (index < 1 || index >= (n-1)) return -1;
    return arr[(index-1)/2];
}

float get_left_value(float arr[], int n, int index) {
    if (index < 0 || index >= (n)/2) return -1;
    return arr[(index*2)+1];
}

float get_right_value(float arr[], int n, int index) {
    if (index < 0 || index >= (n-1)/2) return -1;
    return arr[(index*2)+2];
}

int is_max_heap_helper(float arr[], int n, int index) {
    if (arr[index] > get_left_value(arr, n, index) && arr[index] > get_right_value(arr, n, index)) {
        int left_result = 1;
        int right_result = 1;
        if (get_left_value(arr, n, index) != -1) 
            left_result = is_max_heap_helper(arr, n, (index*2)+1);
        if (get_right_value(arr, n, index) != -1) 
            right_result = is_max_heap_helper(arr, n, (index*2)+2);
        if (left_result == 0 || right_result == 0) return 0;
        return 1;
    }
    else return 0;
}

int is_max_heap(float arr[], int n) {
    return is_max_heap_helper(arr, n, 0);
}

void heapify_helper(float arr[], int n, int index) {
    float left = get_left_value(arr, n, index);
    float right = get_right_value(arr, n, index);
    if (left > right) {
        if (left > arr[index]) {
            arr[(index*2)+1] = arr[index];
            arr[index] = left;
            heapify_helper(arr, n, index*2 + 1);
        }
    }
    else if (left < right) {
        if (right > arr[index]) {
            arr[(index*2)+2] = arr[index];
            arr[index] = right;
            heapify_helper(arr, n, index*2 + 2);
        }
    }
}

void heapify(float arr[], int n) {
    for (int i = n/2 - 1; i >= 0; i--)
        heapify_helper(arr, n, i);
}

void heapsort(float arr[], int n) {
    for (int i = n-1; i >= 0; i--) {
        int temp = arr[i];
        arr[i] = arr[0];
        arr[0] = temp;
        heapify(arr, i);
    }
}

float find_most_common_element(float arr[], int n) {
    int max_count = 0;
    int max_value = 0;

    for (int i = 0; i < n; i++) {
        int curr_count = 0;
        int curr_value = arr[i];
        for (int j = 0; j < n; j++)
            if (arr[j] == curr_value) curr_count++;
        if (curr_count > max_count) {
            max_count = curr_count;
            max_value = curr_value;
        }
    }
    return max_value;
}
