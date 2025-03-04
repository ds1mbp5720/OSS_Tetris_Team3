#include "library.h"
#include "shapes.h"
#include "tetris_function.h"
#include "show.h"
#define first_x 5
#define first_y 0
#define first_direction 0
int score = 0; // game score
int bombsignal = 0; // signal of bombitem
int erasesignal = 0; // signal of erase item
int itemcnt = 0; //count for using item
int reverse_item_on=0; // 0 is off , 1 is on
int reverse_item_time_count = 0;
             /*
             Method to make random int from 0 to limit-1
             Returns an integer that determines one of the seven shapes of the block.
             */
int make_randint(int limit)
{
	int rand_int;
    srand((unsigned)time(NULL));
	rand_int = rand() % limit;
    return rand_int;      //returns random number below the limit; //todo: should not restore the formula immediately.
}

BLOCK* first_block(void)
{
   BLOCK* block;

   block = (BLOCK*)malloc(sizeof(BLOCK));

   //state == 0 is touched base 1 is falling
   block->shape = make_randint(7);      //Store a random number below 7 in block -> shape.
   block->block_x = first_x;      //todo: Use a constant
   block->block_y = first_y; // Specify where the block will occur. //todo: Use a constant
   block->direction = first_direction; // Specify the rotating state of the block.//todo: Use a constant

   return block;
}

BLOCK* item_first_block(void)
{
   BLOCK* block;

   block = (BLOCK*)malloc(sizeof(BLOCK));

   //state == 0 is touched base 1 is falling
   block->shape = make_randint(8);      //Store a random number below 8 in block -> shape.
   block->block_x = first_x;      //todo: Use a constant
   block->block_y = first_y; // Specify where the block will occur. //todo: Use a constant
   block->direction = first_direction; // Specify the rotating state of the block.//todo: Use a constant

   return block;
}
/*
Method of making and returning new blocks.
Return a block that will appear next while the game is running
*/
BLOCK* make_block(BLOCK* pre)
{
   BLOCK* block;

   block = (BLOCK*)malloc(sizeof(BLOCK));

   //state == 0 is touched base 1 is falling
   block->shape = next_shape; //Save the next_shape value in block - > shape.
   next_shape = rand() % 7; //Save the new random number below 7 in the next-shape.
   block->block_x = first_x; //todo: Use a constant
   block->block_y = first_y; //Specify where the block will occur. //todo: Use a constant
   block->direction = first_direction; //Specify the rotating state of the block.//todo: Use a constant
   free(pre); //eliminate the previous block

   return block;
}

BLOCK* item_make_block(BLOCK* pre)
{
   BLOCK* block;

   block = (BLOCK*)malloc(sizeof(BLOCK));

   //state == 0 is touched base 1 is falling
   block->shape = next_shape; //Save the next_shape value in block - > shape.
   next_shape = rand() % 8; //Save the new random number below 8 in the next-shape.
   block->block_x = first_x; //todo: Use a constant
   block->block_y = first_y; //Specify where the block will occur. //todo: Use a constant
   block->direction = first_direction; //Specify the rotating state of the block.//todo: Use a constant
   free(pre); //eliminate the previous block

   return block;
}
/*
Method to change the part of the screen array where the block exists to 1.
*/
void input_block(BLOCK* block)
{
	int x, y;
	int shape;
	int direction;   //todo: Each variable is declared in one row

   shape = block->shape;
   direction = block->direction;   //Save the block's state in each x, y, shape, direction.

                           //update the screen array
   for (y = 0; y < 4; y++) {   //If the block exists,(if the value is 1)
      for (x = 0; x < 4; x++) {
         if (shapes[shape][direction][y][x] == 1) {
            screen[block->block_y + y][block->block_x + x] = 1;//Store 1 in the corresponding location on the screen array.
                                                   //block_x, block_y is the current location of the block
         }
         if (shapes[shape][direction][y][x] == 3) {
            screen[block->block_y + y][block->block_x + x] = 3;//Store 1 in the corresponding location on the screen array.
                                                   //block_x, block_y is the current location of the block
         }
      }
   }
}
/*
Method to change the part of the screen array where the block exists to 0.
*/
void remove_Block(BLOCK* block)      //todo: Replace the first letter with a lowercase letter
{
	int x, y;
	int shape;
	int direction;      //todo: Each variable is declared in one row

   shape = block->shape;
   direction = block->direction;   //Save the block's state in each x, y, shape,

   if (checkdown(block) != 0)
   {
      //update the screen array
      for (y = 0; y < 4; y++) {
         for (x = 0; x < 4; x++) {
            if (shapes[shape][direction][y][x] == 1) {   //If the block exists,(if the value is 1)   
               screen[block->block_y + y][block->block_x + x] = 0;      //Store 0 in the corresponding location on the screen array.
                                                          //block_x, block_y is the current location of the block
            }
            if (shapes[shape][direction][y][x] == 3) {   //If the reverse_item block exists,(if the value is 3)   
               screen[block->block_y + y][block->block_x + x] = 0;      //Store 0 in the corresponding location on the screen array.
                                                          //block_x, block_y is the current location of the block
            }
         }
      }
   }
   
}
/*
Method to check down side of block
if somthing in downside return 0
*/
int checkdown(BLOCK* block)
{
	int x, y;
	int shape;
	int direction;
    int edge[4] = { 0,0,0,0 };// use for marking edge
                       //setting local variable from factor
   shape = block->shape;
   direction = block->direction;

   for (x = 0; x < 4; x++) {
      for (y = 3; y>0; y--) {
         if (shapes[shape][direction][y][x] == 1) {
            edge[x] = y; //set fill edge num
            break;
         }
         if (shapes[shape][direction][y][x] == 3) {
            edge[x] = y; //set fill edge num
            break;
         }
      }
   }
   //if next place is filled return 0; else return 1;
   for (x = 0; x < 4; x++) {
      if (edge[x] != 0) {
         if (screen[block->block_y + edge[x] + 1][block->block_x + x] == 1)
            
            return 0;
         if (screen[block->block_y + edge[x] + 1][block->block_x + x] == 3)
            
            return 0;
      }
   }
   return 1;
}
/*
Method to check left side of block
if somthing in leftside return 0
*/
int checkleft(BLOCK* block)
{
	int x, y;
	int shape;
	int direction;
    int edge[4] = { -1,-1,-1,-1 };// use for marking edge
                          //setting local variable from factor
   shape = block->shape;
   direction = block->direction;

   for (y = 0; y < 4; y++) {
      for (x = 0; x < 4; x++) {
         if (shapes[shape][direction][y][x] == 1)
         {
            edge[y] = x; //set fill edge num
            break;
         }
         if (shapes[shape][direction][y][x] == 3)
         {
            edge[y] = x; //set fill edge num
            break;
         }

      }
   }

   //if next place is filled return 0; else return 1;
   for (y = 0; y < 4; y++) {
      if (edge[y] != -1) {
         if (screen[block->block_y + y][block->block_x + edge[y] - 1] == 1)
            return 0;
         if (screen[block->block_y + y][block->block_x + edge[y] - 1] == 3)
            return 0;
      }
   }
   return 1;
}
/*
Method to check right side of block
if somthing in rightside return 0
*/
int checkright(BLOCK* block)
{
	int x, y;
	int shape;
	int direction;
    int edge[4] = { -1,-1,-1,-1 }; // use for marking edge
                           //setting local variable from factor
   shape = block->shape;
   direction = block->direction;

   for (y = 0; y < 4; y++) {
      for (x = 3; x >0; x--) {
         if (shapes[shape][direction][y][x] == 1)
         {
            edge[y] = x; //set fill edge num
            break;
         }
         if (shapes[shape][direction][y][x] == 3)
         {
            edge[y] = x; //set fill edge num
            break;
         }
      }
   }

   //if next place is filled return 0; else return 1;
   for (y = 0; y < 4; y++) {
      if (edge[y] != -1) {
         if (screen[block->block_y + y][block->block_x + edge[y] + 1] == 1)
            return 0;
         if (screen[block->block_y + y][block->block_x + edge[y] + 1] == 3)
            return 0;
      }
   }
   return 1;
}

//check the next shape place
int checkrotate(BLOCK* block)
{
	int x, y;
	int shape;
	int direction;
	int next_direction;
    int i, flag, plus_y = 0, plus_x = 0;
    int Next_shape[4][4] = { 0 }; // all thing set 0
    int edge[4] = { -1, -1, -1, -1 };
   //setting local variable from factor
   shape = block->shape;
   direction = block->direction;
   next_direction = block->direction + 1;
   //limit direction number 3
   next_direction %= 4;

   //current_shape -> next_shape  scan (4*4)
   for (y = 0; y < 4; y++) {
      for (x = 0; x < 4; x++) {
         if (shapes[shape][direction][y][x] != 1) {
            Next_shape[y][x] = shapes[shape][next_direction][y][x];
         }
         if (shapes[shape][direction][y][x] != 3) {
            Next_shape[y][x] = shapes[shape][next_direction][y][x];
         }
      }
   }

   //0 is check left 1 is check right 2 is check down 3is check up
   for (flag = 0; flag < 4; flag++) {
      //make edge by flag
      switch (flag)
      {
      case 0://left 
            /*
            TODO: if x<3 -> do not rotate
            */
         for (y = 0; y < 4; y++) {
            for (x = 0; x < 4; x++) {
               if (Next_shape[y][x] == 1)
               {
                  edge[y] = x;
                  break;
               }
               if (Next_shape[y][x] == 3)
               {
                  edge[y] = x;
                  break;
               }
            }
         }
         break;

      case 1://right
         for (y = 0; y < 4; y++) {
            for (x = 3; x >0; x--) {
               if (Next_shape[y][x] == 1)
               {
                  edge[y] = x;
                  break;
               }
               if (Next_shape[y][x] == 3)
               {
                  edge[y] = x;
                  break;
               }

            }
         }
         break;

      case 2://down
         for (x = 0; x < 4; x++) {
            for (y = 3; y>0; y--) {
               if (Next_shape[y][x] == 1) {
                  edge[x] = y;
                  break;
               }
               if (Next_shape[y][x] == 3) {
                  edge[x] = y;
                  break;
               }
            }
         }
         break;
      case 3://up
         for (x = 0; x < 4; x++) {
            for (y = 0; y<4; y++) {
               if (Next_shape[y][x] == 1) {
                  edge[x] = y;
                  break;
               }
               if (Next_shape[y][x] == 3) {
                  edge[x] = y;
                  break;
               }
            }
         }
         break;

      }

      //check edge by flag
      for (i = 0; i < 4; i++) {
         if (edge[i] != 0)
         {
            if (i < 2) {
               if (block->block_x > 2)
               {
                  if (screen[block->block_y + i][block->block_x + edge[i]] == 1)
                     return 0;
                  if (screen[block->block_y + i][block->block_x + edge[i]] == 3)
                     return 0;
               }
               else
               {
                  if (screen[block->block_y + i][block->block_x + edge[i] + 2] == 1)
                     return 0;
                  if (screen[block->block_y + i][block->block_x + edge[i] + 2] == 3)
                     return 0;
               }
            }
            else {
               if (screen[block->block_y + edge[i]][block->block_x + i] == 1)
                  return 0;
               if (screen[block->block_y + edge[i]][block->block_x + i] == 3)
                  return 0;
            }
         }
      }
      //clear edge
      for (i = 0; i < 4; i++) {
         edge[i] = -1; //reset
      }
   }
   return 1;
}
/*
Method to make block destination for use screen[y][x]=2
and show block detination
*/
void blockdestination(BLOCK *future)
{

	int x, y;
	int shape;
	int direction;

   while (checkdown(future)) // blcok down while screen[y][x]=1
      future->block_y++;

   shape = future->shape;
   direction = future->direction;

   //shape's fill part to move screen
   for (y = 0; y < 4; y++) {
      for (x = 0; x < 4; x++) {
         if (shapes[shape][direction][y][x] == 1) {
            screen[future->block_y + y][future->block_x + x] = 2;
         }
         if (shapes[shape][direction][y][x] == 3) {
            screen[future->block_y + y][future->block_x + x] = 2;
         }
      }
   }

}
//Method to control block according to the movement keys pressed
void control_shape(BLOCK* block)
{
   char key; // user pressed key

   while (_kbhit()) {
      key = _getch();

      switch (key)
      {
      case UP://rotate block
         if (checkrotate(block) == 1) {
            block->direction++;
            block->direction %= 4; // limit rotate
         }
         break;

      case DOWN:
         if (checkdown(block) == 1)
            block->block_y++;
         break;

      case RIGHT:
         if (reverse_item_on==0){
            if (checkright(block) == 1) {
               block->block_x++;
            }
         }
         else{
            if (checkleft(block) == 1) {
            block->block_x--;
         }
         }
         break;

      case LEFT:
         if (reverse_item_on==0){
            if (checkleft(block) == 1) {
               block->block_x--;
            }
         }
         else{
            if (checkright(block) == 1) {
                  block->block_x++;
               }
         }
         break;
      case SPACEBAR:
         while (checkdown(block))
            block->block_y++;
      }
   }
}

//if game is voer return 1 else return 0
int Check_Over(int score)
{
   int i;

   for (i = 0; i < 4; i++) {
      if (screen[1][5 + i] == 1 )
      {
         system("cls");
         set_cursor(30, 10);
         printf("YOU LOSE!");
		 set_cursor(30, 11);
		 printf("최종 점수:%d", score);
		 set_cursor(30, 12);
         return 1;
      }
      if (screen[1][5 + i] == 3 )
      {
         system("cls");
         set_cursor(30, 10);
         printf("YOU LOSE!");
		 set_cursor(30, 11);
		 printf("최종 점수:%d",score);
		 set_cursor(30, 12);
         return 1;
      }
   }
   return 0;
}
/*
Method to set future block for set dstination
*/
void copyblock(BLOCK *block, BLOCK *copy)
{
   copy->block_y = 0;
   copy->block_x = block->block_x;
   copy->direction = block->direction;
   copy->shape = block->shape;
}
/*
Method to carry out the overall execution of tetris
Function:   Move blocks down, Change the block according to the keyboard input,
Print the block, Remove the completed line, Check whether the game is over
*/
void run_game(BLOCK* block)
{
   int i = 0;
   int line_num;//Initializing for Repetition
   int score = 0; // game score
   BLOCK *future = (BLOCK*)malloc(sizeof(BLOCK));
   int x, y;
   remove_cursor(); //remove cursor on screen

   show_nextshape(); //print the next block
   show_score(score); // print game score
   while (1) { //the loop ends when the game is over
      while (1) { //the loop ends when the block is collide bottom
               //to move block fast
         i++;
         if (i == 3) {
            i = 0;
            block->block_y++;   //if (i==3) move block one down
         }
         copyblock(block, future); // set future block
         blockdestination(future); //make destination
         control_shape(block);   // If there is a keyboard input, change the shape of the block.

         input_block(block);      //Fill the screen array with blocks.

         show_screen();      //print screen
         show_score(score); // print game score
                        //change place(side) and direction


                        //if next place(down) is filled break while and make new block
         if (checkdown(block) == 0)
            break;
         //remove current block
         remove_Block(block);
         for (y = 0; y < SCREEN_HEIGHT; y++) // remove pre dstination
         {
            for (x = 0; x < SCREEN_WIDTH; x++)
            {
               if (screen[y][x] == 2)
                  screen[y][x] = 0;
            }
         }
         Sleep(100);      //Do nothing for 0.1 seconds.
      }
      while (1)
      {
		  line_num = Clear_Line();   /*Check how many lines are cleared, and then insert the value into the line variable. */
         if (line_num == 0)   // If there are no lines to erase, break.
            break;
         Shift_Screen(line_num);   //Erase the line and print out the screen.
         score = score + 10 * 12;
      }

      if (Check_Over(score))   //Check to see if the game is over.
         break;

      //make new block
      block = make_block(block);
      next_shape = make_randint(7);   //make random integer 0~6

      remove_cursor();


      if (block->shape == next_shape && next_shape != 6)   //Make a variety of shapes.
         next_shape += 1;

      show_nextshape();
   }
}
BLOCK *bombitem(BLOCK *pre)
{
   BLOCK* block;

   block = (BLOCK*)malloc(sizeof(BLOCK));

   //state == 0 is touched base 1 is falling
   block->shape = 8; //Save the next_shape value in block - > shape.
   next_shape = rand() % 8; //Save the new random number below 7 in the next-shape.
   block->block_x = first_x; //todo: Use a constant
   block->block_y = first_y; //Specify where the block will occur. //todo: Use a constant
   block->direction = first_direction; //Specify the rotating state of the block.//todo: Use a constant
   free(pre);
   return block;
}
BLOCK *eraseitem(BLOCK *pre)
{
   BLOCK* block;

   block = (BLOCK*)malloc(sizeof(BLOCK));

   //state == 0 is touched base 1 is falling
   block->shape = 8; //Save the next_shape value in block - > shape.
   next_shape = rand() % 8; //Save the new random number below 7 in the next-shape.
   block->block_x = first_x; //todo: Use a constant
   block->block_y = first_y; //Specify where the block will occur. //todo: Use a constant
   block->direction = first_direction; //Specify the rotating state of the block.//todo: Use a constant
   free(pre);
   return block;
}
void effectbomb(BLOCK *bomb) // effect bomb item
{
   int x, y;
   for (y = 1; y < 4; y++) // delete point
   {
      for (x = 1; x < 4; x++)
      {
         if (bomb->block_y + y < SCREEN_HEIGHT - 1 && bomb->block_x + x >= 1 && bomb->block_x + x < SCREEN_WIDTH - 1)
         {
            if (screen[bomb->block_y + y][bomb->block_x + x] == 1) // if block in the point
            {
               screen[bomb->block_y + y][bomb->block_x + x] = 0; // delete

            }
            if (screen[bomb->block_y + y][bomb->block_x + x] == 3) // if block in the point
            {
               screen[bomb->block_y + y][bomb->block_x + x] = 0; // delete

            }
         }
         score = score + 10; // plus point
      }
   }
   score = score - 10; // minus bomb point
   for (; y > 0; y--) // for fiil after delete (not yet)
   {
      for (x = bomb->block_x + 1; x < bomb->block_x + 4; x++)
      {
         if (x >= 1 && x < SCREEN_WIDTH - 1)
         {
            screen[y][x] = screen[y - 1][x]; // move to down // fill empty line after use bomb.
         }
      }
   }
}
void effecterase(BLOCK *erase) //erase item
{
   int x, y;
   for (y = erase->block_y + 1; y > 0; y--) {//check from down to up
      for (x = 1; x < SCREEN_WIDTH - 1; x++)  //one line in the map
      {
         if (screen[y][x] == 1)
         {
            score = score + 10;
            screen[y][x] = 0;
         }
         if (screen[y][x] == 3)
         {
            score = score + 10;
            screen[y][x] = 0;
         }
      }
   }
   screen[erase->block_y + 2][erase->block_x + 2] = 0;
}
void run_itemgame(BLOCK* block) // ITEM mode 
{

   int i = 0; //Initializing for Repetition
   int line_num;
   BLOCK *future = (BLOCK*)malloc(sizeof(BLOCK));
   int x, y;
   remove_cursor(); //remove cursor on screen

   show_nextshape(); //print the next block
   show_score(score); // print game score
   while (1) { //the loop ends when the game is over
      while (1) { //the loop ends when the block is collide bottom
               //to move block fast
         i++;
         if (i == 3) {
            i = 0;
            block->block_y++;   //if (i==3) move block one down
         }
         copyblock(block, future); // set future block
         blockdestination(future); //make destination
         control_shape(block);   // If there is a keyboard input, change the shape of the block.

         input_block(block);      //Fill the screen array with blocks.

         show_screen();      //print screen
         show_score(score); // print game score
                        //change place(side) and direction


                        //if next place(down) is filled break while and make new block
         if (checkdown(block) == 0)
         {
            if (bombsignal == 1) // if block it bomb
            {
               effectbomb(block); // using item
               bombsignal = 0; // reset condition
               break;
            }
            if (erasesignal == 1)
            {
               effecterase(block);
               erasesignal = 0; //reset erasesignal condition
               break;
            }
            break;
         }
         //remove current block
         remove_Block(block);
         for (y = 0; y < SCREEN_HEIGHT - 1; y++) // remove pre dstination
         {
            for (x = 1; x < SCREEN_WIDTH - 1; x++)
            {
               if (screen[y][x] == 2)
                  screen[y][x] = 0;
            }
         }
         Sleep(100);      //Do nothing for 0.1 seconds.
         if(reverse_item_time_count!=0)
         {
            reverse_item_time_count--; // decrease reverse_item_time_count
         }
         else
         {
            reverse_item_on=0;   //reverse_item_time finish
         }
      }
      while (1)
      {
		  line_num = Clear_Line();   /*Check how many lines are cleared, and then insert the value into the line variable. */
         if (line_num == 0)   // If there are no lines to erase, break.
         {
            itemcnt = 0; // reset item count after checking line over
            break;
         }
         else
         {
            itemcnt++;
            
         }
         Shift_Screen(line_num);   //Erase the line and print out the screen.
            score = score + 10 * 12;
         
         if (itemcnt == 2)
         {
            bombsignal = 1;
         }
         else if (itemcnt >= 3)
         {
            bombsignal = 0;
            erasesignal = 1;
         }
         
      }

      if (Check_Over(score))   //Check to see if the game is over.
         break;

      //make new block
      if (bombsignal == 0 && erasesignal == 0) // no anyitem
      {
         if(block->shape==7)
         {               
            if(reverse_item_on==0)
            {
               reverse_item_on=1; //reverse_item active
               reverse_item_time_count = 60; // reverse_item active 10 sec.   
            }
            else
               reverse_item_on=0;
         }
         block = item_make_block(block);
         next_shape = make_randint(8);   //make random integer 0~7
      }
      else if (bombsignal == 1 && erasesignal == 0) // make bombitem
      {
         set_cursor(5, 0);
         block = bombitem(block);
      }
      else if (bombsignal == 0 && erasesignal == 1) // make bombitem
      {
         set_cursor(5, 0);
         block = eraseitem(block);
      }
      remove_cursor();


      if (block->shape == next_shape && next_shape != 7)   //Make a variety of shapes.
         next_shape += 1;

      show_nextshape();
   }
}