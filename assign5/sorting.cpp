#include "vector.h"
#include "testing/SimpleTest.h"
#include "sorting.h"

using namespace std;

/**
 * @brief partition() -  根据提供的基准元素，将当前链表划分为：等于基准，小于基准，和大于基准的三个链表
 * @要求: 输入链表不能为空
 * @param ListNode*& front - 基准元素(头元素)指针
 * @param ListNode*& left - 小于基准元素的链表表头
 * @param ListNode*& equal - 等于基准元素的链表表头
 * @param ListNode*& right - 大于基准元素的链表表头
 * @心得：1.函数内部需要创立一个int变量存储基准值
 *       2.先创建临时指针指向表头，再原链表前进一位，然后将temp　push进新链表
 */
void partition(ListNode*& front, ListNode*& left, ListNode*& equal,  ListNode*& right){
    int pivot = front->data;
    ListNode *temp;
    //遍历总链表
    while(front != nullptr){
        //如果当前的data等于基准data
        if(front->data == pivot){
            //临时指针指向链表表头
            temp = front;
            //先从表头前进一位
            front = front->next;
            //再链表push操作
            temp->next = equal;
            equal = temp;
        //如果当前的data小于基准data
        } else if(front->data < pivot){
            temp = front;
            front = front->next;
            temp->next = left;
            left = temp;
        //如果当前的data大于基准data
        } else {
            temp = front;
            front = front->next;
            temp->next = right;
            right = temp;
        }
    }
}

/**
 * @brief concatenate() - 将小于，等于，和大于基准元素的三个链表合并成一个
 * @param ListNode* left - 小于基准元素的链表表头
 * @param ListNode* equal - 等于基准元素的链表表头
 * @param ListNode* right - 大于基准元素的链表表头
 * @return ListNode* - 合并后链表的表头
 * @心得：为了应对空集，可以把输入的链表分为４种情况：１．left和equal均为空集
 *                                            ２．left为空，equal不为空
 *                                            ３．left不为空，equal为空
 *                                            ４．left和equal均不为空集
 *       很显然，right是否为空并不影响，反正最后node->next链接到一个空集也不会有事
 * 此外，这个函数的参数没有用引用，是因为函数的核心是修改指针指向的内容,具体来说是修改链表末尾的node->next
 */
ListNode* concatenate(ListNode* left, ListNode* equal,  ListNode* right){
    int condition = 0;
    //１．left和equal均为空集
    if(left == nullptr && equal == nullptr)
        condition = 0;
    //２．left为空，equal不为空
    if(left == nullptr && equal != nullptr)
        condition = 1;
    //３．left不为空，equal为空
    if(left != nullptr && equal == nullptr)
        condition = 2;
    //４．left和equal均不为空集
    if(left != nullptr && equal != nullptr)
        condition = 3;
    switch(condition){
        ListNode *node;
        case 0:
            //直接返回right表头就行
            return right;
        case 1:
            //将遍历指针指向equal链表表头left,
            node = equal;
            //遍历直到末尾
            while(node->next != nullptr){
                node = node->next;
            }
            //连接equal和right
            node->next = right;
            return equal;
        case 2:
            //将遍历指针指向left链表表头
            node = left;
            while(node->next != nullptr){
                node = node->next;
            }
            //连接left和right
            node->next = right;
            return left;
        case 3:
            node = left;
            while(node->next != nullptr){
                node = node->next;
            }
            //连接left和equal
            node->next = equal;
            while(node->next != nullptr){
                node = node->next;
            }
            //连接equal和right
            node->next = right;
            return left;
    }
    return nullptr;
}
/**
 * @brief quickSortRec() - 根据输入链表，调整其链接关系以实现排序
 * 递归类型：分治
 * @param ListNode*& front - 输入链表的表头
 * @return ListNode* - 合并后链表的表头
 * @心得：
 */
ListNode* quickSortRec(ListNode* front){
    //每个递归调用栈需要初始化left,equal,right链表
    ListNode *left = nullptr, *equal = nullptr, *right = nullptr;
    //1.Base case
    //当输入链表只有一个元素，或为空时
    if(front == nullptr || front->next == nullptr){
       //返回这个链表的表头
       return front;
    //2.Recursive case
    } else {
       //输入链表有两个以上元素，则三分
       partition(front, left, equal, right);
       //函数有返回值记得接受
       left = quickSortRec(left);
       right = quickSortRec(right);
       //返回合并后新链表的首地址
       return concatenate(left, equal, right);
    }
}

/**
 * @brief quickSort() - quickSortRec的包装函数
 * 心得：1.之所以要包装，是因为分治的递归类型需要返回值，而quickSort是void类型
 *      2.记得更新最终的表头(front接收)
 */
void quickSort(ListNode*& front) {
    //递归函数最终会返回最终合并后的新链表表头，这里注意用front去接收它
    front = quickSortRec(front);
}

/* * * * * * Test Code Below This Point * * * * * */

/*
 * We have provided this utility function as a convenience. This
 * function traverses the linked list represented by the provided
 * front pointer and prints out the data stored in the list along
 * the way in a convenient format. You may find it to be helpful to
 * use this function as a debugging aid.
 */
void printList(ListNode* front) {
   cout << "{";
   for (ListNode *cur = front; cur != nullptr; cur = cur->next) {
       cout << cur->data;
       if (cur->next != nullptr){
           cout << ", ";
       }
   }
   cout << "}" << endl;
}

/**
 * @brief deallocateList() - 释放链表
 * @param ListNode*& front - 链表头
 */
void deallocateList(ListNode* front) {
    //遍历链表
    while(front != nullptr){
        //创建指向链表表头的临时指针
        ListNode *temp = front;
        //先从表头前进一位
        front = front->next;
        //再删除临时指针指向的原表头
        delete temp;
    }
}

/**
 * @brief createList() - 根据vector创建链表
 * 如：   Vector　v = {1,2,3}
 *        链表： 1->2->3->nullptr
 * @param Vector<int> values - 输入向量
 * @return node - 链表头
 */
ListNode* createList(Vector<int> values){
    ListNode* node = nullptr;
    //注意是从后向前创建链表!
    for(int i = values.size() - 1; i >= 0; i--){
        node = new ListNode(values[i], node);
    }
    return node;
}

/**
 * @brief areEquivalent() -  判断链表和vector是否相等
 * 相等例子：链表： 1->2->3->nullptr
 *         Vector　v = {1,2,3}
 * @return 0 或 1
 * 心得：注意链表元素的数量和向量size可能不一致，如果以一边为基准进行迭代可能导致越界问题
 */
bool areEquivalent(ListNode* front, Vector<int> v){
    int i = 0;
    ListNode *cur = front;
    //通过这个迭代，要么链表走到头，要么向量走到头,或者都走到头(注意这里是v.size()不是v.size() - 1)
    while(cur != nullptr && i != v.size()){
        if(v[i] != cur->data){
             return false;
        }
        i++;
        cur = cur->next;
    }
    //如果还有一方没走到头，说明size都不一致，直接返回false
    if(cur != nullptr || i != v.size()){
         return false;
    }
    return true;
}


STUDENT_TEST("createList test"){
    Vector<int> values = {28, 46, 40, 47, 44, 31, 38, 25, 8, 19};
    ListNode* list = createList(values);
    //创建一个表头的备份指针
    ListNode* copyList = list;
    for(int i = 0; i < values.size() - 1; i++){
        EXPECT_EQUAL(list->data, values[i]);
        list = list->next;
    }
    deallocateList(copyList);
}

STUDENT_TEST("areEquivalent test"){
    Vector<int> values1 = {};
    Vector<int> values2 = {28, 46, 40, 47, 44, 31, 38, 25, 8, 19};
    Vector<int> values3 = {10, 46, 40, 47, 44, 31, 38, 25, 8, 19};
    //空集
    ListNode* list1 = createList(values1);
    EXPECT(areEquivalent(list1, values1));
    deallocateList(list1);

    ListNode* list2 = createList(values2);
    //相等
    EXPECT(areEquivalent(list2, values2));
    //不相等
    EXPECT(!areEquivalent(list2, values3));
    deallocateList(list2);
}

STUDENT_TEST("partition test"){
    Vector<int> values1 = {31};
    Vector<int> equalValues1 = {31};

    Vector<int> leftValues1 = {};
    Vector<int> rightValues1 = {};

    ListNode* list = createList(values1);
    ListNode *equal = nullptr, *left = nullptr, *right = nullptr;
    partition(list, left, equal, right);

    EXPECT(areEquivalent(left, leftValues1));
    EXPECT(areEquivalent(equal, equalValues1));
    EXPECT(areEquivalent(right, rightValues1));
    //由于分成了三个链表，不能再deallocateList(list)，事实上此时list->next已经为nullptr了，再用list这个表头删除不对
    deallocateList(left);
    deallocateList(equal);
    deallocateList(right);

    Vector<int> values2 = {31, 46, 40, 47, 44, 28, 38, 25, 8, 19};
    Vector<int> equalValues2 = {31};
    //需要注意的是由于partition中是push构造的链表，所以元素会和原来调换顺序
    Vector<int> leftValues2 = {19, 8, 25, 28};
    Vector<int> rightValues2 = {38, 44, 47, 40, 46};

    list = createList(values2);
    equal = nullptr;
    left = nullptr;
    right = nullptr;
    partition(list, left, equal, right);

    EXPECT(areEquivalent(left, leftValues2));
    EXPECT(areEquivalent(equal, equalValues2));
    EXPECT(areEquivalent(right, rightValues2));
    deallocateList(left);
    deallocateList(equal);
    deallocateList(right);
}

STUDENT_TEST("concatenate test1: left, equal, right均为空"){
    Vector<int> values1 = {};
    ListNode* list1 = createList(values1);
    Vector<int> values2 = {};
    ListNode* list2 = createList(values2);
    Vector<int> values3 = {};
    ListNode* list3 = createList(values3);
    Vector<int> result = {};

    ListNode* front = concatenate(list1,list2,list3);
    EXPECT(areEquivalent(front, result));
    deallocateList(front);
}

STUDENT_TEST("concatenate test2: left为空, equal, right不为空" ){
    Vector<int> values1 = {};
    ListNode* list1 = createList(values1);
    Vector<int> values2 = {1};
    ListNode* list2 = createList(values2);
    Vector<int> values3 = {2,3};
    ListNode* list3 = createList(values3);
    Vector<int> result = {1, 2, 3};

    ListNode* front = concatenate(list1,list2,list3);
    EXPECT(areEquivalent(front, result));
    deallocateList(front);
}

STUDENT_TEST("concatenate test3: equal为空, left, right不为空"){
    Vector<int> values1 = {-1};
    ListNode* list1 = createList(values1);
    Vector<int> values2 = {};
    ListNode* list2 = createList(values2);
    Vector<int> values3 = {2,3};
    ListNode* list3 = createList(values3);
    Vector<int> result = {-1, 2, 3};

    ListNode* front = concatenate(list1,list2,list3);
    EXPECT(areEquivalent(front, result));
    deallocateList(front);
}

STUDENT_TEST("concatenate test4: right为空, left, euqal不为空"){
    Vector<int> values1 = {1};
    ListNode* list1 = createList(values1);
    Vector<int> values2 = {3, 6};
    ListNode* list2 = createList(values2);
    Vector<int> values3 = {};
    ListNode* list3 = createList(values3);
    Vector<int> result = {1, 3, 6};

    ListNode* front = concatenate(list1,list2,list3);
    EXPECT(areEquivalent(front, result));
    deallocateList(front);
}

STUDENT_TEST("concatenate test5: left, equal, right均不为空"){
    Vector<int> values1 = {2, 4, 6};
    ListNode* list1 = createList(values1);
    Vector<int> values2 = {7, 8, 9, 10};
    ListNode* list2 = createList(values2);
    Vector<int> values3 = {11, 15, 32, 56, 101};
    ListNode* list3 = createList(values3);
    Vector<int> result = {2, 4, 6, 7, 8, 9, 10, 11, 15, 32, 56, 101};

    ListNode* front = concatenate(list1,list2,list3);
    EXPECT(areEquivalent(front, result));
    deallocateList(front);
}

STUDENT_TEST("quickSort test"){
    //对空链表进行排序
    Vector<int> values1 = {};
    ListNode* list1 = createList(values1);
    quickSort(list1);
    EXPECT(areEquivalent(list1, {}));
    //对单元素链表进行排序
    Vector<int> values2 = {2};
    ListNode* list2 = createList(values2);
    quickSort(list2);
    EXPECT(areEquivalent(list2, {2}));
    //对正序的链表进行排序
    Vector<int> values3 = {1, 2, 3, 4, 5};
    ListNode* list3 = createList(values3);
    quickSort(list3);
    EXPECT(areEquivalent(list3, {1, 2, 3, 4, 5}));
    //对逆序的链表进行排序
    Vector<int> values4 = {5, 4, 3, 2, 1};
    ListNode* list4 = createList(values4);
    quickSort(list4);
    EXPECT(areEquivalent(list4, {1, 2, 3, 4, 5}));
    //对包含一些重复元素的链表进行排序
    Vector<int> values5 = {2, 2, 1, 3, 4};
    ListNode* list5 = createList(values5);
    quickSort(list5);
    EXPECT(areEquivalent(list5, {1, 2, 2, 3, 4}));
    //对包含一些重复元素的链表进行排序
    Vector<int> values6 = {6, 6, 6, 6, 6};
    ListNode* list6 = createList(values6);
    quickSort(list6);
    EXPECT(areEquivalent(list6, {6, 6, 6, 6, 6}));
    //对包含正/负数的链表进行排序
    Vector<int> values7 = {-1, -8, 6, 2, 3};
    ListNode* list7 = createList(values7);
    quickSort(list7);
    EXPECT(areEquivalent(list7, {-8, -1, 2, 3, 6}));
    //对随机值链表进行排序
    Vector<int> values8(50000);
    for (int i = 49999; i >= 0; i--) {
        values8[i] = randomInteger(-10000, 10000);
    }
    ListNode* list8 = createList(values8);
    quickSort(list8);
    values8.sort();
    EXPECT(areEquivalent(list8, values8));
    deallocateList(list1);
    deallocateList(list2);
    deallocateList(list3);
    deallocateList(list4);
    deallocateList(list5);
    deallocateList(list6);
    deallocateList(list7);
    deallocateList(list8);
}

STUDENT_TEST("quickSort stress test"){
    //对极限长度的链表进行排序
    int startSize = 50000;
    for(int n = startSize; n < 10*startSize; n *= 2) {
        Vector<int> values(n);
        for (int i = 0; i < n; i++) {
            values[i] = randomInteger(-10000, 10000);
        }
        ListNode* list = createList(values);
        TIME_OPERATION(n, quickSort(list));
        deallocateList(list);
    }
}

STUDENT_TEST("quickSort worst case"){
    //当输入序列为正序或者逆序时，每一次三分的时候，left或right那边的大小只减小１，此时时间复杂度为Ｏ(N^2)
    int startSize = 3000;
    for(int n = startSize; n < 10*startSize; n *= 2) {
        Vector<int> values(n);
        for (int i = 0; i < n; i++) {
            values[i] = i;
        }
        ListNode* list = createList(values);
        TIME_OPERATION(n, quickSort(list));
        deallocateList(list);
    }
}

STUDENT_TEST("This is a skeleton to demonstrate an end-to-end student test."){
    /* Initialize vector to sequence of values */
    Vector<int> values = {3, 1, 2, 4, 5};
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

STUDENT_TEST("Time linked list quicksort vs  stl vector and std::sort") {
    int startSize = 50000;

    for(int n = startSize; n < 10*startSize; n *= 2) {
        vector<int> v(n);
        ListNode* list = nullptr;

        /* Create linked list and vector with the same random sequence. */
        for (int i = n-1; i >= 0; i--) {
            v[i] = randomInteger(-10000, 10000);
            list = new ListNode(v[i], list);
        }

        /* NOTE: This test does not check correctness of the linked list sort
         * function. It only times the two operations to compare relative speed. */
        TIME_OPERATION(n, quickSort(list));
        TIME_OPERATION(n, std::sort(v.begin(),v.end()));        /* Standard vector sort operation is backed
                                               with quicksort algorithm. */
        deallocateList(list);
    }
}

PROVIDED_TEST("This tests some of the testing helper functions you are recommended to implement."){
    /* Creates a small test list containing the values 1->2->3. */
    ListNode* testList = nullptr;
    testList = new ListNode(3, testList);
    testList = new ListNode(2, testList);
    testList = new ListNode(1, testList);

    /* Test equality comparison function. */
    EXPECT(areEquivalent(testList, {1, 2, 3}));
    EXPECT(!areEquivalent(testList, {1, 2}));
    EXPECT(!areEquivalent(testList, {1, 2, 3, 4}));
    EXPECT(!areEquivalent(testList, {1, 3, 2}));

    /* Test create list function. */
    ListNode* studentList = createList({1, 2, 3, 4});
    printList(studentList);
    ListNode* cur = studentList;
    for (int i = 1; i <= 4; i++){
        EXPECT(cur != nullptr);
        EXPECT_EQUAL(cur->data, i);
        cur = cur->next;
    }

    /* Test deallocate list functions. There should be no memory leaks from this test. */
    deallocateList(studentList);
    deallocateList(testList);
}

/*
 * The provided test case demonstrates a comparsion between the same
 * algorithm (quicksort) operating on a linked list versus a vector.
 */
PROVIDED_TEST("Time linked list quicksort vs vector quicksort") {
    int startSize = 50000;

    for(int n = startSize; n < 10*startSize; n *= 2) {
        Vector<int> v(n);
        ListNode* list = nullptr;

        /* Create linked list and vector with the same random sequence. */
        for (int i = n-1; i >= 0; i--) {
            v[i] = randomInteger(-10000, 10000);
            list = new ListNode(v[i], list);
        }

        /* NOTE: This test does not check correctness of the linked list sort
         * function. It only times the two operations to compare relative speed. */
        TIME_OPERATION(n, quickSort(list));
        TIME_OPERATION(n, v.sort());        /* Standard vector sort operation is backed
                                               with quicksort algorithm. */

        deallocateList(list);
    }
}

