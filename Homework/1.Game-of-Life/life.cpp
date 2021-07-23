//  Shlomo Stept
//  CS106B - self monitored class, to familiarize myself with data structues in c++
//  Date: 7/16/21
//  Homework 1: Game of Life

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "grid.h"
#include "gwindow.h"
#include "simpio.h"
#include "strlib.h"
#include "lifegui.h"

// initalize namespace
using namespace std;

//------------------------------------------------------------------------------------------------------
        // :: Function prototypes ::
//------------------------------------------------------------------------------------------------------
void Welcome();
void getFile(fstream &);
bool WrapAround();
void setGrid(Grid<char> &, fstream &infile);
void printGrid(Grid<char> &);
char getSelection();
int ncountWrap(Grid<char> &lifeGrid, int i, int j);
int ncountNoWrap(Grid<char> &lifeGrid, int i, int j);
Grid<char> tick(Grid<char> &, bool wraparound);
void printLifeGui(Grid<char> & lifegrid, LifeGUI & lifegui);

//------------------------------------------------------------------------------------------------------
        // :: Main ::
//----------------------------------------------------------------------------------------------------------
int main() {

    // 1.0 - Welcome the use
    Welcome();

    // 2.0 - Get the input file for the starting setup for game of life
    fstream infile;
    getFile(infile);

    // 3.0 Ask the user if they want the game-of-life to wrap around the grid
    bool wrap =WrapAround();

    // 4.0 - Initalize the starting grid
    Grid<char> lifeGrid;
    setGrid(lifeGrid, infile);

    // 5.0 - Print the initial grid
// Method 1: Standard text printout
    printGrid(lifeGrid);


// -----------------------------------------------------------------------------------------------
// :: Alternative Printout Method :: Not Used ---> Visually confusing/unappealing
// -------------------------------------------------------------------------------- 
// Method2: lifegui console printout
    /*LifeGUI lifegui;
    lifegui.resize(lifeGrid.numRows(), lifeGrid.numCols());
    printLifeGui( lifeGrid, lifegui);
    */
   // ---------------------------------------------------------------------------------------------
   // ---------------------------------------------------------------------------------------------

    // 6.0 - Prompt the user for next action, then tick animate or quit
    char selection;
    do{
        selection = getSelection();
        switch(selection){
            case 'A': {
                int frames = getInteger("How many frames?");
                cout<< "("<< integerToString(frames)<< " new generations are shown, with screan clear and 50ms pause before each)" <<endl;
                for(int i =0; i< frames; i++){
                    lifeGrid= tick(lifeGrid, wrap);
                        // Method 1: Standard text printout
                    clearConsole();
                    printGrid(lifeGrid);
                    pause(50);
                        // Method2: lifegui console printout
                    //printLifeGui(lifeGrid, lifegui);
                }
                break;
            }
            // animate();
            case 'T':{
                lifeGrid= tick(lifeGrid, wrap);
                clearConsole();
                printGrid(lifeGrid);
            }
        }
    }while(selection != 'Q');

    cout << "Have a nice Life!" << endl;
    return 0;
}



//---------------------------------------------------------------------------------------------
// :: Functions :::
//---------------------------------------------------------------------------------------------

        //Function 1 --> Welcome to the game of life
    //---------------------------------------------------------------------------------------------------
void Welcome(){
    cout << "Welcome to the CS106B Game of Life,"<<endl;
    cout << "A simulation of the lifecycle of a bacteria colony."<<endl;
    cout << "Cells (X) live and die by the folowoing rules:" << endl;
    cout << " - A cell with 1 or fewer neighbors dies." << endl;
    cout << " - Locations with 2 neighbors remain stable." << endl;
    cout << " - Locations with three neighbors will create life." << endl;
    cout << " - A cell wiith 4 or more neighbors will die.\n" << endl;
}

//---------------------------------------------------------------------------------------------
        //Function 2 --> get the filename from the user
    //--------------------------------------------------------------------------------------------------
void getFile(fstream & infile){
    bool good = true;
    do{
        if(good == false)
            cout<< "\nUnable to open that file.  Try again. "<<endl;

        cout << "Grid input file name: " ;
        string fname;
        getline(cin, fname);
        infile.open(fname);

        if(!infile.is_open())
            good = false;
        else
            good = true;

    }while(!good);
}


//---------------------------------------------------------------------------------------------
        //Function 3 --> prompt user to choose to allow wraparound
    //--------------------------------------------------------------------------------------------------
bool WrapAround(){
    bool wraparound= false;
    char answer;
    do{
        cout << "Should the simulation wraparound the grid (y/n): ";
        string line;
        getline(cin,line);
        // TO DO --> CHECK FOR VALID ENTRY
        answer = toupper(line[0]);  // toupper is only available by char
        if(answer == 'Y')
            wraparound = true;
        }while(answer != 'Y' && answer != 'N');
    return wraparound;
}

//---------------------------------------------------------------------------------------------
        //  Function 4 --> use the file to set the initial grid in main **
    //---------------------------------------------------------------------------------------------
void setGrid(Grid<char> & lifeGrid, fstream & inFile){
    int rows, cols;
    inFile >> rows >> cols >> ws;
    lifeGrid.resize(rows, cols);

    for(int i = 0; i < rows; i++){
        string line;
        getline(inFile, line);
        for( int j = 0; j < cols; j++){
            lifeGrid[i][j] =line[j];
        }
    }

}


//---------------------------------------------------------------------------------------------
        // Function 5 --> Print The grid
    //--------------------------------------------------------------------------------------------------
void printGrid(Grid<char> & lifeGrid){
    for(int i = 0; i< lifeGrid.numRows(); i++){
        string line;
        for( int j =0; j < lifeGrid.numCols(); j++){
            line += lifeGrid[i][j];
        }
        cout << line << endl;
    }
}


//---------------------------------------------------------------------------------------------
        // Function 6 --> get the selection
    //--------------------------------------------------------------------------------------------------
char getSelection(){
    bool first = true;
    char selection;
    do{
        if(!first){
            cout<< "\nInvalid choice; please try again."<<endl;
            //cin.ignore();
        }
        else
            cout <<endl;
        cout << "a)nimate, t)ick, q)uit? : " ;
        //system("pause");
        string line;
        //cin.ignore();
        getline(cin, line);
        // --> THE ERRROR HAS TO DO WITH CHAR VS STRING FIX THIS PROBLEM!!!!!!!!!!!!!!!

        selection = toupper(line[0]);
        first = false;
    }while(selection != 'Q' && selection != 'A' && selection != 'T');

    return selection;
}


//---------------------------------------------------------------------------------------------
        // Function 7 --> Tick forward one generation 
    //--------------------------------------------------------------------------------------------------
Grid<char> tick(Grid<char> & lifeGrid , bool wrap){
    Grid<char> newGrid(lifeGrid.numRows(),lifeGrid.numCols());
    for(int i =0; i< lifeGrid.numRows(); i++){
        for(int j =0; j< lifeGrid.numCols(); j++){
            int neighbors;
            if(wrap)
                neighbors = ncountWrap(lifeGrid, i, j);
            else
                neighbors = ncountNoWrap(lifeGrid, i, j);
            switch(neighbors){
            case 0:
            case 1:
                newGrid[i][j] ='-';
                break;
            case 2:
                if(lifeGrid[i][j] == 'X')
                    newGrid[i][j] ='X';
                else
                    newGrid[i][j] ='-';
                break;
            case 3:
                newGrid[i][j] ='X';
                break;
            default:
                newGrid[i][j] ='-';
            }
        }
    }
    return newGrid;
}


//---------------------------------------------------------------------------------------------
        //Function 8 -->  ncountWrap
    //--------------------------------------------------------------------------------------------------
int ncountWrap(Grid<char> &lifeGrid, int i, int j ){
    int count=0;
    for( int r = i-1; r< i+2; r++){
        for( int c = j-1; c<j+2; c++){
            // the better / other way to do what folow is
            // say the number of rows was 5 and the number of cols is five
            // if r<0 or r>4 then add 5 and then mod 5 it this will get you to the correct wraparound spot
            // ((5 + 5) % 5, (-1 + 5) % 5) = (10 % 5, 4 % 5) = (0, 4)
            int tempr, tempc;
            if(r < 0)
                tempr = lifeGrid.numRows() -1;
            else if(r > lifeGrid.numRows() - 1)
                tempr = 0;
            else
                tempr =r;
            if(c < 0)
                tempc = lifeGrid.numCols() -1;
            else if(c > lifeGrid.numCols()-1)
                tempc = 0;
            else
                tempc =c;

            if(r == i && c ==j)
                continue;
            if(lifeGrid[tempr][tempc] == 'X')
                count++;
        }
    }
    return count;
}


//---------------------------------------------------------------------------------------------
        // Function 9 --> ncountNoWrap
    //--------------------------------------------------------------------------------------------------
int ncountNoWrap(Grid<char> &lifeGrid, int i, int j){
    int count=0;
    for( int r = i-1; r< i+2; r++){
        if(r<0 || r> lifeGrid.numRows()-1)
            continue;
        for( int c = j-1; c<j+2; c++){
            if(c<0 || c> lifeGrid.numCols()-1)
                continue;
            else if(r == i && c ==j)
                continue;
            if(lifeGrid[r][c] == 'X')
                count++;
        }
    }
    return count;
}

//---------------------------------------------------------------------------------------------
        // Function 10 --> printLifeGui: a printout using the life gui class stanford create - I dont like it
   //--------------------------------------------------------------------------------------------------
void printLifeGui(Grid<char> & lifegrid, LifeGUI & lifegui){
    for(int i =0; i< lifegrid.numRows(); i++){
        for(int j =0; j< lifegrid.numCols(); j++){
            bool alive = false;
            if(lifegrid[i][j] == 'X')
                alive = true;
            lifegui.drawCell(i,j,alive);
        }
    }
}

 //--------------------------------------------------------------------------------------------------
 //  :: End of Functions
 //--------------------------------------------------------------------------------------------------














