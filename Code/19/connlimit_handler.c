#include "connlimit_handler.h"

extern int printf(const char *format, ...);

static chain_result_t connlimit_handle(handler_t *self, conn_t *conn)
{
    (void)self;

    if (conn == NULL) {
        printf("[ConnLimit] reject\n");
        return CHAIN_REJECT;
    }

    if (conn->current_conn >= conn->max_conn) {
        printf("[ConnLimit] reject\n");
        return CHAIN_REJECT;
    }

    printf("[ConnLimit] pass\n");
    return CHAIN_CONTINUE;
}

void connlimit_handler_init(connlimit_handler_t *self)
{
    if (self == NULL) {
        return;
    }

    self->base.handle = connlimit_handle;
    self->base.next = NULL;
}
