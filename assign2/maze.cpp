// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "grid.h"
#include "maze.h"
#include "mazegraphics.h"
#include "queue.h"
#include "set.h"
#include "stack.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;


/**
 * @brief generateValidMoves - 查找当前位置所有有效的相邻位置
 *
 * 有效条件: 1. 每次只能从当前位置cur移动一步，并且只能沿东西南北(East, West, Sourth, North)这4个方向进行移动。
 *          2. 只能在迷宫所在的区域内移动，不能越界。
 *          3. 只能沿着通道移动，不能穿过墙壁。
 *
 * @param Grid<bool>& maze ------- 迷宫信息
 * @param GridLocation cur ------- 当前位置
 * @return Set<GridLocation> validMoves -------- 当前位置所有有效的相邻位置
 */
Set<GridLocation> generateValidMoves(Grid<bool>& maze, GridLocation cur) {
    //当前位置为墙壁则报错
    if(!maze[cur]){
        error("current location is wall");
    }
    //原始相邻位置
    Set<GridLocation> neighbors;
    //有效相邻位置(必须创建一个新的Set用于存储最终有效的相邻位置，从而避免在下面的循环判断中操作原始相邻位置的set)
    Set<GridLocation> validMoves;
    //当前位置备份
    GridLocation curLocCopy = cur;
    //北
    curLocCopy.row ++;
    neighbors.add(curLocCopy);
    //南
    curLocCopy = cur;
    curLocCopy.row --;
    neighbors.add(curLocCopy);
    //东
    curLocCopy = cur;
    curLocCopy.col ++;
    neighbors.add(curLocCopy);
    //西
    curLocCopy = cur;
    curLocCopy.col --;
    neighbors.add(curLocCopy);
    //只能在迷宫所在的区域内移动，不能越界,且必须是通道
    for(GridLocation neighborLoc: neighbors){
        if(   neighborLoc.row >= 0
           && neighborLoc.col >= 0
           && neighborLoc.row < maze.numRows()
           && neighborLoc.col < maze.numCols()
           && maze[neighborLoc])
        {
            //加入有效相邻位置
            validMoves.add(neighborLoc);
        }
    }
    return validMoves;
}

/**
 * @brief generateValidMoves - 验证所给路径可以顺利走出迷宫, 如果不满足有效条件则报错
 *
 * 有效条件: 1.路径的起点必须位于迷宫入口（左上角）。
 *          2.路径的终点必须位于迷宫出口（右下角）。
 *          3.从当前位置移动到下一位置时，每一步都必须是符合规则的移动。(利用上面完成的generateValidMoves函数来判断当前移动是否符合规则。)
 *          4.路径中不能出现循环，即同一个位置不能走两次，每次都要走没走过的路。(利用Set<GridLocation>这一数据结构来判断当前位置是否已经走过)
 *
 * @param Grid<bool>& maze ------- 迷宫信息
 * @param Stack<GridLocation> ------- 路径
 * @return 无
 */
void validatePath(Grid<bool>& maze, Stack<GridLocation> path) {
    //走过的路径
    Set<GridLocation> visitedPath;
    //迷宫右下角
    GridLocation mazeExit = {maze.numRows()-1,  maze.numCols()-1};
    //如果路径的终点（栈顶）不是迷宫的右下角，则报错
    if (path.peek() != mazeExit) {
        error("Path does not end at maze exit");
    }
    //从路径终点开始向起点验证有效性
    while(path.size() != 1){        
        //当前位置
        GridLocation curLoc = path.pop();
        //如果路径有重复，则报错(尽管solveMaze里去除了重复的位置，但这里添加一道保险)
        if(visitedPath.contains(curLoc)){
            error("Path revisit");
        }
        //不重复，则将走过的位置加入Set中
        visitedPath.add(curLoc);
        //上一步的有效相邻位置
        Set<GridLocation> validMovesOfLastLocation = generateValidMoves(maze, path.peek());
        //如果当前位置不是上一步的有效相邻位置中的一个，则报错
        if(!validMovesOfLastLocation.contains(curLoc)){
             error("Path is not valid");
        }
    }
    //迷宫左上角
    GridLocation mazeEntry = {0,0};
    //如果路径的起点（栈底）不是迷宫左上角，则报错
    if (path.pop() != mazeEntry) {
        error("Path does not_begin_at_entry");
    }
}

/**
 * @brief solveMaze - 基于广度优先搜索，寻找迷宫的最优路径
 * @param Grid<bool>& maze ------- 迷宫信息
 * @return Stack<GridLocation> path ------- 最优路径
 */
Stack<GridLocation> solveMaze(Grid<bool>& maze) {
    MazeGraphics::drawGrid(maze);
    //搜索队列
    Queue<Stack<GridLocation>> todoList;
    //初始位置
    Stack<GridLocation> init = {{0,0}};
    //初始化队列
    todoList.enqueue(init);
    //迷宫右下角
    GridLocation mazeExit = {maze.numRows()-1,  maze.numCols()-1};
    //路径
    Stack<GridLocation> path;
    while(!todoList.isEmpty()){
       //取出搜索队列最上层的路径
       path = todoList.dequeue();
       //图形界面绘制当前的路径
       MazeGraphics::highlightPath(path, "red", 10);
       //如果当前路径已经到达出口, 搜索结束。当前路径就是最短路径。
       if(path.peek() == mazeExit){
           return path;
       }
       //根据当前路进最顶端的位置寻找可以继续前进的位置
       Set<GridLocation> validMoves = generateValidMoves(maze, path.peek());
       //获取当前路径经过的所有位置的集合
       Set<GridLocation> visited = visitedLoc(path);
       //遍历循环
       for(GridLocation elem: validMoves){
           if(!visited.contains(elem)){
               //创建一个临时的path副本
               Stack<GridLocation> pathCopy = path;
               //更新下一步的有效位置
               pathCopy.push(elem);
               //新的路径放入搜索列表
               todoList.enqueue(pathCopy);
           }
       }
    }
    return path;
}
/**
 * @brief solveMaze - 返回当前路径经过的所有位置,由于栈无法遍历查询，转换为集合类型
 * @param Stack<GridLocation> path ------- 当前路径
 * @return Stack<GridLocation> path ------- 以集合的形式返回路径中的所有位置
 */
Set<GridLocation> visitedLoc(Stack<GridLocation> path){
    Set<GridLocation> visited;
    while (!path.isEmpty()) {
        visited.add(path.pop());
    }
    return visited;
}
/*
 * The given readMazeFile function correctly reads a well-formed
 * maze from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readMazeFile(string filename, Grid<bool>& maze) {
    /* The following code reads data from the file into a Vector
     * of strings representing the lines of the file.
     */
    ifstream in;

    if (!openFile(in, filename))
        error("Cannot open file named " + filename);

    Vector<string> lines;
    readEntireFile(in, lines);

    /* Now that the file data has been read into the Vector, populate
     * the maze grid.
     */
    int numRows = lines.size();        // rows is count of lines
    int numCols = lines[0].length();   // cols is length of line
    maze.resize(numRows, numCols);     // resize grid dimensions

    for (int r = 0; r < numRows; r++) {
        if (lines[r].length() != numCols) {
            error("Maze row has inconsistent number of columns");
        }
        for (int c = 0; c < numCols; c++) {
            char ch = lines[r][c];
            if (ch == '@') {        // wall
                maze[r][c] = false;
            } else if (ch == '-') { // corridor
                maze[r][c] = true;
            } else {
                error("Maze location has invalid character: '" + charToString(ch) + "'");
            }
        }
    }
}

/* The given readSolutionFile correctly reads a path from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readSolutionFile(string filename, Stack<GridLocation>& soln) {
    ifstream in;

    if (!openFile(in, filename)) {
        error("Cannot open file named " + filename);
    }

    if (!(in >> soln)) {// if not successfully read
        error("Maze solution did not have the correct format.");
    }
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("generateValidMoves on location in the center of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation center = {1, 1};
    Set<GridLocation> expected = {{0, 1}, {1, 0}, {1, 2}, {2, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}

PROVIDED_TEST("generateValidMoves on location on the side of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation side = {0, 1};
    Set<GridLocation> expected = {{0,0}, {0,2}, {1, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}

PROVIDED_TEST("generateValidMoves on corner of 2x2 grid with walls") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    GridLocation corner = {0, 0};
    Set<GridLocation> expected = {{1, 0}};

    EXPECT_EQUAL(generateValidMoves(maze, corner), expected);
}

PROVIDED_TEST("validatePath on correct solution") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> soln = { {0 ,0}, {1, 0}, {1, 1} };

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution loaded from file for medium maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/5x7.maze", maze);
    readSolutionFile("res/5x7.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution loaded from file for large maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/25x33.maze", maze);
    readSolutionFile("res/25x33.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on invalid path should raise error") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> not_end_at_exit = { {1, 0}, {0, 0} };
    Stack<GridLocation> not_begin_at_entry = { {1, 0}, {1, 1} };
    Stack<GridLocation> go_through_wall = { {0 ,0}, {0, 1}, {1, 1} };
    Stack<GridLocation> teleport = { {0 ,0}, {1, 1} };
    Stack<GridLocation> revisit = { {0 ,0}, {1, 0}, {0, 0}, {1, 0}, {1, 1} };

    EXPECT_ERROR(validatePath(maze, not_end_at_exit));
    EXPECT_ERROR(validatePath(maze, not_begin_at_entry));
    EXPECT_ERROR(validatePath(maze, go_through_wall));
    EXPECT_ERROR(validatePath(maze, teleport));
    EXPECT_ERROR(validatePath(maze, revisit));
}

PROVIDED_TEST("solveMaze on file 5x7") {
    Grid<bool> maze;
    readMazeFile("res/5x7.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("solveMaze on file 21x23") {
    Grid<bool> maze;
    readMazeFile("res/21x23.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}


STUDENT_TEST("generateValidMoves on corner of 2x2 grid with walls") {
    Grid<bool> maze = {{false, false},
                       {true, true}};
    GridLocation cur = {0, 0};
    EXPECT_ERROR(generateValidMoves(maze, cur));
}
STUDENT_TEST("generateValidMoves on corner of 2x2 grid with walls") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    GridLocation cur = {0, 0};
    Set<GridLocation> expected = {{1, 0}};
    EXPECT_EQUAL(generateValidMoves(maze, cur), expected);
}
STUDENT_TEST("generateValidMoves on corner of 3x3 grid with walls") {
    Grid<bool> maze = {{true, false,false},
                       {true, true, true},
                       {false, true, true}};
    GridLocation cur = {1, 1};
    Set<GridLocation> expected = {{1, 0},{1, 2},{2, 1}};
    EXPECT_EQUAL(generateValidMoves(maze, cur), expected);
}
STUDENT_TEST("validatePath of 2x2 grid") {
    Grid<bool> maze = {{true, true},
                       {false, true}};
    Stack<GridLocation> soln = { {0 ,0}, {0, 1}, {1, 1} };
    EXPECT_NO_ERROR(validatePath(maze, soln));
}
STUDENT_TEST("validatePath of 3x3 grid") {
    Grid<bool> maze = {{true,  true,  true},
                       {false, false, true},
                       {false, false, true}};
    Stack<GridLocation> soln = { {0 ,0}, {0, 1}, {0, 2}, {1, 2},{2, 2} };
    EXPECT_NO_ERROR(validatePath(maze, soln));
}
STUDENT_TEST("validatePath on invalid path should raise error") {
    Grid<bool> maze = {{true,  true,  true},
                       {false, false, true},
                       {false, true, true}};
    Stack<GridLocation> not_end_at_exit = { {0, 0}, {0, 1}, {0, 2}, {1, 2}, {2, 2}, {2, 1}};
    Stack<GridLocation> not_begin_at_entry = { {1, 0}, {1, 1}, {1, 2}, {2, 2} };
    Stack<GridLocation> go_through_wall = { {0 ,0}, {0, 1}, {1, 1}, {1, 2}, {2, 2} };
    Stack<GridLocation> teleport = { {0 ,0}, {0, 2}, {1, 2}, {2, 2}};
    Stack<GridLocation> revisit = { {0 ,0}, {0, 1}, {0, 2}, {1, 2}, {0, 2}, {1, 2}, {2, 2}};

    EXPECT_ERROR(validatePath(maze, not_end_at_exit));
    EXPECT_ERROR(validatePath(maze, not_begin_at_entry));
    EXPECT_ERROR(validatePath(maze, go_through_wall));
    EXPECT_ERROR(validatePath(maze, teleport));
    EXPECT_ERROR(validatePath(maze, revisit));
}
STUDENT_TEST("solveMaze on file 25x33") {
    Grid<bool> maze;
    readMazeFile("res/25x33.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}
STUDENT_TEST("solveMaze on file 33x41") {
    Grid<bool> maze;
    readMazeFile("res/33x41.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}
