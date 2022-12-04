// Copyright (c) 2022 Gustavo Ribeiro Croscato
// SPDX-License-Identifier: MIT

/*
--- Day 5: Doesn't He Have Intern-Elves For This? ---

Santa needs help figuring out which strings in his text file are naughty or
nice.

A nice string is one with all of the following properties:
    - It contains at least three vowels (aeiou only), like aei, xazegov, or
      aeiouaeiouaeiou.
    - It contains at least one letter that appears twice in a row, like xx,
      abcdde (dd), or aabbccdd (aa, bb, cc, or dd).
    - It does not contain the strings ab, cd, pq, or xy, even if they are
      part of one of the other requirements.

For example:
    - ugknbfddgicrmopn is nice because it has at least three vowels
      (u...i...o...), a double letter (...dd...), and none of the
      disallowed substrings.
    - aaa is nice because it has at least three vowels and a double letter,
      even though the letters used by different rules overlap.
    - jchzalrnumimnmhp is naughty because it has no double letter.
    - haegwjzuvuyypxyu is naughty because it contains the string xy.
    - dvszwmarrgswjxmb is naughty because it contains only one vowel.

How many strings are nice?


--- Part Two ---

Realizing the error of his ways, Santa has switched to a better model of
determining whether a string is naughty or nice. None of the old rules
apply, as they are all clearly ridiculous.

Now, a nice string is one with all of the following properties:
    - It contains a pair of any two letters that appears at least twice in
      the string without overlapping, like xyxy (xy) or aabcdefgaa (aa),
      but not like aaa (aa, but it overlaps).
    - It contains at least one letter which repeats with exactly one letter
      between them, like xyx, abcdefeghi (efe), or even aaa.

For example:
    - qjhvhtzxzqqjkmpb is nice because is has a pair that appears twice
      (qj) and a letter that repeats with exactly one letter between them
      (zxz).
    - xxyxx is nice because it has a pair that appears twice and a letter
      that repeats with one between, even though the letters used by each
      rule overlap.
    - uurcxstgmygtbstg is naughty because it has a pair (tg) but no repeat
      with a single letter between them.
    - ieodomkazucvgmuy is naughty because it has a repeating letter with
      one between (odo), but no pair that appears twice.

How many strings are nice under these new rules?

*/

static bool
IsNiceOne(Slice str)
{
    i64 vowels = 0;
    bool has_doubles = false;

    for (u64 i = 1; i <= str.size; ++i) {
        char ch1 = str.data[i - 1];
        char ch2 = str.data[i];

        if (
            (ch1 == 'a') || (ch1 == 'A') ||
            (ch1 == 'e') || (ch1 == 'E') ||
            (ch1 == 'i') || (ch1 == 'I') ||
            (ch1 == 'o') || (ch1 == 'O') ||
            (ch1 == 'u') || (ch1 == 'U')
        ) {
            vowels++;
        }

        if (!has_doubles && ch1 == ch2) {
            has_doubles = true;
        }

        if (
            ((ch1 == 'a' || ch1 == 'A') && (ch2 == 'b' || ch2 == 'B')) ||
            ((ch1 == 'c' || ch1 == 'C') && (ch2 == 'd' || ch2 == 'D')) ||
            ((ch1 == 'p' || ch1 == 'P') && (ch2 == 'q' || ch2 == 'Q')) ||
            ((ch1 == 'x' || ch1 == 'X') && (ch2 == 'y' || ch2 == 'Y'))
        ) {
            return false;
        }
    }

    return (vowels >= 3) && has_doubles;
}

static bool
IsNiceTwo(Slice str)
{
    bool has_pair = false;
    bool has_between = false;

    char sub_string[3] = {0};

    for (u64 i = 0; i < str.size - 3; ++i) {
        sub_string[0] = str.data[i + 0];
        sub_string[1] = str.data[i + 1];

        if (strstr(str.data + i + 2, sub_string) != NULL) {
            has_pair = true;

            break;
        }
    }

    for (u64 i = 0; i < str.size - 2; ++i) {
        sub_string[0] = str.data[i + 0];
        sub_string[1] = str.data[i + 1];

        if (str.data[i] == str.data[i + 2]) {
            has_between = true;

            break;
        }
    }

    return has_pair && has_between;
}

void
Part_One(Slice input)
{
    u64 nice = 0;

    while (1) {
        Slice line = Slice_ReadLine(&input);

        if (line.data == NULL) {
            break;
        }

        if (IsNiceOne(line)) {
            nice++;
        }
    }

    printf("Part one: nice count %lu\n", nice);
}

void
Part_Two(Slice input)
{
    u64 nice = 0;

    while (1) {
        Slice line = Slice_ReadLine(&input);

        if (line.data == NULL) {
            break;
        }

        if (IsNiceTwo(line)) {
            nice++;
        }
    }

    printf("Part two: nice count %lu\n", nice);
}

int
main(void)
{
    Slice input = StdIn_ReadAll();

    if (input.data == NULL || *input.data == '\0') {
        Quit(1, "%s: empty input.", NAME);
    }

    Part_One(input);
    Part_Two(input);

    return 0;
}

