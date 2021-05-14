#include <iostream>
#include <fstream>
#include <cstring>
#include "../include/model.h"
#include "../include/QuadTree.h"

using namespace std;

int main (int argc, char** argv)
{
  Params params = createParams();
  loadParams(&params);
  int imageLoaded = loadHeightMap(params);

}
