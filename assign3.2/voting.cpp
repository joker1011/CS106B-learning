// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "voting.h"
#include "testing/SimpleTest.h"
using namespace std;

#define version_DynamicPorcgramming

#ifdef version_DynamicPorcgramming
/**
 * Thanks for CS Dojo of the Dynamic Programming tutorials!!!
 * Link: https://www.youtube.com/watch?v=nqlNzOcnCfs
 *
 * @brief computePowerIndexesRec -  计算某被排除区块的关键选票
 * 关键票计数规则:  对于某一个联盟（子集）- 如果不加上排除区块的选票，联盟就会失败　但　加上排除区块的选票联盟就会胜利．
 *                                    则被排除区块是该联盟的关键选票(关键选票+1)
 * 递归类型：子集　+ 回溯
 * 递归范式：include/exclude
 * @param Vector<int>& blocks - 每个区块的票数组成的数组
 * @param const int &excludeBlock - 排除区块的序号(blocks索引)
 * @param const int &totalVotes - 大选总票数
 * @param int index - 当前序号(blocks索引)
 * @param int curCoalitionVotes - 当前联盟的总票数
 * @param int keyVotes - 排除区块的关键票数
 * @param Map<string,int> &mem - 动态规划缓存
 * @return int keyVotes - 排除区块的关键票数
 * @心得: 子集问题是不断２分的复杂问题，其原始的算法复杂度为O(2^n).因此，当集合的规模越大，衍生出的分支越多，计算时间呈指数上升．
 *        为了简化该问题，在探索每个分支时可以设立一个缓存，用于将（（当前状态＋分支的层级），已求得的变量）存储起来．
 *        这样，一旦我们达到　相同的分支层级　且　具有以前一样的状态．那么没必要将分支继续衍生下去，直接根据（当前状态＋分支的层级）查缓存得到所求变量
 */
int computePowerIndexesRec(Vector<int>& blocks, const int &excludeBlock, const int &totalVotes, int index, int curCoalitionVotes, int keyVotes, Map<string,int> &mem){
    //将当前联盟的总票数和index转化成用于动态规划索引的key(用字符串的形式不会出错)
    string key = to_string(curCoalitionVotes) + ':' + to_string(index);
    //如果在运行到某一分支的某个节点时，发现相同curCoalitionVotes + index组合的关键票数之前计算过，
    //那么这个分支不必继续进行下去．其结果必然和之前的相同，直接返回Ｍap缓存内的关键票数就行
    if(mem.containsKey(key)){
        return mem.get(key);
    }
    //过程中不满足  一旦发现无论排除区块是否参与，联盟都将获胜，就没有必要进一步探索这条分支。目标区块不可能成为后续一系列分支中形成的联盟的关键选票
    if(curCoalitionVotes > totalVotes / 2){
        return 0;
    }
    //1. Base case
    //过程结束满足 (过程结束:只有index达到blocks数组末端　或者　（被排除区块是最后一个元素　且　index达到blocks数组倒数第二位)）
    if(index == blocks.size() - 1 || (excludeBlock == blocks.size() - 1 && index == blocks.size() - 2)){
        //如果不加上排除区块的选票，联盟就会失败　但　加上排除区块的选票联盟就会胜利, 则被排除区块是该联盟的关键选票(关键选票+1)
        if(curCoalitionVotes <= totalVotes / 2 && curCoalitionVotes + blocks[excludeBlock] > totalVotes / 2){
             return 1;
        }
    } else {
        //2. Recursive case
        //2.1 Reducing problems
        index ++;
        //如果发现当前的序号是被排除区块的序号，则当前序号++略过
        if(index == excludeBlock)
            index ++;
        //2.2 include + exclude
        //注意相加的形式
        keyVotes = computePowerIndexesRec(blocks, excludeBlock, totalVotes, index, curCoalitionVotes + blocks[index], keyVotes, mem)
                 + computePowerIndexesRec(blocks, excludeBlock, totalVotes, index, curCoalitionVotes, keyVotes, mem);
        //更新动态缓存
        mem.put(key, keyVotes);
    }
    //最后记得返回关键票数
    return keyVotes;
}

/**
 * @brief computePowerIndexes -  computePowerIndexesRec的包装函数(计算所有区块的影响力指数)
 * @param Vector<int>& blocks - 每个区块的票数组成的数组
 * @return Vector<int> result - 所有区块的影响力指数
 */
Vector<int> computePowerIndexes(Vector<int>& blocks){
    //所有票数和所有关键票数
    int totalVotes = 0, totalKeyVotes = 0;
    //包装函数的动态缓存，用于存储已经得到的（区块选票数，区块关键票数）的组合
    Map<int,int> keyVotesList;
    //递归函数的动态缓存
    Map<string,int> mem;
    //所有区块的影响力指数
    Vector<int> result;
    //计算大选总票数
    for(int i = 0; i < blocks.size(); i++){
        totalVotes += blocks[i];
    }
    //循环计算每个区块的关键票数
    for(int j = 0; j < blocks.size(); j++){
        //初始化关键票数
        int keyVotes = 0;
        //当循环出现选票数一样区块时，直接查动态缓存就行，不用进入递归函数
        if(keyVotesList.containsKey(blocks[j])){
            keyVotes = keyVotesList.get(blocks[j]);
        } else {
            //如果没出现，则进入递归函数
            //注意这里初始index为-1,因为在recusive case中我们先index++，再进入的递归函数!!!
            keyVotes = computePowerIndexesRec(blocks, j, totalVotes, -1, 0, 0, mem);
            //更新包装函数的动态缓存
            keyVotesList.put(blocks[j], keyVotes);
            //清空递归函数的动态缓存!!!
            mem.clear();
        }
        //当前区块的关键票数加入result
        result.add(keyVotes);
    }
    //计算所有区块的关键票数之和
    for(int k = 0; k < blocks.size(); k++){
        totalKeyVotes += result[k];
    }
    //影响力指数换算
    for(int m = 0; m < blocks.size(); m++){
        result[m] = (100 * result[m]/ totalKeyVotes);
    }
    return result;
}
#endif

//原始方法，算法复杂度O(2^n)
#ifdef version_original
void computePowerIndexesRec(Vector<int>& blocks, const int &excludeBlock, const int &totalVotes, int index, int curCoalitionVotes, int &keyVotes)
{
    if(curCoalitionVotes > totalVotes / 2){
        return;
    }
    //1.Base case
    if(index == blocks.size() - 1 || (excludeBlock == blocks.size() - 1 && index == blocks.size() - 2)){
        if(curCoalitionVotes <= totalVotes / 2 && curCoalitionVotes + blocks[excludeBlock] > totalVotes / 2){
             keyVotes ++;
        }
    } else {
        //2. Recursive case
        //2.1 Reducing problems
        index ++;
        if(index == excludeBlock){
            index ++;
        }
        //2.1 Include
        computePowerIndexesRec(blocks, excludeBlock, totalVotes, index, curCoalitionVotes + blocks[index], keyVotes);
        //2.2 Exclude
        computePowerIndexesRec(blocks, excludeBlock, totalVotes, index, curCoalitionVotes, keyVotes);
    }
    return;
}

Vector<int> computePowerIndexes(Vector<int>& blocks)
{
    int totalVotes = 0, totalKeyVotes = 0;
    Map<int,int> keyVotesList;
    Vector<int> result;

    for(int i = 0; i < blocks.size(); i++){
        totalVotes += blocks[i];
    }

    for(int j = 0; j < blocks.size(); j++){
        int keyVotes = 0;
        if(keyVotesList.containsKey(blocks[j])){
            keyVotes = keyVotesList.get(blocks[j]);
        } else {
            computePowerIndexesRec(blocks, j, totalVotes, -1, 0, keyVotes);
            keyVotesList.put(blocks[j], keyVotes);
        }
        result.add(keyVotes);
    }
    for(int k = 0; k < blocks.size(); k++){
        totalKeyVotes += result[k];
    }
    for(int m = 0; m < blocks.size(); m++){
        result[m] = (100 * result[m]/ totalKeyVotes);
    }
    return result;
}
#endif

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("Test power index, blocks 50-49-1") {
    Vector<int> blocks = {50, 49, 1};
    Vector<int> expected = {60, 20, 20};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks Hempshead 1-1-3-7-9-9") {
    Vector<int> blocks = {1, 1, 3, 7, 9, 9};
    Vector<int> expected = {0, 0, 0, 33, 33, 33};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks CA-TX-NY 55-38-39") {
    Vector<int> blocks = {55, 38, 29};
    Vector<int> expected = {33, 33, 33};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks CA-TX-GA 55-38-16") {
    Vector<int> blocks = {55, 38, 16};
    Vector<int> expected = {100, 0, 0};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks EU post-Nice") {
    // Estonia is one of those 4s!!
    Vector<int> blocks = {29,29,29,29,27,27,14,13,12,12,12,12,12,10,10,10,7,7,7,7,7,4,4,4,4,4,3};
    Vector<int> expected = {8, 8, 8, 8, 7, 7, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
    TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
}

PROVIDED_TEST("Time power index operation") {
    Vector<int> blocks;
    for (int i = 0; i < 15; i++) {
        blocks.add(randomInteger(1, 10));
    }
    TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
}

STUDENT_TEST("Time power index operation, 票数重合度高，重复元素非常多") {
    for (int n = 2; n <= 28; n += 2){
        Vector<int> blocks;
        for (int i = 0; i < n; i++) {
            blocks.add(randomInteger(1, 5));
        }
        TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
    }
}

STUDENT_TEST("Time power index operation, 票数重合度适中，重复元素占一半") {
    for (int n = 2; n <= 28; n += 2){
        Vector<int> blocks;
        for (int i = 0; i < n; i++) {
            blocks.add(randomInteger(1, n / 2));
        }
        TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
    }
}

STUDENT_TEST("Time power index operation, 票数重合度非常低，重复元素几乎没有") {
    for (int n = 2; n <= 28; n += 2){
        Vector<int> blocks;
        for (int i = 0; i < n; i++) {
            blocks.add(randomInteger(1, 5 * n));
        }
        TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
    }
}

