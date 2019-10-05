#include <Gamer.h>
Gamer gamer;

void setup()
{
  gamer.begin();
}

boolean runStart = true; //when re-booted allows initialise once only
int blockX; //used to draw line along each row
int blockY;  //used to draw line along each row 
int eraseBlockX; //used to erase previous active pixel along current row
int eraseBlockY; //used to erase previous active pixel along current row
int pixelX; //used to draw player controlled pixel
int pixelY; //used to draw player controlled pixel
int stackX; //Store x coord of stack base
boolean moveRight = true; //set to true when block is moving left to right
int score; //your score
int speedOMeter; //speed of pixel

int showscore[10][4][8] = {
  {{0,0,0,0,0,0,0,0},
   {0,1,1,1,1,1,1,0},
   {0,1,0,0,0,0,1,0},
   {0,1,1,1,1,1,1,0}},
			  
  {{0,0,0,0,0,0,0,0},
   {0,1,0,0,0,0,1,0},
   {0,1,1,1,1,1,1,0},
   {0,0,0,0,0,0,1,0}},
		  
  {{0,0,0,0,0,0,0,0},
   {0,1,0,0,1,1,1,0},
   {0,1,0,1,0,0,1,0},
   {0,1,1,0,0,0,1,0}},
			  
  {{0,0,0,0,0,0,0,0},
   {0,1,0,1,0,0,1,0},
   {0,1,0,1,0,0,1,0},
   {0,1,1,0,1,1,1,0}},
			  
  {{0,0,0,0,0,0,0,0},
   {0,1,1,1,0,0,0,0},
   {0,0,0,1,0,0,0,0},
   {0,1,1,1,1,1,1,0}},

  {{0,0,0,0,0,0,0,0},
   {0,1,1,1,0,0,1,0},
   {0,1,0,1,0,0,1,0},
   {0,1,0,0,1,1,0,0}},

  {{0,0,0,0,0,0,0,0},
   {0,1,1,1,1,1,1,0},
   {0,1,0,1,0,0,1,0},
   {0,1,0,1,1,1,1,0}},
			  
  {{0,0,0,0,0,0,0,0},
   {0,1,0,0,0,1,1,0},
   {0,1,0,0,1,0,0,0},
   {0,1,1,1,0,0,0,0}},
			  
  {{0,0,0,0,0,0,0,0},
   {0,1,1,1,1,1,1,0},
   {0,1,0,1,0,0,1,0},
   {0,1,1,1,1,1,1,0}},
			  
  {{0,0,0,0,0,0,0,0},
   {0,1,1,1,0,0,0,0},
   {0,1,0,1,0,0,0,0},
   {0,1,1,1,1,1,1,0}}
  };

void displayScore(int result)
{
  //tone(0,75,100);
  delay(600);
  gamer.clear();		  
  if (result < 10)
  {
    for (int x = 4; x < 8; x++)
      {
      for (int y = 0; y < 8; y++)
        {
        gamer.display[x][y] = showscore[result][x - 4][y];
        }
      }
    }
  else
  {	
    int divisor = 10;
    for (int x = 0; x < 4; x++)
      {
      for (int y = 0; y < 8; y++)
        {
        gamer.display[x][y] = showscore[result / divisor][x][y];
        }
      }
      for (int x = 4; x < 8; x++)
	{
	for (int y = 0; y < 8; y++)
	  {
	  gamer.display[x][y] = showscore[result % 10][x-4][y];
	  }
	}
    }
  gamer.updateDisplay();
  delay(600);
  reset(true);
}


void reset(boolean fullReset)
{ 
  blockX = -1; //used to draw line along bottom row
  blockY = 7;  //used to draw line along bottom row 
  eraseBlockX = -1; //used to erase previous active pixel along bottom row
  eraseBlockY = 7; //used to erase previous active pixel along bottom row
  pixelX = blockX; //used to draw player controlled pixel
  pixelY = blockY; //used to draw player controlled pixel
  moveRight = true; //set to true when block is moving left to right
  stackX = 0;
  speedOMeter = 130;
  if (fullReset)
    {
     score = 0;
    }
  gamer.clear();
}

void loop()
{
  if (runStart) //reboot only
   {
   reset(true);
   runStart = false;
   }
  if (gamer.isPressed(START)) //manual reset
    {
    reset(true);
    }
  if (gamer.isPressed(UP))
    {
     boolean doContinue = true;
     // first press up draw fixed pixel on bottom row
     // press 2 -7 check if player has pressed button at right time
     // if so the generated player pixel will have same x coord as current bottom row
     pixelX = blockX;
     if (blockY == 7) //first press of up button
       {
       stackX = blockX;
       pixelX = blockX;
       pixelY = blockY;
       }
     else //up button 2 to 7
       {
        if (pixelX != stackX)
          {
          doContinue = false;
          displayScore(score);
          }
       }
     if (doContinue) //displayScore NOT called
     {  
      score++; // must be in line with stack, so increase score
      blockY--;
      eraseBlockY--;
      if (blockY == -1)
        {
        blockY = 0;
        }
    
      gamer.display[pixelX][(pixelY)] = 1;
      if (blockY == -1)
        {
         reset(false);
         speedOMeter = speedOMeter - 10;
         }
       }
    }

  //move pixel on current row every 1 sec from left to right then reverse
  if (blockX > -1)
    {
    eraseBlockX = blockX;  //starts at -1, increases to 0, 1, 2, 3, 4, 5, 6
    }
  if (moveRight)         //block moving to right side x = 0 to x = 7
  {
    blockX ++;           // starts at -1, increases to 0,1,2,3,4,5,6,7
    if (blockX > 7)      // gone past display[7][0], would be non-existing display[8][0]
    {
      blockX = 6;	 //show display[6][0] as display[7][0] already showed
      eraseBlockX = 7;   // erase display[7][0]
      moveRight = false; //now move R to L
    }
  }
  else                   //block moving right to left side
  {
    blockX --;           // move from x = 7 to x = 0
    if (blockX < 0)      // now at left edge , start moving right again
    {
      blockX = 1;        // already showed display[0][0] so move to display[1][0]
      eraseBlockX = 0;   // delete display[0][0]
      moveRight = true;  //move to right side again
    }
  }
  gamer.display[blockX][blockY] = 1;  //turn display pixel on. first time loop runs [0][0] = 1 goes to display[7][0] = 1
  // turn previous display pixel off first time loop runs [-1][0]
  if (eraseBlockX > -1 && eraseBlockX < 8)
    {
    gamer.display[eraseBlockX][eraseBlockY] = 0;
    }
  gamer.updateDisplay();              //redraw display
  delay(speedOMeter);                 //delay speedometer (1000 = 1 sec)
}


