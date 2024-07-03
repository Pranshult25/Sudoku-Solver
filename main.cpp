#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <math.h>
#include <random>
#include <thread>
#include <chrono>
#include <time.h>
#include <unistd.h>
#include <Windows.h>
using namespace std;
const int n = 9;
int numberOfSolution = 0;
vector<int> values = {1, 2, 3, 4, 5, 6, 7, 8, 9};
void setCursorPosition(int x, int y){
    static const HANDLE h0out = GetStdHandle(STD_OUTPUT_HANDLE);
    cout.flush();
    COORD coord = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(h0out, coord);
}
void sleep(int milliseconds)
{
    usleep(milliseconds * 1000); // usleep takes microseconds
}
void printPuzzle(vector<vector<int>> &puzzle, bool clear = true)
{
    if (clear)
    {
        setCursorPosition(0, 0);
    }   
    std::string text, separator, padding;
    
    for (int i = 0; i < n; i++)
    {
        text = "|";
        separator = " -";
        padding = "|";        
        for (int j = 0; j < n; j++)
        {
            std::string value = puzzle[i][j] == 0 ? " " : std::to_string(puzzle[i][j]);
            text += "  " + value + "  |";
            separator += "------";
            padding += "     |";            
            if (j % 3 == 2 && j != n - 1)
            {
                text += "|";
                padding += "|";
            }
        }
        if (i != 0 && i % 3 == 0)
        {
            std::replace(separator.begin(), separator.end(), '-', '=');
        }        
        std::cout << separator << std::endl;
        std::cout << padding << std::endl;
        std::cout << text << std::endl;
        std::cout << padding << std::endl;
    }
    std::cout << separator << std::endl;
}
bool isValid(vector<vector<int>> &puzzle, int row, int col, int value)
{
    // check in the row if the value is present or not
    for (int c = 0; c < n; c++){
        if (puzzle[row][c] == value)
            return false;
    }    
    // check in the col if the value is present or not
    for (int r = 0; r < n; r++){
        if (puzzle[r][col] == value)
            return false;
    }    
    //check in the current grid if the value is present or not
    int startRow = (row/3)*3, startCol = (col/3)*3;
    for (int r = startRow; r < startRow + 3; r++){
        for (int c = startCol; c < startCol + 3; c++){
            if (puzzle[r][c] == value)
                return false;
        }
    }    
    return true;
}
bool hasEmptyCell(vector<vector<int>> &puzzle){
    for (int r = 0; r < n; r++){
        for (int c = 0; c < n; c++){
            if (puzzle[r][c] == 0)
                return true;
        }
    }    
    return false;
}
// void copyPuzzle(vector<vector<int>> origin, vector<vector<int>> &copy)
// {
//     for (int r = 0; r < n; r++)
//     {
//         for (int c = 0; c < n; c++)
//         {
//             copy[r][c] = origin[r][c];
//         }
//     }
// }
bool fillPuzzle(vector<vector<int>> &puzzle)
{
    int row, col;
    for(int i{0}; i<n*n; i++){
        row = floor(i/n);
        col = i%n;        
        if (puzzle[row][col] == 0)
        {
            unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
            std::shuffle(values.begin(), values.end(), std::default_random_engine(seed));            
            for (int j = 0; j < n; j++){
                if(isValid(puzzle, row, col, values[j])){
                    puzzle[row][col] = values[j];                    
                    if (!hasEmptyCell(puzzle) || fillPuzzle(puzzle))
                    {
                        return true;
                    }
                }
            }
            break;
        }
    }    
    puzzle[row][col] = 0;    
    return false;
}
bool solveSudoku(vector<vector<int>> puzzle, bool visualize = false){
    int row, col;    
    for (int i{0}; i<n*n; i++)
    {
        row = i/n;
        col = i % n;        
        if (puzzle[row][col] == 0){
            for (int value = 1; value <= n; value++){
                if (isValid(puzzle, row, col, value)){
                    puzzle[row][col] = value;                    
                    if (visualize){
                        sleep(100);
                        printPuzzle(puzzle);
                    }                    
                    if(!hasEmptyCell(puzzle)){
                        numberOfSolution++;                        
                        if (visualize){
                            sleep(100);
                            printPuzzle(puzzle);
                            return true;
                        }                        
                        break;
                    }
                    else if(solveSudoku(puzzle, visualize)){
                        return true;
                    }
                }
            }            
            break;
        }
    }    
    puzzle[row][col] = 0;    
    if (visualize)
    {
        sleep(100);
        printPuzzle(puzzle);
    }    
    return false;
}
void generatePuzzle(vector<vector<int>> &puzzle, int difficulty = 1)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            puzzle[i][j] = 0;
        }
    }    
    fillPuzzle(puzzle);  
    // after filling all the cells remove random elements from the cells
       
    srand((unsigned)time(0));
    int attempt = difficulty;    
    while (attempt > 0){
        // choose random row and columns using rand()
        int row = floor(rand() % n);
        int col = floor(rand() % n);        
        while (puzzle[row][col] == 0)
        {
            row = floor(rand() % n);
            col = floor(rand() % n);
        }        
        int val = puzzle[row][col];
        puzzle[row][col] = 0;        
        numberOfSolution = 0;
        solveSudoku(puzzle);        
        // if by removing this cell, if the sudoku have no sol or more than one sol then we cannot remove
        // this cell
        if (numberOfSolution != 1)
        {
            puzzle[row][col] = val;
            attempt--;
        }
    }
}
int main(int , char **){
    vector<vector<int>> puzzle(n, vector<int> (n));
    generatePuzzle(puzzle);
    printPuzzle(puzzle, true);
    cout << "Press Yes(y) if you want to solve the sudoku or else No(n) ";
    char run; cin >> run;
    if(run == 'n'){
        cout.flush();
        return 0;
    }
    sleep(100000);
    solveSudoku(puzzle, true);
    // // printPuzzle(origin, false);
    cout.flush();
    return 0;
}