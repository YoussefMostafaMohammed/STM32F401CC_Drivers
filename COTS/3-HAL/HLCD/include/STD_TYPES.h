#ifndef STD_TYPES_H
#define STD_TYPES_H


#include <stdint.h> // Use standard types

#define NULL    ((void*)0)
#define TRUE    (0==0)
#define FALSE   (0!=0)
#define HIGH    1
#define LOW     0
#define ON      1
#define OFF     0
typedef enum {
    false = 0,
    true = 1,
}bool;
typedef unsigned char uint8_t;
//typedef char int8_t;
typedef unsigned short int uint16_t;
typedef short int int16_t;
//typedef unsigned int uint32_t;
//typedef int int32_t;
typedef char sint8_t;
typedef unsigned long long int uint64_t;
typedef long long int int64_t;
typedef float float32_t;
typedef double float64_t;

#endif