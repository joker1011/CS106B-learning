#include "bits.h"
#include "treenode.h"
#include "huffman.h"
#include "map.h"
#include "vector.h"
#include "priorityqueue.h"
#include "strlib.h"
#include "testing/SimpleTest.h"
using namespace std;

/**
 * @brief decodeText() - 解码
 * @param EncodingTreeNode* tree - 哈夫曼树
 * @param Queue<Bit>& messageBits - 压缩后的序列
 * @return string result - 解码后的字符
 * 心得：只需要根据messageBits在哈夫曼树中选择道路前进，而不需要遍历整个树，所以用迭代就行
 */
string decodeText(EncodingTreeNode* tree, Queue<Bit>& messageBits) {
    string result = "";
    EncodingTreeNode* cur = tree;
    while(!messageBits.isEmpty()){
        //如果当前节点为空指针，说明无法解码，直接返回空字符串
        if(cur == nullptr){
            return "";
        }
        //走到叶节点时，加上对应的字符
        if(cur->isLeaf()){
            result += cur->getChar();
            //解码了一个字符了，需要重置起点
            cur = tree;
        }
        if(messageBits.dequeue() == 0){
            cur = cur->zero;
        } else {
            cur = cur->one;
        }
    }
    //当队列messageBits仅剩一位时，dequeue后队列为空，while循环结束，但是还差最后一次查树的操作没有做记得补上
    if(cur == nullptr){
        return "";
    }
    if(cur->isLeaf()){
        result += cur->getChar();
        cur = tree;
    }
    return result;
}

/**
 * @brief insertOnLeft() - 在当前的父节点左侧插入新的节点
 * @param EncodingTreeNode*& root - 当前的父节点
 * @param Bit rootIsInterior - 当前的父节点是否是内部节点
 * @param EncodingTreeNode*& node - 待插入的新节点
 * @return bool - 插入是否成功
 */
bool insertOnLeft(EncodingTreeNode*& root, Bit& rootIsInterior, EncodingTreeNode*& node){
    if(rootIsInterior == 0){
        return false;
    }
    if(root->zero == nullptr){
        root->zero = node;
        return true;
    }
    return false;
}

/**
 * @brief insertRight() - 在当前的父节点右侧插入新的节点
 * @param EncodingTreeNode*& root - 当前的父节点
 * @param Bit rootIsInterior - 当前的父节点是否是内部节点
 * @param EncodingTreeNode*& node - 待插入的新节点
 * @return bool - 插入是否成功
 */
bool insertOnRight(EncodingTreeNode*& root, Bit& rootIsInterior, EncodingTreeNode*& node){
    if(rootIsInterior == 0){
        return false;
    }
    if(root->one == nullptr){
        root->one = node;
        return true;
    }
    return false;
}

/**
 * @brief unflattenTreeRec() - 由位序列和字符序列重新展开成哈夫曼树
 * 递归类型：排列（迷宫dfs）
 * 递归范式：Choose/Explore/Unchoose
 * @param EncodingTreeNode* root - 当前的父节点 (注意不能引用，没一个递归调用栈要存储自己的当前父节点)
 * @param Bit rootIsInterior - 当前父节点是否是内部节点 (同上)
 * @param EncodingTreeNode*& node - 待插入的新节点 (注意必须引用，这是因为必须在不同的递归调用栈内保持相同的待插入节点．在当前调用栈内可能无法插入该节点，这样必须返回上一级查看能否插入．若不采用引用，返回时待插入的节点又变回上一级的了．)
 * @param Bit& nodeIsInterior - 待插入的新节点是否是内部节点 (同上)
 * @param Queue<Bit>& treeShape - 位序列
 * @param Queue<char>& treeLeaves - 字符序列
 * @return bool true - 构建树完成    false - 无法在当前父节点插入新节点
 * 心得：这题类似迷宫dfs．迷宫：１．Base case为走到右下角　　　　　　　　这题：１．Base case为序列为空
 *                          ２．每次前进方向为当前位置的有效相邻位置　　　　　２．每次前进的方向只有左下和右下两个方向
 *                          ３．Unchoose的方法为path.pop();　　　　　　　 ３．Unchoose的方法为return false
 *      不同的是这题需要两个额外的Bit变量记录父节点和待插入节点是否是内部节点，因为叶节点和内部节点构建时两侧都是空指针，isLeaf没用了，必须通过位序列的数字来判断，并记录下来
 */
bool unflattenTreeRec(EncodingTreeNode* root, Bit rootIsInterior, EncodingTreeNode*& node, Bit& nodeIsInterior, Queue<Bit>& treeShape, Queue<char>& treeLeaves){
    //1.Base case
    //如果位序列或字符序列为空，则构建树完成
    if(treeShape.isEmpty() || treeLeaves.isEmpty()){
        return true;
    }
    //位序列取一位
    nodeIsInterior = treeShape.dequeue();
    //判断是叶节点还是内部节点，并完成待插入节点的构建
    if(nodeIsInterior == 0){
       node = new EncodingTreeNode(treeLeaves.dequeue());
    } else {
       node = new EncodingTreeNode(nullptr, nullptr);
    }
    //2.Recursive case
    //2.1 Choose
    //如果能在左侧插入新节点则以新节点为父节点进入递归调用
    if(insertOnLeft(root, rootIsInterior, node)){
        //2.2 Explore
        if(unflattenTreeRec(node, nodeIsInterior, node, nodeIsInterior, treeShape, treeLeaves)){
            return true;
        }
    }
    //2.3 Choose
    //如果能在右侧插入新节点则以新节点为父节点进入递归调用
    if(insertOnRight(root, rootIsInterior, node)){
        //2.4 Explore
        if(unflattenTreeRec(node, nodeIsInterior, node, nodeIsInterior, treeShape, treeLeaves)){
            return true;
        }
    }
    //2.5 Unchoose
    return false;
}

/**
 * @brief unflattenTree() - unflattenTreeRec的包装函数
 * @param Queue<Bit>& treeShape - 位序列
 * @param Queue<char>& treeLeaves - 字符序列
 * @return EncodingTreeNode* 哈夫曼树根节点指针
 * 心得:位序列是先序遍历，只能从根节点向叶节点创建树
 */
EncodingTreeNode* unflattenTree(Queue<Bit>& treeShape, Queue<char>& treeLeaves) {
    EncodingTreeNode* root;
    //如果位序列和字符序列有一个为空，则不能构成树，返回空指针
    if(treeShape.isEmpty() || treeLeaves.isEmpty()){
        return nullptr;
    }
    //如果位序列和字符序列大小均为1
    if(treeShape.size() == 1 && treeLeaves.size() == 1){
        //当位序列的为{0}时，说明根节点就是叶节点
        if(treeShape.dequeue() == 0){
            root = new EncodingTreeNode(treeLeaves.dequeue());
            return root;
        }
        //位序列为{1},不能构成树，返回空指针
        return nullptr;
    }
    //如果位序列大小大于1（说明树不只一个节点）
    if(treeShape.size() > 1){
        //但根节点还等于0，则位序列首位错误
        if(treeShape.dequeue() == 0){
            return nullptr;
        //位序列首位没错,则进入递归创建树
        } else {
            //初始化根节点
            root = new EncodingTreeNode(nullptr, nullptr);
            EncodingTreeNode* node;
            Bit nodeIsinterior;
            //尽管这里root不是引用传递，但是传递指针，在递归函数中完成了各个节点的连接，所以最后root就是根节点
            unflattenTreeRec(root, 1, node, nodeIsinterior, treeShape, treeLeaves);
            //如果位序列和字符序列不匹配，递归构建完树后有一个序列可能仍有剩余，此时返回空指针
            if(!treeShape.isEmpty() || !treeLeaves.isEmpty()){
                return nullptr;
            }
            //如果不是返回根节点指针
            return root;
        }
    }
    return nullptr;
}

/**
 * @brief decompress() - 解压缩文件
 * @param EncodedData& data - 压缩后的数据
 * @return string - 解压缩后的字符串
 */
string decompress(EncodedData& data) {
    EncodingTreeNode* root = unflattenTree(data.treeShape, data.treeLeaves);
    string result = decodeText(root, data.messageBits);
    //树的节点是在堆区创建的别忘记释放
    deallocateTree(root);
    return result;
}

/**
 * @brief buildHuffmanTree() - 根据输入字符串构建哈夫曼树
 * @param string text - 输入字符串
 * @return EncodingTreeNode* 哈夫曼树根节点指针
 * 心得:优先队列可以存储EncodingTreeNode变量
 */
EncodingTreeNode* buildHuffmanTree(string text) {
    Map<char, int> wordFrequency;
    PriorityQueue<EncodingTreeNode* > pq;
    //先进行词频统计
    for(const char& elem: text){
        wordFrequency.put(elem, wordFrequency.get(elem) + 1);
    }
    //如果统计表key个数小于2（字符串没有两个以上的不同字符），则报错
    if(wordFrequency.size() < 2){
        error("Invalid input");
    }
    //根据词频表的每个key，创建leaves节点，同时将其出现频率推入优先退列
    for(const char& elem: wordFrequency){
        pq.enqueue(new EncodingTreeNode(elem), wordFrequency.get(elem));
    }
    EncodingTreeNode* combinedNode;
    while(pq.size() != 1){
        //先提取优先级，再dequeue
        double leftPriority = pq.peekPriority();
        EncodingTreeNode* leftNode = pq.dequeue();
        double rightPriority = pq.peekPriority();
        EncodingTreeNode* RightNode = pq.dequeue();
        combinedNode = new EncodingTreeNode(leftNode, RightNode);
        //合并后的节点再推入优先退列
        pq.enqueue(combinedNode, leftPriority + rightPriority);
    }
    return combinedNode;
}

/**
 * @brief encodeTextRec() - 遍历哈夫曼树并生成字符和位序列的映射表
 * 递归类型：排列（dfs）
 * @param EncodingTreeNode* tree - 哈夫曼树
 * @param Vector<Bit>& curBitSequence - 当前位序列
 * @param Map<char, Vector<Bit>>& encodeTable - 字符和位序列的映射表
 * 心得：注意两个remove的位置
 */
void encodeTextRec(EncodingTreeNode* tree, Vector<Bit>& curBitSequence, Map<char, Vector<Bit>>& encodeTable){
    //1.Base case
    if(tree == nullptr){
        curBitSequence.remove(curBitSequence.size() - 1);
        return;
    //2.Recursive case
    } else {
        if(tree->isLeaf()){
            encodeTable.put(tree->getChar(), curBitSequence);
        }
        curBitSequence.add(0);
        encodeTextRec(tree->zero, curBitSequence, encodeTable);

        curBitSequence.add(1);
        encodeTextRec(tree->one, curBitSequence, encodeTable);
        //防止退回根节点时还remove
        if(!curBitSequence.isEmpty()){
             curBitSequence.remove(curBitSequence.size() - 1);
        }
    }
}

/**
 * @brief encodeText() - 根据哈夫曼编码数，压缩字符串(encodeTextRec的包装函数)
 * @param EncodingTreeNode* tree - 哈夫曼树
 * @param string text - 输入字符串
 * @return Queue<Bit> - 压缩后的位序列
 * 心得：千万不要每查一个字符都去遍历整个树，先建立字符和位序列之间的映射关系（map）
 */
Queue<Bit> encodeText(EncodingTreeNode* tree, string text) {
    Map<char, Vector<Bit>> encodeTable;
    Vector<Bit> curBitSequence;
    Queue<Bit> result;
    encodeTextRec(tree, curBitSequence, encodeTable);
    for(char elem: text){
        //查映射表
        curBitSequence = encodeTable.get(elem);
        for(Bit item: curBitSequence){
            result.enqueue(item);
        }
    }
    return result;
}

/**
 * @brief flattenTree() - 将哈夫曼树展平为位序列和字符序列
 * 递归类型：排列（dfs）
 * @param EncodingTreeNode* tree - 哈夫曼树
 * @param Queue<Bit>& treeShape - 展平后的位序列
 * @param Queue<char>& treeLeaves - 展平后的字符序列
 * 心得：注意位序列是先序遍历，字符序列是中序遍历
 */
void flattenTree(EncodingTreeNode* tree, Queue<Bit>& treeShape, Queue<char>& treeLeaves) {
    if(tree == nullptr){
        return;
    //2.Recursive case
    } else {
        //treeShape先序遍历
        if(tree->isLeaf()){
            treeShape.enqueue(0);
        } else {
            treeShape.enqueue(1);
        }
        flattenTree(tree->zero, treeShape, treeLeaves);
        //treeLeaves中序遍历
        if(tree->isLeaf()){
            treeLeaves.enqueue(tree->getChar());
        }
        flattenTree(tree->one, treeShape, treeLeaves);
    }
}

/**
 * @brief compress() - 压缩字符串
 * @param string messageText - 输入字符串
 * @return EncodedData result - 压缩后的编码序列
 */
EncodedData compress(string messageText) {
    EncodedData result;
    EncodingTreeNode* tree = buildHuffmanTree(messageText);
    flattenTree(tree, result.treeShape, result.treeLeaves);
    result.messageBits = encodeText(tree, messageText);
    //不要忘记释放内存
    deallocateTree(tree);
    return result;
}

/* * * * * * Testing Helper Functions Below This Point * * * * * */

//心得：从叶节点向根节点创建
EncodingTreeNode* createExampleTree() {
    /* Example encoding tree used in multiple test cases:
     *                *           h = 3
     *              /   \
     *             T     *        h = 2
     *                  / \
     *                 *   E      h = 1
     *                / \
     *               R   S        h = 0
     */
    //创建根节点创建叶节点
    EncodingTreeNode* T = new EncodingTreeNode('T');
    EncodingTreeNode* R = new EncodingTreeNode('R');
    EncodingTreeNode* S = new EncodingTreeNode('S');
    EncodingTreeNode* E = new EncodingTreeNode('E');
    //从最底层开始，向上创建内部节点
    EncodingTreeNode* h1 = new EncodingTreeNode(R,S);
    EncodingTreeNode* h2 = new EncodingTreeNode(h1, E);
    EncodingTreeNode* root = new EncodingTreeNode(T,h2);
    return root;
}

/**
 * @brief deallocateTree() - 释放哈夫曼树
 * 递归类型：排列(dfs)
 * @param EncodingTreeNode* t - 哈夫曼树根节点
 */
void deallocateTree(EncodingTreeNode* t) {
    //注意不是t->zero == nullptr && t->one == nullptr这样会直接返回，叶节点并没有释放
    if(t ==  nullptr){
        return;
    }
    deallocateTree(t->zero);
    deallocateTree(t->one);
    delete t;
}

/**
 * @brief areEqual() - 判断两个哈夫曼树是否相等
 * 递归类型：排列(dfs)
 * @param EncodingTreeNode* a - 哈夫曼树节点
 * @param EncodingTreeNode* b - 哈夫曼树节点
 * @return bool
 * 心得：１．Base case 需要注意　是　a, b有一个为空指针
 *                         而不是　a, b均为叶节点（它是过程中满足的判断）
 *   　 ２．利用bool返回值进行判断　一旦有false时　逐级退出递归
 */
bool areEqual(EncodingTreeNode* a, EncodingTreeNode* b) {
    //1.Base case
    //如果a, b均为空，则返回true
    if(a == nullptr && b == nullptr){
        return true;
    //如果a, b只有一个为空，则之间返回false
    } else if((a == nullptr && b != nullptr)
            ||(a != nullptr && b == nullptr)){
        return false;
    //均不为空
    } else {
        //如果a，b只有一个为叶节点，则直接返回false　(过程中不满足)
        if((a->isLeaf() && !b->isLeaf())
       || (!a->isLeaf() && b->isLeaf())){
            return false;
        }
        //如果a,b都是叶节点，则判断它们的字符是否相等
        if(a->isLeaf() && b->isLeaf()){
            if(a->getChar() == b->getChar()){
                return true;
            }
            return false;
         }
        //2.Recusive case
        //只有当a和b都不是叶节点（均是内部节点）时，才进入递归
        if(!a->isLeaf() && !b->isLeaf()){
            //如果递归过程中有返回false, 则逐级返回false直到退出递归
            if(!areEqual(a->zero, b->zero)){
                return false;
            }
            if(!areEqual(a->one, b->one)){
                return false;
            }
        }
    }
    return true;
}

/* * * * * * Test Cases Below This Point * * * * * */

STUDENT_TEST("deallocateTree tree test"){
    EncodingTreeNode* root = createExampleTree();
    deallocateTree(root);
}

STUDENT_TEST("areEqual test"){
    //创建一个只有一个叶节点的单例树，与一棵空树进行比较并确认 areEqual 返回 false。
    EncodingTreeNode* tree1 = new EncodingTreeNode('A');
    EncodingTreeNode* tree2 = nullptr;
    EXPECT(!areEqual(tree1, tree2));
    deallocateTree(tree1);

    //创建第二个叶节点并与第一个进行比较。如果两个节点都初始化为相同的字符，则它们应该相等，否则为假。
    EncodingTreeNode* tree3 = new EncodingTreeNode('A');
    EncodingTreeNode* tree4 = new EncodingTreeNode('B');
    EXPECT(areEqual(tree1, tree3));
    EXPECT(!areEqual(tree1, tree4));
    deallocateTree(tree3);
    deallocateTree(tree4);

    //调用 createExampleTree 构建树，确认不等于单例树。
    EncodingTreeNode* tree5 = createExampleTree();
    EXPECT(!areEqual(tree1, tree5));

    //第二次调用 createExampleTree 并确认树的两个副本是相等的。
    EncodingTreeNode* tree6 = createExampleTree();
    EXPECT(areEqual(tree5, tree6));
    deallocateTree(tree6);

    //将示例树与其子树之一进行比较，并确认它们不相等。
    EXPECT(!areEqual(tree5, tree5->one));
    deallocateTree(tree5);
}

STUDENT_TEST("decodeText test"){
   /*
    *                *           h = 3
    *              /   \
    *             T     *        h = 2
    *                  / \
    *                 *   E      h = 1
    *                / \
    *               R   S        h = 0
    */
    EncodingTreeNode* tree = createExampleTree();
    Queue<Bit> messageBits1 = {1, 0, 0, 1, 1, 1, 0, 1, 0};
    EXPECT_EQUAL(decodeText(tree, messageBits1), "REST");
    Queue<Bit> messageBits2 = {0, 1, 1, 1, 0, 1, 0};
    EXPECT_EQUAL(decodeText(tree, messageBits2), "TEST");
    Queue<Bit> messageBits3 = {1, 0, 1, 1, 1, 0};
    EXPECT_EQUAL(decodeText(tree, messageBits3), "SET");
    Queue<Bit> messageBits4 = {0};
    EXPECT_EQUAL(decodeText(tree, messageBits4), "T");
    Queue<Bit> messageBits5 = {};
    EXPECT_EQUAL(decodeText(tree, messageBits5), "");
    deallocateTree(tree);
}

STUDENT_TEST("unflattenTree test"){
    //空序列
    EncodingTreeNode* tree1 = nullptr;
    Queue<Bit> treeShape1 = {};
    Queue<char> treeLeaves1 = {};
    EncodingTreeNode* unflattenTree1 = unflattenTree(treeShape1, treeLeaves1);
    EXPECT(areEqual(tree1, unflattenTree1));
    deallocateTree(tree1);
    deallocateTree(unflattenTree1);

    //单字符序列
    EncodingTreeNode* tree2 = new EncodingTreeNode('S');
    Queue<Bit> treeShape2 = {0};
    Queue<char> treeLeaves2 = {'S'};
    EncodingTreeNode* unflattenTree2 = unflattenTree(treeShape2, treeLeaves2);
    EXPECT(areEqual(tree2, unflattenTree2));
    deallocateTree(tree2);
    deallocateTree(unflattenTree2);

    //利用createExampleTree
    EncodingTreeNode* tree3 = createExampleTree();
    Queue<Bit> treeShape3 = {1, 0, 1, 1, 0, 0, 0};
    Queue<char> treeLeaves3 = {'T', 'R', 'S','E'};
    EncodingTreeNode* unflattenTree3 = unflattenTree(treeShape3, treeLeaves3);
    EXPECT(areEqual(tree3, unflattenTree3));
    //和createExampleTree()的树不同
    Queue<Bit> treeShape3different = {1, 0, 1, 1, 0, 0, 0};
    Queue<char> treeLeaves3different = {'A', 'E', 'R','O'};
    EncodingTreeNode* unflattenTree3different = unflattenTree(treeShape3different, treeLeaves3different);
    EXPECT(!areEqual(tree3, unflattenTree3different));
    deallocateTree(tree3);
    deallocateTree(unflattenTree3);
    deallocateTree(unflattenTree3different);

    //重新手动创建一个树
    /*                *           h = 3
     *              /   \
     *             T     *        h = 2
     *                  /
     *                 *          h = 1
     *                / \
     *               R   S        h = 0
     */
    EncodingTreeNode* T = new EncodingTreeNode('T');
    EncodingTreeNode* R = new EncodingTreeNode('R');
    EncodingTreeNode* S = new EncodingTreeNode('S');
    EncodingTreeNode* h1 = new EncodingTreeNode(R,S);
    EncodingTreeNode* h2 = new EncodingTreeNode(h1, nullptr);
    EncodingTreeNode* tree4 = new EncodingTreeNode(T,h2);
    Queue<Bit> treeShape4 = {1, 0, 1, 1, 0, 0 };
    Queue<char> treeLeaves4 = {'T', 'R', 'S'};
    EncodingTreeNode* unflattenTree4 = unflattenTree(treeShape4, treeLeaves4);
    EXPECT(areEqual(tree4, unflattenTree4));
    deallocateTree(tree4);
    deallocateTree(unflattenTree4);
}

STUDENT_TEST("encodeText test"){
    /*    映射表
     *    | T | 0   |
     *    | R | 100 |
     *    | S | 101 |
     *    | E | 11  |
     */
    EncodingTreeNode* tree = createExampleTree();
    Queue<Bit> messageBits = encodeText(tree, "REST");
    EXPECT_EQUAL(messageBits, {1, 0, 0, 1, 1, 1, 0, 1, 0});
    EXPECT_EQUAL(decodeText(tree, messageBits), "REST");
    deallocateTree(tree);
}

STUDENT_TEST("buildHuffmanTree test") {
    EXPECT_ERROR(buildHuffmanTree(""));
    EXPECT_ERROR(buildHuffmanTree("A"));
    EXPECT_ERROR(buildHuffmanTree("SSSSSSSSS"));

    /*                *         h = 3
     *               / \
     *              *   O       h = 2
     *             / \
     *            N   *         h = 1
     *               / \
     *              M   S       h = 0
     */
    EncodingTreeNode* N = new EncodingTreeNode('N');
    EncodingTreeNode* M = new EncodingTreeNode('M');
    EncodingTreeNode* S = new EncodingTreeNode('S');
    EncodingTreeNode* O = new EncodingTreeNode('O');

    EncodingTreeNode* h1 = new EncodingTreeNode(M, S);
    EncodingTreeNode* h2 = new EncodingTreeNode(N, h1);
    EncodingTreeNode* tree1 = new EncodingTreeNode(h2, O);
    EncodingTreeNode* tree2 = buildHuffmanTree("MOOOOONNSMOOOOONNSS"); //M:2  S:3  N:4   O:9
    EXPECT(areEqual(tree1, tree2));
    deallocateTree(tree1);
    deallocateTree(tree2);
}

STUDENT_TEST("compress test") {
    EncodedData data = compress("La La La! I love CS101! Teacher Xue yyds");
    EXPECT_EQUAL(decompress(data), "La La La! I love CS101! Teacher Xue yyds");
}


/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("decodeText, small example encoding tree") {
    EncodingTreeNode* tree = createExampleTree(); // see diagram above
    EXPECT(tree != nullptr);

    Queue<Bit> messageBits = { 1, 1 }; // E
    EXPECT_EQUAL(decodeText(tree, messageBits), "E");

    messageBits = { 1, 0, 1, 1, 1, 0 }; // SET
    EXPECT_EQUAL(decodeText(tree, messageBits), "SET");

    messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1}; // STREETS
    EXPECT_EQUAL(decodeText(tree, messageBits), "STREETS");

    deallocateTree(tree);
}

PROVIDED_TEST("unflattenTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    Queue<Bit>  treeShape  = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> treeLeaves = { 'T', 'R', 'S', 'E' };
    EncodingTreeNode* tree = unflattenTree(treeShape, treeLeaves);

    EXPECT(areEqual(tree, reference));

    deallocateTree(tree);
    deallocateTree(reference);
}

PROVIDED_TEST("decompress, small example input") {
    EncodedData data = {
        { 1, 0, 1, 1, 0, 0, 0 }, // treeShape
        { 'T', 'R', 'S', 'E' },  // treeLeaves
        { 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1 } // messageBits
    };

    EXPECT_EQUAL(decompress(data), "TRESS");
}

PROVIDED_TEST("buildHuffmanTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    EncodingTreeNode* tree = buildHuffmanTree("STREETTEST");
    EXPECT(areEqual(tree, reference));

    deallocateTree(reference);
    deallocateTree(tree);
}

PROVIDED_TEST("encodeText, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above

    Queue<Bit> messageBits = { 1, 1 }; // E
    EXPECT_EQUAL(encodeText(reference, "E"), messageBits);

    messageBits = { 1, 0, 1, 1, 1, 0 }; // SET
    EXPECT_EQUAL(encodeText(reference, "SET"), messageBits);

    messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1 }; // STREETS
    EXPECT_EQUAL(encodeText(reference, "STREETS"), messageBits);

    deallocateTree(reference);
}

PROVIDED_TEST("flattenTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    Queue<Bit>  expectedShape  = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> expectedLeaves = { 'T', 'R', 'S', 'E' };

    Queue<Bit>  treeShape;
    Queue<char> treeLeaves;
    flattenTree(reference, treeShape, treeLeaves);

    EXPECT_EQUAL(treeShape,  expectedShape);
    EXPECT_EQUAL(treeLeaves, expectedLeaves);

    deallocateTree(reference);
}

PROVIDED_TEST("compress, small example input") {
    EncodedData data = compress("STREETTEST");
    Queue<Bit>  treeShape   = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> treeChars   = { 'T', 'R', 'S', 'E' };
    Queue<Bit>  messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0 };

    EXPECT_EQUAL(data.treeShape, treeShape);
    EXPECT_EQUAL(data.treeLeaves, treeChars);
    EXPECT_EQUAL(data.messageBits, messageBits);
}

PROVIDED_TEST("Test end-to-end compress -> decompress") {
    Vector<string> inputs = {
        "HAPPY HIP HOP",
        "Nana Nana Nana Nana Nana Nana Nana Nana Batman"
        "Research is formalized curiosity. It is poking and prying with a purpose. – Zora Neale Hurston",
    };

    for (string input: inputs) {
        EncodedData data = compress(input);
        string output = decompress(data);

        EXPECT_EQUAL(input, output);
    }
}
