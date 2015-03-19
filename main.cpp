/*
 * main.cpp
 *
 *  Created on: Jan 30, 2015
 *      Author: Artur
 */


#include <iostream>
#include <Socket.h>
#include <map>
#include <string>
#include <sstream>
#include <EmotivEpocController.h>
#include <ProsthesisConsoleApplication.h>

using namespace std;



int main(int argc, char *argv[]){


    int portNr = (argc != 2) ? -1 : stoi(string(argv[1])) ;

//    cout << portNr;

    try {

        ProsthesisConsoleApplication application(portNr);

        application.applicationStart();

    } catch(const char*e) {
        std::cerr << "\nERROR: " << e << std::endl;
        std::exit(-1);
    }


    return 0;
}


