//  Shlomo Stept
//  CS106B - self monitored class, to familiarize myself with data structues in c++
//  Date: 7/22/21
//  Homework 2: Serafini --> Part A: Word ladder 

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "queue.h"
#include "lexicon.h"
#include "stack.h"

using namespace std;  // Lesson 1: Must put the "namespace std" before prototypes in order for some (ex. fstream) to be recognized

//------------------------------------------------------------------------------------------------------
        // :: Function prototypes ::
//----------------------------------------------------------------------------------------------------------
void Welcome();
void createDictionary(Lexicon &);
void getFile(fstream & );
bool getTwoWords(string &, string&,  Lexicon&);
string toLowercase(string word);
Stack<string> getWordLadder(string word1, string word2, Lexicon&, bool & failed);

//cout << " "<<endl;

//------------------------------------------------------------------------------------------------------
        // :: Main ::
//----------------------------------------------------------------------------------------------------------
int main() {
    // 1. welcome the user and introduce how the program works
    Welcome();

    // 2. create a dictionary from the user entered filename(dictionary)
    Lexicon dictionary;
    createDictionary(dictionary);

    // 3. continuiously get two ("good") words from user and find their word ladder, untill [Enter] is pressed
    string word1 = "", word2 = "";
    bool runLoop = true;
    while(runLoop){
        // 3A- get the two words or [Enter] from user - and test to determine if user wants to quit
        runLoop = getTwoWords(word1, word2, dictionary);
        if(!runLoop){
            continue;
        }
        else{
            // 3B - compute the word ladder
            bool failed;
            Stack<string> ladder = getWordLadder(word1, word2, dictionary, failed);

            // 3C- print the word-laddder or if failed inform user
            if(!failed){
                cout << "A ladder from "<< word2 << " back to " << word1 <<":" <<endl;
                do{
                    cout<< ladder.pop() << " ";
                }while(!ladder.isEmpty());
                cout<<"\n";
            } else{
                cout << "No word ladder found from "<< word2 << " back to " << word1 <<"." <<endl;
            }
        }
    }

    cout << "Have a nice day." << endl;
    return 0;
}

//------------------------------------------------------------------------------------------------------
        // :: Functions ::
//----------------------------------------------------------------------------------------------------------


        //  Function 1 --> Welcome: introduce how the programs works to the user
//----------------------------------------------------------------------------------------------------------
void Welcome(){
    cout << "Welcome to CS 106B Word Ladder." <<endl;
    cout << "Please give me two English words, and I will change the"<<endl;
    cout << "first into the second by changing one letter at a time."<<endl;
}


//------------------------------------------------------------------------------------------------------
        //  Function 2 --> createDictionary: create a dictionary from the user entered filename(dictionary)\
                        // --> (A) Must make sure file is valid
    //----------------------------------------------------------------------------------------------------------
void createDictionary(Lexicon & dict){
    // A --> Get the correct file.
    fstream infile;
    getFile(infile);

    // B --> populate the dictionary
    string word;
    while(infile >> word ){
        dict.add(word);
    }

    // C --> must close the file.
    infile.close();

}


//------------------------------------------------------------------------------------------------------
        //  Function 2.5 --> getFile: personal adaptation for prompting user for file untill a valid file is entered
    //----------------------------------------------------------------------------------------------------------
void getFile(fstream & infile){
    string filename;
    cout << "\nDictionary file name? ";
    getline(cin,filename);

    infile.open(filename);
    while(infile.fail()){       // testing to see how .fail() is diff that (!.is_open())
        cout <<"Unable to open that file.  Try again." << endl;
        cout << "Dictionary file name? ";
        getline(cin,filename);
        infile.open(filename);
    }
}


//------------------------------------------------------------------------------------------------------
        //  Function 3 --> Prompt user to enter two valid, distict words of equal length, or untill [enter] is pressed
    //----------------------------------------------------------------------------------------------------------
bool getTwoWords(string &word1, string &word2, Lexicon& dict){
    bool done = false;

    // 1.0 --> run while loop untill user enters two valid, distict words of equal length, or pressed [enter] to quit
    while( !done ){
        // A --> get the two words, and if user selects [enter] return false to quit program
        cout << "\nWord #1 (or Enter to quit): ";
        getline(cin, word1);
        if(word1 ==""){
            return false;
        }else{
            cout << "Word #2 (or Enter to quit): ";
            getline(cin, word2);
            if(word2 ==""){
                return false;
            }
        }

        // B --> make both words lowercase
        word1 = toLowercase(word1);
        word2 = toLowercase(word2);

        // C --> test that both words are: (i) same length, (ii) found in the dictionary, (iii) not the same words
        if(word1.size() != word2.size()){
            cout << "The two words must be the same length."<<endl;
        }
        else if(!dict.contains(word1)){
            cout << "The two words must be found in the dictionary."<<endl;
        }
        else if(!dict.contains(word2)){
            cout << "The two words must be found in the dictionary."<<endl;
        }
        else if( word1 == word2){
            cout << "The two words must be different." <<endl;
        }
        else{
            done = true;
            return true;
        }
    }
    return true;
}

string toLowercase(string word){
    string lower="";
    for(int i=0; i<word.size(); i++){
        lower+= tolower(word.at(i));
    }
    return lower;
}


//------------------------------------------------------------------------------------------------------
        //  Function 4 --> getWordLadder
    //----------------------------------------------------------------------------------------------------------
Stack<string> getWordLadder(string start, string end, Lexicon& dictionary, bool & failed){
    // A- initalize all the requred datastructures and variables
    Queue<Stack<string>> library;
    Stack<string> s1, current;
    Lexicon usedWords;
    string aToz = "abcdefghijklmnopqrstuvwxyz";
    failed = true;


    // B- initalize the queue of stacks (put the first stack on the queue)
    s1.push(start);
    library.enqueue(s1);

    // C- untill the end word is first found, continuiously dequeue the stack and carry out the following:
        // (i) peek at the top word and
        // (ii) loop through the each indice one at a time, replacing each indice, with a->z (not including the word letter currently there)
        //        if the newly created word is valid, copy the stack, push the word to the top, add it to the
        //          usedWords lexicon, and enqueue the newly created stack
        // (iii) as long as the end word isnt found once z is reached the next stack is dequeued and the process repeats
        // (iiii) if the end word is found, its pushed to the current stack (not a new one) and returned. (ending the loop)
        ///(iiiii) -->if there is no ladder between the two words, eventually queue will be empty

    bool done = false;
    while( !done && !library.isEmpty() ){
        current = library.dequeue();
        string topWord = current.peek();

        for(int i =0; i<topWord.size(); i++){ // loop through word indices
            for(int j = 0; j<aToz.size(); j++){ // loop through a-z

                string letter(1,aToz[j]);   // could also use stringstream or push_back(c) methods
                if(letter[1] == topWord.at(i)){ // test to see if replacement letter == current letter
                    continue;
                } else{
                    string newWord = topWord;
                    newWord.replace(i,1,letter);

                    if( dictionary.contains(newWord) && !(usedWords.contains(newWord)) ){ // could also use newword[i] ==letter[1]

                        if(newWord == end){
                            current.push(newWord);
                            failed = false;
                            return current;
                        }
                        else{
                            usedWords.add(newWord);
                            Stack<string> newStack = current;
                            newStack.push(newWord);
                            library.enqueue(newStack);
                        }
                    }else{
                        continue;
                    }
                }
            }
        }
    }
    return current;
}


















