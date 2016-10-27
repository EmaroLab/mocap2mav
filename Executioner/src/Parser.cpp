//
// Created by andreanisti on 27/10/16.
//

#include "Parser.h"



Parser::Parser() {





}

bool Parser::loadFile(std::string file) {

    std::ifstream myfile (file);
    if (myfile.is_open()) {
        //Clear token vector
        _tokens.clear();
        // Why not?
        _tokens.shrink_to_fit();

        //Build tokenized vector line by line
        std::string line;
        while ( getline (myfile,line)){

            std::vector<std::string> str_vector;
            strtk::parse(line,"=",str_vector);

            _tokens.push_back(str_vector);

        }

        for (int i = 0; i < _tokens.size()-1; ++i) {

            std::cout<<_tokens[i][0] << " " << _tokens[i][1] << std::endl;

        }


        return true;
    }
    else{

        std::cout << "Unable to open file" << std::endl;
        return false;


    }


}

void Parser::parse() {

    //Save type position
    std::vector<int> type_pos;

    for (int i = 0; i < _tokens.size()-1; ++i) {

        if(_tokens[i][0] == "type") type_pos.push_back(i);

    }
    //Start Parsing
    for (int j = 0; j < type_pos.size()-1; ++j) {




    }


}

