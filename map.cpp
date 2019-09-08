//
// Created by lebou on 2019-09-08.
//

#include "map.h"
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>


std::vector<std:: vector<int>> getVertices(std::string cFile){
    //vector that will hold the vertices
    std::vector<std::vector <int>> vertices;
    //create file stream to read file line by line
    std::ifstream infile(cFile);
    std::string line;
    while (std::getline(infile, line)){
        //vector to hold current vertex
        std::vector<int> curr_vertex;
        //split the string by delimiter ,
        std::stringstream ss(line);
        std::string token;
        while(std::getline(ss,token,',')){
            curr_vertex.push_back(std::stoi(token));
        }
        //check that the line was a vertex line (with two numbers) and add it to the vertex list
        if(curr_vertex.size() == 2){
            vertices.push_back(curr_vertex);
        }
    }
    // return vertex list
    return vertices;
}

void printVertices(std::vector<std:: vector<int>> vList) {
    //print all vertices
    for (int i = 0; i < vList.size(); i++) {
        std::cout << "vertex " << i << " : \n";
        for (int j = 0; j < vList[i].size(); j++) {
            std::cout << vList[i][j] << "\n";
        }
    }
}