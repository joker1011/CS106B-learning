// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include <cctype>
#include "error.h"
#include "filelib.h"
#include "map.h"
#include "search.h"
#include "set.h"
#include "strlib.h"
#include "vector.h"
#include "simpio.h"
#include "testing/SimpleTest.h"

using namespace std;

/**
 * @brief cleanToken - 接收一个在正文文本中出现的字符串，返回一个“纯净版”的字符串
 *
 * 具体功能: 1.去除字符串首尾的标点符号，但不要管字符串内部的标点。
 *          2.确认字符串至少包含一个字符
 *          3.将字符串转换为小写
 *
 * @param string s ------- 输入字符串
 * @return "" ---- 空字符串 或 string s ----- “纯净版”字符串
 */
string cleanToken(string s)
{
    //字符的个数
    int alphaCount = 0;
    //开头和结尾标点符号的个数
    int beginPunctCount = 0, endPunctCount = 0;
    //如果输入的字符串长度为0则返回空字符串
    if(s.length() == 0){
        return "";
    }
    //如果输入的字符串长度为１且是字符，则转换成小写后返回．否则直接返回空字符串
    if(s.length() == 1){
        if(isalpha(s[0])){
            s = toLowerCase(s);
            return s;
        }
        return "";
    }
    //由于下面需要进行erase操作，可能造成空的字符串．
    //因此，这里先判断字符串中有无字符，如没有则直接返回空字符串
    for(char c:s){
        if(isalpha(c)){
            alphaCount++;
        }
    }
    if(alphaCount == 0){
        return "";
    }
    //从头开始,只要当前字符仍是标点符号,则计数器加１
    while(ispunct(s[beginPunctCount])){
            beginPunctCount++;
    }
    //删除标点符号段
    s.erase(0,beginPunctCount);
    //同理从后向前删除标点符号
    while(ispunct(s[s.size() - 1 - endPunctCount])){
            endPunctCount++;
    }
        s.erase(s.end() - endPunctCount, s.end());
    //将字符串转换为小写
    s = toLowerCase(s);
    return s;
}
/**
 * @brief gatherTokens -  从正文文本中提取出所有不重复的字符串
 * @param string text ------- 包含网页正文文本的字符串
 * @return Set<string>tokens ------- 网页正文文本中不重复的字符串
 */
Set<string> gatherTokens(string text)
{
    Set<string> tokens;
    Vector<string> textSplited = stringSplit(text, " ");
    for(string elem: textSplited){
        tokens.add(cleanToken(elem));
    }
    //去除空字符串
    tokens.remove("");
    return tokens;
}

/**
 * @brief buildIndex -  从数据库文件读取内容，并将其处理为反向索引的形式
 * @param string dbfile ------- 数据库文件路径
 * @param Map<string, Set<string>>& index ------- 反向索引
 * @return int lineNum/2 ------- 从数据库文件中处理过的网页数量
 */
int buildIndex(string dbfile, Map<string, Set<string>>& index)
{
    ifstream in;
    if (!openFile(in, dbfile))
        error("Cannot open file named " + dbfile);
    //将数据库文件以字符串形式读至Vector中
    Vector<string> URLandText;
    readEntireFile(in, URLandText);
    //建立一个用于存储所有URL的Set以避免URL出现重复
    Set<string> URL;
    //数据库文件的行数
    int lineNum = URLandText.size();
    //由于URL和正文内容是成两行一对的关系，所以这里i += 2
    for(int i = 0; i < lineNum; i += 2){
        URL.add(URLandText[i]);
        //从正文文本中提取出所有不重复的字符串
        Set<string> text = gatherTokens(URLandText[i + 1]);
        for(string elem: text){
            //存储所有和字符串相关的URL
            Set<string> textLinkedURL;
            //如果这个单词已经在索引的map键中，则取出其对应的值
            if(index.containsKey(elem)){
                textLinkedURL = index.get(elem);
            }
            //将新的URL添加到set中
            textLinkedURL.add(URLandText[i]);
            //重置map
            index.put(elem, textLinkedURL);
        }
    }
    return URL.size();
}
/**
 * @brief findQueryMatches - 根据反向索引，查询和输入关键词相关的网站
 *
 * 具体功能: 1.对于单个搜索关键词，结果是包含该关键词的所有URL。
 *          2.对于组合关键词，如果每个关键词的匹配结果有相同的URL，那么这些URL将被合并成一个
 *          3.组合关键词中使用 +/- 修饰符时，意思会有不同。
 *            - 如果没有使用修饰符连接关键词，那么搜索结果将是每个关键词匹配项的并集。（可以使用 Set 中的 unionWith 接口）
 *            - 如果关键词使用 + 号连接，那么搜索结果将是每个关键词匹配项的交集。（可以使用 Set 中的 intersect 接口）
 *            - 如果关键词使用 - 号连接，那么搜索结果将会从前面关键词的匹配项中，去除 - 号后面关键词的匹配项。（可以使用 Set 中的 difference接口）
 *          4.查询关键词中的每一个单词，也需要使用 cleanToken 做标准化处理
 *
 * 假设条件: 1.查询语句不存在空字符的情况，并且至少包含一个关键词
 *          2.如果一个查询语句包含修饰符，则 +、- 号一定是关键词的第一个字符
 *          3.查询语句的第一个关键词不可以有修饰符
 *          4.使用 cleanToken 标准化后的关键词也不能为空，所以至少需要包含一个字母。
 *
 * @param Map<string, Set<string>>& index ------- 反向索引
 * @param string query ------- 输入字符串
 * @return Set<string> result ------- 输入字符串关联的URL
 */
Set<string> findQueryMatches(Map<string, Set<string>>& index, string query)
{
    //存储与输入字符串关联的URL
    Set<string> result;
    //将输入的字符串分隔后存入Vector
    Vector<string> querySplited = stringSplit(query, " ");
    //处理字符串后，首先查找与第一个关键词关联的URL
    string cleanWord = cleanToken(querySplited[0]);
    result = index.get(cleanWord);
    //如果querySplited的长度大于１则会进入下面的循环，继续查找别的关键词的URL
    for(int i = 1; i < querySplited.size(); i++ ){
        //处理字符串后
        cleanWord = cleanToken(querySplited[i]);
        //如果后续字符串的开头是'+'，则取交集
        if(querySplited[i].front() == '+'){
            result = result.intersect(index.get(cleanWord));
        }
        //如果后续字符串的开头是'-'，则去除后面关键词的匹配项
        //注意：这里一定是else if而不是if,，否则下面的else会包含开头为'+'的情况
        else if(querySplited[i].front() == '-'){
            result = result.difference(index.get(cleanWord));
        }
        //如果后续字符串的开头没有'+/-'号，则取并集
        else{
            result = result.unionWith(index.get(cleanWord));
        }
    }
    return result;
}

/**
 * @brief searchEngine - 搜索引擎总函数
 *
 * 执行顺序:1.首先根据数据库文件内容，构建一个反向索引
 *         2.打印出构建反向索引用到的网页数量以及所有页面中包含了多少独一无二的单词
 *         3.进入一个循环，提示用户输入查询关键词
 *         4.对于每一个查询关键词，找出所有匹配页面并打印出 URLs
 *         5.当用户输入空字符串时，提示查询完成，程序结束
 *
 * @param string dbfile ------- 数据库文件路径
 * @return 无
 */
void searchEngine(string dbfile)
{
    //反向索引
    Map<string, Set<string>> index;
    cout << "stand by while building index" << endl;
    //建立反向索引并取得网页数
    int PageNum = buildIndex(dbfile, index);
    cout << "Indexed " << PageNum << " pages containing " << index.size() << " unique terms" << endl << endl;
    while(true){
        string query = getLine("Enter query sentence (RETURN/ENTER to quit):");
        //只要输入的字符串为空就return
        if(query.empty()){
            return;
        }
        //输入字符串不为空，则通过findQueryMatches函数获取和关键词关联的所有URL
        Set<string> result = findQueryMatches(index, query);
        cout << "Found " << result.size() << " matching pages" << endl;
        cout << result << endl << endl;
    }

}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("cleanToken on strings with no punctuation at beginning or end") {
    EXPECT_EQUAL(cleanToken("hello"), "hello");
    EXPECT_EQUAL(cleanToken("WORLD"), "world");
    EXPECT_EQUAL(cleanToken("CS*106B"), "cs*106b");
}

PROVIDED_TEST("cleanToken on strings with some punctuation at beginning and end") {
    EXPECT_EQUAL(cleanToken("/hello/"), "hello");
    EXPECT_EQUAL(cleanToken("~woRLD!"), "world");
}

PROVIDED_TEST("cleanToken on non-word strings (no letters)"){
    EXPECT_EQUAL(cleanToken("106"), "");
    EXPECT_EQUAL(cleanToken("~!106!!!"), "");
}

PROVIDED_TEST("gatherTokens from simple string") {
    Set<string> expected = {"go", "gophers"};
    EXPECT_EQUAL(gatherTokens("go go go gophers"), expected);
}

PROVIDED_TEST("gatherTokens correctly cleans tokens") {
    Set<string> expected = {"i", "love", "cs*106b"};
    EXPECT_EQUAL(gatherTokens("I _love_ CS*106B!"), expected);
}

PROVIDED_TEST("gatherTokens from seuss, 5 unique words, mixed case, punctuation") {
    Set<string> tokens = gatherTokens("One Fish Two Fish *Red* fish Blue fish ** 10 RED Fish?");
    EXPECT_EQUAL(tokens.size(), 5);
    EXPECT(tokens.contains("fish"));
    EXPECT(!tokens.contains("Fish"));
}

PROVIDED_TEST("buildIndex from tiny.txt, 4 pages, 11 unique tokens") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/tiny.txt", index);
    EXPECT_EQUAL(nPages, 4);
    EXPECT_EQUAL(index.size(), 11);
    EXPECT(index.containsKey("fish"));
}

PROVIDED_TEST("findQueryMatches from tiny.txt, single word query") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRed = findQueryMatches(index, "red");
    EXPECT_EQUAL(matchesRed.size(), 2);
    EXPECT(matchesRed.contains("www.dr.seuss.net"));
    Set<string> matchesHippo = findQueryMatches(index, "hippo");
    EXPECT(matchesHippo.isEmpty());
}

PROVIDED_TEST("findQueryMatches from tiny.txt, compound queries") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRedOrFish = findQueryMatches(index, "red fish");
    EXPECT_EQUAL(matchesRedOrFish.size(), 4);
    Set<string> matchesRedAndFish = findQueryMatches(index, "red +fish");
    EXPECT_EQUAL(matchesRedAndFish.size(), 1);
    Set<string> matchesRedWithoutFish = findQueryMatches(index, "red -fish");
    EXPECT_EQUAL(matchesRedWithoutFish.size(), 1);
}


// TODO: add your test cases here
STUDENT_TEST("cleanToken on strings with no punctuation at beginning or end") {
    EXPECT_EQUAL(cleanToken(""), "");
    EXPECT_EQUAL(cleanToken("1"), "");
    EXPECT_EQUAL(cleanToken("!"), "");
    EXPECT_EQUAL(cleanToken("W"), "w");
    EXPECT_EQUAL(cleanToken("!@#$%^&*()_+"), "");
    EXPECT_EQUAL(cleanToken("<<hel_l88o>>!"), "hel_l88o");
}
STUDENT_TEST("gatherTokens correctly cleans tokens") {
    Set<string> expected = {"this", "is", "te*st", "example"};
    EXPECT_EQUAL(gatherTokens("! && @this is*& te*st #_example!"), expected);
}
STUDENT_TEST("buildIndex from tiny.txt, 4 pages, 11 unique tokens") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/tiny.txt", index);
    EXPECT_EQUAL(nPages, 4);
    EXPECT_EQUAL(index.size(), 11);
    EXPECT(index.containsKey("fish"));
    TIME_OPERATION("buildIndex",buildIndex("res/tiny.txt", index));
}
STUDENT_TEST("findQueryMatches from tiny.txt, single word query") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesFish = findQueryMatches(index, "fish");
    EXPECT_EQUAL(matchesFish.size(), 3);
    EXPECT(matchesFish.contains("www.dr.seuss.net"));
    EXPECT(matchesFish.contains("www.shoppinglist.com"));
    EXPECT(matchesFish.contains("www.bigbadwolf.com"));
    Set<string> matchesCs106b = findQueryMatches(index, "cs106b");
    EXPECT(matchesCs106b.isEmpty());
}

STUDENT_TEST("findQueryMatches from tiny.txt, compound queries") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesFishOrBlue = findQueryMatches(index, "fish blue");
    EXPECT_EQUAL(matchesFishOrBlue.size(), 4);
    EXPECT(matchesFishOrBlue.contains("www.shoppinglist.com"));
    EXPECT(matchesFishOrBlue.contains("www.rainbow.org"));
    EXPECT(matchesFishOrBlue.contains("www.dr.seuss.net"));
    EXPECT(matchesFishOrBlue.contains("www.bigbadwolf.com"));
    Set<string> matchesFishAndBlue = findQueryMatches(index, "fish +blue");
    EXPECT_EQUAL(matchesFishAndBlue.size(), 1);
    EXPECT(matchesFishAndBlue.contains("www.dr.seuss.net"));
    Set<string> matchesFishWithoutBlue = findQueryMatches(index, "fish -blue");
    EXPECT_EQUAL(matchesFishWithoutBlue.size(), 2);
    EXPECT(matchesFishWithoutBlue.contains("www.shoppinglist.com"));
    EXPECT(matchesFishWithoutBlue.contains("www.bigbadwolf.com"));
}
