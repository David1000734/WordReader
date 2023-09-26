# WordReader
Read a text file up to 50,000 word long and spit out number of unigram and bigram in the list

### Introduction
Consider the following sentence: **Did you do it? What did it cost?**

#### Results
Each individual word will be cleaned up and counted
* Unigrams are sorted by frequency

|Unigram |Count|
|--------|-----|
|did     | 2   |
|it      | 2   |
|you     | 1   |
|do      | 1   |
|what    | 1   |
|cost    | 1   |

* Bigrams are sorted alphabetically

|Bigram  |Count|
|--------|-----|
|did it  | 1   |
|did you | 1   |
|do it   | 1   |
|it cost | 1   |
|it what | 1   |
|what did| 1   |
|you do  | 1   |

## How to use
* Build by using `make`

Run by using `./wordReader <textfile.txt>`

