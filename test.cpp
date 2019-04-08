#include<ctime>

#include<iostream>

using namespace std;

int main(){
    time_t current;
    current = time(NULL);
    cout << current << "This is the time in seconds \n";
    return 0;
}