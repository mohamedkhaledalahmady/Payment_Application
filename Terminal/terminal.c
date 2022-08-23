#include "terminal.h"

EN_terminalError_t getTransactionDate(ST_terminalData_t *termData)
{
    Get_Date_From_OS(termData->transactionDate);
    return OK;
}
EN_terminalError_t isCardExpired(ST_cardData_t *cardData, ST_terminalData_t *termData)
{
    // card expiry date => mm/yy
    // transaction date => dd/mm/yyyy
    uint8_t state = EXPIRED_CARD;
    uint8_t expiry_year[3] = "00";
    uint8_t expiry_month[3] = "00";
    uint8_t Transaction_year[3] = "00";
    uint8_t Transaction_month[3] = "00";

    // printf("cardExpiryDate: %s\n", cardData->cardExpirationDate);
    printf("TransactionDate: %s\n", termData->transactionDate);
    for (uint8_t i = 3; i < 5; i++)
        expiry_year[i - 3] = cardData->cardExpirationDate[i];
    expiry_year[2] = '\0';
    for (uint8_t i = 0; i < 3; i++)
        expiry_month[i] = cardData->cardExpirationDate[i];
    expiry_month[2] = '\0';
    for (uint8_t i = 8; i < 10; i++)
        Transaction_year[i - 8] = termData->transactionDate[i];
    Transaction_year[2] = '\0';
    for (uint8_t i = 3; i < 5; i++)
        Transaction_month[i - 3] = termData->transactionDate[i];
    Transaction_month[2] = '\0';

    // printf("expiry_year: %s\n", expiry_year);
    // printf("expiry_month: %s\n", expiry_month);
    // printf("Transaction_year: %s\n", Transaction_year);
    // printf("Transaction_month: %s\n", Transaction_month);

    if (String_2_Interger(expiry_year) > String_2_Interger(Transaction_year))
        state = OK;
    else if (String_2_Interger(expiry_year) == String_2_Interger(Transaction_year))
        if (String_2_Interger(expiry_month) >= String_2_Interger(Transaction_month))
            state = OK;
    return state;
}
EN_terminalError_t isValidCardPAN(ST_cardData_t *cardData)
{
    uint8_t state = Check_PAN(cardData->primaryAccountNumber);
    state = state == True ? OK : INVALID_CARD; // converte state from (True & False) to (OK & WRONG_EXP_DATE)
    return state;
}
EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData)
{
    printf("Enter transaction amount: ");
    scanf("%f", &termData->transAmount);
    // printf("Ammount: %f\n", termData->transAmount);
    return termData->transAmount > 0 ? OK : INVALID_AMOUNT;
}
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData)
{
    return termData->transAmount <= termData->maxTransAmount ? OK : EXCEED_MAX_AMOUNT;
}
EN_terminalError_t setMaxAmount(ST_terminalData_t *termData)
{
    printf("Enter Maximum Allowed Amount: ");
    scanf("%f", &termData->maxTransAmount);
    // printf("Ammount: %f\n", termData->maxTransAmount);
    return termData->maxTransAmount > 0 ? OK : INVALID_MAX_AMOUNT;
}
