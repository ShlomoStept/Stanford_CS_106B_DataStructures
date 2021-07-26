//  Shlomo Stept
//  CS106B - self monitored class, to familiarize myself with data structues in c++
//  Date: 7/26/21
//  Homework 2: Serafini --> Part B: Ngrams

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <iomanip>
#include <cctype>
#include <random>
#include <chrono>
#include "simpio.h"
#include "console.h"
#include "vector.h"
#include "map.h"

using namespace std;



//------------------------------------------------------------------------------------------------------
        // :: Function prototypes ::
//----------------------------------------------------------------------------------------------------------
void Welcome();
bool getFile(fstream &);
int getN();
bool buildNGram(fstream & infile, Map<Vector<string>,Vector<string>> & ngram, int & ngsize );
int numWordsGenerate(int ngsize);
void generateWords(Map<Vector<string>,Vector<string>> & ngram, int );
int getRandInt(int lowbound, int highbound);



//------------------------------------------------------------------------------------------------------
        // :: Main ::
//----------------------------------------------------------------------------------------------------------

int main() {
        // 1. welcome the user and introduce how the program works
        Welcome();

        //2. get a valide file
        fstream infile;
        getFile(infile);

        //3. get a valid value for n and build ngram
        int ngsize =0;
        Map<Vector<string>,Vector<string>> ngram;
        buildNGram(infile, ngram, ngsize);

        //4. generate the random collection of words
        generateWords(ngram, ngsize);

        cout << "Exiting." << endl;
        return 0;
}




//------------------------------------------------------------------------------------------------------
        // :: Functions ::
//----------------------------------------------------------------------------------------------------------


        //  Function 1 --> Welcome: introduce how the programs works to the user
//----------------------------------------------------------------------------------------------------------
void Welcome(){
    cout << "Welcome to CS 106B Random Writer ('N-Grams')." << endl;
    cout << "This program makes random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups" << endl;
    cout << "of words, and I'll create random text for you." << endl;

}



//------------------------------------------------------------------------------------------------------
        //  Function 2 --> getFile: get a valid file from the user. to use to build n-gram.
    //----------------------------------------------------------------------------------------------------------
bool getFile(fstream & infile){
    // A --> Get the correct file.
    cout << "Input file name? " ;
    string filename;
    getline(cin, filename);
    infile.open(filename);

    while(!infile.is_open()){
        cout<< "Unable to open that file.  Try again." << endl;
        cout << "Input file name? " ;
        getline(cin, filename);
        infile.open(filename);
    }

    return true;
}



//------------------------------------------------------------------------------------------------------
        //  Function 3 --> buildNGram: get a valid size for the Ngram and then build the ngram for that size.
    //----------------------------------------------------------------------------------------------------------
bool buildNGram(fstream & infile,Map<Vector<string>,Vector<string>> & ngram, int & ngsize ){
    ngsize = getN();

    Vector<string> window, start;
    string word;

    //1. initalize the size of the starting window --> keep the start to use to wrap around fully
    for( int i = 0; i< ngsize-1; i++){
        infile >> word;
        start.add(word);
    }

    //2. map the window to the next word (if ngram already exists add the word to existing list of words)
    window = start;
    while(infile >> word){
        Vector<string> temp;
        // (i) check to see if a vector matching the current wndow already exists, of if a new one should be created
        if(ngram.containsKey(window)){
            temp = ngram[window];
            temp.add(word);
        }else{
        temp.add(word);
        }

        ngram[window]=temp;
        //cout << window<< " : "<<temp <<endl;
        // (ii) the wondow is shifted by one word ( first word is deleted, and the next word on line is added --> this is always the word just used to create the newest mapping)
        window.remove(0);
        window.add(word);
    }

    //3. wrap aound the last values, going through the same evualtion process used above
    for(int i=0; i < ngsize -1; i++){
        // (i) map the window to the words in the start vector ( ie. connect the last __(ngsize-1) words to the first (ngsize-1) words
        //   -- this way the last (ngsize-1) words, get mapped in the same way all the other words do.
        word = start[i];
        Vector<string> temp;
        if(ngram.containsKey(window)){
            temp = ngram[window];
            temp.add(word);
        }else{
        temp.add(word);
        }
        ngram[window]=temp;
        //cout << window<< " : "<<temp <<endl;
        window.remove(0);
        window.add(start[i]);
    }

    infile.close();
    return true; // change this it serves no purpose to send a bool back
}



//------------------------------------------------------------------------------------------------------
        //  Helper-Function 3.5 --> getN: get a valid size for the ngram from the user. to use to build n-gram.
    //----------------------------------------------------------------------------------------------------------
int getN(){

    // 1.0 - first ask for the value of N, then if its not an integer or its < 2 rerun the loop and output the specific issue
    int ngsize = 0;
    bool nvalid = true, intvalid =true;
    do{
        // (i) - called every time the loop runs
        cout << "Value of N? ";

        string N;
        getline(cin, N);
        stringstream ss(N);

        // (ii) - check if user entered an integer, ifnot inform user of issue
        if(ss >> ngsize){
            // (iii) - check to make sure the user entered a value greater that 2, if not output issue
            if(ngsize < 2){
                nvalid = false;
                cout <<"N must be 2 or greater." << endl;
            }else{
                intvalid =true;
                nvalid =true;
            }
         }else{
            intvalid = false;
            cout<< "Illegal integer format. Try again." << endl;
        }
    }while(!intvalid || !nvalid);

    return ngsize;
}



//------------------------------------------------------------------------------------------------------
        //  Function 4 -->
    //----------------------------------------------------------------------------------------------------------
void generateWords(Map<Vector<string>,Vector<string>> & ngram, int ngsize){

    bool again = true;
    Vector<string> window;

    do{
        int size = numWordsGenerate(ngsize);
        if(size == 0){
            again = false;
            break;
        }else{

            // (i) generate random start point, by getting a random number and then running a foreach loop untill that index
            int start = getRandInt(1,ngram.size());

            for(Vector<string> skey:ngram){
                if(start == 0){
                    window = skey;
                    break; // break out of for loop
                }
                start --;
            }
            // (ii) print out the starting window
            cout << "... " ;
            for(string word:window){
                cout << word <<" ";
                size--;
            }


            //(iii) randomly choose a next word from the possible values for the current window, for the size the user specifies
            Vector<string> choose;
            for(int i = 0; i< size; i++){
                choose = ngram.get(window);

                string next;
                if(choose.size()==1){
                    next = choose[0];
                    cout << next <<" ";

                    window.remove(0);
                    window.add(next);
                }else{
                    int start = getRandInt(0, choose.size()-1);
                    next = choose[start];
                    cout << next <<" ";

                    window.remove(0);
                    window.add(next);
                }
            }
            cout << "..."<<endl;
        }
    }while(again);
}



//------------------------------------------------------------------------------------------------------
        //  Helper-Function 4.5 numWordsGenerate
    //----------------------------------------------------------------------------------------------------------
int numWordsGenerate(int ngsize){

    // 1.0 --> get a VALID number of random words to generate from user, and make sure it is at least the size of the Ngram groups

    int generateNum;
    bool valid = false;
    cout << "\n";

    while(!valid){
        cout << "# of random words to generate (0 to quit)? " ;
        string num;
        getline(cin,num);

        stringstream ss(num);

        if(ss >> generateNum){
            if(generateNum==0){
                return generateNum;
            }else if(generateNum<ngsize){
                cout << "Must be at least " << ngsize << " words. \n" << endl;
            }else{
                valid = true;
            }
        }else{
            cout << "Illegal integer format. Try again." << endl;
        }
    }
        return generateNum;
}



//------------------------------------------------------------------------------------------------------
        //  Helper-Function 4.75 -->getRandint: get a random integer in the range of [lowbound -> highbound] inclusize
    //----------------------------------------------------------------------------------------------------------
int getRandInt(int lowbound, int highbound){
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    default_random_engine r(seed);
    uniform_int_distribution<int> d(lowbound, highbound);
    int test = d(r); // ERROR CHECK :: Sometimes this seems to produce a spillover.
    return test;
}











