/*
 * Elements.cpp
 *
 *  Created on: 25.11.2014 ã.
 *      Author: Niel-Home
 */
#include "Elements.h"

int To_int(std::string tmp)
{
    int numbre=0;
    if ('0' <= tmp[0] && tmp[0] <= '9')
        {
            numbre=atoi(tmp.c_str());
            tmp="";
        }
    return numbre;
}


