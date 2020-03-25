#include <fstream>
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <cmath>
#include <bitset>
using namespace std;

void calc(string ori, string dec) {
  ifstream o;
  o.open(ori, std::ifstream::binary);
  ifstream d;
  d.open(dec, std::ifstream::binary);
  
  double psnr;
  double nmse=0;
  double mse=0;
  int n=0;

  while (!o.eof() && !d.eof())
  {
      unsigned char temp1[sizeof(float)];
      o.read(reinterpret_cast<char*>(temp1), sizeof(float));
      unsigned char t1 = temp1[0];
      temp1[0] = temp1[3];
      temp1[3] = t1;
      t1 = temp1[1];
      temp1[1] = temp1[2];
      temp1[2] = t1;  
      float f1= reinterpret_cast<float&>(temp1);

      unsigned char temp2[sizeof(float)];
      d.read(reinterpret_cast<char*>(temp2), sizeof(float));
      unsigned char t2 = temp2[0];
      temp2[0] = temp2[3];
      temp2[3] = t2;
      t2 = temp2[1];
      temp2[1] = temp2[2];
      temp2[2] = t2;  
      float f2= reinterpret_cast<float&>(temp2);
      n++;
      nmse = nmse + pow(f2-f1,2);
      //cout<<"nmse:"<<nmse<<endl;
  }

  o.close();
  d.close();
  mse=nmse/n;
  psnr = 20*log10(pow(2,32)-1) - 10*log10(mse);
  cout<<"The PSNR is: "<<psnr<<" dB"<<endl;
  
}


int main(int argc, char** argv)
{

  string a = "";
  a = argv[1];
  string b = "";
  b = argv[2];
  cout<<"Doing Analysis..."<<endl;
  calc(a,b);
  
  return 0;
  
}