#include "test.h" 
 
generated_list0* generated_add_generated_list0(generated_list0* current, int item) { 
    generated_list0* res = malloc(sizeof(generated_list0));
    res->value = item; 
    res->next = current; 
    res->empty = 0; 
    res->size = current->size + 1; 
    return res; 
} 
 
generated_list0* generated_create_generated_list0(int count, ...) { 
    int i; 
    va_list args; 
    generated_list0* res = malloc(sizeof(generated_list0)); 
    res->empty = 1; 
    res->size = 0; 

    va_start(args, count); 

    for (i = 0; i < count; i++) { 
        res = generated_add_generated_list0(res, va_arg(args, int)); 
    } 

    va_end(args); 
    return res; 
} 
 
generated_list0* generated_at_generated_list0(generated_list0* current, int index) { 
    while (index-- > 0) { 
        if (current->empty) { 
            return current; 
        } 

        current = current->next; 
    } 

    return current; 
} 
 
int generated_valueat_generated_list0(generated_list0* current, int index) { 
    generated_list0* res = generated_at_generated_list0(current, index); 

    if (res->empty) { 
        printf("Out of bound! 0\n"); 
        exit(1); 
    }
    return res->value; 
} 
 
int generated_compare_generated_list0(generated_list0* list1, generated_list0* list2) { 
    int i; 
    if (list1->size != list2->size) { 
         return 0; 
    } 
 
    for (i = 0; i < list1->size; i++) { 
        if(generated_valueat_generated_list0(list1, i) != generated_valueat_generated_list0(list2, i)) 
            return 0; 
    } 
 
    return 1; 
} 
 
generated_list0* generated_concat_generated_list0(generated_list0* list1, generated_list0* list2) { 
    int i; 
    generated_list0** elements = malloc(sizeof(generated_list0*) * list1->size); 
 
    for (i = 0; !list1->empty; ++i) { 
        elements[i] = list1; 
        list1 = list1->next; 
    } 
 
    for (--i; i >= 0; --i) { 
        list2 = generated_add_generated_list0(list2, elements[i]->value); 
    } 
 
    free(elements); 
    return list2; 
} 

generated_list0* generated_tostring_generated_list0(generated_list0* value) { 
    generated_list0* comma = generated_create_string(", "); 
    generated_list0* res = generated_create_string("]"); 
    int i; 

    for (i = value->size - 1; i >= 0; i--) { 
        res = generated_concat_generated_list0(generated_tostring_char(generated_valueat_generated_list0(value, i)), res); 
 
        if (i != 0) 
            res = generated_concat_generated_list0(comma, res); 
    } 
 
    res = generated_add_generated_list0(res, '['); 
    return res; 
} 
 
generated_list1* generated_add_generated_list1(generated_list1* current, generated_list0* item) { 
    generated_list1* res = malloc(sizeof(generated_list1));
    res->value = item; 
    res->next = current; 
    res->empty = 0; 
    res->size = current->size + 1; 
    return res; 
} 
 
generated_list1* generated_create_generated_list1(int count, ...) { 
    int i; 
    va_list args; 
    generated_list1* res = malloc(sizeof(generated_list1)); 
    res->empty = 1; 
    res->size = 0; 

    va_start(args, count); 

    for (i = 0; i < count; i++) { 
        res = generated_add_generated_list1(res, va_arg(args, generated_list0*)); 
    } 

    va_end(args); 
    return res; 
} 
 
generated_list1* generated_at_generated_list1(generated_list1* current, int index) { 
    while (index-- > 0) { 
        if (current->empty) { 
            return current; 
        } 

        current = current->next; 
    } 

    return current; 
} 
 
generated_list0* generated_valueat_generated_list1(generated_list1* current, int index) { 
    generated_list1* res = generated_at_generated_list1(current, index); 

    if (res->empty) { 
        printf("Out of bound! 1\n"); 
        exit(1); 
    }
    return res->value; 
} 
 
int generated_compare_generated_list1(generated_list1* list1, generated_list1* list2) { 
    int i; 
    if (list1->size != list2->size) { 
         return 0; 
    } 
 
    for (i = 0; i < list1->size; i++) { 
        if(generated_valueat_generated_list1(list1, i) != generated_valueat_generated_list1(list2, i)) 
            return 0; 
    } 
 
    return 1; 
} 
 
generated_list1* generated_concat_generated_list1(generated_list1* list1, generated_list1* list2) { 
    int i; 
    generated_list1** elements = malloc(sizeof(generated_list1*) * list1->size); 
 
    for (i = 0; !list1->empty; ++i) { 
        elements[i] = list1; 
        list1 = list1->next; 
    } 
 
    for (--i; i >= 0; --i) { 
        list2 = generated_add_generated_list1(list2, elements[i]->value); 
    } 
 
    free(elements); 
    return list2; 
} 

generated_list0* generated_tostring_generated_list1(generated_list1* value) { 
    generated_list0* comma = generated_create_string(", "); 
    generated_list0* res = generated_create_string("]"); 
    int i; 

    for (i = value->size - 1; i >= 0; i--) { 
        res = generated_concat_generated_list0(generated_tostring_string(generated_valueat_generated_list1(value, i)), res); 
 
        if (i != 0) 
            res = generated_concat_generated_list0(comma, res); 
    } 
 
    res = generated_add_generated_list0(res, '['); 
    return res; 
} 
 
generated_list0* generated_create_string(char* values) { 
    int i, str_length = strlen(values); 
    generated_list0* res = generated_create_generated_list0(0); 
 
    for (i = str_length - 1; i >= 0; i--) { 
        res = generated_add_generated_list0(res, values[i]); 
    } 
 
    return res; 
} 
 
int generated_compare_string(generated_list0* string, char* values, int offset) { 
    int i, size = strlen(values); 
 
    if (size == string->size) { 
        for (i = 0; i < size; i++) { 
            if (generated_valueat_generated_list0(string, i) != values[i]) 
                return 0; 
        } 
    } else { 
        return 0; 
    } 
 
    return 1; 
} 
 
generated_list0* generated_print_string(generated_list0* string) { 
    char* buffer = malloc(sizeof(char) * (string->size + 1)); 
    int i; 
 
    for (i = 0; i < string->size; i++) { 
        buffer[i] = (char)generated_valueat_generated_list0(string, i); 
    } 
 
    buffer[i] = '\0'; 
    printf("%s\n", buffer); 
    free(buffer); 
    return string; 
} 
 
generated_list0* generated_tostring_int(int64_t value) { 
    char buffer[100]; 
    sprintf(buffer, "%" PRId64 "", value); 
    return generated_create_string(buffer); 
} 
 
generated_list0* generated_tostring_bool(int value) { 
    if (value) 
        return generated_create_string("True"); 
    else 
        return generated_create_string("False"); 
} 
 
generated_list0* generated_tostring_float(double value) { 
    char buffer[100]; 
    sprintf(buffer, "%lf", value); 
    return generated_create_string(buffer); 
} 
 
generated_list0* generated_tostring_char(int value) { 
    generated_list0* res = generated_create_string("\'"); 
    res = generated_add_generated_list0(res, value); 
    res = generated_add_generated_list0(res, '\''); 
    return res; 
} 
 
generated_list0* generated_tostring_string(generated_list0* value) { 
    generated_list0* res = generated_create_string("\""); 
    res = generated_concat_generated_list0(value, res); 
    res = generated_add_generated_list0(res, '"'); 
} 
 
int main(int argc, char** argv) { 
    generated_list1 *args = generated_create_generated_list1(0); 
    int i; 
 
    for(i = argc - 1; i >= 0; i--) { 
        args = generated_add_generated_list1(args, generated_create_string(argv[i])); 
    } 
 
    generated_signature1arg* main_args = malloc(sizeof(generated_signature1arg)); 
    main_args->generated_arg0 = args; 
    task_t *main_task = taskcreate((void *)&user_main, (void *)main_args); 
    rmain(4, main_task); 

    generated_print_string(generated_tostring_int(main_args->generated_res)); 
    taskdealloc(main_task); 
    return 0; 
} 
 
void user_main(task_t *t) { 
    generated_list1* generated_arg0 = (generated_list1*)((generated_signature1arg*)t->startarg)->generated_arg0;
Start: 

    { 
        generated_signature2arg* task0arg = malloc(sizeof(generated_signature2arg));
        task0arg->generated_arg0 = 10; 
        task_t *task0 = taskcreate((void *)&user_f, (void *)task0arg);
        subtaskadd(t, task0); 
        taskyield(t); 
        generated_signature2arg* task1arg = malloc(sizeof(generated_signature2arg));
        task1arg->generated_arg0 = 10; 
        task_t *task1 = taskcreate((void *)&user_g, (void *)task1arg);
        subtaskadd(t, task1); 
        taskyield(t); 
        ((generated_signature1arg*)t->startarg)->generated_res = (((generated_signature2arg*)task0->startarg)->generated_res+((generated_signature2arg*)task1->startarg)->generated_res); 
        taskexit(t); 
    } 
 

    printf("No cases realized!\n"); 
    exit(1); 
} 
 

void user_f(task_t *t) { 
    int64_t generated_arg0 = (int64_t)((generated_signature2arg*)t->startarg)->generated_arg0;
Start: 

    { 
        int64_t user_n = generated_arg0;

        ((generated_signature2arg*)t->startarg)->generated_res = user_n; 
        taskexit(t); 
    } 
 

    printf("No cases realized!\n"); 
    exit(1); 
} 
 

void user_g(task_t *t) { 
    int64_t generated_arg0 = (int64_t)((generated_signature2arg*)t->startarg)->generated_arg0;
Start: 

    { 
        int64_t user_n = generated_arg0;

        ((generated_signature2arg*)t->startarg)->generated_res = user_n; 
        taskexit(t); 
    } 
 

    printf("No cases realized!\n"); 
    exit(1); 
} 
 

