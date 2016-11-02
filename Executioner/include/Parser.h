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
#include "lcm_messages/exec/task.hpp"
#include "common/common.h"

class Parser{

public:

    Parser();

    bool parse();
    bool loadFile(std::string file);
    std::vector<std::vector<std::string>> _tokens;
    std::deque<exec::task> _taskListParsed;
    bool parseAction(std::string a, int pos);

private:




};

#endif //MOCAP2MAV_PARSER_H
