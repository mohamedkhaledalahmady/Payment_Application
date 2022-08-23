#ifndef HELP_H
#define HELP_H
#include "..\DataTypes\data_types.h"
#include <stdio.h>
#include <string.h>
// ------------------------------ //
uint8_t String_Length(uint8_t *str);
uint8_t Check_String_Not_Contains_Numbers(uint8_t *str);
uint8_t Check_String_Not_Contains_Characters(uint8_t *str);
uint8_t Check_Expiry_Date_Format(uint8_t *str);
uint8_t Check_PAN(uint8_t *str);
uint8_t Sum_of_2_digits(uint8_t num);
// ------------------------------ //
void String_Month_to_Number(uint8_t str[]);
void Get_Date_From_OS(uint8_t str[]);
uint8_t String_2_Interger(uint8_t str[]);
// ------------------------------ //

#endif