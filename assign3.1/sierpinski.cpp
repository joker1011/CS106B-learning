/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include "recursion.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * This function draws a filled, black triangle on the provided GWindow
 * that is defined by the corner GPoints one, two, and three. This
 * function has been provided for you and does not need to be modified.
 */
void fillBlackTriangle(GWindow& window, GPoint one, GPoint two, GPoint three) {
    window.setColor("black");
    window.fillPolygon( {one, two, three} );
}

/**
* @brief drawSierpinskiTriangle - 生成Sierpinski自相似三角形
*
* 递归类型：tree Recursion
*
* @param GWindow& window ------- 提取出的括号运算符
* @param GPoint& one, GPoint& two, GPoint& three ------- 三角形顶点
* @param int order ------- 阶数
* @param int& count -------- 三角形数
* @return Base Case ------- void
*/
void drawSierpinskiTriangle(GWindow &window, GPoint &one, GPoint &two, GPoint &three, int order, int &count) {
    //Base Case
    //如果阶数减到了0，则说明递归达到了最底层．此时，可以开始绘制最小的三角形并且将三角形的数量加１
    if(order == 0) {
        count++;
        fillBlackTriangle(window, one, two, three);
        return;
    }
    //求中点
    GPoint midPoint12 ((one.x + two.x)   / 2, (one.y + two.y)   / 2);
    GPoint midPoint13 ((one.x + three.x) / 2, (one.y + three.y) / 2);
    GPoint midPoint23 ((two.x + three.x) / 2, (two.y + three.y) / 2);
    //Recursive Case
    //新的顶点传给递归函数
    //阶数减１．为了确保单一分支不影响其他分支的阶数，这里必须采用值传递的形式以确保每一个level的阶数保持一致
    drawSierpinskiTriangle(window, one       , midPoint12, midPoint13, order - 1, count);
    drawSierpinskiTriangle(window, midPoint12, two       , midPoint23, order - 1, count);
    drawSierpinskiTriangle(window, midPoint13, midPoint23, three     , order - 1, count);
}
/**
 *  @brief    drawSierpinskiTriangle 的包装函数
 *  @newParam int count ------- 三角形计数
 *  @return   count ------- 三角形数
 */
int drawSierpinskiTriangle(GWindow &window, GPoint one, GPoint two, GPoint three, int order) {
    int count = 0;
    drawSierpinskiTriangle(window, one, two, three, order, count);
    return count;
}


/* * * * * * Test Cases * * * * * */

/*
 * Do not modify the code beyond this line! There are no
 * unit tests for this problem. You should do all testing
 * via the interactive demo.
 */

PROVIDED_TEST("Test fractal drawing interactively using graphical demo") {
    runInteractiveDemo();
}

