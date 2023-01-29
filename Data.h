#ifndef DATA_H
#define DATA_H

#include <iostream>
#include <vector>

using Points = std::vector<std::string>;

struct Data
{
  int id;
  Points points;
};

#endif // DATA_H
