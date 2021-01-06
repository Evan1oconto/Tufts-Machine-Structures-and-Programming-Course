#include <inttypes.h>
#include <stdio.h>

#define ENROLLED      0x8000   
#define DROPPED       0x4000
#define INCOMPLETE    0x2000
#define GRADED        0x1000
#define STATUS_LSB    12
#define STATUS_WIDTH  4

#define POSTBAC       0x0800   
#define PHD           0x0400
#define MASTERS       0x0200
#define UNDERGRAD     0x0100
#define LEVEL_LSB     8
#define LEVEL_WIDTH   4

#define HOURS_LSB     3
#define HOURS_WIDTH   5

#define CREDITS_LSB   0
#define CREDITS_WIDTH 3

/*
 *  Returns a non-zero result if the provided
 *  student is in the post-bac program and zero
 *  otherwise.
 */
uint16_t is_postbac( uint16_t student);

/*
 *  Sets the number of hours spent on COMP40
 *  to zero for the provided student.
 *  Returns: the updated student
 */
uint16_t no_40( uint16_t student);

/*
 *  Sets the provided student's registration status 
 *  to Incomplete.
 *  Returns: the updated student
 */
uint16_t set_incomp( uint16_t student);

/*
 *  Increments the provided student's credit count
 *  by one.
 *  Returns: the updated student
 */
uint16_t inc_credit( uint16_t student);
