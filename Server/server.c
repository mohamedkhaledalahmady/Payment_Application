#include "server.h"

ST_accountsDB_t DB_Accounts[MAX_DB];
ST_transaction_t DB_Transaction[MAX_DB] = {0};

void Crate_DB_with_Data()
{
    const float balance[] = {3500, 1000, 5000, 6500, 7800, 650, 9800, 10000, 2000, 6500};
    const uint8_t state[] = {RUNNING, BLOCKED, BLOCKED, RUNNING, RUNNING, RUNNING, RUNNING, BLOCKED, RUNNING, RUNNING};
    const uint8_t PANs[10][20] = {"4003600000000014", "8989374615436851", "1234500006789007", "1234500006789015", "1234560000789023", "1234560000789114", "1234560000789379", "1234500006789999", "8989374615436851", "1234500006789932"};
    for (int8_t i = 0; i < 10; i++)
    {
        DB_Accounts[i].balance = balance[i];
        DB_Accounts[i].state = state[i];
        strcpy(DB_Accounts[i].primaryAccountNumber, PANs[i]);
    }
}

EN_transState_t recieveTransactionData(ST_transaction_t *transData)
{
    uint8_t index;
    uint8_t state;
    for (index = 0; index < MAX_DB; index++)
    {
        // printf("index: %d\t%s\n", index, DB_Accounts[index].primaryAccountNumber);
        if (strcmp(transData->cardHolderData.primaryAccountNumber, DB_Accounts[index].primaryAccountNumber) == 0)
        {
            // printf("index: %d\n", index);
            break;
        }
    }
    // printf("index: %d\n", index);
    // if account not exist
    if (index == MAX_DB)
        state = FRAUD_CARD;
    // if transAmount > max Amount
    else if (transData->terminalData.transAmount > transData->terminalData.maxTransAmount)
        state = DECLINED_INSUFFECIENT_FUND;
    // if account blocked in DB
    else if (DB_Accounts[index].state == BLOCKED)
        state = DECLINED_STOLEN_CARD;
    // else approved
    else
    {
        state = APPROVED;
        // DB_Accounts[index].balance -= transData->terminalData.transAmount;
    }
    transData->transState = state;
    // printf("transactio Sequence number: %d\n", transData->transactionSequenceNumber);
    transData->transactionSequenceNumber++;
    return state;
}
EN_serverError_t isValidAccount(ST_cardData_t *cardData, ST_accountsDB_t accountRefrence)
{
    uint8_t index;
    uint8_t state;
    for (index = 0; index < 10; index++)
    {
        if (strcmp(cardData->primaryAccountNumber, DB_Accounts[index].primaryAccountNumber) == 0)
        {
            // printf("index: %d\n", index);
            break;
        }
    }
    if (index == MAX_DB)
        state = ACCOUNT_NOT_FOUND;
    else
        state = SERVER_OK;
    return state;
}

uint8_t Search_In_DB(ST_cardData_t *cardData)
{
    uint8_t index;
    uint8_t state;
    for (index = 0; index < 10; index++)
    {
        if (strcmp(cardData->primaryAccountNumber, DB_Accounts[index].primaryAccountNumber) == 0)
        {
            // printf("index: %d\n", index);
            break;
        }
    }
    return index == MAX_DB ? -1 : index;
}
EN_serverError_t isAmountAvailable(ST_terminalData_t *termData, uint8_t index)
{
    uint8_t state;
    if (termData->transAmount > DB_Accounts[index].balance)
        state = LOW_BALANCE;
    else
    {
        DB_Accounts[index].balance -= termData->transAmount;
        state = SERVER_OK;
    }
    return state;
}

EN_serverError_t saveTransaction(ST_transaction_t *transData)
{
    static uint8_t index = 0;
    DB_Transaction[index].cardHolderData = transData->cardHolderData;
    DB_Transaction[index].terminalData = transData->terminalData;
    DB_Transaction[index].transState = transData->transState;
    DB_Transaction[index].transactionSequenceNumber = transData->transactionSequenceNumber;
    index++;

    // printf("--------- Transaction Data ---------\n");
    // printf("transactionSequenceNumber: %d\n", transData->transactionSequenceNumber);
    // transData->transState == 0 ? printf("state: RUNNING\n") : printf("state: BLOCKED\n");
    // printf("PAN: %s\n", transData->cardHolderData.primaryAccountNumber);
    // printf("Balance: %.2f\n", DB_Accounts[Search_In_DB(&transData->cardHolderData)].balance);

    getTransaction(transData->transactionSequenceNumber, transData);
    return SERVER_OK;
}

void Dipslay_DB_Transaction()
{
    for (uint8_t i = 0; i < DB_Transaction[i].transactionSequenceNumber; i++)
    {
        printf("%s\n", DB_Transaction[i].cardHolderData.cardHolderName);
        printf("%s\n", DB_Transaction[i].cardHolderData.primaryAccountNumber);
        printf("%s\n", DB_Transaction[i].cardHolderData.cardExpirationDate);

        printf("%f\n", DB_Transaction[i].terminalData.transAmount);

        printf("%d\n", DB_Transaction[i].transactionSequenceNumber);
        printf("%d\n", DB_Transaction[i].transState);
        printf("-------------------\n");
    }
}
EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t *transData)
{
    uint8_t index = 0;
    for (index = 0; index < MAX_DB; index++)
    {
        if (DB_Transaction[index].transactionSequenceNumber == transactionSequenceNumber)
        {
            printf("--------- Transaction Data ---------\n");
            printf("transactionSequenceNumber: %d\n", transData->transactionSequenceNumber);
            transData->transState == 0 ? printf("state: RUNNING\n") : printf("state: BLOCKED\n");
            printf("PAN: %s\n", transData->cardHolderData.primaryAccountNumber);
            printf("Balance: %.2f\n", DB_Accounts[Search_In_DB(&transData->cardHolderData)].balance);
            break;
        }
    }
    return index < MAX_DB ? SERVER_OK : TRANSACTION_NOT_FOUND;
}

void Store_in_File(ST_transaction_t *transData)
{
    uint8_t index = Search_In_DB(&transData->cardHolderData);
    FILE *file = fopen("Transaction Data.txt", "a");
    if (file == NULL)
        printf("Error!! Unable to open file\n");

    fprintf(file, "--------- Card Data ---------\n");
    fprintf(file, "Name: %s\n", transData->cardHolderData.cardHolderName);
    fprintf(file, "Expiry Date: %s\n", transData->cardHolderData.cardExpirationDate);
    fprintf(file, "PAN: %s\n", transData->cardHolderData.primaryAccountNumber);

    fprintf(file, "--------- Terminal Data ---------\n");
    fprintf(file, "TranAmount: %.2f\n", transData->terminalData.transAmount);
    fprintf(file, "maxTransAmount: %.2f\n", transData->terminalData.maxTransAmount);
    fprintf(file, "transactionDate: %s\n", transData->terminalData.transactionDate);

    fprintf(file, "--------- Transaction Data ---------\n");
    fprintf(file, "Sequence Number: %d\n", transData->transactionSequenceNumber);
    DB_Accounts[index].state == 0 ? fprintf(file, "state: RUNNING\n") : fprintf(file, "state: BLOCKED\n");
    fprintf(file, "PAN: %s\n", DB_Accounts[index].primaryAccountNumber);
    fprintf(file, "Balance: %.2f\n", DB_Accounts[index].balance);

    fprintf(file, "----------------------------\n");
    fclose(file);
}
