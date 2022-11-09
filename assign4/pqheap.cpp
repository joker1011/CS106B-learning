#include "pqheap.h"
#include "error.h"
#include "random.h"
#include "strlib.h"
#include "datapoint.h"
#include "testing/SimpleTest.h"
using namespace std;

const int INITIAL_CAPACITY = 10;
const int NONE = -1; // used as sentinel index

/**
 * @brief PQHeap::PQHeap() -  二叉堆的构造函数
 * @bigoh O(1)
 */
PQHeap::PQHeap() {
    _numAllocated = INITIAL_CAPACITY;
    _elements = new DataPoint[_numAllocated](); // allocated zero'd memory
    _numFilled = 0;
}

/**
 * @brief PQHeap::PQHeap() -  二叉堆的初始化列表构造（仅用于测试）
 * @bigoh O(N)
 */
PQHeap::PQHeap(initializer_list<DataPoint> list){
    _numAllocated = INITIAL_CAPACITY;
    _elements = new DataPoint[_numAllocated](); // allocated zero'd memory
    _numFilled = 0;
    int index = 0;
    for(const DataPoint &elem: list){
        if(_numFilled == _numAllocated)
            expand();
        _elements[index] = elem;
        _numFilled ++;
        index ++;
    }
}
/**
 * @brief ~PQHeap::PQHeap() -  二叉堆的析构函数
 * @bigoh O(1)
 */
PQHeap::~PQHeap() {
    delete[] _elements;
}

/**
 * @brief PQHeap::enqueue() -  向二叉堆插入元素
 * @param DataPoint elem - 插入的元素
 * @bigoh O(logN)
 * 心得:插入元素时，只要和父节点比就行(上浮操作)     PQArray.enqueue()　-　Ｏ（Ｎ）
 *                                            PQHeap.enqueue()   - Ｏ（logN）
 */
void PQHeap::enqueue(DataPoint elem) {
//    validateInternalState();
    //如果发现二叉堆已满，则调用扩容函数
    if(size() == _numAllocated){
        expand();
    }
    //先把元素放至而插堆末尾
    _elements[size()] = elem;
    //初始化父节点索引为0，初始化当前节点(子节点)索引为size()
    int partent = 0, child = size();
    //查询当前节点的父节点
    partent = getParentIndex(child);
    //只要当前节点仍存在父节点，则不断循环
    while(partent != NONE){
        //若当前节点元素的优先级　高于（小于）　父节点元素的优先级
        if(_elements[child].priority < _elements[partent].priority){
            //交换
            swap(partent,child);
            //父节点索引赋值给当前节点索引
            child = partent;
            //根据新的当前节点索引查找父节点索引
            partent = getParentIndex(child);
            //开启下一轮循环
            continue;
        }
        //跳出，这代表当前节点元素的优先级　低于（大于）　父节点元素的优先级, 不需要调整位置
        break;
    }
    _numFilled ++;
}

/**
 * @brief PQHeap::expand() -  二叉堆扩容
 * @bigoh O(N)
 */
void PQHeap::expand(){
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

/**
 * @brief PQHeap::peek() - 查看（不删除）二叉堆的根节点元素
 * @return DataPoint _elements[0] - 根节点元素
 * @bigoh O(1)
 */
DataPoint PQHeap::peek() const {
    if (isEmpty()) {
        error("PQHeap is empty!");
    }
    return _elements[0];
}

/**
 * @brief PQHeap::dequeue() -  取出(删除)二叉堆的根节点元素
 * @return DataPoint _elements[0] - 根节点元素
 * @bigoh O(logN)
 * 心得:交换首尾元素后进行下沉的操作,需要注意的是，下沉操作先对比左右子节点元素的优先级，再和优先级小的那个交换
 */
DataPoint PQHeap::dequeue() {
    //先peek二叉堆根元素，并将尾部元素赋值给根元素
    DataPoint front = peek();
    _elements[0] = _elements[size() - 1];
    //大小减１
    _numFilled --;
    //初始化索引
    int partent = 0;
    int left = getLeftChildIndex(partent);
    int right = getRightChildIndex(partent);
    //只要还能找到左侧子节点，则继续循环（因为有右侧子节点必有左侧字节点，所以左侧子节点才是必要条件）
    while(left != NONE){
        //(先判断要不要下沉)若父节点的优先级　大于　左右两个子节点的优先级,则交换元素
        //注意右侧子节点对比时同样要加（right != NONE）这个条件以防止它为-1
        if (_elements[partent].priority > _elements[left].priority
        || (right != NONE && _elements[partent].priority > _elements[right].priority)){
            //如果右侧没有子节点　或　左侧优先级比右侧更小
            if(right == NONE || _elements[left].priority < _elements[right].priority){
                swap(partent, left);
                //更新父节点索引
                partent = left;
            } else {
                swap(partent, right);
                partent = right;
            }
            //重新查找子节点索引
            left = getLeftChildIndex(partent);
            right = getRightChildIndex(partent);
            continue;
        }
        //跳出，这代表当前节点元素的优先级　高于（小于））　左右子节点元素的优先级, 不需要调整位置
        break;
    }
    return front;
}

/**
 * @brief PQHeap::isEmpty() -  二叉堆是否为空
 * @bigoh O(1)
 */
bool PQHeap::isEmpty() const {
    return size() == 0;

}

/**
 * @brief PQHeap::size() -  二叉堆元素个数
 * @bigoh O(1)
 */
int PQHeap::size() const {
    return _numFilled;
}

/**
 * @brief PQHeap::clear() -  清空二叉堆
 * @bigoh O(1)
 */
void PQHeap::clear() {
     _numFilled = 0;
}

/**
 * @brief PQHeap::swap -  交换两元素
 * @bigoh O(1)
 */
void PQHeap::swap(int &indexA, int &indexB) {
    DataPoint tmp = _elements[indexA];
    _elements[indexA] = _elements[indexB];
    _elements[indexB] = tmp;
}

/**
 * @brief PQHeap::printDebugInfo() -  打印调试信息(二叉堆底层数组的所有元素)
 * @bigoh O(N)
 */
void PQHeap::printDebugInfo(string msg) const {
    cout << msg << endl;
    for (int i = 0; i < size(); i++) {
        cout << "[" << i << "] = " << _elements[i] << endl;
    }
}

/**
 * @brief PQHeap::validateInternalState() -  检查二叉堆的内部状态是否正确
 * 正确条件:1.已有元素的个数不能大于二叉堆分配的内存空间大小
 *         2.从根节点开始，父节点的优先级必须 高于（小于等于） 左右两个子节点的优先级
 * @bigoh O(N) - 需要遍历每个元素是否符合二叉堆条件
 */
void PQHeap::validateInternalState() const {
    //若已有元素的个数　大于　二叉堆分配的内存空间大小，报错
    if (_numFilled > _numAllocated) error("Too many elements in not enough space!");
    //初始化索引
    int index = 0;
    int left = getLeftChildIndex(index);
    int right = getRightChildIndex(index);
    //若当前节点还能找到子节点，则继续循环
    while(left != NONE){
        //若父节点的优先级　大于　左右两个子节点的优先级,则打印调试信息并报错
        //注意右侧子节点对比时同样要加（right != NONE）这个条件以防止它为-1
        if (_elements[index].priority > _elements[left].priority
        || (right != NONE && _elements[index].priority > _elements[right].priority)) {
            printDebugInfo("validateInternalState");
            error("PQHeap has elements out of order at index " + integerToString(index));
        }
        //当前节点索引 + 1
        index++;
        //根据父节点索引查找左右子节点的索引
        left = getLeftChildIndex(index);
        right = getRightChildIndex(index);
     }
}

/**
 * @brief PQHeap::getParentIndex() -  根据当前节点的索引寻找父节点的索引
 * @param child - 子节点索引
 * @return NONE　- 没有父节点
 * @return parent - 父节点索引
 * @bigoh O(1)
 */
int PQHeap::getParentIndex(int child) const {    
    //如果字节点的索引已经是0,则没有父节点
    if(child == 0){
        return NONE;
    }
    return (child - 1) / 2;
}

/**
 * @brief PQHeap::getLeftChildIndex() -  根据当前节点的索引寻找左侧子节点的索引
 * @param parent - 父节点索引
 * @return NONE　- 没有左侧子节点
 * @return parent - 左侧子节点索引
 * @bigoh O(1)
 */
int PQHeap::getLeftChildIndex(int parent) const {
    int left = 2 * parent + 1;
    //如果左节点的索引越界(大于已有元素的个数 - 1),则没有左子节点
    if(left > _numFilled - 1){
        return NONE;
    }
    return left;
}

/**
 * @brief PQHeap::getLeftChildIndex() -  根据当前节点的索引寻找右侧子节点的索引
 * @param parent - 父节点索引
 * @return NONE　- 没有右侧子节点
 * @return parent - 右侧子节点索引
 * @bigoh O(1)
 */
int PQHeap::getRightChildIndex(int parent) const {
    int right = 2 * parent + 2;
    //如果右节点的索引越界(大于已有元素的个数 - 1),则没有右子节点
    if(right > _numFilled - 1){
        return NONE;
    }
    return right;
}

/* * * * * * Test Cases Below This Point * * * * * */


void fillQueue(PQHeap& pq, int n) {
    pq.clear(); // start with empty queue
    for (int i = 0; i < n; i++) {
        pq.enqueue({ "", randomReal(0, 10) });
    }
}

void emptyQueue(PQHeap& pq, int n) {
    for (int i = 0; i < n; i++) {
        pq.dequeue();
    }
}

STUDENT_TEST("二叉堆功能测试") {
     PQHeap pq;
     Vector<DataPoint> input = {{ "R", 4 }, { "A", 5 }, { "B", 3 }, { "K", 7 }, { "G", 2 },{ "V", 9 }, { "T", 1 }, { "O", 8 }, { "S", 6 } };
     Vector<DataPoint> result = {{ "T", 1 }, { "G", 2 }, { "B", 3 }, { "R", 4 },  { "A", 5 }, { "S", 6 }, { "K", 7 }, { "O", 8 }, { "V", 9 } };
     for(int i = 0; i < input.size(); i++){
         pq.enqueue(input[i]);
     }
     for(int j = 0; j < input.size(); j++){
         EXPECT_EQUAL(pq.dequeue(), result[j]);
     }
}

STUDENT_TEST("Enqueue stress test") {
    PQHeap pq;
    for(int i = 50000 ; i <= 1600000; i += 100000){
        TIME_OPERATION(i, fillQueue(pq, i));
    }
}

STUDENT_TEST("Dequeue stress test") {
    PQHeap pq;
    for(int i = 50000 ; i <= 1600000; i += 100000){
        fillQueue(pq, i);
        TIME_OPERATION(i, emptyQueue(pq, i));
    }
}

STUDENT_TEST("peek test") {
    PQHeap pq;
    EXPECT_ERROR(pq.peek());
    DataPoint a = {"Zoe", -3};
    pq.enqueue( { "Zoe", -3 } );
    pq.enqueue( { "Elmo", 10 } );
    pq.enqueue( { "Bert", 6 } );
    EXPECT_EQUAL(pq.peek(), a);
}

STUDENT_TEST("isEmpty test") {
    PQHeap pq;
    EXPECT(pq.isEmpty());
    pq.enqueue( { "Zoe", -3 } );
    EXPECT(!pq.isEmpty());
}

STUDENT_TEST("size test") {
    PQHeap pq;
    EXPECT_EQUAL(pq.size(),0);
    pq.enqueue( { "Zoe", -3 } );
    EXPECT_EQUAL(pq.size(),1);
    pq.enqueue( { "Elmo", 10 } );
    pq.enqueue( { "Bert", 6 } );
    EXPECT_EQUAL(pq.size(),3);
    fillQueue(pq, 100);
    EXPECT_EQUAL(pq.size(),100);
}

STUDENT_TEST("clear test") {
    PQHeap pq;
    pq.clear();
    EXPECT(pq.isEmpty());
    fillQueue(pq, 100);
    pq.clear();
    EXPECT(pq.isEmpty());
}

STUDENT_TEST("validateInternalState test") {
    PQHeap pq1 = {{ "Elmo", 10 },{"Zoe", -3},{ "Bert", 6 }};
    EXPECT_ERROR(pq1.validateInternalState());
    PQHeap pq2 = {{"Zoe", -3},{ "Bert", 6 },{ "Elmo", 10 }};
    pq2.validateInternalState();
    PQHeap pq3 = {};
    pq3.validateInternalState();
}




///* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("PQHeap example from writeup, validate each step") {
    PQHeap pq;
    Vector<DataPoint> input = {
        { "R", 4 }, { "A", 5 }, { "B", 3 }, { "K", 7 }, { "G", 2 },
        { "V", 9 }, { "T", 1 }, { "O", 8 }, { "S", 6 } };

    pq.validateInternalState();
    for (DataPoint dp : input) {
        pq.enqueue(dp);
        pq.validateInternalState();
    }
    while (!pq.isEmpty()) {
        pq.dequeue();
        pq.validateInternalState();
    }
}
