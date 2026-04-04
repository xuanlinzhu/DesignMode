#include "auth_handler.h"

extern int printf(const char *format, ...);

static chain_result_t auth_handle(handler_t *self, conn_t *conn)
{
    (void)self;

    if (conn == NULL) {
        printf("[Auth] reject\n");
        return CHAIN_REJECT;
    }

    if (conn->token_valid == 0) {
        printf("[Auth] reject\n");
        return CHAIN_REJECT;
    }

    printf("[Auth] pass\n");
    return CHAIN_CONTINUE;
}

void auth_handler_init(auth_handler_t *self)
{
    if (self == NULL) {
        return;
    }

    self->base.handle = auth_handle;
    self->base.next = NULL;
}
