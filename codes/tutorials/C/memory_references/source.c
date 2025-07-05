// including the required header files
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
// ===================================================================
// pointer arithmetrics


uint8_t variable = 0x03;
uint8_t *var_addr = &variable;
uint8_t **addr_of_var_addr = &var_addr;
uint8_t ***addr_of_addr_of_var_addr = &addr_of_var_addr;

uint8_t array[5] = {0x01, 0x02, 0x03, 0x04, 0x05};
uint8_t *array_addr = array;
uint8_t *array_addr2 = &array[0]; // &array[x] is the same as *(array + x)
uint8_t *calc_ptr = array + 2; // pointer arithmetic, points to the third element of the array

typedef struct {
    uint8_t status;
    uint8_t node_id;
    uint8_t counter;
} node_t;

node_t node[4] = {0};
node_t *calc_node_ptr = node + 1;

// defining a new type for an array of 4 nodes
typedef node_t node_array_t[4]; 
node_array_t node_array = {0};

node_array_t *array_address = &node_array;
node_t *array_element_address = &node_array[0];

// arrays of pointers:
node_t *node_address_array[4] = {&node[0], &node[1], &node[2], &node[3]};
// this is a very interesting case.
// node_address_array decays to the pointer that points to the first element of the array
// the first element of the array is a pointer to node_t element, &node[0].
// so, the address of the first element of the array is like &&node[0] (not a valid syntax).
// the correct syntax is a double pointer to node_t, which is node_t **.
node_t **node_address_array_ptr = node_address_array;



// creating a type for array of pointers to node_t
typedef node_t *node_pointer_array_t[4];
node_pointer_array_t new_node_pointer_array = {&node[0], &node[1], &node[2], &node[3]};


// functions
int list_pointers_from_pointer_array(node_pointer_array_t *array, size_t length){
    int ret = 0;
    if (array == NULL) return -1;
    if (length == 0) return -2;

    // what is wrong:
    // node_pointer_array_t array_obj = *array;
    // THERE is no such a thing as array assignment in C!
    // instead, we grab the address of the first element of the array:
    node_t **start_array_address = &((*array)[0]);
    // NOTES:
    // 1) address of a pointer to node_t is a double pointer to node_t
    // 2) we can use the fact that arrays decay to a pointer to the first element in assignment
    // so this would be the same as:
    start_array_address = *array;

    for (int i=0; i < length; i++){
        // note array[i] is the same as *(array + i)
        printf("element[%d] address: %p\n\r", i, &start_array_address[i]);
        printf("element[%d] status value: %d\n\r", i, start_array_address[i]->status);
        printf("element[%d] node_id value: %d\n\r", i, start_array_address[i]->node_id);
        printf("element[%d] counter value: %d\n\r", i, start_array_address[i]->counter);
    }
    return ret;
}


int list_array_elements_from_array(node_array_t *array, size_t length){
    int ret = 0;
    if (array == NULL) return -1;
    if (length == 0) return -2;

    // what is wrong:
    // node_array_t array_obj = *array;
    // THERE is no such a thing as array assignment in C!

    // we grab the address of the first element of the array:
    node_t *start_array_address = &((*array)[0]);
    // NOTE:
    // we can use the fact that arrays decay to a pointer to the first element in assignment
    // so this would be the same as:
    start_array_address = *array;

    for (int i=0; i < length; i++){
        // note array[i] is the same as *(array + i)
        printf("element[%d] address: %p\n\r", i, &start_array_address[i]);
        printf("element[%d] status value: %d\n\r", i, start_array_address[i].status);
    }
    return ret;
}


int main(void){

    // printing the addresses of the variable and its value
    printf("variable address: %p\n\r", var_addr);
    printf("variable value: %d\n\r", *var_addr);
    printf("address of variable address: %p\n\r", addr_of_var_addr);
    printf("address of address of variable address: %p\n\r", addr_of_addr_of_var_addr);

    // pointer arithmetics for the variable
    uint8_t *ptr = var_addr + 1;
    printf("ptr address: %p\n\r", ptr);

    // printing the addresses of the array and its values
    printf("array decays to %p\n\r", array_addr);
    printf("&array[0] is %p\n\r", array_addr2);
    printf("calc_ptr points to %p\n\r", calc_ptr);
    printf("value at calc_ptr: %d\n\r", *calc_ptr);

    // analyzing the structure addresses
    // first populating the sturcture with some random values
    for (int i=0; i < 4; i++) {
        node[i].status = i + 1;
        node[i].node_id = i + 10;
        node[i].counter = i * 2;
    }

    // printing some addresses:
    printf("node decase to %p\n\r", node);
    printf("node[0] address: %p\n\r", &node[0]);
    printf("node + 1 address: %p\n\r", node + 1);
    printf("byte difference between 'node' and 'node + 1': %ld\n\r", (uint8_t *)(node + 1) - (uint8_t *)node);


    // printing diognostics for the node array
    printf("node array address: %p\n\r", node_array);
    printf("node array first element address: %p\n\r", &node_array[0]);
    printf("size of node_t: %zu bytes\n\r", sizeof(node_t));
    printf("size of node_array_t: %zu bytes\n\r", sizeof(node_array_t));
    printf("size of node_t increment: %u\n\r", (uint8_t)((uint8_t*)(node + 1) - (uint8_t*)node));
    printf("size of node_array_t increment: %u\n\r", (uint8_t)((uint8_t*)(array_address + 1) - (uint8_t*)array_address));


    printf("--------------------------------------------------\n\r");
    printf("\n\r");
    printf("               functions testing                  \n\r");
    printf("\n\r");
    printf("---------------------------------------------------\n\r");

    int ret = 0;

    printf("\n\r");
    printf("Listing pointers from pointer array:\n\r");
    ret = list_pointers_from_pointer_array(&new_node_pointer_array, 4);
    if (ret < 0) {
        printf("Error in list_pointers_from_pointer_array: %d\n\r", ret);
    }

    printf("\n\r");
    printf("Listing elements from node array:\n\r");
    ret = list_array_elements_from_array(array_address, 4);
    if (ret < 0) {
        printf("Error in list_array_elements_from_array: %d\n\r", ret);
    }

    return 0;
}
