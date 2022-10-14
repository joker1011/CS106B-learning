/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "recursion.h"
#include "testing/SimpleTest.h"

using namespace std;

/**
 * @brief operatorsFrom - 从字符串中提取出所有的括号运算符
 *
 * 递归类型：tree Recursion
 *
 * @param string str ------- 输入字符串
 * @return Base Case ------- "" 空字符串    Recursive Case ------- r str[0] + operatorsFrom(str.substr(1)) 或 operatorsFrom(str.substr(1))
 */
string operatorsFrom(string str) {
    //Base Case
    if(str.empty()){
        return "";
    }
    //Recursive Case
    //如果当前字符串的第一个字符是括号运算符,则返回第一个字符加上以字符串剩余部分为参数的递归函数
    else if (   str[0] == '(' || str[0] == ')'
             || str[0] == '{' || str[0] == '}'
             || str[0] == '[' || str[0] == ']') {
        return str[0] + operatorsFrom(str.substr(1));
    }
    //如果当前字符串的第一个字符不是括号运算符,则直接返回以字符串剩余部分为参数的递归函数
    else {
        return operatorsFrom(str.substr(1));
    }
}

 /**
 * @brief operatorsAreMatched - 检查括号运算符序列是否正确平衡
 *
 * 递归类型：tree Recursion
 *
 * @param string ops ------- 提取出的括号运算符
 * @return Base Case ------- true    Recursive Case ------- operatorsAreMatched(ops)
 */
bool operatorsAreMatched(string ops) {
    //Base Case
    //如果字符串已经删减为空则直接返回true
    if(ops.empty()){
        return true;
    }
    //Recursive Case
    int found = ops.find("()");
    if(found != string::npos){
        ops.erase(found, 2);
        return operatorsAreMatched(ops);
    }
    found = ops.find("[]");
    if(found != string::npos){
        ops.erase(found, 2);
        return operatorsAreMatched(ops);
    }
    found = ops.find("{}");
    if(found != string::npos){
        ops.erase(found, 2);
        return operatorsAreMatched(ops);
    }
    //Base Case
    //如果最后剩下的不是空字符串，则返回false
    return false;
}

/**
 * 经测试采用for循环的版本要快一点
 */
//bool operatorsAreMatched1(string ops) {
//    //Base Case
//    //如果字符串已经删减为空则直接返回true
//    if(ops.empty()){
//        return true;
//    }
//    //Recursive Case
//    //从头到尾遍历字符串．如果在字符串某一处左括号运算符后面紧接着右括号运算符，则将这一对括号去除并将剩下的字符串作为参数传递到递归函数中
//    for(int i = 0 ; i < ops.size() - 1; i++){
//        if(ops[i] == '(' && ops[i + 1] == ')'){
//            ops.erase(i,2);
//            return operatorsAreMatched(ops);
//        }
//        if(ops[i] == '[' && ops[i + 1] == ']'){
//            ops.erase(i,2);
//            return operatorsAreMatched(ops);
//        }
//        if(ops[i] == '{' && ops[i + 1] == '}'){
//            ops.erase(i,2);
//            return operatorsAreMatched(ops);
//        }
//    }
//    //Base Case
//    //如果最后剩下的不是空字符串，则返回false
//    return false;
//}

/*
 * The isBalanced function assumes correct implementation of
 * the above two functions operatorsFrom and operatorsMatch.
 * It uses operatorsFrom to extract the operator characters
 * from the input string and then confirms that those
 * operators are balanced by using operatorsMatch.
 * You should not modify the provided code in the isBalanced
 * function.  If the previous two functions have been implemented
 * correctly, the provided isBalanced will correctly report whether
 * the input string has balanced bracketing operators.
 */
bool isBalanced(string str) {
    string ops = operatorsFrom(str);
    return operatorsAreMatched(ops);
}

//void timeTest1(){
//    for(int i = 0; i < 5000000; i++) {
//        operatorsAreMatched("([{}])");
//    }
//}

//void timeTest2(){
//    for(int i = 0; i < 5000000; i++) {
//        operatorsAreMatched1("([{}])");
//    }
//}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("operatorsFrom on simple example") {
    EXPECT_EQUAL(operatorsFrom("vec[3]"), "[]");
}

PROVIDED_TEST("operatorsAreMatched on simple example") {
    EXPECT(operatorsAreMatched("{}"));
}

PROVIDED_TEST("isBalanced on example from writeup") {
    string example ="int main() { int x = 2 * (vec[2] + 3); x = (1 + random()); }";
    EXPECT(isBalanced(example));
}

PROVIDED_TEST("isBalanced on non-balanced examples from writeup") {
    EXPECT(!isBalanced("( ( [ a ] )"));
    EXPECT(!isBalanced("3 ) ("));
    EXPECT(!isBalanced("{ ( x } y )"));
}
STUDENT_TEST("operatorsFrom on simple example") {
    EXPECT_EQUAL(operatorsFrom("{basa}[A](3)"), "{}[]()");
}

STUDENT_TEST("operatorsFrom on simple example") {
    EXPECT_EQUAL(operatorsFrom("{a[s(d)d]s}"), "{[()]}");
}

STUDENT_TEST("operatorsFrom on simple example") {
    EXPECT_EQUAL(operatorsFrom("}(s]sd[1})"), "}(][})");
}

STUDENT_TEST("operatorsAreMatched on simple example") {
    EXPECT(operatorsAreMatched("[]{}()"));
    EXPECT(operatorsAreMatched("([{}])"));
}

STUDENT_TEST("operatorsAreMatched on simple example") {
    EXPECT(!operatorsAreMatched("[{](})"));
    EXPECT(!operatorsAreMatched("}{"));
}

STUDENT_TEST("isBalanced on example from writeup") {
    string example ="for(int i = 0; i < ops.size(); i++){cout << ops[i];}";
    EXPECT(isBalanced(example));
}

STUDENT_TEST("isBalanced on non-balanced examples from writeup") {
    EXPECT(!isBalanced("}as{[a]s(s)"));
    EXPECT(!isBalanced("[s(a[s]{sd}"));
}

//STUDENT_TEST("time usage of two operatorsAreMatched") {
//    TIME_OPERATION(5000000, timeTest1());
//}

//STUDENT_TEST("time usage of two operatorsAreMatched") {
//    TIME_OPERATION(5000000, timeTest2());
//}
