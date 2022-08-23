#include "app.h"

void appStart()
{
    ST_cardData_t cardData;
    ST_terminalData_t termData;
    ST_transaction_t transData;
    uint8_t test = Fasle;
    Crate_DB_with_Data();

    while (True)
    {
        while (getCardHolderName(&cardData) == WRONG_NAME)
            printf("Invalid Name\n");

        while (getCardExpiryDate(&cardData) == WRONG_EXP_DATE)
            printf("Invalid Date Format (mm/yy)\n");

        while (getCardPAN(&cardData) == WRONG_PAN)
            printf("Invalid PAN (not Luhn number)\n");

        getTransactionDate(&termData);

        if (isCardExpired(&cardData, &termData) == EXPIRED_CARD)
        {
            printf("Sorry!! Expired Date\n");
            continue;
        }

        while (setMaxAmount(&termData) == INVALID_MAX_AMOUNT)
            printf("Invalid Max Amount\n");

        while (getTransactionAmount(&termData) == INVALID_AMOUNT)
            printf("Invalid Amount!!\n");

        if (isBelowMaxAmount(&termData) == EXCEED_MAX_AMOUNT)
        {
            printf("Sorry!! Transaction Amount Exceed Max Amount\n");
            continue;
        }

        transData.cardHolderData = cardData;
        transData.terminalData = termData;
        if (test == Fasle)
        {
            transData.transactionSequenceNumber = 0;
            test = True;
        }
        uint8_t state = recieveTransactionData(&transData);
        switch (state)
        {
        case FRAUD_CARD:
            printf("Invalid Acoount\n");
            goto save;
            break;
        case DECLINED_INSUFFECIENT_FUND:
            printf("Sorry!! Transaction Amount Exceed Max Amount\n");
            goto save;
            break;
        case DECLINED_STOLEN_CARD:
            printf("Blocked Account\n");
            goto save;
            break;

        default:
            break;
        }

        if (isAmountAvailable(&termData, Search_In_DB(&cardData)) == LOW_BALANCE)
        {
            printf("Low balance\n");
            goto save;
        }

    save:
        saveTransaction(&transData);
        Store_in_File(&transData);
    }
exit:
    return;
}