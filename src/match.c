#include "match.h"

#include <ctype.h>
#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "bonus.h"
#include "config.h"

char* strcasechr(const char* s, char c) {
    const char accept[3] = {c, toupper(c), 0};
    return strpbrk(s, accept);
}

bool has_match(const char* needle, const char* haystack) {
    while (*needle) {
        char nch = *needle++;

        if (!(haystack = strcasechr(haystack, nch))) {
            return false;
        }
        haystack++;
    }
    return true;
}

#define swap(x, y)                                                             \
    ({                                                                         \
        typeof(x) _t = (x);                                                    \
        (x) = (y);                                                             \
        (y) = _t;                                                              \
    })

#define max(a, b)                                                              \
    ({                                                                         \
        typeof(a) _a = (a);                                                    \
        typeof(b) _b = (b);                                                    \
        _a > _b ? _a : _b;                                                     \
    })

static void precompute_bonus(const char* haystack, score_t* match_bonus) {
    /* Which positions are beginning of words */
    char last_ch = '/';
    for (int i = 0; haystack[i]; i++) {
        char ch = haystack[i];
        match_bonus[i] = COMPUTE_BONUS(last_ch, ch);
        last_ch = ch;
    }
}

#define match_row(needle, haystack, haystack_len, match_bonus)                 \
    ({                                                                         \
        score_t prev_score = SCORE_MIN;                                        \
        for (size_t j = 0; j < haystack_len; j++) {                            \
            if (needle == haystack[j]) {                                       \
                score_t score = SCORE_MIN;                                     \
                if (!row) {                                                    \
                    score = (j * SCORE_GAP_LEADING) + match_bonus[j];          \
                } else if (j) {                                                \
                    score = max(last_M[j - 1] + match_bonus[j],                \
                                last_D[j - 1] + SCORE_MATCH_CONSECUTIVE);      \
                }                                                              \
                curr_D[j] = score;                                             \
                curr_M[j] = prev_score = max(score, prev_score + gap_score);   \
            } else {                                                           \
                curr_D[j] = SCORE_MIN;                                         \
                curr_M[j] = prev_score = prev_score + gap_score;               \
            }                                                                  \
        }                                                                      \
    })


score_t match_lower(const char* lower_needle,
               size_t needle_len,
               const char* haystack,
               size_t haystack_len) {
    if (!lower_needle || !needle_len || !haystack || !haystack_len ||
        haystack_len > MATCH_MAX_LEN || needle_len > haystack_len) {
        return SCORE_MIN;
    }
    if (needle_len == haystack_len) {
        return SCORE_MAX;
    }

    score_t D[2][haystack_len], M[2][haystack_len];
    score_t *last_D, *last_M;
    score_t *curr_D, *curr_M;

    last_D = D[0];
    last_M = M[0];
    curr_D = D[1];
    curr_M = M[1];

    char lower_haystack[haystack_len];
    score_t match_bonus[haystack_len];
    char last_ch = '/';
    for (size_t i = 0; i < haystack_len; i++) {
        char ch = haystack[i];
        match_bonus[i] = COMPUTE_BONUS(last_ch, ch);
        last_ch = ch;
        lower_haystack[i] = tolower(ch);
    }

    for (size_t row = 0; row < needle_len; row++) {
        score_t gap_score =
            row == needle_len - 1 ? SCORE_GAP_TRAILING : SCORE_GAP_INNER;
        char ndl = lower_needle[row];
        match_row(ndl, lower_haystack, haystack_len, match_bonus);
        swap(curr_D, last_D);
        swap(curr_M, last_M);
    }
    return last_M[haystack_len - 1];
}


score_t match1(const char* needle,
               size_t needle_len,
               const char* haystack,
               size_t haystack_len) {
    if (!needle || !needle_len || !haystack || !haystack_len ||
        haystack_len > MATCH_MAX_LEN || needle_len > haystack_len) {
        return SCORE_MIN;
    }
    if (needle_len == haystack_len) {
        return SCORE_MAX;
    }

    score_t D[2][haystack_len], M[2][haystack_len];
    score_t *last_D, *last_M;
    score_t *curr_D, *curr_M;

    last_D = D[0];
    last_M = M[0];
    curr_D = D[1];
    curr_M = M[1];

    char lower_haystack[haystack_len];
    score_t match_bonus[haystack_len];
    char last_ch = '/';
    for (size_t i = 0; i < haystack_len; i++) {
        char ch = haystack[i];
        match_bonus[i] = COMPUTE_BONUS(last_ch, ch);
        last_ch = ch;
        lower_haystack[i] = tolower(ch);
    }

    for (size_t row = 0; row < needle_len; row++) {
        score_t gap_score =
            row == needle_len - 1 ? SCORE_GAP_TRAILING : SCORE_GAP_INNER;
        char ndl = tolower(needle[row]);
        match_row(ndl, lower_haystack, haystack_len, match_bonus);
        swap(curr_D, last_D);
        swap(curr_M, last_M);
    }
    return last_M[haystack_len - 1];
}

score_t match_positions1(const char* needle,
                         size_t needle_len,
                         const char* haystack,
                         size_t haystack_len,
                         size_t* positions,
                         size_t positions_len) {
    if (!needle || !needle_len || !haystack || !haystack_len ||
        haystack_len > MATCH_MAX_LEN || needle_len > haystack_len) {
        return SCORE_MIN;
    }
    if (needle_len == haystack_len) {
        for (int i = 0; i < needle_len; i++) {
            positions[i] = i;
        }
        return SCORE_MAX;
    }

    score_t D[needle_len][haystack_len], M[needle_len][haystack_len];

    score_t *last_D, *last_M;
    score_t *curr_D, *curr_M;

    char lower_haystack[haystack_len];
    score_t match_bonus[haystack_len];
    char last_ch = '/';
    for (size_t i = 0; i < haystack_len; i++) {
        char ch = haystack[i];
        match_bonus[i] = COMPUTE_BONUS(last_ch, ch);
        last_ch = ch;
        lower_haystack[i] = tolower(ch);
    }

    for (size_t row = 0; row < needle_len; row++) {
        curr_D = &D[row][0];
        curr_M = &M[row][0];
        score_t gap_score =
            row == needle_len - 1 ? SCORE_GAP_TRAILING : SCORE_GAP_INNER;
        char ndl = tolower(needle[row]);
        match_row(ndl, lower_haystack, haystack_len, match_bonus);
        last_D = curr_D;
        last_M = curr_M;
    }

    int match_required = 0;
    for (int i = needle_len - 1, j = haystack_len - 1; i >= 0; i--) {
        for (; j >= 0; j--) {
            if (D[i][j] != SCORE_MIN &&
                (match_required || D[i][j] == M[i][j])) {
                match_required =
                    i && j &&
                    M[i][j] == D[i - 1][j - 1] + SCORE_MATCH_CONSECUTIVE;
                positions[i] = j--;
                break;
            }
        }
    }

    return M[needle_len - 1][haystack_len - 1];
}

score_t match(const char* needle, const char* haystack) {
    return match1(needle, strlen(needle), haystack, strlen(haystack));
}

score_t match_positions(const char* needle,
                        const char* haystack,
                        size_t* positions,
                        size_t positions_len) {
    if (!needle || !*needle || !haystack || !*haystack) {
        return SCORE_MIN;
    }
    size_t const needle_len = strlen(needle);

    if (!positions || positions_len < needle_len) {
        return match1(needle, needle_len, haystack, strlen(haystack));
    }

    return match_positions1(needle,
                            needle_len,
                            haystack,
                            strlen(haystack),
                            positions,
                            positions_len);
}

void fzy_ctx_init(struct fzy_ctx* ctx, const char* needle, size_t needle_len) {
    ctx->needle_len = needle_len;
    ctx->lower_needle = malloc(needle_len + 1);
    for (int i = 0; i < ctx->needle_len; i++) {
        ctx->lower_needle[i] = tolower(needle[i]);
    }
}

score_t fzy_ctx_add_haystack(struct fzy_ctx* ctx,
                             const char* haystack,
                             size_t haystack_len) {

    if (haystack_len < ctx->needle_len) {
        return SCORE_MIN;
    }
    return match_lower(ctx->lower_needle, ctx->needle_len, haystack, haystack_len);
}
