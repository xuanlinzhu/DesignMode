#include "blacklist_handler.h"

extern int printf(const char *format, ...);

static chain_result_t blacklist_handle(handler_t *self, conn_t *conn)
{
    (void)self;

    if (conn == NULL) {
        printf("[Blacklist] reject\n");
        return CHAIN_REJECT;
    }

    if (conn->ip == 0) {
        printf("[Blacklist] reject\n");
        return CHAIN_REJECT;
    }

    printf("[Blacklist] pass\n");
    return CHAIN_CONTINUE;
}

void blacklist_handler_init(blacklist_handler_t *self)
{
    if (self == NULL) {
        return;
    }

    self->base.handle = blacklist_handle;
    self->base.next = NULL;
}
