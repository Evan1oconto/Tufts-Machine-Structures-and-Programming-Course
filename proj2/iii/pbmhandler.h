/*

 * pbmhandler.h
 * Authors:      Ankur Dahal (adahal01), Evan Loconto (elocon01)
 * Date:         09/28/2020
 * Assignment:   iii 

 * Interface for pbmhandler that handles storing and displaying 
 * pbm images. 

*/


#ifndef PBMHANDLER_INCLUDED
#define PBMHANDLER_INCLUDED


/* pbmread:        Stores the bit in the reader (first parameter) to a 
                   Bit2 instance and returns the newly created Bit2 instance.
*  Expectations:   The reader is non-null. The function returns a newly
                   created and populated Bit2_T instance.
*/
Bit2_T pbmread(Pnmrdr_T reader);


/* pbmwrite:       Prints to outputfp (the first parameter) the pbm image 
                   stored in bitmap (the second parameter)
*  Expectations:   outputfp is expected to be non-null and bitmap is expected
                   to be non-null. The function does not return anything.
*/
void pbmwrite(FILE *outputfp, Bit2_T bitmap);


#endif