#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "words.h"

#define LETTER_IDX(lt) ((lt) - 'a')
#define LETTER_MASK(lt) (1U << LETTER_IDX(lt))

#define WORD_LENGTH 5
#define CANDIDATE_COUNT 5

typedef struct FilterInput {
    uint32_t mustHave;
    uint32_t mustNotHave;
    uint32_t mustHavePos[WORD_LENGTH];
    uint32_t mustNotHavePos[WORD_LENGTH];
} FilterInput;

typedef struct FilterOutput {
    word** filtered;
    int filteredLen;
    int letterCounts[26];
    int letterPositionCounts[WORD_LENGTH][26];
} FilterOutput;

typedef struct ScoreCell {
    word* w;
    double score;
} ScoreCell;

static void freeFilterOutput(FilterOutput* fo) { free(fo->filtered); }

static double scoreWord(word* w, const FilterInput* fi,
                        const FilterOutput* fo) {
    const double flen = (double)fo->filteredLen;
    double score = 0.0;
    uint32_t seen = 0;
    for (int i = 0; i < WORD_LENGTH; i++) {
        const char c = w->w[i];
        if (fi->mustHavePos[i] != 0) {
            // Don't count the score of letters we're already sure about.
            continue;
        } else if ((seen & LETTER_MASK(c)) > 0) {
            // Don't count the same letter twice.
            continue;
        }

        const int idx = LETTER_IDX(c);
        score += (fo->letterCounts[idx] / flen) *
                 (fo->letterPositionCounts[i][idx] / flen);

        seen |= LETTER_MASK(c);
    }
    return score;
}

static void filterWords(word* words, int wordLen, const FilterInput* in,
                        FilterOutput* out) {
    int filteredCap = 2;
    out->filtered = calloc(filteredCap, sizeof(word*));
    out->filteredLen = 0;
    for (int i = 0; i < wordLen; i++) {
        word* w = &words[i];
        if ((w->contains & in->mustHave) != in->mustHave ||
            (w->contains & in->mustNotHave) != 0) {
            continue;
        }
        bool isGood = true;
        for (int j = 0; j < WORD_LENGTH; ++j) {
            if ((in->mustHavePos[j] &&
                 ((w->letterMasks[j] & in->mustHavePos[j]) == 0)) ||
                (w->letterMasks[j] & in->mustNotHavePos[j]) != 0) {
                isGood = false;
                break;
            }
        }
        if (!isGood) {
            continue;
        }
        out->filtered[out->filteredLen++] = w;

        for (int i = 0; i < WORD_LENGTH; i++) {
            out->letterCounts[LETTER_IDX(w->w[i])] += 1;
            out->letterPositionCounts[i][LETTER_IDX(w->w[i])] += 1;
        }
        if (out->filteredLen == filteredCap) {
            filteredCap *= 2;
            out->filtered = realloc(out->filtered, filteredCap * sizeof(word*));
        }
    }
}

static bool isValidArgument(const char* arg, int* len) {
    *len = strlen(arg);

    if (*len < 2) {
        return false;
    }
    if (arg[0] != '+' && arg[0] != '-' && arg[0] != '%') {
        return false;
    }
    if ((arg[0] == '%' || arg[0] == '+') &&
        ((*len != 3) || (arg[2] - '0' < 1 || arg[2] - '0' > WORD_LENGTH))) {
        return false;
    }
    if (!islower(arg[1])) {
        return false;
    }
    if (arg[0] == '-') {
        for (int i = 1; i < *len; ++i) {
            if (!islower(arg[i])) {
                return false;
            }
        }
    }
    return true;
}

static int compareScores(const void* a, const void* b) {
    const ScoreCell* sa = (const ScoreCell*)a;
    const ScoreCell* sb = (const ScoreCell*)b;
    // Sort descending so reverse the return values.
    if (sa->score < sb->score) {
        return 1;
    } else if (sa->score > sb->score) {
        return -1;
    } else {
        return 0;
    }
}

int main(int argc, char** argv) {
    FilterInput in = {0};

    for (int i = 1; i < argc; ++i) {
        char* arg = argv[i];
        int len;
        if (!isValidArgument(arg, &len)) {
            fprintf(stderr, "Invalid argument: %s\n", arg);
            return EXIT_FAILURE;
        }
        int pos;
        switch (arg[0]) {
            case '+':
                // This is a mustHave argument.
                in.mustHave |= LETTER_MASK(arg[1]);
                if (len == 3) {
                    // This is a mustHavePosArgument.
                    pos = (int)(arg[2] - '0' - 1);
                    // Each position can only be a single letter, so just assign
                    // don't OR.
                    in.mustHavePos[pos] = LETTER_MASK(arg[1]);
                }
                break;
            case '-':
                for (int j = 1; j < len; ++j) {
                    in.mustNotHave |= LETTER_MASK(arg[j]);
                }
                break;
            case '%':
                // This letter must exist, but not at the location.
                in.mustHave |= LETTER_MASK(arg[1]);
                pos = (int)(arg[2] - '0' - 1);
                in.mustNotHavePos[pos] |= LETTER_MASK(arg[1]);
                break;
            default:
                assert(false);
        }
    }

    FilterOutput out = {0};
    filterWords(words, wordCount, &in, &out);

    ScoreCell* scores = calloc(out.filteredLen, sizeof(ScoreCell));

    for (int i = 0; i < out.filteredLen; ++i) {
        scores[i].w = out.filtered[i];
        scores[i].score = scoreWord(out.filtered[i], &in, &out);
    }

    qsort(scores, out.filteredLen, sizeof(ScoreCell), compareScores);
    const int wordCount =
        out.filteredLen >= CANDIDATE_COUNT ? CANDIDATE_COUNT : out.filteredLen;
    for (int i = 0; i < wordCount; ++i) {
        printf("%s\n", scores[i].w->w);
    }
    printf("Total Matches: %d\n", out.filteredLen);

    freeFilterOutput(&out);
    free(scores);

    return EXIT_SUCCESS;
}
