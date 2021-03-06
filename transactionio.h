#ifndef TRANSACTIONIO_H
#define TRANSACTIONIO_H

#include "coin.h"
#include "bytes.h"

// Output de transação
typedef struct _transactionout {
    buffer id;
    unsigned long recipientKey;
    float value;
    buffer transactionId;
} transactionout;

// Input de transação
typedef struct _transactionin {
    buffer outputId;
    transactionout *output;
} transactionin;

buffer get_transactionout_hash(transactionout t);
transactionout *new_transactionout(unsigned long recipientKey, float value, buffer transactionId);

buffer get_transactionout_hash(transactionout t) {
    buffer input = new_buffer(sizeof(long) + sizeof(float));

    int i;
    for(i = 0; i < sizeof(long); i++) {
        input.bytes[i] = t.recipientKey & (0xff << (i * 8));
    }
    int v = t.value * pow(10, DECIMAL_PLACES);
    for(i; i < sizeof(long) + sizeof(float); i++) {
        input.bytes[i] = v & (0xff << (i * 8));
    }
    concat_buffer(&input, t.transactionId);

    buffer output = hash(input);
    free(input.bytes);
    return output;
}

// Novo output de transação
transactionout *new_transactionout(unsigned long recipientKey, float value, buffer transactionId) {
    transactionout *output = malloc(sizeof(transactionout));
    output->recipientKey = recipientKey;
    output->value = value;
    output->transactionId = transactionId;
    output->id = get_transactionout_hash(*output);
    return output;
}

// Novo input de transação
transactionin *new_transactionin(buffer outputId) {
    transactionin *output = malloc(sizeof(transactionin));
    output->outputId = outputId;
    return output;
}
#endif