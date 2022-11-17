#include "labyrinth.h"
#include "demo/MazeGenerator.h"
#include "testing/SimpleTest.h"
#include <string>
using namespace std;

/* Change kYourName constant to your full name.
 *
 * WARNING: Once you've set set this constant and started exploring your labyrinth,
 * do NOT edit the value of kYourName. Changing kYourName will change which
 * maze you get back, which might invalidate all your hard work!
 */
const string kYourName = "Chenyu";

/* Change this constant to the path out of your labyrinth. */
const string kPathOutOfNormalMaze = "NNESEESSNNWWSESNWSW";

/**
 * @brief generateNextStep() -  根据当前位置和输入字符　给出　下一步的位置
 * 备注：输入字符为如果＂NSEW＂以外的字符，则报错
 * @param const MazeCell *cur - 当前位置指针
 * @param char ch - 输入字符
 * @return MazeCell* next - 下一步位置的指针
 */
MazeCell* generateNextStep(const MazeCell *cur, char ch) {
    MazeCell* next;
    string validDirections = "NSEW";
    if(ch == 'N')
        next = cur->north;
    if(ch == 'S')
        next = cur->south;
    if(ch == 'E')
        next = cur->east;
    if(ch == 'W')
        next = cur->west;
    //输入字符为如果＂NSEW＂以外的字符，则报错
    if(validDirections.find(ch) == string::npos){
        error("路径包含非法字符");
    }
    return next;
}

/**
 * @brief isPathToFreedom() -  验证给出的路径是否能走出迷宫
 * 迷宫规则：１．路径是合法的。每一步都是从当前单元格出发的有效方向。
 *          ２．走过的路径必须能够收集所有物品
 *          ３．收集完所有物品后，路径中剩余的任何步骤都将被忽略。
 * @param const MazeCell *cur - 当前位置指针
 * @param char ch - 输入字符
 * @return MazeCell* next - 下一步位置的指针
 */
bool isPathToFreedom(MazeCell* start, string moves, Set<string> needs) {
    //初始化当前位置
    MazeCell* curLoc = start;
    //检查起点的contents是否在needs中，如果在，则将其删除
    if(needs.contains(curLoc->contents)){
        needs.remove(curLoc->contents);
    }
    //遍历路径字符串
    for(char ch: moves){
        //更新当前位置
        curLoc = generateNextStep(curLoc, ch);
        //如果当前位置的指针为空，则返回false
        if(curLoc == nullptr){
            return false;
        }
        //检查当前位置的contents是否在needs中，如果在，则将其删除
        if(needs.contains(curLoc->contents)){
            needs.remove(curLoc->contents);
        }
        //如果needs为空，说明已经取到了所有的needs，返回true
        if(needs.isEmpty()){
            return true;
        }
    }
    //以上均不满足，返回false
    return false;
}


/* * * * * * Test Cases Below This Point * * * * * */

PROVIDED_TEST("Check paths in the sample from writeup") {
    Set<string> allThree = {"Spellbook", "Potion", "Wand"};
    auto maze = toMaze({"* *-W *",
                        "| |   |",
                        "*-* * *",
                        "  | | |",
                        "S *-*-*",
                        "|   | |",
                        "*-*-* P"});

    /* These paths are the ones in the handout. They all work. */
    EXPECT(isPathToFreedom(maze[2][2], "ESNWWNNEWSSESWWN", allThree));
    EXPECT(isPathToFreedom(maze[2][2], "SWWNSEENWNNEWSSEES", allThree));
    EXPECT(isPathToFreedom(maze[2][2], "WNNEWSSESWWNSEENES", allThree));

    /* This path works to pick up just potion */
    EXPECT(isPathToFreedom(maze[2][2], "ESNW", {"Potion"}));

    /* This path doesn't work, since doesn't pick up all items. */
    EXPECT(!isPathToFreedom(maze[2][2], "ESNW", allThree));

    /* This path doesn't work, cannot walk through walls. */
    EXPECT(!isPathToFreedom(maze[2][2], "NNWWSSSEEE", allThree));
}

PROVIDED_TEST("Works when start on an item") {
    auto maze = toMaze({"P-S-W"});

    EXPECT(isPathToFreedom(maze[0][0], "E", {"Potion"}));
    EXPECT(isPathToFreedom(maze[0][1], "W", {"Potion", "Spellbook"}));
}

PROVIDED_TEST("Reports errors if given illegal characters") {
    auto maze = toMaze({"P-S-W"});

    EXPECT_ERROR(isPathToFreedom(maze[0][0], "Q", {"Wand"}));
    EXPECT_ERROR(isPathToFreedom(maze[0][0], "Ee", {"Wand"}));
}

PROVIDED_TEST("Escape from your personal labyrinth!") {
    Set<string> allThree = {"Spellbook", "Potion", "Wand"};
    /* A maze for you to escape from. This maze will be personalized
     * based on the constant kYourName.
     */
    EXPECT(kYourName != "TODO");
    MazeCell* startLocation = mazeFor(kYourName);

    /* Set a breakpoint here. As a reminder, the labyrinth you
     * get will be personalized to you, so don't start exploring the labyrinth
     * unless you've edited the constant kYourName to include your name(s)!
     * Otherwise, you'll be escaping the wrong labyrinth.
     */
    EXPECT(kPathOutOfNormalMaze != "TODO");
    //迷宫图见(res/maze.png)!!!
    EXPECT(isPathToFreedom(startLocation, kPathOutOfNormalMaze, allThree));
    //其它正确路径
    EXPECT(isPathToFreedom(startLocation, "NNESSSWENESNWNEESS", allThree));
    EXPECT(isPathToFreedom(startLocation, "NNESSESNWSWENNEESS", allThree));
    //错误路径
    EXPECT(!isPathToFreedom(startLocation, "NNESEENSNNWNSESNWSW", allThree));
    //非法字符
    EXPECT_ERROR(isPathToFreedom(startLocation, "NNE!EESSNNWWSESNWS!", allThree));
}

