/*
 * File: boggleplay.cpp
 * --------------------------
 * Name: Shlomo Stept
 * Section leader: N/A --> self monitored class, to familiarize myself with data structues in c++
 * This file contains the boggle playOneGame fucntion and its helper functions
 */

#include "lexicon.h"
#include "console.h"
#include <iostream>
#include "simpio.h"
#include "strlib.h"
#include <string>
#include <cctype>
#include "Boggle.h"
#include "bogglegui.h"


using namespace std;


//------------------------------------------------------------------------------------------------------
        // :: Function prototypes ::
//----------------------------------------------------------------------------------------------------------
string getUserBoard();
string getRandBoard();
bool isAlpha(string test);
void humanTurn(Lexicon& dictionary, Boggle& boggle);
void compTurn(Boggle& game);
bool getValidWord(string& Word, Lexicon& dictionary);

void startBoggleGui(Boggle& game);
//----------------------------------------------------------------------------------------------------------



//------------------------------------------------------------------------------------------------------------------
        // :: Primary Function :: playOneGame --> plays one full game of boogle, Human goes first, then computer
//------------------------------------------------------------------------------------------------------------------
void playOneGame(Lexicon& dictionary) {

    // 1.0 -- do you want to generate a random board.
    bool genRandBd = getYesOrNo("Do you want to generate a random board? ");

    // 2.0 -- initalize the bogggle board to the configuration string from user or randomly generate the random board
    string user16letters="";
    if(!genRandBd){
        user16letters = getUserBoard();
    }
    Boggle game = Boggle(dictionary, user16letters);

    ///----------------------------------START---GUI-FUNCTIONALITY-----------------------------------------
    /// 1.0 --> :: Initalize the boggleGui
     startBoggleGui(game);
    ///----------------------------------END-----GUI-FUNCTIONALITY-----------------------------------------

    // 3.0 -- humans turn
    clearConsole();
    humanTurn(dictionary, game);


    // 4.0 -- computers turn
    compTurn(game);

    ///----------------------------------START---GUI-FUNCTIONALITY-----------------------------------------
    /// ---> 4.0 ::: Add a new word, and update the score, for computer
    Set<string> compwords = game.getComputerWords();
    for(string word:compwords){
        BoggleGUI::recordWord(toLowerCase(word), BoggleGUI::COMPUTER);
    }
    BoggleGUI::setScore(game.getScoreComputer(), BoggleGUI::COMPUTER);
    ///----------------------------------END-----GUI-FUNCTIONALITY-----------------------------------------

}


//------------------------------------------------------------------------------------------------------
        // :: Helper Functions ::
//----------------------------------------------------------------------------------------------------------

    // Helper-Function 1 --> getUserBoard():  a funtion that gets a valid 16 letter board from the user.
//----------------------------------------------------------------------------------------------------------
string getUserBoard(){
    string input = getLine("Type the 16 letters to appear on the board: ");
    while(input.size()!=16 || !isAlpha(input)){
        cout << "That is not a valid 16-letter board string. Try again. " << endl;
        input = getLine("Type the 16 letters to appear on the board: ");
    }
    return toUpperCase(input);
}


//------------------------------------------------------------------------------------------------------
        //  Helper-Function 1.5 (helper for 1) --> isAlpha(): returns true if string is entierly composed of alphanumeric charachters
    //----------------------------------------------------------------------------------------------------------

// 1.5 (helper for 1) --> isAlpha() -- returns true if string is entierly composed of alphanumeric charachters
bool isAlpha(string test){
    for(char c:test){
        if(isalpha(c)){
            continue;
        }else{
            return false;
        }
    }
    return true;
}



//------------------------------------------------------------------------------------------------------
        //  Helper-Function 2--> humanTurn(): get a word from the human and check if it passes all the validity checks -except forverifiying if the word can be formed from the cubes on the board
    //----------------------------------------------------------------------------------------------------------
void humanTurn(Lexicon& dictionary, Boggle& boggle){
    bool validWord=true;
    string userWord;

    cout << "It's your turn!" << endl;

    ///----------------------------------START---GUI-FUNCTIONALITY-----------------------------------------
    /// PRINT OUT STATEMENT
    BoggleGUI::setStatusMessage("It's your turn!");
    ///----------------------------------END-----GUI-FUNCTIONALITY-----------------------------------------

    do{
        cout << boggle<<endl;
        cout << "Your words (" << boggle.getHumanWords().size() << "): " << boggle.getHumanWords() << endl;
        cout << "Your score: " << boggle.getScoreHuman() << endl;

        // Get a word from the user and convert it to be lowercase.
        userWord = toLowerCase(getLine("Type a word (or Enter to stop): "));

        // --> TEST !!!!!!!!!!!!!
        //cout<<userWord<<endl;
        //pause(1000);

        if(userWord.size()<1){  // if the user pressed enter --> end while loop
            continue;
        }else{
            clearConsole();

            // --> check 1: is the word a valid word
            validWord = boggle.checkWord(toUpperCase(userWord));

            if(validWord){
                // --> check 2: can the word be formed from the cubes on the boggle board
                if( boggle.humanWordSearch( toUpperCase(userWord) ) ){
                    cout << "You found a new word! \"" << toUpperCase(userWord)<< "\"" <<endl;

                    ///----------------------------------START---GUI-FUNCTIONALITY-----------------------------------------
                    /// ---> 3.0 ::: Add a new word, and update the score, for the human player
                    BoggleGUI::setStatusMessage("You found a new word! \"" +toLowerCase(userWord) + "\"" );
                    BoggleGUI::recordWord(toLowerCase(userWord), BoggleGUI::HUMAN);
                    BoggleGUI::setScore(boggle.getScoreHuman(), BoggleGUI::HUMAN);
                    ///----------------------------------END-----GUI-FUNCTIONALITY-----------------------------------------


                }else{
                    cout << "That word can't be formed on this board."<<endl;

                    ///----------------------------------START---GUI-FUNCTIONALITY-----------------------------------------
                    /// PRINT OUT STATEMENT
                    BoggleGUI::setStatusMessage( "That word can't be formed on this board.");
                    ///----------------------------------END-----GUI-FUNCTIONALITY-----------------------------------------
                }
            }else{
                cout << "You must enter an unfound 4+ letter word from the dictionary." << endl;

                ///----------------------------------START---GUI-FUNCTIONALITY-----------------------------------------
                /// PRINT OUT STATEMENT
                BoggleGUI::setStatusMessage("You must enter an unfound 4+ letter word from the dictionary." );
                ///----------------------------------END-----GUI-FUNCTIONALITY-----------------------------------------
            }
        }
    } while(userWord.size()>1);

}



//------------------------------------------------------------------------------------------------------
        //  Helper-Function 3--> getN: get a valid size for the ngram from the user. to use to build n-gram.
    //----------------------------------------------------------------------------------------------------------
void compTurn(Boggle& game){
     cout << "\nIt's my turn!" << endl;

     // Computers Now looks for all words
     game.computerWordSearch();

     cout << "My words (" << game.getComputerWords().size() << "): " << game.getComputerWords() << endl;
     cout << "My score: " << game.getScoreComputer() << endl;

     if(game.getScoreComputer()>game.getScoreHuman()){
         cout << "Ha ha ha, I destroyed you. Better luck next time, puny human!" << endl;

         ///----------------------------------START---GUI-FUNCTIONALITY-----------------------------------------
         /// PRINT OUT STATEMENT
         BoggleGUI::setStatusMessage( "Ha ha ha, I destroyed you. Better luck next time, puny human!" );
         ///----------------------------------END-----GUI-FUNCTIONALITY-----------------------------------------

     }else{
         cout << "WOW, you defeated me! Congratulations!" << endl;

         ///----------------------------------START---GUI-FUNCTIONALITY-----------------------------------------
         /// PRINT OUT STATEMENT
         BoggleGUI::setStatusMessage("WOW, you defeated me! Congratulations!" );
         ///----------------------------------END-----GUI-FUNCTIONALITY-----------------------------------------

     }

}


///------------------------------------------------------------------------------------------------------
///----------------------------------START---GUI-FUNCTIONALITY-----------------------------------------
///------------------------------------------------------------------------------------------------------
    ///  Helper-Function 4--> startBoggleGui() : initalize the boggle state
///----------------------------------------------------------------------------------------------------------
void startBoggleGui(Boggle& game){
    // 1- initalize the gui
     BoggleGUI::initialize(4, 4);

     // 2- set up the gui to match the boggle
     for( int row =0; row<4; row++){
         for (int col =0; col<4; col++){
             BoggleGUI::labelCube(row,col,game.getLetter(row,col));
         }
     }
}

///----------------------------------END-----GUI-FUNCTIONALITY-----------------------------------------



