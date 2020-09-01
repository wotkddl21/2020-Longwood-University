gcc -o proj3.c proj3.c
./proj3.c <filename>

The function check_word will get the candidate.
If it is a kind of word, then edit it into plausible word and return 1.
If not, return 0.
The every word is stored at word_list.
The struct word_list contains char* word, int num.
From the first word to last word, the program will add the new word into word_list or increase the num of the old word.

