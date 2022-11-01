/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "backtracking.h"
#include "gridlocation.h"
#include "grid.h"
#include "set.h"
#include "lexicon.h"
#include "testing/SimpleTest.h"
using namespace std;

/**
 * @brief points - 根据单词长度进行记分(默认单词长度大于３)
 * 记分规则:  1分 - 单词长度为4
 *           2分 - 单词长度为5
 *           3分 - 单词长度为6   以此类推
 * @param string str - 输入的单词
 * @return int str.size() - 3  ---- 根据记分规则返回相应的分数
 */
int points(string str) {
    return str.size() - 3;
}

/**
 * @brief scoreBoardRec -  根据boggle规则拼单词
 * boggle规则:  1.单词必须由相邻字母组成（对角线上的也算）
 *              2.单词不能使用重复的立方体（不能走回头路）
 *              3.单词必须在有效的英语单词列表中
 *              4.单词不能重复记分，每个单词只记分一次。
 * 递归类型：排列 +  回溯
 * 递归范式：Choose/Explore/Unchoose
 * @param Grid<char>& board - 拼字盘
 * @param Lexicon& lex - 有效英语单词列表
 * @param Vector<GridLocation> &path - 当前路径(注意这里用了vector容器是因为需要根据路径查表得到路径指代的单词，而迷宫中不需要所以用stack就行)
 * @param Set<string> &allFindedWords - 所有已经找到的单词
 * @param int &score - 总分
 * @return void
 * @心得: 迷宫和boggle的不同之处
              迷宫:1.递归终止条件就是目标 　因此仅需　　　过程结束满足
                        　              判断位置： （Base case中）
                   ２.需要bool类型的返回值　来在循环内部　判断当前分支是否能继续走下去
                   ３.需要visited来记录所有已经走过的位置

            boggle:1.递归终止条件不是目标　需要实时判断 　即　过程结束满足  +       过程中满足 　　　　　+   过程中不满足（起优化作用，不满足前置条件就不走这个分支）
                        　                    判断位置：（Base case中）（Recursive case中,循环前）   (Base case前)
                 　２．不需要bool类型的返回值，而是通过Base case决定当前分支是否还能走下去
                　 ３．只需要当前路径不出现重复位置，不需要visited来记录所有已经走过的位置
 */
void scoreBoardRec(Grid<char>& board, const Lexicon& lex, Vector<GridLocation> &path, Set<string> &allFindedWords, int &score) {
    //根据当前路径查表得到路径指代的单词
    string word = vecToString(board, path);
    //过程中不满足（这是为了优化而添加的约束条件，实际上一个分支只有找不到下一步的有效位置才结束．因此，这里是过程中不满足）
    if(!lex.containsPrefix(word)){
        return;
    }
    //查找下一步的有效位置
    auto neighbors = generateValidMoves(board, path);
    //1. Base case
    //如果下一步的有效位置为空，说明当前分支走到了尽头，需要return回上一层
    if(neighbors.isEmpty()){
        //过程结束满足  (注意在分支达到终点时仍要判断是否满足设定的目标！！！！)
        //只有当前单词长度大于3 且 在有效单词列表中 且 不是已经找到过的单词　才进行记分 并 加入已找到的单词列表中
        if(word.size() > 3 && lex.contains(word) && !allFindedWords.contains(word)){
            score += points(word);
            allFindedWords.add(word);
        }
        return;
    }
    else {
        //过程中满足（注意满足判断一般放在范式之前)
        //只有当前单词长度大于3 且 在有效单词列表中 且 不是已经找到过的单词　才进行记分 并 加入已找到的单词列表中）
        if(word.size() > 3 && lex.contains(word) && !allFindedWords.contains(word)){
            score += points(word);
            allFindedWords.add(word);
        }
        //2. Recursive case
        for(const GridLocation &next :neighbors){
            //2.1 Choose
            path.add(next);
            //2.2 Explore
            scoreBoardRec(board, lex, path, allFindedWords, score);
            //2.3 Unchoose
            path.remove(path.size() - 1);
        }
    }
    //所有当前位置衍生出的分支都走到了尽头 则 再返回上一层
    return;
}

/**
 * @brief scoreBoard - scoreBoardRec的 包装函数
 * @param Grid<char>& board - 拼字盘
 * @param Lexicon& lex - 有效英语单词列表
 * @return int score  - 拼字盘中所有有效单词的总分
 */
int scoreBoard(Grid<char>& board, Lexicon& lex) {
    int score = 0;
    //所有已经找到的单词
    Set<string> allFindedWords = {};
    //嵌套循环以从拼字盘不同的起点出发开始探索
    for(int i = 0; i < board.numRows(); i++){
        for(int j = 0; j < board.numCols(); j++){
            //初始化路径
            Vector<GridLocation> initPath = {{GridLocation(i,j)}};
            scoreBoardRec(board, lex, initPath, allFindedWords, score);
        }
    }
    return score;
}

/**
 * @brief vecTostring -  根据当前的路径查表得到路径指代的单词
 * @param Grid<char>& board - 拼字盘
 * @param Vector<GridLocation> &path - 路径
 * @return string str  - 查表得到的单词
 */
string vecToString(Grid<char>& board, Vector<GridLocation> &path){
    string str = "";
    for(int i = 0; i < path.size(); i++){
        str += board[path[i]];
    }
    return str;
}

/**
 * @brief vectorToSet -  把存储路径的Vector转化成Set　以方便后面用contains函数
 * @param const Vector<GridLocation> &path - 路径
 * @return Set<GridLocation> pathCopy  - 转化成Set后的路径
 */
Set<GridLocation> vectorToSet(const Vector<GridLocation> &path){
    Set<GridLocation> pathCopy;
    for(int i = 0; i < path.size(); i++){
        pathCopy.add(path[i]);
    }
    return pathCopy;
}

/**
 * @brief generateValidMoves -  根据拼字盘和当前位置寻找下一步的有效位置
 * 有效条件: 1. 每次只能从当前位置移动一步，并且只能沿上，下，左，右，左上角，左下角，右上角，右下角这8个方向进行移动。
 *          2. 只能在拼字盘内部移动，不能越界。
 *          3. 下一步移动的位置不能已经出现在当前路径中
 * @param Grid<char>& board - 拼字盘
 * @param const Vector<GridLocation> &path - 当前路径
 * @return Set<GridLocation> neighbors  - 所有相邻的有效字
 */
Set<GridLocation> generateValidMoves(Grid<char> &board, const Vector<GridLocation> &path) {
    Set<GridLocation> neighbors;
    GridLocation curLoc = path[path.size() - 1];
    Set<GridLocation> pathCopy = vectorToSet(path);
    //不用担心(i = 0, j =0)的情况，因为它包含在pathCopy中，所以判断不会通过
    for(int i = -1; i <= 1; i++){
        for(int j = -1; j <= 1; j++){
               GridLocation next = GridLocation(curLoc.row + i, curLoc.col + j);
               //只能在拼字盘内部移动，不能越界 且 下一步移动的位置不能已经出现在当前路径中
               if(board.inBounds(next) && !pathCopy.contains(next))
                  neighbors.add(next);
        }
    }
    return neighbors;
}



/* * * * * * Test Cases * * * * * */

/* Test helper function to return shared copy of Lexicon. Use to
 * avoid (expensive) re-load of word list on each test case. */
static Lexicon& sharedLexicon() {
    static Lexicon lex("res/EnglishWords.txt");
    return lex;
}

PROVIDED_TEST("Load shared Lexicon, confirm number of words") {
    Lexicon lex = sharedLexicon();
    EXPECT_EQUAL(lex.size(), 127145);
}

PROVIDED_TEST("Test point scoring") {
    EXPECT_EQUAL(points("and"), 0);
    EXPECT_EQUAL(points("quad"), 1);
    EXPECT_EQUAL(points("quint"), 2);
    EXPECT_EQUAL(points("sextet"), 3);
    EXPECT_EQUAL(points("seventh"), 4);
    EXPECT_EQUAL(points("supercomputer"), 10);
}

PROVIDED_TEST("Test scoreBoard, board contains no words, score of zero") {
    Grid<char> board = {{'B','C','D','F'}, //no vowels, no words
                        {'G','H','J','K'},
                        {'L','M','N','P'},
                        {'Q','R','S','T'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 0);
}

PROVIDED_TEST("Test scoreBoard, board contains one word, score of 1") {
    Grid<char> board = {{'C','_','_','_'},
                        {'Z','_','_','_'},
                        {'_','A','_','_'},
                        {'_','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

PROVIDED_TEST("Test scoreBoard, alternate paths for same word, still score of 1") {
    Grid<char> board = {{'C','C','_','_'},
                        {'C','Z','C','_'},
                        {'_','A','_','_'},
                        {'R','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

PROVIDED_TEST("Test scoreBoard, small number of words in corner of board") {
    Grid<char> board = {{'L','I','_','_'},
                        {'M','E','_','_'},
                        {'_','S','_','_'},
                        {'_','_','_','_'}};
    Set<string> words = {"SMILE", "LIMES", "MILES", "MILE", "MIES", "LIME", "LIES", "ELMS", "SEMI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()),  2 + 2 + 2 + 1 + 1 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, full board, small number of words") {
    Grid<char> board = {{'E','Z','R','R'},
                        {'O','H','I','O'},
                        {'N','J','I','H'},
                        {'Y','A','H','O'}};
    Set<string> words = { "HORIZON", "OHIA", "ORZO", "JOHN", "HAJI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 4 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, full board, medium number of words") {
    Grid<char> board = {{'O','T','H','X'},
                        {'T','H','T','P'},
                        {'S','S','F','E'},
                        {'N','A','L','T'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 76);
}

PROVIDED_TEST("Test scoreBoard, full board, large number of words") {
    Grid<char> board = {{'E','A','A','R'},
                        {'L','V','T','S'},
                        {'R','A','A','N'},
                        {'O','I','S','E'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 234);
}

STUDENT_TEST("Test scoreBoard, Eempty board") {
    Grid<char> board = {{}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 0);
}
