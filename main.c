#include <stdio.h>
#include <string.h>

#include "match.h"

#define XX(n, h, s)                                                            \
    ({                                                                         \
        printf("(\"%s\", \"%s\", %lf, %lf),\n",                                \
               n,                                                              \
               h,                                                              \
               s,                                                              \
               fzy_ctx_add_haystack(&ctx, h, strlen(h)));                      \
    })

int main(int argc, char* argv[]) {
    const char* needle = "a";
    const char* haystack = "bA";
    score_t ret = match(needle, haystack);
    struct fzy_ctx ctx;
    fzy_ctx_init(&ctx, "fr", 2);

    XX("fr", "free", 0.8666666666666667);
    XX("fr", "front_insert_iterator", 0.7587301587301588);
    XX("fr", "free", 0.8666666666666671);
    XX("fr", "free", 7.663544400533041);
    XX("fr", "fread", 1.5571811342239386);
    XX("fr", "fread_unlocked", 1.430064306940352);
    XX("fr", "freopen", 1.5006847665423422);
    XX("fr", "free_args", 1.4059915524941928);
    XX("fr", "frexp", 1.2631391572952282);
    XX("fr", "fromfp", 1.236406053437129);
    XX("fr", "fromfpx", 1.2173109792527725);
    XX("fr", "fcvt_r", 1.1122722387313857);
    XX("fr", "frexpf", 0.24728121558825222);
    XX("fr", "frexpf32", 0.2405979394912734);
    XX("fr", "frexpf32x", 0.23837018079228045);
    XX("fr", "frexpf64", 0.2405979394912734);
    XX("fr", "frexpf64x", 0.23837018079228045);
    XX("fr", "frexpl", 0.24728121558825222);
    XX("fr", "fromfpf", 0.2434622006756939);
    XX("fr", "fromfpf32", 0.23837018079228045);
    XX("fr", "fromfpf32x", 0.23658797383308183);
    XX("fr", "fromfpf64", 0.23837018079228045);
    XX("fr", "fromfpf64x", 0.23658797383308183);
    XX("fr", "fromfpl", 0.2434622006756939);
    XX("fr", "fromfpxf", 0.2405979394912734);
    XX("fr", "fromfpxf32", 0.23658797383308183);
    XX("fr", "fromfpxf32x", 0.23512980450283294);
    XX("fr", "fromfpxf64", 0.23658797383308183);
    XX("fr", "fromfpxf64x", 0.23512980450283294);
    XX("fr", "fromfpxl", 0.2405979394912734);
    XX("a", "bA", 0.2405979394912734);

    return 0;
}
