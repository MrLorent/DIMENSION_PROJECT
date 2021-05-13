#include <iostream>
#include <fstream>
#include <cstring>
#include "../include/model.h"
#include <math.h>

using namespace std;

int main (int argc, char** argv)
{
  Scene s = createScene();
  loadHeightmap(&s);
  cout << "lien recup : " << s.linkImg << endl ;
  cout << "scene recup : " << s.xsize << " "<< s.ysize << " "<< s.zmin <<" "<< s.zmax <<" " << s.znear <<" " <<s.zfar <<" " <<s.fov << endl ;

  

}
