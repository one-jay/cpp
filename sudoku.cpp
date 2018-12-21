
/***********************************************************************                     
* Program:                                                                                   
*    Project 13, Sudoku                                                                      
*    Brother Ellsworth, CS124                                                                
* Author:                                                                                    
*    Clayton Thomas                                                                          
* Summary:                                                                                   
*    Sudoku!                                                                                 
*                                                                                            
*    Estimated:  20.0 hrs                                                                    
*    Actual:     30.0 hrs                                                                    
*      Please describe briefly what was the most difficult part.                             
*          troubleshooting when the testbed gave no clues why timeout                        
************************************************************************/
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <cassert>
#include <cstdlib>
#include <cctype>
using namespace std;

#ifdef DEBUG
#define Debug(x) x
#else // !DEBUG                                                                              
#define Debug(x)
#endif // !DEBUG                                                                             

/**********************************************************************                      
* get file name                                                                              
* gets file name from user                                                                   
***********************************************************************/
string getFileName()
{
   cout << "Where is your board located? ";
   string fileName;
   cin >> fileName;
   return fileName;
}

/**********************************************************************                      
* read file                                                                                  
* reads file data to board array                                                             
***********************************************************************/
int readFile(string fileName, int board[][9])
{
   ifstream fin(fileName.c_str()); // open the input stream to fileName                      
   if (fin.fail()) // never forget the error checking                                        
   {
      cout << "ERROR: Unable to read file "
           << fileName << endl; // display the filename we tried to read                     
      return 0; // return the error condition: none read                                     
   }

   for (int row = 0; row < 9; row++)
      for (int col = 0; col < 9; col++)
         fin >> board[row][col];

   fin.close(); // never forget to close the file                                            
   return 0;
}

// prototype for interact                                                                    
void interact(int board[][9]);

/**********************************************************************                      
* main                                                                                       
* calls get file, read file, interact                                                        
***********************************************************************/
int main()
{
   string fileName = getFileName();
   int board[9][9];
   readFile(fileName,board);
   interact(board);
   return 0;
}

// interact prototypes                                                                       
void displayOptions();
void displayBoard(int board[][9]);
void editSquare(int board[][9]);
void showPossibilities(int board[][9]);
void saveQuit(int board[][9]);

/**********************************************************************                      
* interact                                                                                   
* gets action from user and calls reaction function                                          
***********************************************************************/
void interact(int board[][9])
{
   displayOptions();
   displayBoard(board);

   char input;
   while (input != 'Q')
   {
      cout << "\n> ";
      cin >> input;
      switch (input)
      {
         case '?':
            displayOptions();
            break;
         case 'd': case 'D':
            displayBoard(board);
            break;
         case 'e': case 'E':
            editSquare(board);
            break;
         case 's': case 'S':
            showPossibilities(board);
            break;
         case 'Q':
            saveQuit(board);
            break;
      }
   }

}


/**********************************************************************                      
* display options                                                                            
* displays game options                                                                      
***********************************************************************/
void displayOptions()
{
   cout << "Options:\n"
        << "   ?  Show these instructions\n"
        << "   D  Display the board\n"
        << "   E  Edit one square\n"
        << "   S  Show the possible values for a square\n"
        << "   Q  Save and Quit\n"
        << endl;
}



/**********************************************************************                      
* display board                                                                              
* displays game board                                                                        
***********************************************************************/
void displayBoard(int board[][9])
{
   cout << "   A B C D E F G H I\n"; //column header                                         
   for (int row = 0; row < 9; row++)
   {
      if (row == 3 || row == 6)
         cout << "   -----+-----+-----" << endl;
      cout << row + 1 << "  ";       //row header                                            
      for (int col = 0; col < 9; col++)
      {
         if (col == 8)
         {
            if (board[row][col] == 0)
               cout << " \n";
            else cout << board[row][col] << endl;
         }
         else if (col == 2 || col == 5)
         {
            if (board[row][col] == 0)
               cout << " |";
            else cout << board[row][col] << "|";
         }
         else
         {
            if (board[row][col] == 0)
               cout << "  ";
            else cout << board[row][col] << " ";
         }
      }
   }
//   cout << endl;                                                                           
}

// prototype for compute possibilities                                                       
void computePossibilities(int board[][9],int row,int col,int possibilities[]);


/**********************************************************************                      
* show possibilities                                                                         
* shows possible values to users for a cell                                                  
***********************************************************************/
void showPossibilities(int board[][9])
{
   int possibilities[9] = {1,2,3,4,5,6,7,8,9};

   cout << "What are the coordinates of the square: ";
   char coordinate[2];
   cin >> coordinate;
   coordinate[0] = toupper(coordinate[0]);

   cout << "The possible values for '" << coordinate << "' are: ";

   //convert ascii char's to numbers for array positions                                     
   int row = coordinate[1] - 49;
   int col = coordinate[0] - 48 - 17;

   computePossibilities(board, row, col, possibilities);

   bool start = true;
   for (int i = 0; i < 9; i++)
      if (possibilities[i] == 0)
         cout << "";
      else
         if (start == true)
         {
            cout << possibilities[i];
            start = false;
         }
         else
            cout << ", " << possibilities[i];
   cout << endl;
}


/**********************************************************************                      
* edit square                                                                                
* allows users to enter value for a cell                                                     
***********************************************************************/
void editSquare(int board[][9])
{
   cout << "What are the coordinates of the square: ";
   char coordinate[2];
   cin >> coordinate;
   coordinate[0] = toupper(coordinate[0]);

   //convert ascii char's to numbers for array positions                                     
   int row = coordinate[1] - 49;
   int col = coordinate[0] - 48 - 17;

   if (coordinate[0] < 65 || coordinate[0] > 73
       || coordinate[1] < 49 || coordinate[1] > 57)
      cout << "ERROR: Square '" << coordinate << "' is invalid\n";

   else if (board[row][col] > 0)
      cout << "ERROR: Square '" << coordinate << "' is filled\n";

   else
   {
      cout << "What is the value at '" << coordinate << "': ";
      int value;
      cin >> value;

      bool valueIsPossible = false;
      int possibilities[9] = {1,2,3,4,5,6,7,8,9};
      computePossibilities(board, row, col, possibilities);

      for (int i = 0; i < 9; i++)
         if (possibilities[i] == value)
            valueIsPossible = true;
      if (valueIsPossible == true)
         board[row][col] = value;
      else
         cout << "ERROR: Value '" << value
              << "' in square '" << coordinate << "' is invalid\n";

   }
   displayBoard(board); //display board after each edit                                      
}

/**********************************************************************                      
* compute possibilities                                                                      
* calculates which values are valid for a cell based on sudoku rules                         
***********************************************************************/
void computePossibilities(int board[][9],int row,int col,int possibilities[])
{
   // check for values in row of coordinate                                                  
   for (int i = 0; i < 9; i++)
      if (board[row][i] != 0)
      {
         int value = board[row][i];
         possibilities[value - 1] = 0;
      }

   // check for values in column of coordinate                                               
   for (int i = 0; i < 9; i++)
      if (board[i][col] != 0)
      {
         int value = board[i][col];
         possibilities[value - 1] = 0;
      }

   // check for values in 3x3 square of coordinate                                           
   if (row < 3) // first row                                                                 
      if (col < 3) // first column                                                           
      {
         for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
               if (board[i][j] != 0)
               {
                  int value = board[i][j];
                  possibilities[value - 1] = 0;
               }
      }
      else if (col < 6) // second column                                                     
      {
         for (int i = 0; i < 3; i++)
            for (int j = 3; j < 6; j++)
               if (board[i][j] != 0)
               {
                  int value = board[i][j];
                  possibilities[value - 1] = 0;
               }
      }
      else // third column                                                                   
      {
         for (int i = 0; i < 3; i++)
            for (int j = 6; j < 9; j++)
               if (board[i][j] != 0)
               {
                  int value = board[i][j];
                  possibilities[value - 1] = 0;
               }
      }

   else if (row < 6) // second row                                                           
      if (col < 3) // first column                                                           
      {
         for (int i = 3; i < 6; i++)
            for (int j = 0; j < 3; j++)
               if (board[i][j] != 0)
               {
                  int value = board[i][j];
                  possibilities[value - 1] = 0;
               }
      }
      else if (col < 6) // second column                                                     
      {
         for (int i = 3; i < 6; i++)
            for (int j = 3; j < 6; j++)
               if (board[i][j] != 0)
               {
                  int value = board[i][j];
                  possibilities[value - 1] = 0;
               }
      }
      else // third column                                                                   
      {
         for (int i = 3; i < 6; i++)
            for (int j = 6; j < 9; j++)
               if (board[i][j] != 0)
               {
                  int value = board[i][j];
                  possibilities[value - 1] = 0;
               }
      }

   else // third row                                                                         
      if (col < 3) // first column                                                           
      {
         for (int i = 6; i < 9; i++)
            for (int j = 0; j < 3; j++)
               if (board[i][j] != 0)
               {
                  int value = board[i][j];
                  possibilities[value - 1] = 0;
               }
      }
      else if (col < 6) // second column                                                     
      {
         for (int i = 6; i < 9; i++)
            for (int j = 3; j < 6; j++)
               if (board[i][j] != 0)
               {
                  int value = board[i][j];
                  possibilities[value - 1] = 0;
               }
      }
      else // third column                                                                   
      {
         for (int i = 6; i < 9; i++)
            for (int j = 6; j < 9; j++)
               if (board[i][j] != 0)
               {
                  int value = board[i][j];
                  possibilities[value - 1] = 0;
               }
      }


} // end computePossibilities                                                                


/**********************************************************************                      
* save quit                                                                                  
* saves board to a file and quits the program                                                
***********************************************************************/
void saveQuit(int board[][9])
{
   cout << "What file would you like to write your board to: ";
   char fileName[256];
   cin >> fileName;

   ofstream fout(fileName); // declare and initialize in one step                            
//   if (fout.fail()) // check for error with fail()                                         

   for (int row = 0; row < 9; row++)
      for (int col = 0; col < 9; col++)
         if (col == 8)
            fout << board[row][col] << endl;
         else
            fout << board[row][col] << " ";

   fout.close(); // don’t forget to close when done                                          

   cout << "Board written successfully\n";
}
