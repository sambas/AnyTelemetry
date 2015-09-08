/**
 * @file vector.h
 * vector interface
 */
#ifndef _VECTOR_H
#define _VECTOR_H


#define VECTOR_TYPE_CHAR    1   /* 1 byte  */
#define VECTOR_TYPE_INT     4   /* 4 bytes */
#define VECTOR_TYPE_FLOAT   4   /* 4 bytes */
#define VECTOR_TYPE_LONG    8   /* 8 bytes */
#define VECTOR_TYPE_DOUBLE  8   /* 8 bytes */

/** helper macros */
#define VECTOR_INDEX(v, i)  ((char *)v->elem + v->elem_type * i)

typedef void (*free_func_t)(void *);

typedef struct vector_t {
    void    *elem;          /**< vector element pointer */
    int     mum_elem;       /**< numbers of elements */
    int     elem_type;      /**< element type, see VECTOR_TYPE_* above */
    free_func_t free_func;  /**< free resource function */
} vector_t;

/* vector ops */
vector_init();
vector_copy();
vector_insert();
vector_delete();
vector_insert_at();
vector_delete_at();
vector_push();
vector_pull();
vector_shift();
vector_get();
vector_size();
vector_qsort();

#endif /* _VECTOR_H */
