#include "print.h"
#include "stdio.h"

void _print(value_t *val, type_t *type, int top) {
    switch (type->id) {
        case INT:
            printf("%ld", val->int_v);
            break;
        case FLOAT:
            printf("%f", val->float_v);
            break;
        case CHAR:
            if (top)
                printf("%c", (char)val->char_v);
            else
                printf("'%c'", (char)val->char_v);

            break;
        case BOOL:
            if (val->bool_v)
                printf("True");
            else
                printf("False");
            break;
        case LIST:
            _print_list((*val).list_v, type);
            break;
        case EMPTYLIST:
            printf("[]");
            break;
        case STRING:
            _print_string((*val).list_v, top);
            break;
        case TUPLE:
            _print_tuple(val, type);
            break;
        case SIGNATURE:
            _print_signature(type);
            break;
        default:
            printf("Not supported");
            break;
    }
}

void _print_list(list_t *val, type_t *type) {
    printf("[");
    while (val) {
        _print(&val->data, type->subtypes[0], 0);

        val = val->next;
        if (val)
            printf(", ");
    }
    printf("]");
}

void _print_string(list_t *val, int top) {
    if (!top)
        printf("\"");
    while (val) {
        printf("%c", (char)val->data.char_v);

        val = val->next;
    }
    if (!top)
        printf("\"");
}

void _print_tuple(value_t *val, type_t *type) {
    printf("(");
    int i = 0;
    while (type->subtypes[i]) {
        _print(&val->tuple_v[i], type->subtypes[i], 0);
        if (type->subtypes[++i])
            printf(", ");
    }
    printf(")");
}

void _print_type(type_t *type) {
    int i = 0;
    switch (type->id) {
        case INT:
            printf("Int");
            break;
        case FLOAT:
            printf("Float");
            break;
        case CHAR:
            printf("Char");
            break;
        case BOOL:
            printf("Bool");
            break;
        case STRING:
            printf("String");
            break;
        case LIST:
            printf("[");
            while (type->subtypes[i]) {
                _print_type(type->subtypes[i++]);
                if (type->subtypes[i])
                    printf(", ");
            }
            printf("]");
            break;
        case EMPTYLIST:
            printf("[]");
            break;
        case TUPLE:
            printf("(");
            while (type->subtypes[i]) {
                _print_type(type->subtypes[i++]);
                if (type->subtypes[i])
                    printf(", ");
            }
            printf(")");
            break;
        case SIGNATURE:
            _print_signature(type);
            break;
        default:
            printf("Not supported");
            break;
    }
}

void _print_signature(type_t *type) {
    int i = 0;
    printf("(");
    while (type->subtypes[i]) {
        _print_type(type->subtypes[i++]);
        if (type->subtypes[i])
            printf(" -> ");
    }
    printf(")");
}
