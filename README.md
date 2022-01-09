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

There are three kinds of filters: exact match filters (using `+`), negative matches (using `-`), and partial matches (using `%`).

#### Exact Matches

Exact match filters are in the format `+a1`, with the first character `+`, the second character the matched character, and the third character the letter's index (between 1 and 5).

#### Negative Matches

Negative match filters start with `-` and contain one or more characters that don't exist in the target word. For example, `-qwerty` would filter out all words that contain `q`, `w`, `e`, `r`, `t`, or `y`.

#### Partial Matches

Partial matches are in the format `?a1`, with the first character `?`, the second character the matched characters, and the third character the index where the character **isn't**. That is, `?z1` would filter all words that either don't have a `z` in them or that have a `z` in position 1.
