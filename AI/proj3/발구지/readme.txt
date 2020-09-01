g++ -o proj3 main.cpp
./proj3 <filename>

After running this program, it will read the training data from filename.
It will generate unigram and bigram of training data.
And then it will print "Insert your sentence: (insert q or Q to terminate)"
You just insert your sentence that might be corrected.


Reading and correcting the user input
 - This program will read input from stdin line by line.
 - This program will generate noisy-channel-score for each word in input sentence.

For each word of input, program generates and prints a complete list
of candidates...
 - This program will find same length, with exactly one letter changed.
 - ...with one inserted letter.
 -  ...with one deleted letter.
 - ...with insertions up to an edit distance of 2.

Candidate words menu:
 - After reading user input, it will print the list of candidate of each word.
 - The list of candidate is sorted by the score.

Error model
 - This program permit all three edit types that accounts for keyboard adjacency when generating changes and insertions.

Training language model:
 - Since this program read the training data by word, it will check that this current word has appeared before. If not, add to the pool.
 
Language model evaluates
 - After inserting phase ends, this program will show you the unigram probability of each word of input.
 - After showing the unigram probability, then this program will print bigram probability of each word of input (with “previous word” as edited, if menu of candidate words is implemented).


