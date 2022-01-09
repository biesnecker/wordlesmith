import json

with open("dictionary.json") as fp:
    c = fp.read()
    words = json.loads(c)

with open("words.h", "w") as fp:
    fp.write("#pragma once\n\n")
    fp.write("#include <stdint.h>\n\n")
    fp.writelines(
        [
            "typedef struct word {\n",
            "    const char* w;\n",
            "    const uint32_t letterMasks[5];\n",
            "    const uint32_t contains;\n",
            "} word;\n\n",
        ]
    )

    fp.write("word words[] = {\n")
    wc = 0
    for word in words:
        if len(word) != 5:
            continue
        letters = []
        seen = 0
        for c in word:
            m = 1 << (ord(c) - ord("a"))
            seen |= m
            letters.append(m)
        letterMasks = "{" + ", ".join(str(lm) + "U" for lm in letters) + "}"
        fp.write(f'    {{"{word}", {letterMasks}, {seen}U}},\n')
        wc += 1
    fp.write("};\n\n")
    fp.write(f"const int wordCount = {wc};\n")
