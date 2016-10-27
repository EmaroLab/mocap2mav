//
// Created by andreanisti on 27/10/16.
//

#ifndef MOCAP2MAV_PARSER_H
#define MOCAP2MAV_PARSER_H

#define strtk_no_tr1_or_boost
#include "strtk/strtk.hpp"
#include <iostream>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <list>

class Parser{

public:

    Parser();

    void parse();
    bool loadFile(std::string file);
    std::vector<std::vector<std::string>> _tokens;
    int _ntakeoff, _nmove, _nrotate,_nland;

private:




};

#endif //MOCAP2MAV_PARSER_H
