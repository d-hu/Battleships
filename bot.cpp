/* 
Name: Dennis Hu
Class: 135/136
Project 2


This program starts from 0,0 and works through the board form left to right. 
If HIT checks for vertical ship and marks around it and the program avoids those spots
If Horizontal Ship it marks and x all around it and the program avoids those spots

*/
#include <cstdlib>
#include <iostream>
#include "bot.h"
#include "screen.h"

using namespace std;



int r;
int c;
int ROWS;
int COLS;
int iter = 0; // iter counter
int HITS = 0; // HIT n SUNK counter
int vHITS = 0; // vertical hit counter



/* for later testings
int game[50][50];
*/

/* Initialization procedure, called when the game starts:

   init (rows, cols, num, screen, log) 
 
   Arguments:
    rows, cols = the boards size
    num        = the number of ships 
    screen     = a screen to update your knowledge about the game
    log        = a cout-like output stream
*/
void init(int rows, int cols, int num, Screen &screen, ostream &log) 
{
    ROWS = rows;
    COLS = cols;
    log << "Start." << endl;
}


/* The procedure handling each turn of the game:
 
   next_turn(sml, lrg, num, gun, screen, log)
 
   Arguments:
    sml, lrg = the sizes of the smallest and the largest ships that are currently alive
    num      = the number of ships that are currently alive
    gun      = a gun.
               Call gun.shoot(row, col) to shoot: 
                  Can be shot only once per turn. 
                  Returns MISS, HIT, HIT_N_SUNK, ALREADY_HIT, or ALREADY_SHOT.
    screen   = a screen to update your knowledge about the game
    log      = a cout-like output stream
*/
void next_turn(int sml, int lrg, int num, Gun &gun, Screen &screen, ostream &log){
    result res;
    r = iter / COLS;
    c = iter % COLS; 
    iter += 1;       
    
    // checks if the next spot is a sunk          
    while(screen.read(r, c) == 'S' || screen.read(r,c) == 'x'||screen.read(r,c) == '@'){ // loops through all markings and skips over
        r = iter / COLS;
        c = iter % COLS;
        iter += 1;
    }
    // shoot
    res = gun.shoot(r, c);
    log << "Smallest: " << sml << " Largest: " << lrg << ". ";
    log << "Shoot at " << r << " " << c << endl;
       
    // result returned
    if (res == MISS ){
        if(HITS > 0) // counter reset
            iter -= COLS * HITS; 
        
        
        if(screen.read(r,c-1) == '@'){ // for vertical fix
            iter -= COLS * vHITS+1;
            vHITS = 0;
        }
        
        screen.mark(r, c, 'x', BLUE);
        

    }
    
    else if (res == HIT){ 
        HITS += 1; // required incase of a 2 size shiped
        iter -= 1; // removes the automated counter to iterate the tactic
        screen.mark(r, c, '@', GREEN);
        

        // Vertical ship check
        if(screen.read(r-1,c) == '@'){
            HITS = 0;
            vHITS += 1; 
            if(screen.read(r,c) == '@'){
                screen.mark(r-1, c+1, 'x', BLUE); // marks off the right side of the first hit.
                }
                screen.mark(r, c+1, 'x', BLUE);
                screen.mark(r, c-1, 'x', BLUE);

        }
        // Horizontal ship check
       else if(screen.read(r,c-1) == '@'){ // if horizontal mark underneath
            
            HITS = 0;
            screen.mark(r+1,c, 'x', BLUE);
            iter += 1;
            
        }
        if( r*c <= ROWS*COLS){
            if(screen.read(r, c-1) != '@'){ // if vertical ship move down
                iter += COLS;
                r = iter / COLS;
                c = iter % COLS;
            }
        }
    }
    else if (res == HIT_N_SUNK){

        screen.mark(r, c, 'S', RED);
        screen.mark(r+1, c, 'x', BLUE); // marks space under SUNK
        screen.mark(r, c+1, 'x', BLUE); // marks space right of SUNK
        
        if(screen.read(r, c-1) != '@')
            screen.mark(r, c-1, 'x', BLUE); // marks space left of SUNK
        
        if(HITS == 1){  // marks first space next to the first vertical hit
            if(screen.read(r-1, c+1) == '@'){
              screen.mark(r-1, c+1, 'x', BLUE);    
              HITS = 0;

              
            }
        }
        if(vHITS > 0){
            iter -= COLS * (vHITS+1); // reset counter to previous spot before hit
            vHITS = 0;
        }
        if(HITS > 0){ // HITS RESET FOR 1 HIT
            iter -= COLS * HITS;
            HITS = 0;
        }
    }
}
