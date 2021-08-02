/*
 * File: fractals.cpp
 * --------------------------
 * Name: Shlomo Stept
 * Section leader: N/A --> Self administered course
 * This file contains fractal problems for CS106B.
 */

//#include "console.h"
#include "fractals.h"
#include <cmath>

using namespace std;

const int LEAF_COLOR = 0x2e8b57;   /* Color of all leaves of recursive tree (level 1) */
const int BRANCH_COLOR = 0x8b7765; /* Color of all branches of recursive tree (level >=2) */



//----------------------------------------------------------------------------------------------------------
        // PART A --> :: Sierpinski triangle ::
//----------------------------------------------------------------------------------------------------------

/**
 * Draws a Sierpinski triangle of the specified size and order, placing its
 * top-left corner at position (x, y).
 *
 * This will be called by fractalgui.cpp.
 *
 * @param gw - The window in which to draw the Sierpinski triangle.
 * @param x - The x coordinate of the top-left corner of the triangle.
 * @param y - The y coordinate of the top-left corner of the triangle.
 * @param size - The length of one side of the triangle.
 * @param order - The order of the fractal.
 */
void drawSierpinskiTriangle(GWindow& gw, double x, double y, double size, int order) {
    if(x<0)
        throw("Error: X-Coordinate cannot be negative");
    if(y<0)
        throw("Error: X-Coordinate cannot be negative");
    if(size<0)
        throw("Error: X-Coordinate cannot be negative");
    if(order<0)
        throw("Error: X-Coordinate cannot be negative");

    // key Measurments
    double height = (size * sqrt(3.0))/2.0;

    // --------------------------------------------------------------------------
    // this is a list of key measurements --> using appropriate names simplified the processs -> but I changed back afterwards -- no need for extra varaibles in each stack call
    // --------------------------------------------------------------------------
    //double ybottom = y+height;
    //double ytop = y;
    //double ymiddle = y+(height/2); // THIS IS A QQ

    //double xmin = x;
    //double xmax = x+size;
    //double xmiddle = x+(size/2);// THIS IS A QQ
    //double x1of4 = x + size/4; // THIS IS A QQ
    // --------------------------------------------------------------------------

    if(order == 1){
        //top
        gw.drawLine(x , y , x+size , y);
        //left
        gw.drawLine(x ,y , x+(size/2), y+height);
        //right
        gw.drawLine(x+size, y, x+(size/2), y+height);
    }else if(order>1){
        //top left
        //int t = order;
        drawSierpinskiTriangle( gw, x, y, (size)/2, order-1);

        // top right
        drawSierpinskiTriangle( gw, x+(size/2), y, (size)/2, order-1);

        //bottom middle
        drawSierpinskiTriangle(gw, x + size/4, y+(height/2), (size)/2, order-1);
    }

}



//----------------------------------------------------------------------------------------------------------
        // PART B --> :: Mandelbrot Set ::
//----------------------------------------------------------------------------------------------------------

    //Function 1 --> Draws a Mandelbrot Set, using recursion
        // --> note: errors exist in the x/y/window.width/window.height/ infomation intake in the GUI component of the program given to students
//---------------------------------------------------------------------------------------------------
/**
 * Draws a Mandelbrot Set in the graphical window give, with maxIterations
 * (size in GUI) and in a given color (zero for palette)
 *
 * This will be called by fractalgui.cpp.
 *
 * @param gw - The window in which to draw the Mandelbrot set.
 * @param minX - left-most column of grid
 * @param incX - increment value of columns of grid
 * @param minY - top-most row of grid
 * @param incY - increment value of rows of grid
 * @param maxIterations - The maximum number of iterations to run recursive step
 * @param color - The color of the fractal; zero if palette is to be used
 */
void mandelbrotSet(GWindow& gw, double minX, double incX,
                   double minY, double incY, int maxIterations, int color) {

    // Creates palette of colors
    Vector<int> palette = setPalette();

    int width = gw.getCanvasWidth();
    int height = gw.getCanvasHeight();
    GBufferedImage image(width,height,0xffffff);
    gw.add(&image);
    Grid<int> pixels = image.toGrid(); // Convert image to grid

    // 1.0 --> through the grid and if its part of the set set its color, to color or result%pallete.size(), if color !=0 which makes no sence to me--> why do we check if color !=0??
    //  if not set to a diff color
    for( int y = 0; y < width; y++){
        for( int x = 0; x < height; x++){           // i thought it would be y<height and x<width

            // There seems to be a GUI based error when intaking the X and Y window height components
            // beacuse  for some reason y maps to width, x maps to height and the, MinY &y are swaped with the minX and x values , BUT the incX and incY are fine
            // something is not right but when all these swaps are made the program works as intended
            Complex point = Complex ( minY + (y * incX), (minX+ (x*incY) ));

            int result = mandelbrotSetIterations(point, maxIterations);
            //originally i didnt use the if color !=0 check but i dont think it changed much by adding it
            if(color != 0){
                if(result == maxIterations){
                     pixels[x][y] =  color;
                }else{
                    // the GUI does not print/output clear white backgrounds only colored and the pallete option does not work --> the GUI component given to utilize seems to have some bugs!!
                    pixels[x][y] = palette[result % palette.size()];
                }
            }
        }
    }
    image.fromGrid(pixels); // Converts and puts the grid back into the image
}


//---------------------------------------------------------------------------------------------
        //Function 2.1 --> calls the Mandelbrot Set recursive formula, and returns # of runs
    //--------------------------------------------------------------------------------------------------
/**
 * Runs the Mandelbrot Set recursive formula on complex number c a maximum
 * of maxIterations times.
 *
 * This will be called by you. Think about how this fits with the other two functions.
 *
 * @param c - Complex number to use for recursive formula.
 * @param maxIterations - The maximum number of iterations to run recursive step
 * @return number of iterations needed to determine if c is unbounded
 */
int mandelbrotSetIterations(Complex c, int maxIterations) {
    // originally messed up by having (c, 0 , maxinterations) which clearly is wrong
    int runs = mandelbrotSetIterations(Complex(0,0), c, maxIterations);
    if(runs == 0){
        return maxIterations;
    } else {
        return maxIterations-runs;
    }
}

//---------------------------------------------------------------------------------------------
        //Function 2.2 --> Carries out the Mandelbrot Set recursive formula
    //--------------------------------------------------------------------------------------------------
/**
 * An iteration of the Mandelbrot Set recursive formula with given values z and c, to
 * run for a maximum of maxIterations.
 *
 * This will be called by you. Think about how this fits with the other two functions.
 *
 * @param z - Complex number for a given number of iterations
 * @param c - Complex number to use for recursive formula.
 * @param remainingIterations - The remaining number of iterations to run recursive step
 * @return number of iterations needed to determine if c is unbounded
 */
int mandelbrotSetIterations(Complex z, Complex c, int remainingIterations) {
    if(z.abs() > 4 || remainingIterations == 0){
        return remainingIterations;
    }else{
        //cout<< (z*z) + c <<endl;
        return mandelbrotSetIterations( ((z*z) + c), c , remainingIterations-1);
    }
}




//----------------------------------------------------------------------------------------------------------
        // PART C --> :: recursive tree ::
//----------------------------------------------------------------------------------------------------------


/**
 * Draws a recursive tree fractal image of the specified size and order,
 * placing the bounding box's top-left corner at position (x,y).
 *
 * This will be called by fractalgui.cpp.
 *
 * @param gw - The window in which to draw the recursive tree.
 * @param x - The x coordinate of the top-left corner of the bounding box.
 * @param y - The y coordinate of the top-left corner of the bounding box.
 * @param size - The length of one side of the bounding box.
 * @param order - The order of the fractal.
 */
void drawTree(GWindow& gw, double x, double y, double size, int order) {
    // TODO: write this function
}





//----------------------------------------------------------------------------------------------------------
        // --> :: Helper Function ::
//----------------------------------------------------------------------------------------------------------



// Helper function to set the palette
Vector<int> setPalette() {
    Vector<int> colors;

    // Feel free to replace with any palette.
    // You can find palettes at:
    // http://www.colourlovers.com/palettes

    // Example palettes:
    // http://www.colourlovers.com/palette/4480793/in_the_middle
    // string colorSt = "#A0B965,#908F84,#BF3C43,#9D8E70,#C9BE91,#A0B965,#908F84,#BF3C43";

    // http://www.colourlovers.com/palette/4480786/Classy_Glass
     string colorSt = "#9AB0E9,#C47624,#25269A,#B72202,#00002E,#9AB0E9,#C47624,#25269A";

    // The following is the "Hope" palette:
    // http://www.colourlovers.com/palette/524048/Hope
    //string colorSt =  "#04182B,#5A8C8C,#F2D99D,#738585,#AB1111,#04182B,#5A8C8C,#F2D99D";
    Vector<string>colorsStrVec = stringSplit(colorSt,",");
    for (string color : colorsStrVec) {
        colors.add(convertColorToRGB(trim(color)));
    }
    return colors;
}

