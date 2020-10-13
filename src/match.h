#ifndef MATCH_H
#define MATCH_H MATCH_H

#include <math.h>
#include <stdbool.h>
#include <stddef.h>

typedef double score_t;
#define SCORE_MAX INFINITY
#define SCORE_MIN -INFINITY

#define MATCH_MAX_LEN 1024

bool has_match(const char* needle, const char* haystack);
score_t match(const char* needle, const char* haystack);

score_t match_positions(const char* needle,
                        const char* haystack,
                        size_t* positions,
                        size_t num_positions);

struct fzy_ctx {
    size_t needle_len;
    char *lower_needle;
};

void fzy_ctx_init(struct fzy_ctx *ctx, const char *needle, size_t needle_len);
score_t fzy_ctx_add_haystack(struct fzy_ctx *ctx, const char *haystack, size_t haystack_len);
void fzy_ctx_reset(struct fzy_ctx *ctx);


#endif
