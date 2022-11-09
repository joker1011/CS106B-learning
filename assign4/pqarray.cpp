#include "pqarray.h"
#include "error.h"
#include "random.h"
#include "strlib.h"
#include "datapoint.h"
#include "testing/SimpleTest.h"
using namespace std;

// program constant
static const int INITIAL_CAPACITY = 10;

/*
 * The constructor initializes all of the member variables needed for
 * an instance of the PQArray class. The allocated capacity
 * is initialized to a starting constant and a dynamic array of that
 * size is allocated. The number of filled slots is initially zero.
 */
PQArray::PQArray() {
    _numAllocated = INITIAL_CAPACITY;
    _elements = new DataPoint[_numAllocated](); // allocated zero'd memory
    _numFilled = 0;
}

/* The destructor is responsible for cleaning up any resources
 * used by this instance of the PQArray class. The array
 * memory that was allocated for the PQArray is deleted here.
 */
PQArray::~PQArray() {
    delete[] _elements;
}

/**
 * @brief PQArray::enqueue() -  向PQArray队列中按优先级推入一个元素
 * @param DataPoint elem　- 推入的元素
 * 心得：本质就是在递减数列中插入一个元素仍为递减数列
 *      方法是从已有的元素尾部开始比较．如果，已有元素的priority比插入元素的priority小，则将这一位的元素移到下一位．以此向前迭
 * 　　　代，直到某一位元素的priority不小于插入元素的priority，则将推入元素插在其后面就行．
 */
void PQArray::enqueue(DataPoint elem) {
    //validateInternalState();
    //如果发现队列已满，则调用扩容函数
    if(size() == _numAllocated){
        expand();
    }
    //如果队列是空的，则直接放到第一个位置就行
    if(size() == 0){
        _elements[0] = elem;
    } else {
        //这里需要在循环外定义计数器，方便跳出循环后插入元素
        int i = size();
        //从已有的元素尾部开始,直到数组头部
        for(i; i > 0; i--){
            //如果前一个元素的priority大于插入元素的priority，则跳出循环
            if(_elements[i - 1].priority >= elem.priority)
                break;
            //元素右移
            _elements[i] = _elements[i - 1];
        }
        //通过循环已定位了元素插入的位置，直接赋值就行
        _elements[i] = elem;
    }
    _numFilled ++;
}

/**
 * @brief PQArray::expand() -  队列扩容
 */
void PQArray::expand(){
    //1.创建一个指针oldElements指向原有的队列
    DataPoint *oldElements = _elements;
    //2.new一个新的翻倍的数组，还是以_elements指向它
    _numAllocated *= 2;
    _elements = new DataPoint[_numAllocated];
    //3.将原有队列的元素拷贝至新的数组
    for(int i = 0; i < size(); i++){
        _elements[i] = oldElements[i];
    }
    //4.释放原有队列
    delete[] oldElements;
}
/*
 * The count of enqueued elements is tracked in the
 * member variable _numFilled.
 */
int PQArray::size() const {
    return _numFilled;
}

/*
 * The array elements are stored in decreasing order of priority value.
 * The element at index 0 is the least urgent (largest priority value)
 * and the element in the last-filled index is the most urgent
 * (minimum priority value), this element is frontmost. peek returns
 * the frontmost element (most urgent, minimum priority value).
 */
DataPoint PQArray::peek() const {
    if (isEmpty()) {
        error("PQueue is empty!");
    }
    return _elements[size() - 1];
}

/*
 * This function returns the value of the frontmost element and removes
 * it from the queue.  Because the frontmost element was at the
 * last-filled index, decrementing filled count is sufficient to remove it.
 */
DataPoint PQArray::dequeue() {
    DataPoint front = peek();
    _numFilled--;
    return front;
}

/*
 * Returns true if no elements in the queue, false otherwise
 */
bool PQArray::isEmpty() const {
    return size() == 0;
}

/*
 * Updates internal state to reflect that the queue is empty, e.g. count
 * of filled slots is reset to zero. The array memory stays allocated
 * at current capacity. The previously stored elements do not need to
 * be cleared; the slots would be overwritten when additional elements
 * are enqueued, but as a defensive programming technique, we mark
 * each with a sentinel value as a debugging aid.
 */
void PQArray::clear() {
    _numFilled = 0;
}

/*
 * Private member function. This helper exchanges the element at
 * indexA with the element at indexB.
 */
void PQArray::swap(int indexA, int indexB) {
    DataPoint tmp = _elements[indexA];
    _elements[indexA] = _elements[indexB];
    _elements[indexB] = tmp;
}

/*
 * Prints the contents of internal array for debugging purposes.
 */
void PQArray::printDebugInfo(string msg) const {
    cout << msg << endl;
    for (int i = 0; i < size(); i++) {
        cout << "[" << i << "] = " << _elements[i] << endl;
    }
}

/*
 * Confirm the internal state of member variables appears valid.
 * In this case, check that the elements in the array are stored in
 * priority order. Report an error if problem iis found.
 */
void PQArray::validateInternalState() const {
    /*
     * If there are more elements than spots in the array, we have a problem.
     */
    if (_numFilled > _numAllocated) error("Too many elements in not enough space!");

    /* Loop over the elements in the array and compare priority of
     * neighboring elements. If current element has larger priority
     * than the previous, array elements are out of order with respect
     * to our intention. Use error to report this problem.
     */
    for (int i = 1; i < size(); i++) {
        if (_elements[i].priority > _elements[i-1].priority) {
            printDebugInfo("validateInternalState");
            error("PQArray has elements out of order at index " + integerToString(i));
        }
    }
}

/* * * * * * Test Cases Below This Point * * * * * */

void fillQueue(PQArray& pq, int n) {
    pq.clear(); // start with empty queue
    for (int i = 0; i < n; i++) {
        pq.enqueue({ "", randomReal(0, 10) });
    }
}

void emptyQueue(PQArray& pq, int n) {
    for (int i = 0; i < n; i++) {
        pq.dequeue();
    }
}

//STUDENT_TEST("Enqueue stress test") {
//    PQArray pq;
//    for(int i = 5000 ; i <= 160000; i += 10000){
//        TIME_OPERATION(i, fillQueue(pq, i));
//    }
//}

//STUDENT_TEST("Dequeue stress test") {
//    PQArray pq;
//    for(int i = 10000000 ; i <= 1000000000; i += 100000000){
//        //这里入队相同的值　以避免在后面的循环选位置浪费时间
//        for(int j = 0; j < i; j++){
//            pq.enqueue({"", 0});
//        }
//        TIME_OPERATION(i, emptyQueue(pq, i));
//    }
//}


/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("PQArray example from writeup") {
    PQArray pq;

    pq.enqueue( { "Zoe", -3 } );
    pq.enqueue( { "Elmo", 10 } );
    pq.enqueue( { "Bert", 6 } );
    EXPECT_EQUAL(pq.size(), 3);
    pq.printDebugInfo("After enqueue 3 elements");

    pq.enqueue( { "Kermit", 5 } );
    EXPECT_EQUAL(pq.size(), 4);
    pq.printDebugInfo("After enqueue one more");

    DataPoint removed = pq.dequeue();
    DataPoint expected = { "Zoe", -3 };
    EXPECT_EQUAL(removed, expected);
    pq.printDebugInfo("After dequeue one");
}

PROVIDED_TEST("PQArray, enqueue only, validate at every step") {
    PQArray pq;

    pq.enqueue({ "e", 2.718 });
    pq.validateInternalState();
    pq.enqueue({ "pi", 3.14 });
    pq.validateInternalState();
    pq.enqueue({ "phi", 1.618 });
    pq.validateInternalState();
    EXPECT_EQUAL(pq.size(), 3);
}

PROVIDED_TEST("PQArray: operations size/isEmpty/clear") {
    PQArray pq;

    EXPECT(pq.isEmpty());
    pq.clear();
    EXPECT_EQUAL(pq.isEmpty(), pq.size() == 0);
    pq.enqueue({ "", 7 });
    EXPECT_EQUAL(pq.size(), 1);
    pq.enqueue({ "", 5 });
    EXPECT_EQUAL(pq.size(), 2);
    pq.enqueue({ "", 5 });
    EXPECT_EQUAL(pq.size(), 3);
    pq.clear();
    pq.validateInternalState();
    EXPECT(pq.isEmpty());
    EXPECT_EQUAL(pq.size(), 0);
}

PROVIDED_TEST("PQArray: dequeue or peek on empty queue raises error") {
    PQArray pq;
    DataPoint point = { "Programming Abstractions", 106 };

    EXPECT(pq.isEmpty());
    EXPECT_ERROR(pq.dequeue());
    EXPECT_ERROR(pq.peek());

    pq.enqueue(point);
    pq.dequeue();
    EXPECT_ERROR(pq.dequeue());
    EXPECT_ERROR(pq.peek());

    pq.enqueue(point);
    pq.clear();
    EXPECT_ERROR(pq.dequeue());
    EXPECT_ERROR(pq.peek());
}

PROVIDED_TEST("PQArray, dequeue, validate at every step") {
    PQArray pq;

    pq.enqueue({ "e", 2.718 });
    pq.enqueue({ "pi", 3.14 });
    pq.enqueue({ "phi", 1.618 });

    for (int i = 0; i < 3; i++) {
        pq.dequeue();
        pq.validateInternalState();
    }
}

PROVIDED_TEST("PQArray, test enlarge array memory") {
    for (int size = 5; size <= 500; size *= 5) {
        PQArray pq;

        for (int i = 1; i <= size; i++) {
            pq.enqueue({"", double(i) });
        }
        pq.validateInternalState();

        for (int i = 1; i <= size; i++) {
            DataPoint expected = {"", double(i) };
            EXPECT_EQUAL(pq.dequeue(), expected);
        }
    }
}

PROVIDED_TEST("PQArray, sequence of mixed operations") {
    PQArray pq;
    int size = 30;
    double val = 0;

    for (int i = 0; i < size; i++) {
        pq.enqueue({"", --val });
    }
    val = 0;
    for (int i = 0; i < pq.size(); i++) {
        DataPoint front = pq.peek();
        EXPECT_EQUAL(pq.dequeue(), front);
        pq.enqueue( { "", ++val });
    }
    EXPECT_EQUAL(pq.size(), size);
    val = 0;
    while (!pq.isEmpty()) {
        DataPoint expected = { "", ++val };
        EXPECT_EQUAL(pq.dequeue(), expected);
    }
}

PROVIDED_TEST("PQArray stress test, cycle many random elements in and out") {
    PQArray pq;
    int n = 0, maxEnqueues = 1000;
    double sumEnqueued = 0, sumDequeued = 0;

    setRandomSeed(42); // make test behavior deterministic

    DataPoint mostUrgent = { "", 0 };
    pq.enqueue(mostUrgent);
    while (true) {
        //以0.9的概率返回true
        if (++n < maxEnqueues && randomChance(0.9)) {
            DataPoint elem = { "", randomInteger(-10, 10) + 0.5 };
            if (elem.priority < mostUrgent.priority) {
                mostUrgent = elem;
            }
            sumEnqueued += elem.priority;
            pq.enqueue(elem);
        } else {
            DataPoint elem = pq.dequeue();
            sumDequeued += elem.priority;
            EXPECT_EQUAL(elem, mostUrgent);
            if (pq.isEmpty()) break;
            mostUrgent = pq.peek();
        }
    }
    EXPECT_EQUAL(sumEnqueued, sumDequeued);
}



PROVIDED_TEST("PQArray timing test, fillQueue and emptyQueue") {
    PQArray pq;
    int n = 20000;

    TIME_OPERATION(n, fillQueue(pq, n));
    TIME_OPERATION(n, emptyQueue(pq, n));
}


