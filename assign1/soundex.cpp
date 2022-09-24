/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "testing/SimpleTest.h"
#include "strlib.h"
#include "filelib.h"
#include "simpio.h"
#include <fstream>
#include <cctype>
#include <string>
#include <regex>
#include "vector.h"
using namespace std;

/* This function is intended to return a string which
 * contains only the letter characters from the original
 * (all non-letter characters are removed)
 *
 * WARNING: The provided code is buggy!
 *
 * Use unit tests to identify which inputs to this function
 * are incorrectly handled. Then, remove this comment and
 * replace it with a description of the bug you fixed.
 */
string removeNonLetters(string s) {
    string result;
    int n = s.length();
    for (int i = 0; i < n; i++) {
        if (isalpha(s[i])) {
            result += s[i];
        }
    }
    return result;
}
/* This function Encode Name string according to the soundex rule
 */
string soundexEncode(string s){
    regex reg0("[AEIOUHWY]"), reg1("[BFPV]"), reg2("[CGJKQSXZ]"),
          reg3("[DT]"), reg4("[L]"), reg5("[MN]"), reg6("[R]");
    string temp = s;
    temp = regex_replace(temp, reg0, "0");
    temp = regex_replace(temp, reg1, "1");
    temp = regex_replace(temp, reg2, "2");
    temp = regex_replace(temp, reg3, "3");
    temp = regex_replace(temp, reg4, "4");
    temp = regex_replace(temp, reg5, "5");
    temp = regex_replace(temp, reg6, "6");
    s = temp;
    return s;
}
/* This function remove the repetitive letters of the Encoded name
 */
string removeDuplicates(string s){
    int k = 0;
    int n = s.length();
    //prev---previous '\0'-------NULL
    char prev = '\0';
    for(int i = 0; i < n; i++){

        //if current char is different from the previous char
        if(prev != s[i]){

            //move the first distinct character behind the previous char
            s[k++] = s[i];
        }

        // update prev for the next loop
        prev = s[i];
    }
    //All the distinct characters are moved to the front part of string, then erase the rest duplicated part
    s.erase(k);
    return s;
}
/* This function remove certain character from a string
 */
string removeCertainCharacter(string s, char target)
{
    int n = s.length();
    string temp;
    for(int i = 0; i < n; i++){
        if(s.at(i) != target){
            temp += s.at(i);
        }
    }
    s = temp;
    return s;
}
/* This function check whether the size of soundex code is 4.
 * if not, then modified it.
 */
string soundexLengthCheck(string s)
{
    int n = s.length();
    if (n < 4){
        for(int i = 0; i < 4 - n; i++){
            s.append("0");
        }
    }
    else if(n > 4){
        s.erase(4);
    }
    return s;
}
/* This is a top-level function that transfer a name string into a soundex code
 */
string soundex(string s) {
    string temp = s;
    temp = removeNonLetters(temp);
    temp = toUpperCase(temp);
    temp = soundexEncode(temp);
    temp = removeDuplicates(temp);
    temp.at(0) = toUpperCase(s.at(0));
    temp = removeCertainCharacter(temp, '0');
    temp = soundexLengthCheck(temp);
    s = temp;
    return s;
}
/* Accroding to the input name, this is function find Names in
 * database file that have the same soundex code.
 */
void soundexSearch(string filepath) {
    // The proivded code opens the file with the given name
    // and then reads the lines of that file into a vector.
    ifstream in;
    Vector<string> databaseNames;

    if (openFile(in, filepath)) {
        readEntireFile(in, databaseNames);
    }
    cout << "Read file " << filepath << ", "
         << databaseNames.size() << " names found." << endl;
    // The names in the database are now stored in the provided
    // vector named databaseNames
    Vector<string> matchNames;
    while(true){
        cout << endl;
        string inputName = getLine("Enter a surname (RETURN to quit)");
        //Enter to quit
        if(inputName.empty()){
            cout << "All done!"<< endl;
            break;
        }

        inputName = soundex(inputName);
        cout << "Soundex code is " << inputName << endl;
        //find matching names in the database and store them in vector matchNames
        for (int i = 0 ; i < databaseNames.size(); i++){
            if(inputName == soundex(databaseNames.get(i))){
                matchNames.add(databaseNames.get(i));
            }
            if (i % 1000 == 0) cout << "." << flush; // progress bar
        }
        //cannot find matching name
        if(matchNames.isEmpty()){
            cout << "No matches from database.";
            continue;
        }
        matchNames.sort();
        cout << endl << "Matches from database: {";
        int j = 0;
        for(j ; j < matchNames.size() - 1; j++){
            cout << "\"" << matchNames.get(j) << "\", ";
        }
        cout << "\"" <<matchNames.get(j) << "\"}" << endl;
        //clean the vector
        matchNames.clear();
    }

}


/* * * * * * Test Cases * * * * * */


PROVIDED_TEST("Test removing puntuation, digits, and spaces") {
    string s = "O'Hara";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "OHara");
    s = "Planet9";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "Planet");
    s = "tl dr";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "tldr");
    s = " ";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "");
    s = "1213213213";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "");
}


PROVIDED_TEST("Sample inputs from handout") {
    EXPECT_EQUAL(soundex("Curie"), "C600");
    EXPECT_EQUAL(soundex("O'Conner"), "O256");
}

PROVIDED_TEST("hanrahan is in lowercase") {
    EXPECT_EQUAL(soundex("hanrahan"), "H565");
}

PROVIDED_TEST("DRELL is in uppercase") {
    EXPECT_EQUAL(soundex("DRELL"), "D640");
}

PROVIDED_TEST("Liu has to be padded with zeros") {
    EXPECT_EQUAL(soundex("Liu"), "L000");
}

PROVIDED_TEST("Tessier-Lavigne has a hyphen") {
    EXPECT_EQUAL(soundex("Tessier-Lavigne"), "T264");
}

PROVIDED_TEST("Au consists of only vowels") {
    EXPECT_EQUAL(soundex("Au"), "A000");
}

PROVIDED_TEST("Egilsdottir is long and starts with a vowel") {
    EXPECT_EQUAL(soundex("Egilsdottir"), "E242");
}

PROVIDED_TEST("Jackson has three adjcaent duplicate codes") {
    EXPECT_EQUAL(soundex("Jackson"), "J250");
}

PROVIDED_TEST("Schwarz begins with a pair of duplicate codes") {
    EXPECT_EQUAL(soundex("Schwarz"), "S620");
}

PROVIDED_TEST("Van Niekerk has a space between repeated n's") {
    EXPECT_EQUAL(soundex("Van Niekerk"), "V526");
}

PROVIDED_TEST("Wharton begins with Wh") {
    EXPECT_EQUAL(soundex("Wharton"), "W635");
}

PROVIDED_TEST("Ashcraft is not a special case") {
    // Some versions of Soundex make special case for consecutive codes split by hw
    // We do not make this special case, just treat same as codes split by vowel
    EXPECT_EQUAL(soundex("Ashcraft"), "A226");
}

// TODO: add your test cases here
STUDENT_TEST("removeNonLetters test") {
     EXPECT_EQUAL(removeNonLetters("Curie"), "Curie");
     EXPECT_EQUAL(removeNonLetters("O'Conner"), "OConner");
     EXPECT_EQUAL(removeNonLetters("Angelou"), "Angelou");
}
STUDENT_TEST("soundexEncode test") {
     EXPECT_EQUAL(soundexEncode(toUpperCase("Curie")), "20600");
     EXPECT_EQUAL(soundexEncode(toUpperCase("OConner")), "0205506");
     EXPECT_EQUAL(soundexEncode(toUpperCase("Angelou")), "0520400");
}
STUDENT_TEST("removeDuplicates test") {
     EXPECT_EQUAL(removeDuplicates("20600"), "2060");
     EXPECT_EQUAL(removeDuplicates("0205506"), "020506");
     EXPECT_EQUAL(removeDuplicates("0520400"), "052040");
}
STUDENT_TEST("removeCertainCharacter test") {
     EXPECT_EQUAL(removeCertainCharacter("C060", '0'), "C6");
     EXPECT_EQUAL(removeCertainCharacter("O20506", '0'), "O256");
     EXPECT_EQUAL(removeCertainCharacter("A520400", '0'), "A524");
}
STUDENT_TEST("soundexLengthCheck test") {
     EXPECT_EQUAL(soundexLengthCheck("C6"), "C600");
     EXPECT_EQUAL(soundexLengthCheck("O256"), "O256");
     EXPECT_EQUAL(soundexLengthCheck("A524"), "A524");
}
STUDENT_TEST("soundex test") {
     EXPECT_EQUAL(soundex("Curie"), "C600");
     EXPECT_EQUAL(soundex("O'Conner"), "O256");
     EXPECT_EQUAL(soundex("Angelou"), "A524");
}

