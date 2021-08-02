/*
 * File: grammarsolver.cpp
 * --------------------------
 * Name: Shlomo Stept
 * Section leader: N/A --> self monitored class, to familiarize myself with data structues in c++
 * Extra Credit: Robust grammer solver implimented in accordance with the "extra feature" specs
 * This file contains grammar generating code for CS106B.
 */

#include "grammarsolver.h"
#include "map.h"
#include "vector.h"
#include "strlib.h"
#include "filelib.h"
#include "random.h"
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;


//------------------------------------------------------------------------------------------------------
        // :: Function prototypes ::
//----------------------------------------------------------------------------------------------------------
void mapGrammer( Map<string, Vector<string>> &gm, istream &input);
string generateExpression(Map<string, Vector<string>> &gm, string symbol);
string returnNoSpaces(string start);

//------------------------------------------------------------------------------------------------------------------
        // :: Primary Function :: grammarGenerate --> Generates grammar for a given symbol a certain number of times
//------------------------------------------------------------------------------------------------------------------

/**
 * Generates grammar for a given symbol a certain number of times given
 * a BNF input file.
 *
 * This will be called by grammarmain.cpp.
 *
 * @param input - Input stream of BNF file.
 * @param symbol - Symbol to generate
 * @param times - Number of times grammar is generated
 * @return Vector of strings of size times with random generations of symbol
 */
Vector<string> grammarGenerate(istream& input, string symbol, int times) {

    // Part A: create the grammer map from the given file
    Map<string, Vector<string>> grammerMap;
    mapGrammer(grammerMap, input);

    // Part B: generate the vector contianing the expression
    Vector<string> endExp;
    for (int i = 0; i<times; i++){
        endExp += generateExpression(grammerMap, symbol);

    }

    // Part C: print the vector
    for(int i=0; i <endExp.size(); i++){
        cout << right << setw(2) <<(i+1) << ":" << endExp[i] <<endl;
    }


    return endExp;           // This is only here so it will compile
}



//------------------------------------------------------------------------------------------------------
        // :: Helper Functions ::
//----------------------------------------------------------------------------------------------------------


        // Helper-Function 1 --> mapGrammer: generate the grammer map given teh input file
//----------------------------------------------------------------------------------------------------------

void mapGrammer( Map<string, Vector<string>> &gm, istream &input){
    string line;
    while(getline(input, line)){
        // ::Bonus function == trim the values for each unique value or group fo values
        line = returnNoSpaces(line);

        string key = trim(line.substr(0,line.find("::=")));
        //cout<<gm<<endl;
       // cout<<key<<endl;

        // error check 1--> is there more than one line for a single key
        if(gm.containsKey(key))
            throw("Error: "+key+" contains more than one line for the same non-terminal");


        if(line.find("|") == string::npos){
            gm[key] += trim(line.substr(line.find("=")+1));
        }else{
            Vector<string> value = stringSplit(line.substr(line.find("=")+1), "|");
            gm[key] +=value;
        }
    }
}

//------------------------------------------------------------------------------------------------------
        //  Helper-Function 2--> getN: get a valid size for the ngram from the user. to use to build n-gram.
    //----------------------------------------------------------------------------------------------------------

string generateExpression(Map<string, Vector<string>> &gm, string symbol){
    //error check 2--> is the symbol parameter empty
    if(trim(symbol) =="")
        throw("Error: Symbol passed is empty/or null");

    string expansion;

    if(!gm.containsKey(symbol)){
        return " " + symbol;
    }else{
        int random = randomInteger( 0 , (gm[symbol].size()-1) );
        string S = gm[symbol].get(random);
        Vector<string> expressions = stringSplit(S," ");

        for(string e: expressions){

            expansion += generateExpression(gm,e);
        }
    }
    return expansion;
}

//------------------------------------------------------------------------------------------------------
        //  Extra credit:" extra functionality functions::   1.0 --> returnNoSpaces
    //----------------------------------------------------------------------------------------------------------
string returnNoSpaces(string start){
    istringstream ss(start);
    string s1, sf;
    while(ss>>s1){
        sf+=s1;
    }
    int next=0;
    while(sf.find("><")!= string::npos){
          next = sf.find("><");
          sf.insert(next+1," ");
    }
    return sf;
}

