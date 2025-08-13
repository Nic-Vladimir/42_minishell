#include <stddef.h>
#include "../../inc/tokenizer.h"
#include <stdlib.h>

void clear_tok_data(t_tokenizer_data *tok_data)
{
    if (!tok_data)
        return;
    t_token *curr = tok_data->tokens;
    t_token *next;
    while (curr)
    {
        next = curr->next;
        if (curr->value)
            free(curr->value);
        free(curr);
        curr = next;
    }
    tok_data->tokens = NULL;
    tok_data->tail = NULL;
}


void free_tok_data(t_tokenizer_data *tok_data)
{
    if (!tok_data)
        return;
    t_token *curr = tok_data->tokens;
    t_token *next;
    while (curr)
    {
        next = curr->next;
        if (curr->value)
            free(curr->value);
        free(curr);
        curr = next;
    }
    tok_data->tokens = NULL;
    tok_data->tail = NULL;
}
