#include "src/runtime/task.h" 
#include <stdarg.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <stdint.h> 
#include <inttypes.h> 
 
typedef struct generated_list0 { 
    struct generated_list0* next; 
    int value; 
    int empty; 
    int size; 
} generated_list0; 
 
generated_list0* generated_add_generated_list0(generated_list0* current, int item); 
generated_list0* generated_create_generated_list0(int count, ...); 
generated_list0* generated_at_generated_list0(generated_list0* current, int index); 
int generated_valueat_generated_list0(generated_list0* current, int index); 
int generated_compare_generated_list0(generated_list0* list1, generated_list0* list2); 
generated_list0* generated_concat_generated_list0(generated_list0* list1, generated_list0* list2); 
generated_list0* generated_tostring_generated_list0(generated_list0* value); 
 
typedef struct generated_list1 { 
    struct generated_list1* next; 
    generated_list0* value; 
    int empty; 
    int size; 
} generated_list1; 
 
generated_list1* generated_add_generated_list1(generated_list1* current, generated_list0* item); 
generated_list1* generated_create_generated_list1(int count, ...); 
generated_list1* generated_at_generated_list1(generated_list1* current, int index); 
generated_list0* generated_valueat_generated_list1(generated_list1* current, int index); 
int generated_compare_generated_list1(generated_list1* list1, generated_list1* list2); 
generated_list1* generated_concat_generated_list1(generated_list1* list1, generated_list1* list2); 
generated_list0* generated_tostring_generated_list1(generated_list1* value); 
generated_list0* generated_create_string(char* values);
int generated_compare_string(generated_list0* string, char* values, int offset); 
generated_list0* generated_print_string(generated_list0* string); 
generated_list0* generated_tostring_int(int64_t value); 
generated_list0* generated_tostring_bool(int value); 
generated_list0* generated_tostring_float(double value); 
generated_list0* generated_tostring_char(int value); 
generated_list0* generated_tostring_string(generated_list0* value); 
typedef struct generated_signature1arg {
    generated_list1* generated_arg0;
    int64_t generated_res; 
} generated_signature1arg; 
typedef void (* generated_signature1)(task_t*);
void user_main(task_t *t); 
 
typedef struct generated_signature2arg {
    int64_t generated_arg0;
    int64_t generated_res; 
} generated_signature2arg; 
typedef void (* generated_signature2)(task_t*);
void user_f(task_t *t); 
 
void user_g(task_t *t); 
 
