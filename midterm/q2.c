#include "q2.h"


uint16_t is_postbac( uint16_t student) 
{
        if ((student & POSTBAC) != 0) {
                return student;
        }
        return 0;
}


uint16_t no_40( uint16_t student) 
{
        uint16_t mask;
        uint16_t mask1 = ~0;
        uint16_t mask2 = ~0;
        
        mask1 = (mask1 << (HOURS_WIDTH + HOURS_LSB));
        mask2 = (mask2 >> (16 - HOURS_LSB));

        mask = mask1 | mask2;

        return (student & mask);
        
}


uint16_t set_incomp( uint16_t student)
{
        uint16_t mask = ~0;
        mask = mask >> STATUS_WIDTH;
        student = (student & mask);
        student = (student | INCOMPLETE);

        return student;
}


uint16_t inc_credit( uint16_t student)
{
        uint16_t mask1 = ~0;
        uint16_t mask2 = ~0;
        uint16_t temp_student = student;

        mask1 = mask1 >> (16 - CREDITS_WIDTH);
        mask2 = mask2 << CREDITS_WIDTH;
        temp_student = (temp_student & mask1);
        temp_student++;
        if (temp_student >= 8) {
                temp_student = 0;
        }

        student = (student & mask2);
        return (student | temp_student);

}



