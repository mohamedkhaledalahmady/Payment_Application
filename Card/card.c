#include "card.h"
#include "..\helper\help.h"
#include <stdio.h>

EN_cardError_t getCardHolderName(ST_cardData_t *cardData)
{
    uint8_t State = 0;
    printf("Enter CardHolderName: ");
    gets(cardData->cardHolderName);
    // fgets(cardData->cardHolderName, 24, stdin);
    // scanf("%[^\n]s", cardData->cardHolderName); // using %[^\n]s to read input with spaces
    uint8_t length = String_Length(cardData->cardHolderName);
    // printf("length: %d\n", length);
    State = Check_String_Not_Contains_Numbers(cardData->cardHolderName);
    if (length >= 20 && length <= 24 && State)
        State = OK;
    else
        State = WRONG_NAME; // if user input null or less than 20 characters or more than 24 characters
    return State;
}
EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData)
{
    // mm/yy
    printf("Enter Card Expire Date: ");
    scanf("%s", cardData->cardExpirationDate);
    uint8_t state = Check_Expiry_Date_Format(cardData->cardExpirationDate); // return True if valid format, else return false
    state = state == True ? OK : WRONG_EXP_DATE;                            // converte state from (True & False) to (OK & WRONG_EXP_DATE)
    return state;
}
EN_cardError_t getCardPAN(ST_cardData_t *cardData)
{
    printf("Enter Card PAN: ");
    scanf("%s", cardData->primaryAccountNumber);
    // printf("test: %s\n", cardData->primaryAccountNumber);
    uint8_t state = Check_PAN(cardData->primaryAccountNumber);
    state = state == True ? OK : WRONG_PAN; // converte state from (True & False) to (OK & WRONG_EXP_DATE)
    return state;
}
