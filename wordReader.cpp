/*
... description: This program will read a text and sort it through unigrams
... and bigrams. Utilize a menu which can be used for more control
... input: Input a text file less than 50,000 words long
... output: multiple files: 1gms.txt 2gms.txt ast1log.txt ast1.txt
... output of unigrams, output of bigrams, output of changes made into text,
... and collection of all words from input file.
...
... Copyright 2023, David Lam, All rights reserved.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

const int ARRAYMAX = 50000;

struct unigram {
    string word;
    int frequency;
    int umpa;
};

struct bigram {
    string word1;
    string word2;
    int frequency;
    int lumpa;
};


/// -------------------------------------------------------------------
/// Function Prototypes
/// -------------------------------------------------------------------

int readdata(string file, string array[]);
int getlongestword(string word[], int wordCount);
int cleanup(string word[], int wordCount);
int inList(string word, struct unigram list[], int count);
int inList(string wordFirst, string wordSecond, struct bigram list[],
           int count);
void sortgms(struct unigram uni[], int wordCount);
void sortgms(struct bigram bi[], int wordCount);
void generate_unigrams(struct unigram unigram[], int& unigramCount,
                       string word[], int wordCount);
void generate_bigrams(struct bigram bigram[], int& bigramCount,
                      string word[], int wordCount);
void write1gmslog(struct unigram unigram[], int uniCount);
void write2gmslog(struct bigram bi[], int biCount);
string cleanInput(string str);

/// -------------------------------------------------------------------
/// Main entry-point for this application is to
/// create a program that can find the unigrams and bigrams of a text file
/// @returns Exit-code for the process - 0 for success,
/// else an error code.
/// -------------------------------------------------------------------

int main(int argc, char *argv[]) {
    int status = EXIT_SUCCESS;

    // Creates output Files
    ofstream outFile;
    outFile.open("ast1.txt");

    if (argc != 2) {
        cout << "Invalid arguments" << endl;
        status = EXIT_FAILURE;
    } else {            // If argc, END
        string word[ARRAYMAX];                // Words of text file stored here
        int wordCount;                        // Counts number of words
        int uniCount = 0;                     // Number of words in unigram
        int biCount = 0;                      // Number of words in bigram
        int largeWordidx = 0;                 // Index of the largest word
        string largeWord = "NA";              // Save the largest word

        string input = "NA";                  // User input for commands
        int inputIdx = 0;                     // User input for index
        int attempt = 10;                     // Max user attempts for input
        int loop;                             // attempts user made
        string str = "NA";                    // Word user wants to search for
        string str2 = "NA";                   // Second word to search for
        int foundIDX = 0;                     // index of word if found


        // Struct of unigram and bigram
        // Struct initialization done in generate_grams
        unigram uni[ARRAYMAX];
        bigram bi[ARRAYMAX];

        cout << string(60, '*') << endl;
        cout << "Array Size: " << ARRAYMAX << endl;
        cout << "Opening File: " << "\"" << string(argv[1]) << "\"" << endl;

        wordCount = readdata(argv[1], word);
        cout << endl << "Finished Reading " << wordCount
             << " words. Closing File." << endl;

        // Terminates while loop and if statement on bad input
        loop = cleanup(word, wordCount);

        if (loop < attempt) {
            /// Word processing
            largeWordidx = getlongestword(word, wordCount);
            largeWord = word[largeWordidx];
            generate_unigrams(uni, uniCount, word, wordCount);
            generate_bigrams(bi, biCount, word, wordCount);
            /// Word processing

            cout << "Longest word is: " << "\""
                 << largeWord << "\"" << endl;
        } else {
            cout << "Bad input, Program terminated" << endl;
        }

        /// User interactive Menu ↓↓↓
        while (loop < attempt) {
            /// Menu Items
            cout << endl << "Commands" << endl;
            cout << "Unigram: search for unigram" << endl;
            cout << "Bigram : search for bigram" << endl;
            cout << "Save   : save 1gms.txt and 2gms.txt" << endl;
            cout << "Index  : print index of unigram or bigram" << endl;
            cout << "Quit/q : terminate program" << endl;
            /// Menu Items

            cin >> input;
            input = cleanInput(input);

            /// Unigram Menu ↓↓↓
            if ((input == "unigram") && (loop < attempt)) {
                cout << "Enter word to search: ";

                if (cin >> str) {
                    str = cleanInput(str);

                    cout << "Searching for " << str << "..." << endl;
                    foundIDX = inList(str, uni, wordCount);

                    if (foundIDX != -1) {
                        cout << "Unigram found at Index: " << foundIDX
                             << " with frequency of "
                             << uni[foundIDX].frequency << endl;
                    } else {
                        cout << "Unable to find word: "
                             << str << endl;
                    }      // Inner if else, END

                    /// Unigram Failed input ↓↓↓
                } else {
                    cout << "Invalid input, please try again." << endl;

                    // Clear input buffer
                    cin.clear();
                    cin.ignore(ARRAYMAX, '\n');
                }          // Outer if else, END

                /// Unigram Failed input ^^^

                loop = 0;    // reset attempts
                /// Unigram Menu, END ^^^
                /// Bigram Menu ↓↓↓
            } else if ((input == "bigram") && (loop < attempt)) {
                cout << "Enter word1 to search: ";

                if (cin >> str) {
                    str = cleanInput(str);
                    cout << "Enter word2 to search: ";

                    if (cin >> str2) {
                        str2 = cleanInput(str2);
                        cout << "Searching for " << str << " " << str2
                             << "..." << endl;
                        foundIDX = inList(str, str2, bi, wordCount);

                        if (foundIDX != -1) {
                            cout << "bigram found at Index: " << foundIDX
                                 << " with frequency of "
                                 << bi[foundIDX].frequency << endl;
                        } else {
                            cout << "Unable to find word: "
                                 << str << " " << str2 << endl;
                        }        // Inner if else, END
                    }      // outer if, END

                    /// Bigram Failed input ↓↓↓
                } else {
                    cout << "Invalid input, please try again." << endl;

                    // Clear input buffer
                    cin.clear();
                    cin.ignore(ARRAYMAX, '\n');
                }          // Outer if else, END

                /// Bigram Failed input ^^^

                loop = 0;    // reset attempts
                /// Bigram Menu, END ^^^
            } else if ((input == "save") && (loop < attempt)) {
                cout << "Writing log to \"1gms.txt\"" << endl;
                cout << "Writing log to \"2gms.txt\"" << endl;
                write1gmslog(uni, uniCount);
                write2gmslog(bi, biCount);

                loop = 0;    // reset attempts
                // Clear input buffer
                cin.clear();
                cin.ignore(ARRAYMAX, '\n');
                /// Index Menu ↓↓↓
            } else if ((input == "index") && (loop < attempt)) {
                cout << "Enter Index: ";

                if (cin >> inputIdx) {
                    cout << "Unigram or Bigram: ";
                    cin >> input;
                    input = cleanInput(input);

                    if ((input == "unigram") &&
                        ((inputIdx < ARRAYMAX) && inputIdx > -1)) {
                        if (!uni[inputIdx].word.empty()) {
                            cout << "Index " << inputIdx << ": '"
                                 << uni[inputIdx].word
                                 << "' with a frequency of "
                                 << uni[inputIdx].frequency << "." << endl;
                        } else {
                            cout << "Unigram at " << inputIdx
                                 << " is empty." << endl;
                        }      // Unigram if else, END

                    } else if ((input == "bigram") &&
                              ((inputIdx < ARRAYMAX) && inputIdx > -1)) {
                        if (!(bi[inputIdx].word1 +
                              bi[inputIdx].word2).empty()) {
                            cout << "Index " << inputIdx << ": '"
                                 << bi[inputIdx].word1 << " "
                                 << bi[inputIdx].word2
                                 << "' with a frequency of "
                                 << bi[inputIdx].frequency << "." << endl;
                        } else {
                            cout << "Bigram at " << inputIdx
                                 << " is empty." << endl;
                        }    // Bigram if else, END

                    } else {
                        cout << "Please enter a numeric value "
                         << "between 0 and 49999." << endl;
                        cout << "Failed input" << endl;

                        // Clear input buffer
                        cin.clear();
                        cin.ignore(ARRAYMAX, '\n');
                    }
                } else {
                    cout << "Please enter a numeric value." << endl;
                    cout << "Failed input" << endl;

                    // Clear input buffer
                    cin.clear();
                    cin.ignore(ARRAYMAX, '\n');
                }

                loop = 0;    // reset attempts
                /// Index Menu, END ^^^
                /// Quit and q exit Menu ↓↓↓
            } else if ((input == "quit") && (loop < attempt)) {
                cout << endl << "Goodbye!" << endl;
                cout << string(60, '*') << endl;
                loop = 100;      // Terminates loop
            } else if ((input == "q") && (loop < attempt)) {
                cout << endl << "Goodbye!" << endl;
                cout << string(60, '*') << endl;
                loop = 100;      // Terminates loop
                /// Quit and q exit Menu, END ^^^
                /// Failed input exit screen ↓↓↓
            } else {
                loop++;
                cout << "Invalid input, please try again." << endl;

                // Clear input buffer
                cin.clear();
                cin.ignore(ARRAYMAX, '\n');
            }    // all the if and else, END

            /// Failed input exit screen, END ^^^
        }    // while, END

        /// User interactive menu, END ^^^

        // Writing info into ast1.txt
        for (int i = 0; i < wordCount; i++) {
            outFile << "Word " << i << ": ";
            outFile << word[i] << endl;
        }       // For, END
    }           // If argc else, END

    return status;
}



/// -------------------------------------------------------------------
/// Function Implementation
/// -------------------------------------------------------------------


/// @brief Function will read data, store in array, and count number of words
///
/// @param file Name of file to be opened
///
/// @param array word array storing text
///
/// @return number of words in text
///
/// @note Function will be lowercase all letters before storing into array
int readdata(string file, string array[]) {
    // Open whatever specified in file
    ifstream inFile;
    inFile.open(file);

    // Used to store words into array, Also is # of words
    int idx = 0;

    // Keeps getting until end of text
    while (inFile >> array[idx]) {
        idx++;
    }               // while, END

    inFile.close();
    return idx;
}

/// @brief Function will find the longest word in an array
///
/// @param word The string array to be searched
///
/// @param wordCount number of words in array
///
/// @return the index that has the largest word
int getlongestword(string word[], int wordCount) {
    int longestWord = 0;
    int index = 0;

    for (int i = 0; i < wordCount; i++) {
        if (static_cast<int>(word[i].length()) > longestWord) {
            longestWord = static_cast<int>(word[i].length());
            index = i;
        }
    }

    return index;
}

/// @brief Function will remove anything that is not a letter
/// or numeric value
///
/// @param word Array to be checked
///
/// @param wordCount Number of words in word array
///
/// @note Function will ask user if cleanup is wanted.
/// Function is changed to an int so it can terminate while loop outside
/// upon bad input.
int cleanup(string word[], int wordCount) {
    char input = 'X';           // User input for log cleanup
    int attempts = 0;           // User attempts to enter y or n...
    bool editMade = false;      // Keeps track when something is changed
    string str = "NA";          // If edit is made, save to string first

    // Used to end while loop outside function if bad input was found
    // (Enter value larger than 10 to terminate outside while loop)
    int status = 0;

    ofstream outFile;
    outFile.open("ast1log.txt");

    // number of user attempts - 1 Ex. 3 attempts... so 2
    int trys = 2;

    while ((input != 'y' && input != 'n') && !(attempts > trys)) {
        cout << "Log cleanup? y/n: ";
        cin >> input;
        input = tolower(input);      // lower case char

        if ((input == 'y') && (attempts < trys)) {
            for (int idx = 0; idx < wordCount; idx++) {
                // If edit is made, original string is saved
                str = word[idx];

                /// https://www.programiz.com/cpp-programming/
                /// library-function/cctype/tolower
                /// Where I found out about tolower
                // For loop will change all letters into lowercase and erase
                // unwanted characters
                for (long unsigned int i = 0; i < str.length(); i++) {
                    // 63 through 90 are all capital letters. If it is
                    // a capital letter, change to lower case
                    if ((64 < str[i]) && (str[i] < 91)) {
                        str[i] = tolower(str[i]);
                        editMade = true;
                    }    // if capital letters, END

                    // If char is not a number or letter, erase it
                    if ((!isalpha(str[i])) && (!isdigit(str[i]))) {
                        str.erase(str.begin() + i);
                        editMade = true;

                        // As to ensure when undesirable is erase, immediate
                        // char is not ignored. EX.  u.ELL
                        // without i--   output is uEll
                        i--;
                    }    // If non-alpha, END

                    if ((!isalpha(str[i])) && (isdigit(str[i]))) {
                        // if value is a digit, change to NONALPHA
                        // and close the loop
                        str = "NONALPHA";
                        i = 1000;      // Terminates loop
                        editMade = true;
                    }
                }               // Inner for, END

                if (editMade) {
                    // output unedited string first
                    outFile << word[idx] << setw(20) << "-->";

                    // output edited string
                    outFile << str << endl;
                    editMade = false;        // toggle
                    word[idx] = str;
                }        // if editMade, END
            }           // Outer for, END
        } else if (input == 'n') {         // if, END
            for (int idx = 0; idx < wordCount; idx++) {
                // If edit is made, original string is saved
                str = word[idx];

                /// https://www.programiz.com/cpp-programming/
                /// library-function/cctype/tolower
                /// Where I found out about tolower
                // For loop will change all letters into lowercase and erase
                // unwanted characters
                for (long unsigned int i = 0; i < str.length(); i++) {
                    // 63 through 90 are all capital letters. If it is
                    // a capital letter, change to lower case
                    if ((64 < str[i]) && (str[i] < 91)) {
                        str[i] = tolower(str[i]);
                        editMade = true;
                    }

                    if ((!isalpha(str[i])) && (!isdigit(str[i]))) {
                    // If char is not a letter, erase it
                        str.erase(str.begin() + i);
                        editMade = true;

                        // As to ensure when undesirable is erase, immediate
                        // char is not ignored. EX.  u.ELL
                        // without i--   output is uEll
                        i--;
                    }

                    // if value is a digit, change to NONALPHA
                    // and close the loop
                    if ((!isalpha(str[i])) && (isdigit(str[i]))) {
                        str = "NONALPHA";
                        i = 1000;      // Terminates loop
                        editMade = true;
                    }        // if value is a digit, END
                }               // Inner for, END

                if (editMade) {
                    editMade = false;        // toggle
                    word[idx] = str;
                }        // if editMade, END

            }           // Outer for, END
            attempts = 25;        // Terminate loop
        } else {
            attempts++;
            cout << "Attempts: " << (trys + 1) - attempts << endl;

            status = 404;
        }
    }        // While, END

    outFile.close();
    return status;
}

/// @function will check if word is in the unigram
///
/// @param word that is to be searched for in unigram
///
/// @param list unigram that is to be searched through
///
/// @param count Number of words in the array
///
/// @note only needs the word and function will return index found
int inList(string word, struct unigram list[], int count) {
    int index = 0;

    for (int idx = 0; idx < count; idx++) {
        if (word.compare(list[idx].word) == 0) {
            index = idx;         // Word found

            idx = 49999;        // terminate code if found
        } else {
            index = -1;        // Failed to find word
        }
    }

    return index;
}

/// @function will check if word is in the bigram
///
/// @param wordFirst the first word to be searched for
///
/// @param wordSecond the second word to be searched for
///
/// @param list bigram that is to be searched through
///
/// @param count number of words in the array
///
/// @note only needs the word and function will return index if found
int inList(string wordFirst, string wordSecond, struct bigram list[],
           int count) {
    int index = 0;
    string wordComplete = (wordFirst + wordSecond);

    for (int idx = 0; idx < count; idx++) {
        if (wordComplete.compare(list[idx].word1 + list[idx].word2) == 0) {
            index = idx;         // Word found

            idx = 49999;        // terminate code if found
        } else {
            index = -1;        // Failed to find word
        }
    }

    return index;
}

/// @function will sort a string array and organize it from the
/// longest frequency to shortest
///
/// @param uni the struct array which contains the words
///
/// @param wordCount number of words in the array
///
/// @note Using a selection sort method, unigram has to already have frequency
/// in the array already. Also duplicate words must be removed already.
/// Function takes into account 0 frequencies and ignores them
void sortgms(struct unigram uni[], int wordCount) {
    string temp = "NA";
    int tempFQ;

    for (int idx = 0; idx < wordCount; idx++) {
        // Checks second freq to compare in unigram

        for (int j = idx + 1; j < wordCount; j++) {
            // && is needed as to ensure NULL is not considered
            if ((uni[j].frequency > uni[idx].frequency)
                && uni[idx].frequency != 0) {

                // Will move idx word into largest index word
                // and largest index word will move down to idx word
                temp = uni[j].word;
                uni[j].word = uni[idx].word;
                uni[idx].word = temp;

                // Moves idx freq into largest index freq
                // and largest index freq into idx freq
                tempFQ = uni[j].frequency;
                uni[j].frequency = uni[idx].frequency;
                uni[idx].frequency = tempFQ;
            }      // if, END
        }    // Inner for, END
    }      // Outer for, END
}

/// @function will sort a string array and organize it
/// alphabetically. It looks at both words
///
/// @param bi the struct array which contains the words
///
/// @param wordCount Number of words in the array
///
/// @note Function sorts alphabetically without messing with original array
/// This function took me longer than it should of.
/// This physically and mentally hurt me.
void sortgms(struct bigram bi[], int wordCount) {
    for (int idx = 0; idx < wordCount - 1; idx++) {
        for (int j = idx + 1; j < wordCount; j++) {

            // alphabetical letters are moved up
            if ((bi[idx].word1 + bi[idx].word2) > (bi[j].word1 + bi[j].word2)) {
                swap(bi[idx].word1, bi[j].word1);
                swap(bi[idx].word2, bi[j].word2);
            }    // if, END
        }      // Inner for, END
    }      // outer for, END
}


/// @function will populate and format the unigram to standard
///
/// @param unigram Struct of the unigram to be sorted
///
/// @param unigramCount The new number of words in unigram
///
/// @param word The word array with the words
///
/// @param wordCount The number of words in original array
///
/// @note The second and third outer for loops here will count
/// frequency of words and removed duplicate words while keeping
/// the words and frequencies matching each other.
void generate_unigrams(struct unigram unigram[], int& unigramCount,
                       string word[], int wordCount) {
    unigramCount = wordCount;    // Used for finding new number in unigram

    // Copy the word array into the unigram
    for (int idx = 0; idx < wordCount; idx++) {
        unigram[idx].word = "NA";    // Initialization of word
        unigram[idx].word = word[idx];
    }

    for (int idx = 0; idx < wordCount; idx++) {
        // Initializes unigram to 1
        unigram[idx].frequency = 1;

        // Checks second word to compare in unigram
        for (int j = idx + 1; j < wordCount; j++) {
            // && is needed as to ensure NULL is not
            // compared with NULL
            if ((unigram[idx].word.compare(unigram[j].word) == 0)
                && !unigram[idx].word.empty()) {
                unigram[idx].frequency++;
                unigramCount--;

                // erases the additional of the same word.
                // Prepares it for the next loop
                unigram[j].word.erase(0, unigram[j].word.length());
                //unigram[j].frequency = 0;
            }      // if, END
        }    // Inner for, END
    }      // Outer for, END

    // These loops will move the frequency as well as words
    // up. It also erases the word it copied
    for (int idx = 0; idx < wordCount; idx++) {
        for (int j = idx + 1; j < wordCount; j++) {
            if (unigram[idx].word.empty()) {        // Look, empty word array
                // replace word with the once after it
                unigram[idx].word = unigram[j].word;

                // replace frequency with the one after it
                unigram[idx].frequency = unigram[j].frequency;

                // The copied word is erased
                unigram[j].word.erase(0, unigram[j].word.length());
            }    // If, END
        }    // Inner for, END
    }    // Outer for, END

    // Moves words of larger freq up
    // unigramCount or wordCount can be used here
    sortgms(unigram, unigramCount);
}

/// @function will populate and format the bigram to standard
///
/// @param bigram the struct to be organized
///
/// @param bigramCount new number of words in array
///
/// @param word The word array with the words
///
/// @param wordCount The number of words in original array
///
/// @note words in bigram are moved up here and frequencies are counted
void generate_bigrams(struct bigram bigram[], int& bigramCount,
                      string word[], int wordCount) {
    bigramCount = wordCount;      // Used for finding out new number in bigram

    // save info from words to bigram
    for (int idx = 0; idx < wordCount; idx++) {
        // Initialization of word1 and word2
        bigram[idx].word1 = "NA";
        bigram[idx + 1].word2 = "NA";

        bigram[idx].word1 = word[idx];
        bigram[idx].word2 = word[idx + 1];
    }

    sortgms(bigram, wordCount);        // Alphabetized

    for (int idx = 0; idx < wordCount; idx++) {
        bigram[idx].frequency = 1;    // Initializing

        for (int j = idx + 1; j < wordCount; j++) {
            // alphabetical letters are moved up
            // within if statement, The two words are combined and compared
            // with the next two words. Furthermore, makes sure if it is
            // empty, don't compare.
            if (((bigram[idx].word1 + bigram[idx].word2).compare(
                     (bigram[j].word1 + bigram[j].word2)) == 0)
                && !(bigram[idx].word1 + bigram[idx].word2).empty()) {
                bigram[idx].frequency++;
                bigramCount--;

                bigram[j].word1.erase(0, bigram[j].word1.length());
                bigram[j].word2.erase(0, bigram[j].word2.length());
            }    // if, END
        }      // Inner for, END
    }      // outer for, END

    // These loops will move the frequency as well as words
    // up. It also erases the word it copied
    for (int idx = 0; idx < wordCount; idx++) {
        for (int j = idx + 1; j < wordCount; j++) {
            // if first word is empty, then second is too
            if (bigram[idx].word1.empty()) {        // Look, empty word array
                /// For word1 ↓↓↓
                // replace word with the once after it
                bigram[idx].word1 = bigram[j].word1;

                // The copied word is erased
                bigram[j].word1.erase(0, bigram[j].word1.length());
                /// For word1 ^^^

                /// For word2 ↓↓↓
                // replace word with the once after it
                bigram[idx].word2 = bigram[j].word2;

                // The copied word is erased
                bigram[j].word2.erase(0, bigram[j].word2.length());
                /// For word2 ^^^

                // replace frequency with the one after it
                // Only needs to be ran once per
                bigram[idx].frequency = bigram[j].frequency;
            }    // If, END
        }    // Inner for, END
    }    // Outer for, END
}

/// @function will output the unigram logs
/// into an outfile "1gms.txt"
///
/// @param unigram Struct of the unigram to be outputted
///
/// @param uniCount run loop this many times
///
/// @note Now working with 50% less bugs
void write1gmslog(struct unigram unigram[], int uniCount) {
    ofstream outFile;
    outFile.open("1gms.txt");

    for (int idx = 0; idx < uniCount; idx++) {
        if (!unigram[idx].word.empty()) {    // Hot fix :(
            outFile << unigram[idx].word << setw(25)
                    << unigram[idx].frequency << endl;
        }
    }

    outFile.close();
}

/// @function will output the bigram logs
/// into an outfile "2gms.txt"
///
/// @param bi Struct of the bigram to be outputted
///
/// @param biCount run loop this many times
void write2gmslog(struct bigram bi[], int biCount) {
    ofstream outFile;
    outFile.open("2gms.txt");

    for (int idx = 0; idx < biCount; idx++) {
        if (!(bi[idx].word1 + bi[idx].word2).empty()) {    // Hot fix :(
            outFile << bi[idx].word1 << " " << bi[idx].word2
                    << setw(25) << bi[idx].frequency << endl;
        }
    }

    outFile.close();
}

/// @function will clean up a single string and return it
///
/// @param str string to be cleaned up
///
/// @return returns the string after processing
///
/// @note Function is intended to be used in the user menu
/// lowercase letters. Random punctuations will
/// NOT be removed. Output this.
string cleanInput(string str) {
    for (long unsigned int i = 0; i < str.length(); i++) {
        // Function will just lower all letters
        str[i] = tolower(str[i]);
    
    }               // Inner for, END
    return str;
}
