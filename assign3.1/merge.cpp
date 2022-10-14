/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include "queue.h"
#include "testing/SimpleTest.h"
using namespace std;

/**
 * @brief rankCheck - 检查队列是否严格从小到大排列
 *
 * @param Queue<int> q ------- 输入的队列
 * @return void
 */
void rankCheck(Queue<int> q){
    //当序队列大于1个元素时
    while(q.size() > 1) {
        //如果前面的元素大于后面的元素，则报错
        if(q.dequeue()> q.peek()) {
            error("队列元素排序错误!");
        }
    }
}

/**
 * @brief binaryMerge - 将两个独立的有序序列合并成一个更大的有序序列
 *
 * @param Queue<int> a, Queue<int> b ------- 两个独立的有序序列
 * @return Queue<int> result ------- 合并后的有序队列
 */
Queue<int> binaryMerge(Queue<int> a, Queue<int> b) {
    Queue<int> result;
    rankCheck(a);
    rankCheck(b);
    //当两个队列都不为空时
    while(!a.isEmpty() && !b.isEmpty()) {
        //分别peek a,b队列最前面的元素. 如果a的元素小于b的元素
        if(a.peek() <= b.peek()){
            //a队列元素入result
            result.enqueue(a.dequeue());
        }
        //否则b队列元素入result
        else{
            result.enqueue(b.dequeue());
        }
    }
    //如果a队列不为空，则将剩下的元素继续推入result
    while(!a.isEmpty()){
        result.enqueue(a.dequeue());
    }
    //如果b队列不为空，则将剩下的元素继续推入result
    while(!b.isEmpty()){
        result.enqueue(b.dequeue());
    }
    return result;
}

/*
 * This function assumes correct functionality of the previously
 * defined binaryMerge function and makes use of this function to
 * iteratively merge a collection of sequences, starting off with
 * an empty sequence and progressively merging in one sequence at
 * a time. This function is provided for you fully implemented –
 * it does not need to modified at all.
 */
Queue<int> naiveMultiMerge(Vector<Queue<int>>& all) {
    Queue<int> result;

    for (Queue<int>& q : all) {
        result = binaryMerge(q, result);
    }
    return result;
}

/**
* @brief recMultiMerge - 基于二分法和递归将多个有序的数列合并为一个有序数列
*
* 递归类型：tree Recursion
*
* @param Vector<Queue<int>>& all ------- 存放所有独立的有序数组
* @param int start, int end ------- 起始和终止位置标记
* @param Queue<int> &result -------- 最后的结果
* @return Base Case ------- all[start]  Recursive Case ------- binaryMerge(left, right) 合并后的序列
*/
Queue<int> recMultiMerge(Vector<Queue<int>>& all, int start ,int end, Queue<int> &result) {
    //Base Case
    //二分后，单侧仅含一个序列，则直接返回该序列
    if(end - start == 1){
        return all[start];
    }
    //Recursive Case
    //计算二分点
    int mid = (start + end) / 2;
    //通过递归调用不断二分多个序列，直到满足Base Case条件
    auto left = recMultiMerge(all, start, mid, result);
    auto right = recMultiMerge(all, mid, end, result);
    //此时的left和right均是独立的有序序列，可以直接调用binaryMerge函数合并
    //合并之后它又成为一个独立的有序序列，返回上一层和另一个合并后的有序序列合并
    return binaryMerge(left, right);
}

//包装函数
Queue<int> recMultiMerge(Vector<Queue<int>>& all) {
    Queue<int> result = recMultiMerge(all, 0, all.size(), result);
    return result;
}


/* * * * * * Test Cases * * * * * */

Queue<int> createSequence(int size);
void distribute(Queue<int> input, Vector<Queue<int>>& all);

PROVIDED_TEST("binaryMerge, two short sequences") {
    Queue<int> a = {2, 4, 5};
    Queue<int> b = {1, 3, 3};
    Queue<int> expected = {1, 2, 3, 3, 4, 5};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}

PROVIDED_TEST("naiveMultiMerge, small collection of short sequences") {
    Vector<Queue<int>> all = {{3, 6, 9, 9, 100},
                             {1, 5, 9, 9, 12},
                             {5},
                             {},
                             {-5, -5},
                             {3402}
                            };
    Queue<int> expected = {-5, -5, 1, 3, 5, 5, 6, 9, 9, 9, 9, 12, 100, 3402};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
}

PROVIDED_TEST("recMultiMerge, compare to naiveMultiMerge") {
    int n = 20;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(n);
    distribute(input, all);
    EXPECT_EQUAL(recMultiMerge(all), naiveMultiMerge(all));
}

PROVIDED_TEST("Time binaryMerge operation") {
    int n = 1000000;
    Queue<int> a = createSequence(n);
    Queue<int> b = createSequence(n);
    TIME_OPERATION(a.size() + b.size(), binaryMerge(a, b));
}

PROVIDED_TEST("Time naiveMultiMerge operation") {
    int n = 11000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), naiveMultiMerge(all));
}
PROVIDED_TEST("Time recMultiMerge operation") {
    int n = 90000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), recMultiMerge(all));
}

STUDENT_TEST("binaryMerge, two short sequences") {
    Queue<int> a = {};
    Queue<int> b = {};
    Queue<int> expected = {};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}

STUDENT_TEST("binaryMerge, two short sequences") {
    Queue<int> a = {};
    Queue<int> b = {1, 3, 5};
    Queue<int> expected = {1, 3, 5};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}

STUDENT_TEST("binaryMerge, two short sequences") {
    Queue<int> a = {7, 5, 9};
    Queue<int> b = {1, 5, 3};
    Queue<int> expected = {1, 3, 5, 7, 9};
    EXPECT_ERROR(binaryMerge(a, b));
}

STUDENT_TEST("naiveMultiMerge, empty sequences") {
    Vector<Queue<int>> all = {{}};
    Queue<int> expected = {};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
}

STUDENT_TEST("naiveMultiMerge, mutiple empty sequences") {
    Vector<Queue<int>> all = {{},{},{},{}};
    Queue<int> expected = {};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
}

STUDENT_TEST("naiveMultiMerge, mutiple short sequences") {
    Vector<Queue<int>> all = {{1,3,9,12},
                              {2,4,8,17},
                              {15,18,25,29},
                              {},
                              {30},
                              {28,32,35,69},};
    Queue<int> expected = {1,2,3,4,8,9,12,15,17,18,25,28,29,30,32,35,69};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
}

STUDENT_TEST("recMultiMerge, compare to naiveMultiMerge") {
    Vector<Queue<int>> all = {{1,3,9,12},
                              {2,4,8,17},
                              {15,18,25,29},
                              {},
                              {30},
                              {28,32,35,69},};
    EXPECT_EQUAL(recMultiMerge(all), naiveMultiMerge(all));
}

/* Test helper to fill queue with sorted sequence */
Queue<int> createSequence(int size) {
    Queue<int> q;
    for (int i = 0; i < size; i++) {
        q.enqueue(i);
    }
    return q;
}

/* Test helper to distribute elements of sorted sequence across k sequences,
   k is size of Vector */
void distribute(Queue<int> input, Vector<Queue<int>>& all) {
    while (!input.isEmpty()) {
        all[randomInteger(0, all.size()-1)].enqueue(input.dequeue());
    }
}

