#include "lab2.h"
#include <math.h>
#include <stdlib.h>

void print_tree(float arr[], int n) {
    double f = n;
    double height = floor(log(f));
    double j = 0;
    double binary = 2;
    double k;
    for(int i = 0; i < n; i++){
        for(double k = height - j; k > 0; k--){
            printf("  ");
        }
        int num = arr[i];
        printf("%i",num);
        printf("\t");
        k = i + 1;
        if (k = pow(binary,j)){
            printf("\n");
            j++;
        }
    }
}

float get_parent_value(float arr[], int n, int index) {
    int parent = floor((index-1)/2);
    if ((index <= n-1) && (index >= 0))
        return arr[parent];
    return -1;
}

float get_left_value(float arr[], int n, int index) {
    if ((index <= n-1) && (2*index+1 <= n-1))
        return arr[2*index+1];
    return -1;
}

float get_right_value(float arr[], int n, int index) {
    if ((index <= n-1) && (2*index+2 <= n-1))
        return arr[2*index+2];
    return -1;
}

int is_max_heap(float arr[], int n) {
    if (n == 1)
        return 1;
    for(int i = 1; i < n; i++){
        if(arr[i] > arr[(i-1)/2])
            return 0;
    }
    return 1;
}

void heapify(float arr[], int n) {
    int parent;
    int j;
    for(int i = 1; i < n; i++){ 
        parent = (i-1)/2;
        if(arr[i] > arr[parent]){ 
            j = i; 
            parent = (j-1)/2;
            while(arr[j] > arr[parent]){ 
                arr[j] -= arr[parent];
                arr[parent] += arr[j];
                arr[j] = arr[parent] - arr[j];
                j = parent; 
            } 
        } 
    } 
}

void heapsort(float arr[], int n) {
    heapify(arr,n);
    for(int i=n-1; i > 0; i--){
        arr[0] -= arr[i];
        arr[i] += arr[0];
        arr[0] = arr[i] - arr[0];
        int j = 0;
        int left_child, right_child;
        do{
            left_child = (2*j + 1);
            right_child = (2*j + 2);
            if((arr[right_child] > arr[left_child]) && (left_child < (i - 1))) 
                left_child = right_child;
            if((arr[j] < arr[left_child]) && (left_child < i)){
                arr[j] -= arr[left_child];
                arr[left_child] += arr[j];
                arr[j] = arr[left_child] - arr[j];
            }
            j = left_child; 
        }while (left_child < i); 
    } 
}

float find_most_common_element(float arr[], int n) {
    heapsort(arr, n);
    int identical = 1;
    int curr_ID = 1;
    float ans;
    for(int i=1; i<n; i++){
        if(arr[i] - arr[i-1] == 0)
            curr_ID += 1;
        if(curr_ID > identical)
            ans = arr[i];
            identical = curr_ID;
            curr_ID = 1;
    }
    return ans;
}

int main(void){
    float input1[3] = {8, 15, 13};
    float input2[3] = {15, 8, 13};
    float output1 = is_max_heap(input1, 3);
    float output2 = is_max_heap(input2, 3);

    printf("test_is_max_heap (first test case): ");
    if (output1 == 0) printf("PASSED\n");
    else printf("FAILED\n");

    printf("test_is_max_heap (second test case): ");
    if (output2 == 1) printf("PASSED\n");
    else printf("FAILED\n");
}
