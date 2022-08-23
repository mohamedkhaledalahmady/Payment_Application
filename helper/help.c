#include "help.h"

uint8_t String_Length(uint8_t *str)
{
    uint8_t count = 0;
    while (str[count])
        count++;
    return count;
}

uint8_t Check_String_Not_Contains_Numbers(uint8_t *str)
{
    uint8_t index = 0;
    while (str[index])
    {
        if (str[index] >= '0' && str[index] <= '9')
            return Fasle;
        index++;
    }
    return True;
}

uint8_t Check_String_Not_Contains_Characters(uint8_t *str)
{
    uint8_t index = 0;
    while (str[index])
    {
        if (str[index] >= '0' && str[index] <= '9')
        {
            index++;
            continue;
        }
        return Fasle;
    }
    return True;
}

uint8_t Check_Expiry_Date_Format(uint8_t *str)
{
    uint8_t length = String_Length(str);
    uint8_t state = Fasle;
    if (length == 5 && str[2] == '/')
        state = True;
    else
        goto exit;
    uint8_t index = 0;
    uint8_t count = 0;
    for (uint8_t index = 0; str[index]; index++)
    {
        if (index == 2)
            continue;
        if (str[index] >= '0' && str[index] <= '9')
            count++;
    }
    if (count == length - 1)
        if ((str[0] == '0' && str[1] == '0') || (str[0] > '0' && str[1] > '2')) // check valid month (01 to 12)
            state = Fasle;
        else
            state = True;
    else
        state = Fasle;
exit:
    return state;
}

uint8_t Sum_of_2_digits(uint8_t num)
{
    return (num % 10 + (num / 10) % 10);
}

uint8_t Check_PAN(uint8_t *str)
{
    uint8_t state = Check_String_Not_Contains_Characters(str);
    uint8_t length = String_Length(str);
    uint8_t sum = 0;
    uint8_t turn = 0;
    if (state && length >= 16 && length <= 19)
    {
        for (int8_t index = length - 1; index >= 0; index--)
        {
            if (turn == 0)
            {
                // printf("str: %d\n", str[index] - '0');
                sum += (str[index] - '0');
                turn = 1;
            }
            else if (turn == 1)
            {
                uint8_t test = (str[index] - '0') * 2;
                // printf("test: %d\n", test);
                sum += test > 9 ? Sum_of_2_digits(test) : test;
                turn = 0;
            }
        }
        // printf("sum: %d\n", sum);
        state = sum % 10 == 0 ? True : Fasle;
    }
    else
    {
        state = Fasle;
    }
    return state;
}

void String_Month_to_Number(uint8_t str[])
{
    uint8_t *months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    uint8_t i = 0;
    for (i = 0; i < 12; i++)
        if (strcmp(str, months[i]) == 0)
            break;
    if (i < 9)
    {
        str[0] = '0';
        str[1] = (i + 1) + '0';
    }
    else
    {
        str[1] = (i + 1) % 10 + '0';
        str[0] = ((i + 1) / 10) % 10 + '0';
    }
    str[2] = '\0';
}
void Get_Date_From_OS(uint8_t Date[])
{
    // __DATE__ => Aug 23 2022
    // Format   => 23/08/2022
    
    uint8_t str[] = __DATE__;
    uint8_t day[] = "00";
    uint8_t month[] = "000";
    uint8_t year[] = "0000";
    for (uint8_t i = 0; i < 3; i++)
        month[i] = str[i];

    for (uint8_t i = 4; i < 6; i++)
        day[i - 4] = str[i];

    for (uint8_t i = 7; i < 11; i++)
        year[i - 7] = str[i];

    String_Month_to_Number(month);
    strcpy(Date, day);
    strcat(Date, "/");
    strcat(Date, month);
    strcat(Date, "/");
    strcat(Date, year);
}
uint8_t String_2_Interger(uint8_t str[])
{
    return (str[0] - '0') * 10 + (str[1] - '0');
}