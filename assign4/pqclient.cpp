#include "pqclient.h"
#include "pqarray.h"
#include "pqheap.h"
#include "vector.h"
#include "strlib.h"
#include <sstream>
#include "testing/SimpleTest.h"
using namespace std;

/**
 * Given a Vector of DataPoints, modify the vector to re-arrange the
 * elements into increasing order by priority.
 *
 * The expected Big O runtime of pqSort is
 *   N*(O(enqueue) + O(dequeue)) where enqueue/dequeue for PQueue of size N
 */
void pqSort(Vector<DataPoint>& v) {
//    PQArray pq;
    PQHeap pq;
    /* Using the Priority Queue data structure as a tool to sort, neat! */

    /* Add all the elements to the priority queue. */
    for (int i = 0; i < v.size(); i++) {
        pq.enqueue(v[i]);
    }

    /* Extract all the elements from the priority queue. Due
     * to the priority queue property, we know that we will get
     * these elements in sorted order, in order of increasing priority
     * value. Store elements back into vector, now in sorted order.
     */
    for (int i = 0; !pq.isEmpty(); i++) {
        v[i] = pq.dequeue();
    }
}

/**
 * @brief reSort() -  递增/递减数组切换
 * @param Vector<DataPoint> &v　- 待切换的数组
 * 方法：交换数组两边的数据
 */
void reSort(Vector<DataPoint> &v){
    int start = 0, end = v.size() - 1;
    DataPoint cur;
    //swap
    while(start < end){       
        cur = v[start];
        v[start] = v[end];
        v[end] = cur;
        start++;
        end--;
    }
}

/**
 * @brief topK() -  将流对象中优先级最高的k个元素取出
 * @param istream& stream - 流对象
 * @param int k　- 要取出的元素个数
 * ＠return Vector<DataPoint> result - 结果
 */
Vector<DataPoint> topK(istream& stream, int k) {    
    DataPoint cur;
//    PQArray pq;
    PQHeap pq;
    Vector<DataPoint> result = {};
    // 只要流对象还有内容就一直读取
    while(stream >> cur){
        //如果队列中k个元素还没取满，则只入队就行
        if(pq.size() < k){
            pq.enqueue(cur);
        }
        //如果队列中k个元素已满　且　从流对象中读取的当前元素优先级　比　队列中最后一个元素的优先级　更大
        if(pq.size() == k && cur.priority > pq.peek().priority){
            //则先出队伍再入队
            pq.dequeue();
            pq.enqueue(cur);
        }
    }
    //将队列元素添加至结果中
    for(int i = pq.size() - 1; i >= 0; i--){
        result.add(pq.dequeue()) ;
    }
    //由递增向量切换为递减向量
    reSort(result);
    return result;
}




/* * * * * * Test Cases Below This Point * * * * * */

/* Helper function that, given a list of data points, produces a stream from them. */
stringstream asStream(const Vector<DataPoint>& dataPoints) {
    stringstream result;
    for (const DataPoint& pt: dataPoints) {
        result << pt;
    }
    return result;
}

/* Helper function that, given range start and stop, produces a stream of values. */
stringstream asStream(int start, int stop) {
    stringstream result;
    for (int i = start; i <= stop; i++) {
        DataPoint pt = { "", double(i) };
        result << pt;
    }
    return result;
}

/* Helper function to fill vector with n random DataPoints. */
void fillVector(Vector<DataPoint>& vec, int n) {
    vec.clear();
    for (int i = 0; i < n; i++) {
        DataPoint pt = { "", randomReal(0, 100) };
        vec.add(pt);
    }
}

/* TODO: Add your own custom tests here! */
STUDENT_TEST("pqSort: time trial") {
    int startSize = 1000;
    for (int n = startSize; n < 100 * startSize; n *= 2) {
        Vector<DataPoint> v;
        fillVector(v, n);
        TIME_OPERATION(n, pqSort(v));
    }
}

STUDENT_TEST("topK: time trial of n (k = 5)") {
    int k = 5;
    stringstream ss;
    for(int i = 1000000; i <= 16000000; i += 2000000){
        ss = asStream(0,i);
        TIME_OPERATION(i, topK(ss, k));
    }
}

STUDENT_TEST("topK: time trial of k (n = 6000000)") {
    int n = 10000000;
    stringstream ss;
    for(int i = 1; i <= 20; i += 3){
        ss = asStream(0,n);
        TIME_OPERATION(i, topK(ss, i));
    }
}
/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("pqSort: vector of random elements") {
    setRandomSeed(137); // why might it be a good idea to set seed here?

    Vector<DataPoint> input;
    Vector<double> expected;
    fillVector(input, 100);
    for (DataPoint dp : input) {
        expected.add(dp.priority);
    }
    expected.sort();

    pqSort(input);
    for (int i = 0; i < input.size(); i++) {
        EXPECT_EQUAL(input[i].priority, expected[i]);
    }
}

PROVIDED_TEST("pqSort: time trial") {
    int startSize = 1000;
    for (int n = startSize; n < 10*startSize; n *= 2) {
        Vector<DataPoint> v;
        fillVector(v, n);
        TIME_OPERATION(n, pqSort(v));
    }
}


/* Constant used for sizing the tests below this point. */
const int kMany = 100000;

PROVIDED_TEST("topK: stream 0 elements, ask for top 1") {
    stringstream emptyStream = asStream({});
    Vector<DataPoint> expected = {};
    EXPECT_EQUAL(topK(emptyStream, 1), expected);
}

PROVIDED_TEST("topK: stream 1 element, ask for top 1") {
    stringstream stream = asStream({ { "" , 1 } });
    Vector<DataPoint> expected = { { "" , 1 } };
    EXPECT_EQUAL(topK(stream, 1), expected);
}

PROVIDED_TEST("topK: smalPROVIDEDl hand-constructed input") {
    Vector<DataPoint> input = { { "A", 1 }, { "B", 2 }, { "C", 3 }, { "D", 4 } };

    stringstream stream = asStream(input);
    Vector<DataPoint> expected = { { "D", 4 }, { "C", 3 } };
    EXPECT_EQUAL(topK(stream, 2), expected);

    stream = asStream(input);
    expected = { { "D", 4 }, { "C", 3 }, { "B", 2 }, { "A", 1 } };
    EXPECT_EQUAL(topK(stream, 5), expected);
}

PROVIDED_TEST("topK: stream many elements, ask for top 1") {
    stringstream stream = asStream(1, kMany);
    Vector<DataPoint> expected = { { "" , kMany } };
    EXPECT_EQUAL(topK(stream, 1), expected);
}

PROVIDED_TEST("topK: stream many elements, ask for top 5") {
    stringstream stream = asStream(1, kMany);
    Vector<DataPoint> expected = {
        { "" , kMany     },
        { "" , kMany - 1 },
        { "" , kMany - 2 },
        { "" , kMany - 3 },
        { "" , kMany - 4 },
    };
    EXPECT_EQUAL(topK(stream, 5), expected);
}

PROVIDED_TEST("topK: stress test, many elements, random values") {
    Vector<double> expected;
    Vector<DataPoint> points;
    fillVector(points, 10000);
    for (DataPoint dp : points) {
        expected.add(dp.priority);
    }
    sort(expected.begin(), expected.end(), greater<double>());

    stringstream stream = asStream(points);
    int k = 10;
    Vector<DataPoint> result = topK(stream, k);
    EXPECT_EQUAL(result.size(), k);
    for (int i = 0; i < k; i++) {
        EXPECT_EQUAL(result[i].priority, expected[i]);
    }
}

PROVIDED_TEST("topK: stress test - many elements, ask for top half") {
    stringstream stream = asStream(1, kMany);
    Vector<DataPoint> result = topK(stream, kMany/2);
    EXPECT_EQUAL(result.size(), kMany/2);
    EXPECT_EQUAL(result[0].priority, kMany);
    EXPECT_EQUAL(result[result.size()-1].priority, kMany - result.size() + 1);
}

PROVIDED_TEST("topK: time trial") {
    int startSize = 200000;
    int k = 10;
    for (int n = startSize; n < 10*startSize; n *= 2) {
        Vector<DataPoint> input;
        fillVector(input, n);
        stringstream stream = asStream(input);
        TIME_OPERATION(n, topK(stream, k));
    }
}




