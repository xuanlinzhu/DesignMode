#include "handler.h"

void handler_set_next(handler_t *self, handler_t *next)
{
    if (self == NULL) {
        return;
    }

    self->next = next;
}

chain_result_t run_chain(handler_t *head, conn_t *conn)
{
    handler_t *node;
    chain_result_t result;

    if (head == NULL || conn == NULL) {
        return CHAIN_REJECT;
    }

    node = head;

    while (node != NULL)
    {
        if (node->handle == NULL) {
            return CHAIN_REJECT;
        }

        result = node->handle(node, conn);

        if (result == CHAIN_REJECT) {
            return CHAIN_REJECT;
        }

        if (result == CHAIN_ACCEPT) {
            return CHAIN_ACCEPT;
        }

        node = node->next;
    }

    return CHAIN_ACCEPT;
}
