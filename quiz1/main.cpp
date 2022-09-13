#include <iostream>
#include <string>
#include <cmath>
#include <iomanip>
#include "console.h"
#include "simpio.h"


using namespace std;

void cplusplus_Version();
int number_Figures(int num);
void triangle_Display();


int main() {

    cplusplus_Version();
    triangle_Display();
    return 0;
}

void cplusplus_Version(){

    cout << "Current C++ version is: ";
    if(__cplusplus == 199711L){
        cout << "C++98"<<endl;
    }
    else if(__cplusplus == 201103L){
        cout << "C++11"<<endl;
    }
    else if(__cplusplus == 201402L){
        cout << "C++14"<<endl;
    }
    else if(__cplusplus == 201703L){
        cout << "C++17"<<endl;
    }
    else if(__cplusplus == 202002L){
        cout << "C++20"<<endl;
    }
    else{
        cout << "Unkonw"<<endl;
    }
}

//get width of cout
int number_Figures(int num){

    int figure = 0;
    while(num){
        num /= 10;
        figure++;
    }
    return figure;
}

void triangle_Display(){

    cout<<"Please input layer number: ";
    int layer = getInteger();
    int width = number_Figures(layer);

    //number to display
    int number = 1;

    //i:horizontal axis
    //j:vertical axis
    //start from the top  (j = layer - 1)!!!!
    for(int j = layer - 1; j > 0 ; j--){
        for(int i = 1 - layer; i <= layer - 1; i++){
            if(abs(i)+abs(j) == layer - 1){
               cout << setw(width) << number;
            }
            else{
               cout << setw(width) << " ";
            }
        }
        cout << endl;

        number++;

    }
    //last line
    for(int i = 0 ; i < layer; i++){
        cout<< setw(width) << number << setw(width) << " ";
    }
}


