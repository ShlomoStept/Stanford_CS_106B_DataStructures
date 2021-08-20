/*
 * File: boggle.h
 * --------------------------
 * Name: Shlomo Stept
 * Section leader: N/A --> Self administered course
 * This file contains the header file for the boggle class
 */


#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>
#include "lexicon.h"
#include "hashset.h"
#include "grid.h"

using namespace std;


//----------------------------------------------------------------------------------------------------------
        // Boggle Class Definition
//----------------------------------------------------------------------------------------------------------

class Boggle {

//----------------------------------------------------------------------------------------------------------
                // PART A --> :: Public Member functions --> setters/modifiers ::
//----------------------------------------------------------------------------------------------------------
public:
    Boggle(Lexicon& dictionary, string boardText = "");
    const bool checkWord(string word);
    bool humanWordSearch(string word);
    Set<string> computerWordSearch();
    void generateRandBoard();
    void createBoard(string setupstring);


//----------------------------------------------------------------------------------------------------------
                // PART B --> :: getter member functions ::
//----------------------------------------------------------------------------------------------------------
    const int getScoreHuman();
    const int getScoreComputer();
    const Set<string> getHumanWords();
    const Set<string> getComputerWords();
    const char getLetter(int row, int col);
    friend ostream& operator<<(ostream& out, Boggle& boggle);


//----------------------------------------------------------------------------------------------------------
                // PART C --> :: Private Data-Fields and Member-Functions ::
//----------------------------------------------------------------------------------------------------------
private:
    Lexicon dictionary;
    Set<string> humanWords;
    Set<string> computerWords;
    Grid<char> current;
    Grid<bool> marked;
    int userScore;
    int compScore;

        // ::  methods/functions only used by the boggle class internally
    //-----------------------------------------------------------------------
        // Core helper-functions
    bool humanWordSearch(string word, string& sofar, int row, int col);
    void computerWordSearch(string& sofar, int row, int col);

    // Marking functions
    const bool isMarked(int row, int col);
    void mark(int row, int col);
    void unmark(int row, int col);

    // Extra Internally used helper functions
    const bool isOutofBounds(int row, int col);
    const bool containsPrefix(string parent, string child);





};

#endif // _boggle_h
