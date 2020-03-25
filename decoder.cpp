

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <cmath>
#include <bitset>
using namespace std;
short pre[2];
short curr[2];
float k1,k2;

int floatto16int(float *f, short *s){
  int range;
  short max=0,min=0;
  for(int i=0; i<16; i++)
  {
    float temp = f[i] * 32767;
    s[i] = static_cast<short>(temp);
    if(s[i]>max)
      max=s[i];
    if(s[i]<min)
      min=s[i];
  }
  if(max>14336 && min<-16384)
    range=12;
  else if(max>7168 && min <-8192)
    range=11;
  else if(max>3584 && min <-4096)
    range=10;
  else if(max>1792 && min <-2048)
    range=9;
  else if(max>896 && min <-1024)
    range=8;
  else if(max>448 && min <-512)
    range=7;
  else if(max>224 && min <-256)
    range=6;
  else if(max>112 && min <-128)
    range=5;
  else if(max>56 && min <-64)
    range=4;
  else if(max>28 && min <-32)
    range=3;
  else if(max>14 && min <-32)
    range=2;
  else if(max>7 && min <-16)
    range=2;
  else
    range=0;
  return range;

}

void fil(int f){
    if(f==0){
      k1=0;
      k2=0;
    }else if(f==1){
      k1=15/16;
      k2=0;
    }else if(f==2){
      k1=61/32;
      k2=15/16;
    }else{
      k1=115/64;
      k2=13/16;
    }
}

unsigned char binarytonumber(int *bin){
  unsigned char temp=0;
  temp = bin[0] * 128 + temp;
  temp = bin[1] * 64 + temp;
  temp = bin[2] * 32 + temp;
  temp = bin[3] * 16 + temp;
  temp = bin[4] * 8 + temp;
  temp = bin[5] * 4 + temp;
  temp = bin[6] * 2 + temp;
  temp = bin[7] * 1 + temp;
  return temp;
}
short trans(short tr){
    tr = tr + 16;
    return tr;
}

unsigned char nibbletobyte(short a,short b){
  int numberarray[8];
  if(a<0){
    a=trans(a);
  }
  if(b<0){
    b=trans(b);
  }
  for(int i=3; i>=0;i--) { 
        numberarray[i] = a % 2; 
        a = a / 2; 
  }
  for(int i=7; i>3;i--) { 
        numberarray[i] = b % 2; 
        b = b / 2; 
  }
  unsigned char temp = binarytonumber(numberarray);
  return temp;
}



void cheader(int he, int *co){
  int f=0;
  int r=0;
  int array[8];
  int n = he;
  for(int i=7; i>=0;i--) { 
        array[i] = n % 2; 
        n = n / 2; 
  }
  r = array[0] * 8 + r;
  r = array[1] * 4 + r;
  r = array[2] * 2 + r;
  r = array[3] + r;
  f = array[4] * 2 + f;
  f = array[5] + f;
  co[0]=r;
  co[1]=f;


}

void bytetonibble(int te,short *fs){
  int array[8];
  int n = te;
  for(int i=7; i>=0;i--) { 
        array[i] = n % 2; 
        n = n / 2; 
  }
  int r=0;
  r = array[0] * 8 + r;
  r = array[1] * 4 + r;
  r = array[2] * 2 + r;
  r = array[3] + r;
  int f=0;
  f = array[4] * 8 + f;
  f = array[5] * 4 + f;
  f = array[6] * 2 + f;
  f = array[7] + f;
  if(r>7){
    r = r - 16;
  }
  if(f>7){
    f = f - 16;
  }
  fs[0]=r;
  fs[1]=f;
}

void rawData(unsigned char *ch,float *raw,int r,int f,int index){
  fil(f);
  short nibble[16];
  short sh[16];
  for(int i=0;i<8;i++){
    int temp=ch[i+1];
    short fs[2];
    bytetonibble(temp,fs);
    nibble[i*2]=fs[0];
    nibble[i*2+1]=fs[1];
  }
  for(int i=0;i<16;i++){
    if(i<2){
          if(index>1)
          {
            sh[0]=(nibble[i]*pow(2,r))+k1*pre[1]-k2*pre[0];
            sh[1]=(nibble[i+1]*pow(2,r))+k1*nibble[i]-k2*pre[1];
            i++;
          }
          else{
            sh[i]=nibble[i]*pow(2,r);
          }
    }
    else{
      sh[i]=(nibble[i]*pow(2,r))+k1*sh[i-1]-k2*sh[i-2];
      if(i==14){
          curr[0]=sh[i];
      }
      if(i==15){
          curr[1]=sh[i];
      }
    }
  }
  for(int i=0;i<16;i++){
    raw[i]=(float)sh[i]/32767;
  }



}
void decoder(unsigned char *c, string outs, int index) {
  ofstream offF;
  offF.open(outs, std::ofstream::binary|std::ofstream::app);
  //unsigned char he;
  unsigned char he = c[0];
  int head = (int) he;

  int co[2];
  cheader(head, co);
  int range=co[0];
  int filter=co[1];
  float raw[16];
  rawData(c,raw,range,filter,index);
  
  for(int i=0;i<16;i++){
    float ff=raw[i];
    unsigned char temp[sizeof(float)];
    memcpy(temp, &ff, sizeof(ff));
    unsigned char t = temp[0];
    temp[0] = temp[3];
    temp[3] = t;
    t = temp[1];
    temp[1] = temp[2];
    temp[2] = t;
    for(int i =0; i<4;i++){
      char tt = reinterpret_cast<unsigned char&>(temp[i]);
      offF.write(&tt, sizeof(char));
      //printf("%02x\n", tt);
    }
  }
  offF.close(); 

}
void read(string ins, string outs) {
  ifstream inF;
  inF.open(ins, std::ifstream::binary);

  ofstream offF;
  offF.open(outs, std::ofstream::binary);
  offF.close();
  int index=0;
  //int i=0;
  while (!inF.eof())
  {
    index++;
    unsigned char ch[9];
    for(int i=0;i<9;i++)
    {
      unsigned char temp[sizeof(char)];
      inF.read(reinterpret_cast<char*>(temp), sizeof(char));
      ch[i] = reinterpret_cast<unsigned char&>(temp);
    }
    
    decoder(ch,outs,index);
    pre[0]=curr[0];
    pre[1]=curr[1];

  } 
  inF.close();
}



int main(int argc, char** argv)
{
  string in = "";
  in = argv[1];
  string out = "";
  out = argv[2];
  cout<<"Doing Decoding..."<<endl;
  read(in,out);
  cout<<"Decoding Finished!"<<endl;
  return 0;
  
}
