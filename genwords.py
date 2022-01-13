import json

with open("accepted.json") as fp:
    c = fp.read()
    words = json.loads(c)
    accepted = set(w for w in words)

with open("answers.json") as fp:
    c = fp.read()
    words = json.loads(c)
    answers = set(w for w in words)

with open("scrabble.json") as fp:
    c = fp.read()
    words = json.loads(c)
    scrabble = set(w for w in words)

ALL_WORDS = sorted(set().union(accepted, answers, scrabble))

with open("words.h", "w") as fp:
    fp.write("#pragma once\n\n")
    fp.write("#include <stdbool.h>\n")
    fp.write("#include <stdint.h>\n")
    fp.write("\n")
    fp.writelines(
        [
            "typedef struct word {\n",
            "    const char* w;\n",
            "    const uint32_t letterMasks[5];\n",
            "    const uint32_t contains;\n",
            "    const bool canBeAnswer;\n",
            "    const bool isScrabbleWord;\n",
            "} word;\n\n",
        ]
    )

    fp.write("word words[] = {\n")
    wc = 0
    for word in ALL_WORDS:
        if len(word) != 5:
            continue
        canBeAnswer = word in answers
        scrabbleWord = word in scrabble
        letters = []
        seen = 0
        for c in word:
            m = 1 << (ord(c) - ord("a"))
            seen |= m
            letters.append(m)
        letterMasks = "{" + ", ".join(str(lm) + "U" for lm in letters) + "}"
        cba = "false"
        scr = "false"
        if canBeAnswer:
            cba = "true"
        if scrabbleWord:
            scr = "true"
        fp.write(f'    {{"{word}", {letterMasks}, {seen}U, {cba}, {scr}}},\n')
        wc += 1
    fp.write("};\n\n")
    fp.write(f"const int wordCount = {wc};\n")
