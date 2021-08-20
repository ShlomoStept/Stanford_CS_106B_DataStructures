/*
 * File: boggle.cpp
 * --------------------------
 * Name: Shlomo Stept
 * Section leader: N/A --> Self administered course
 * This file contains the implimentation of the boggle class
 */



#include "Boggle.h"
#include "random.h"
#include "shuffle.h"
#include "bogglegui.h"

//----------------------------------------------------------------------------------------------------------
        // Part 1 --> :: Static and Constant variables  ::
//----------------------------------------------------------------------------------------------------------

// letters on all 6 sides of every cube
static string CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

// letters on every cube in 5x5 "Big Boggle" version (extension)
static string BIG_BOGGLE_CUBES[25] = {
   "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
   "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
   "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
   "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
   "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};




//----------------------------------------------------------------------------------------------------------
        // Part 2--> :: Implimentation of Boggle Board Initalization Member-Functions ::
//----------------------------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------------------
    // Start of  ::::  PART 2-A  -->  :: Boggle Constructor ::   <--
//--------------------------------------------------------------------------------------------------
Boggle::Boggle(Lexicon& dictionary, string boardText) {

    this->dictionary = dictionary;
    userScore =0;
    compScore =0;

    marked.resize(4,4,false); // initalizes the grid to have false
    current.resize(4,4);
    if(boardText.size()< 16){
        generateRandBoard();
    } else{
        createBoard(boardText);
    }

}

//---------------------------------------------------------------------------------------------
        // Helper Function 1 --> createBoard: uses the user given string to initalize the Boggle board
    //-------------------------------------------------------------------------------------------------

void Boggle::createBoard(string boardText){
    int iterator =0;
    for(int row = 0 ; row < 4 ; row++){
        for (int col = 0 ; col < 4; col++){
            this->current[row][col] = boardText[iterator++];
        }
    }

}

//--------------------------------------------------------------------------------------------------------------------------
        // Helper Function 2 --> generateRandBoard: randomly creates a boggle board configuration using the cubes predefined
    //----------------------------------------------------------------------------------------------------------------------

void Boggle::generateRandBoard(){
    int iterator =0;
    for( int row = 0; row < 4 ; row++){
        for( int col = 0; col < 4; col++){
            // 1.0 --> pick a random letter from each cube (in the array of cubes)
                    // then save to the current location.
            string temp = CUBES[iterator++]; // selects the current cube and then increments the itterator
            char choice = temp[randomInteger(0,5)];
            this->current[row][col] = choice;
        }
    }

    // 2.0 --> shuffle the whole boogle board (current)
    shuffle(this->current);

}

//--------------------------------------------------------------------------------------------------
    // End of  ::::  PART 2-A  -->  :: Boggle Constructor / initalizer methods ::   <----------------   :::::::
//--------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------------------------
//******************************************************************************************************************************
//------------------------------------------------------------------------------------------------------------------------------




//--------------------------------------------------------------------------------------------------
    // Start of  ::::  PART 2-B  -->  :: Boggle general Member/function implimentions ::   <--
//--------------------------------------------------------------------------------------------------

    //Function 1 --> getLetter(): returns the letter in the row,col location of the Current Boggle setup
//---------------------------------------------------------------------------------------------------
const char Boggle::getLetter(int row, int col) {
    if(row>4 || row<0){
        throw "Error :: Row is out of bounds: " + integerToString(row);
    } else if(col>4 || col<0){
        throw "Error :: Column is out of bounds: " + integerToString(col);
    } else {
        return this->current[row][col];
    }
}


//---------------------------------------------------------------------------------------------
        //Function 2 --> checks if the word is >= 4 charachters, in the dictionary, and not been found already
    //--------------------------------------------------------------------------------------------------

const bool Boggle::checkWord(string word) {
    if(word.size()<1){
        return true;
    }else{
        // note the word passed is lowerCase --> and the words saved in human & comp sets are uppperCase, so it required passing in upperCase versions to the .Contains methods
        if(word.size()<4 || !dictionary.contains(word) || humanWords.contains(toUpperCase(word)) || computerWords.contains(toUpperCase(word)) ){
            return false;
        }else{
            return true;
        }
    }
}

//---------------------------------------------------------------------------------------------
        //Function 3 --> calls the Mandelbrot Set recursive formula, and returns # of runs
    //--------------------------------------------------------------------------------------------------


bool Boggle::humanWordSearch(string word) {
    string sofar = "";
    for(int row =0; row<4; row++){
        for(int col = 0 ; col < 4 ; col++){
            // 1 & 2-> choose, exlore, and unchoose each cube one by one
           bool done = humanWordSearch(word, sofar, row, col);
           if(done){
               return true;
           }
        }
    }
   return false;
}


    //----------------------------------------------------------------------------------------------------------------
        // ::::: 3.5 -> Helper for  humanWordSearch(): Does most of the work. i.e the recursive backtraking and exploration
    //-----------------------------------------------------------------------------------------------------------------
bool Boggle::humanWordSearch(string word, string& sofar, int row, int col){

    if(!isOutofBounds(row,col) && !isMarked(row,col)){

        // 1-Choose A: make the choice
        char choice = this->current[row][col];
        // TODO **********************HIGHLIGHT IN IN THE GUI, with a 100ms pause
        mark(row, col);
        sofar += choice;

        ///----------------------------------START---GUI-FUNCTIONALITY-----------------------------------------
        /// **************HIGHLIGHT --> IN IN THE GUI, with a 100ms pause
        BoggleGUI::setHighlighted(row,col,true);
        BoggleGUI::setAnimationDelay(100);
        ///----------------------------------END-----GUI-FUNCTIONALITY-----------------------------------------


        // 1-Choose  B: basecase- has this choce resulted in finding the word
        if(sofar == word){
            humanWords.add(word);
            userScore += (word.size()-3);
            unmark(row,col);

            ///----------------------------------START---GUI-FUNCTIONALITY-----------------------------------------
            /// **************UN-HIGHLIGHT --> IN IN THE GUI, with a 100ms pause
            BoggleGUI::setHighlighted(row,col,false);
            BoggleGUI::setAnimationDelay(100);
            ///----------------------------------END-----GUI-FUNCTIONALITY-----------------------------------------


            return true;

        } else if(containsPrefix(word,sofar)){

            // 2-Explore --> : exploe all the nighbors
                for(int tempRow = row-1; tempRow < row+2; tempRow++){
                    for (int tempCol =col-1; tempCol< col+2 ; tempCol++){

                        bool done = humanWordSearch(word, sofar,tempRow, tempCol);
                        if(done){
                            ///----------------------------------START---GUI-FUNCTIONALITY-----------------------------------------
                            /// **************UN-HIGHLIGHT --> IN IN THE GUI, with a 100ms pause
                            BoggleGUI::setHighlighted(row,col,false);
                            BoggleGUI::setAnimationDelay(100);
                            ///----------------------------------END-----GUI-FUNCTIONALITY-----------------------------------------

                            unmark(row,col);
                            return true;
                        }
                    }
                }
            }
        // 3-Unchoose: unchoose
        unmark(row,col);
        sofar= sofar.substr(0, sofar.size()-1);
        ///----------------------------------START---GUI-FUNCTIONALITY-----------------------------------------
        /// **************UN-HIGHLIGHT --> IN IN THE GUI, with a 100ms pause
        BoggleGUI::setHighlighted(row,col,false);
        BoggleGUI::setAnimationDelay(100);
        ///----------------------------------END-----GUI-FUNCTIONALITY-----------------------------------------

    }
    return false;
}





//---------------------------------------------------------------------------------------------
        //Function 4 -->computerWordSearch(): runs an exaustive serch of the boogle board, using recursive backtracking
    //--------------------------------------------------------------------------------------------------

Set<string> Boggle::computerWordSearch() {
    string sofar = "";

    // 1 --> Loop through the boggle board, (1) select each cube and explore all the words that can start from it
    for(int row =0; row<4; row++){
        for(int col = 0 ; col < 4 ; col++){

           // 1, 2 & 3-> choose, exlore, and unchoose each cube one by one
           computerWordSearch( sofar, row, col);
        }
    }

    // 2 --> then return the set of all the computers words
    return computerWords;
}


    //-----------------------------------------------------------------------------------------------------------------
        // ::::: 4.5 --> Helper for humanWordSearch() : Does most of the work. i.e the recursive backtraking and exploration
    //-----------------------------------------------------------------------------------------------------------------
void Boggle::computerWordSearch( string& sofar, int row,  int col){

    if(!isOutofBounds(row, col) && !isMarked(row,col)){

        // 1-Choose A: make the choice
        char choice = this->current[row][col];
        mark(row, col);
        sofar += choice;

        // 1-Choose  B: basecase- has this choce resulted in finding a valid word that was not found before
        if(checkWord( toLowerCase(sofar) )){

            computerWords.add(sofar);
            compScore += sofar.size() - 3;
        }

        // 2-Explore --> : explore all the nighbors
        if(dictionary.containsPrefix( toLowerCase(sofar) )) {
            for(int tempRow = row-1; tempRow < row+2; tempRow++){
                for (int tempCol =col-1; tempCol< col+2 ; tempCol++){
                    computerWordSearch(sofar,tempRow, tempCol);
                }
            }
        }

        // 3-Unchoose: unchoose
        unmark(row,col);
        sofar= sofar.substr(0, sofar.size()-1);
    }
}





//---------------------------------------------------------------------------------------------
        //Function 5--> getScoreHuman(): returns the humans current
    //--------------------------------------------------------------------------------------------------

const int Boggle::getScoreHuman() {
    return userScore;
}

//---------------------------------------------------------------------------------------------
        //Function 6 --> getScoreComputer(): returns the computers score
    //--------------------------------------------------------------------------------------------------

const int Boggle::getScoreComputer() {
    return compScore;
}

//---------------------------------------------------------------------------------------------
        //Function 7 --> getHumanWords(): returns the valid word the human entered
    //--------------------------------------------------------------------------------------------------

const Set<string> Boggle::getHumanWords(){
    return humanWords;
}

//---------------------------------------------------------------------------------------------
        //Function 8 --> getComputerWords(): returns the computers words
    //--------------------------------------------------------------------------------------------------

const Set<string> Boggle::getComputerWords(){
    return computerWords;
}


//---------------------------------------------------------------------------------------------
        //Function 9 --> isMarked(): returns true is the location(row,col) is marked
    //--------------------------------------------------------------------------------------------------
const bool Boggle::isMarked(int row, int col){
    return marked[row][col];
}


//---------------------------------------------------------------------------------------------
        //Function 10 --> mark(): marks the location[row][col]
    //--------------------------------------------------------------------------------------------------

void Boggle::mark(int row, int col){
    marked[row][col] = true;
}


//---------------------------------------------------------------------------------------------
        //Function 11 --> mark(): unmarks the location[row][col]
    //--------------------------------------------------------------------------------------------------

void Boggle::unmark(int row, int col){
    marked[row][col] = false;
}


//---------------------------------------------------------------------------------------------
        //Function 12 --> operator<< : allows for the printing of the the boggle boards current state
    //--------------------------------------------------------------------------------------------------

ostream& operator<<(ostream& out, Boggle& boggle) {
    // ---> I did not use the out<< code becuase it resulted in printing double of everything
    // I have one question regarding this: why does the << work even when i dont add the string to "out", and the << is chained
    for(int row = 0; row < 4; row++){
        for (int col =0; col < 4; col++){
            if(col+1 ==4){
                cout << boggle.current[row][col]<<endl;
            } else {
                cout << boggle.current[row][col];
            }
        }
    }
    return out;
}


//--------------------------------------------------------------------------------------------------
    // End of  ::::  PART 2-B  -->  :: Boggle Member function implimentions ::   <----------  ::::::
//--------------------------------------------------------------------------------------------------





//---------------------------------------------------------------------------------------------------------
    // :::::: --> UseFull Helper-Methods that are used Internally in several member/functions <--- ::::::
//---------------------------------------------------------------------------------------------------------

    // helper-function 1: isOutofBounds() : returns true if the row or col is out of bounds
//---------------------------------------------------------
const bool Boggle::isOutofBounds(int row, int col){
    if(col > 3  || col < 0 || row > 3 || row < 0 ){
        return true;
    }else{
        return false;
    }
}
// helper function 2: containsPrefix(): check to se if the parent contains the prefix of the child string
//--------------------------------------------------
const bool Boggle::containsPrefix(string parent, string child){
    for(int start =0; start < child.size(); start++ ){
        if(parent[start]==child[start]){
            continue;
        }else{
            return false;
        }
    }
    return true;
}





