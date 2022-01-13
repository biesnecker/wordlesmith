import json

with open("accepted.json") as fp:
    c = fp.read()
    words = json.loads(c)
    accepted = [(w, False) for w in words]

with open("answers.json") as fp:
    c = fp.read()
    words = json.loads(c)
    answers = [(w, True) for w in words]


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
            "} word;\n\n",
        ]
    )

    fp.write("word words[] = {\n")
    wc = 0
    for lst in [accepted, answers]:
        for (word, canBeAnswer) in lst:
            if len(word) != 5:
                continue
            letters = []
            seen = 0
            for c in word:
                m = 1 << (ord(c) - ord("a"))
                seen |= m
                letters.append(m)
            letterMasks = "{" + ", ".join(str(lm) + "U" for lm in letters) + "}"
            cba = "false"
            if canBeAnswer:
                cba = "true"
            fp.write(f'    {{"{word}", {letterMasks}, {seen}U, {cba}}},\n')
            wc += 1
    fp.write("};\n\n")
    fp.write(f"const int wordCount = {wc};\n")
