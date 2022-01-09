# Wordlesmith

A tiny little helper utility for [Wordle](https://www.powerlanguage.co.uk/wordle/) written in C.

## Building

```
make
```

Tested with `clang-1300.0.29.3` but ought to work on anything remotely modern.

## Usage

Wordlesmith starts with an 8,636-word Scrabble dictionary for five letter words. You then use filters, in the form of command line arguments, to whittle down the wordlist until you get the correct answer.

### Filters

There are three kinds of filters: exact match (using `+`), negative matches (using `-`), and partial matches (using `%`).

#### Exact Matches

Exact match filters are in the format `+a1`, with the first character `+`, the second character the matched character, and the third character the letter's index (between 1 and 5).

#### Negative Matches

Negative match filters start with `-` and contain one or more characters that don't exist in the target word. For example, `-qwerty` would filter out all words that contain `q`, `w`, `e`, `r`, `t`, or `y`.

#### Partial Matches

Partial matches are in the format `?a1`, with the first character `?`, the second character the matched characters, and the third character the index where the character **isn't**. That is, `?z1` would filter all words that either don't have a `z` in them or that have a `z` in position 1.

### Filter Examples

![](https://via.placeholder.com/64/808080/000000?text=Q) ![](https://via.placeholder.com/64/808080/000000?text=W) ![](https://via.placeholder.com/64/808080/000000?text=E) ![](https://via.placeholder.com/64/808080/000000?text=R) ![](https://via.placeholder.com/64/808080/000000?text=T)

`wordlesmith -qwert`

Filters out all words that contain any of the characters `q`, `w`, `e`, `r`, or `t`.

![](https://via.placeholder.com/64/ffff00/000000?text=Q) ![](https://via.placeholder.com/64/808080/000000?text=W) ![](https://via.placeholder.com/64/808080/000000?text=E) ![](https://via.placeholder.com/64/808080/000000?text=R) ![](https://via.placeholder.com/64/ffff00/000000?text=T)

`wordlesmith %q1 -wer %t5`

The order of the filters doesn't matter, and negative matches can be all combined into a single argument, or split into multiple arguments. The two partial matches in this example are saying:

- The word contains a `q`, but not in the first position.
- The word contains a `t`, but not in the fifth position.

![](https://via.placeholder.com/64/ffff00/000000?text=Q) ![](https://via.placeholder.com/64/808080/000000?text=W) ![](https://via.placeholder.com/64/808080/000000?text=E) ![](https://via.placeholder.com/64/00ff00/000000?text=R) ![](https://via.placeholder.com/64/ffff00/000000?text=T)

`wordlesmith %q1 -we %t5 +r4`

This is the same as the previous filter, but with an exact match on the `r` in the fourth position.

