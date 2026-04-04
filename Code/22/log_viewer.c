#include "log_viewer.h"

#include <stdio.h>

void log_viewer_show(const char *source_name, log_iterator_t *it)
{
    if (it == 0) {
        printf("[Viewer] %s iterator is null\n", source_name != 0 ? source_name : "UNKNOWN");
        return;
    }

    printf("[Viewer] show %s logs\n", source_name != 0 ? source_name : "UNKNOWN");

    it->first(it);
    while (!it->is_done(it)) {
        log_entry_t *entry = it->current(it);
        if (entry != 0) {
            printf("[Log] id=%d level=%d msg=%s\n", entry->id, entry->level, entry->message);
        }
        it->next(it);
    }
}
