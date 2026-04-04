#include "ratelimit_handler.h"

extern int printf(const char *format, ...);

static chain_result_t ratelimit_handle(handler_t *self, conn_t *conn)
{
    (void)self;

    if (conn == NULL) {
        printf("[RateLimit] reject\n");
        return CHAIN_REJECT;
    }

    if (conn->request_freq > 100) {
        printf("[RateLimit] reject\n");
        return CHAIN_REJECT;
    }

    printf("[RateLimit] pass\n");
    return CHAIN_CONTINUE;
}

void ratelimit_handler_init(ratelimit_handler_t *self)
{
    if (self == NULL) {
        return;
    }

    self->base.handle = ratelimit_handle;
    self->base.next = NULL;
}
