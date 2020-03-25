// brr.cpp : Defines the entry point for the console application.
//

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <cmath>
#include <bitset>
using namespace std;
short pre[2];
short curr[2];

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

void fil(int f,int *k){
    if(f==0){
      k[0]=0;
      k[1]=0;
    }else if(f==1){
      k[0]=15.0/16.0;
      k[1]=0;
    }else if(f==2){
      k[0]=61.0/32.0;
      k[1]=15.0/16.0;
    }else{
      k[0]=115.0/64.0;
      k[1]=13.0/16.0;
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

unsigned char header(int range, int ffff) {
  unsigned char he;
    int head[8];
    int n = range;
    for(int i=3; i>=0;i--) { 
        head[i] = n % 2; 
        n = n / 2; 
    }
    int quo = ffff / 2;
    int rem = ffff % 2;
    if(quo==0){
      head[4]=0;
    }
    else
      head[4]=1;
    if(rem==0){
      head[5]=0;
    }
    else
      head[5]=1;

    head[6]=0;
    head[7]=0;
   
    he = binarytonumber(head);
    return he;
    
}

void encoder(short *s,int range, int ffff, string outs, int index) {

  ofstream offF;
  offF.open(outs, std::ofstream::binary|std::ofstream::app);
  unsigned char he;
  he = header(range,ffff);
  char tt = reinterpret_cast<unsigned char&>(he);
  offF.write(&tt, sizeof(char));
  int k[2];
  fil(ffff,k);
  int k1=k[0];
  int k2=k[1];
  for(int i=0;i<16;i+=2)
  { short num1,num2;

    
    if(i<2)
    {
      if(index>1)
      {
            num1 = ceil((s[i]-k1*pre[1]+k2*pre[0])/pow(2,range));
            num2 = ceil((s[i+1]-k1*s[i]+k2*pre[1])/pow(2,range));
      }
      else{
            num1 = ceil(s[i]/pow(2,range));
            num2 = ceil(s[i+1]/pow(2,range));
      }
    }else
    {
      num1 = ceil((s[i]-k1*s[i-1]+k2*s[i-2])/pow(2,range));
      num2 = ceil((s[i+1]-k1*s[i]+k2*s[i-1])/pow(2,range));
    }
    if(num1>7){
        num1=7;
    }
    if(num1<-8){
        num1=-8;
    }
    if(num2>7){
        num2=7;
    }
    if(num2<-8){
        num2=-8;
    }

    unsigned char byt = nibbletobyte(num1,num2);
    tt = reinterpret_cast<unsigned char&>(byt);
    offF.write(&tt, sizeof(char));
  }
  offF.close();

}

void compare(float *f, short*s, int * choices, int range, int index){
  double err[13][4];
  for(int rr=0;rr<13;rr++){
    for(int eff=0;eff<4;eff++){
      err[rr][eff]=0;
      int k[2];
      fil(eff,k);
      float fff[16];
      short array[16];
      int k1=k[0];
      int k2=k[1];
      for(int i=0;i<16;i+=2)
      { 
        short num1,num2;
        if(i<2)
        {
          if(index>1)
          {
            num1 = ceil((s[i]-k1*pre[1]+k2*pre[0])/pow(2,rr));
            num2 = ceil((s[i+1]-k1*s[i]+k2*pre[1])/pow(2,rr));
          }
          else{
            num1 = ceil(s[i]/pow(2,rr));
            num2 = ceil(s[i+1]/pow(2,rr));
          }
        }else
        {
          num1 = ceil((s[i]-k1*s[i-1]+k2*s[i-2])/pow(2,rr));
          num2 = ceil((s[i+1]-k1*s[i]+k2*s[i-1])/pow(2,rr));
        }
        if(num1>7){
            num1=7;
        }
        if(num1<-8){
            num1=-8;
        }
        if(num2>7){
            num2=7;
        }
        if(num2<-8){
            num2=-8;
        }
        array[i]=num1;
        array[i+1]=num2;
      }
      short sss[16];
      for(int i=0;i<16;i++){
        if(i<2){
          if(index>1)
          {
            sss[0]=(array[i]*pow(2,rr))+k1*pre[1]-k2*pre[0];
            sss[1]=(array[i+1]*pow(2,rr))+k1*array[i]-k2*pre[1];
            i++;
          }
          else{
            sss[i]=array[i]*pow(2,rr);
          }
        }
        else{
          sss[i]=(array[i]*pow(2,rr))+k1*sss[i-1]-k2*sss[i-2];
        }
      }
      for(int i=0;i<16;i++){
        fff[i]=(float)sss[i]/32767;
      }
      double temp=0;
      for(int i=0;i<16;i++){
        temp = temp + pow(fff[i]-f[i],2);
      }
      err[rr][eff]=temp;
    }
  }
  int filt=0;
  int ra = range;
  double error=err[range][0];
  for(int rr=0;rr<13;rr++){
    for(int eff=0;eff<4;eff++){
      if(err[rr][eff]<error)
      {
        error=err[rr][eff];
        filt=eff;
        ra=rr;
      }
    }
  }
  choices[0]=filt;
  choices[1]=ra;

}

void read(string ins, string outs) {
  ifstream inF;
  inF.open(ins, std::ifstream::binary);
  ofstream offF;
  offF.open(outs, std::ofstream::binary);
  offF.close();

  int index=0;
  while (!inF.eof())
  {
   //for(int z=0;z<16;z++){
      index++;
      float f[16];
      float comp[16];
      short c[16];
      short com[16];
      for(int i=0;i<16;i++)
      {
        unsigned char temp[sizeof(float)];
        inF.read(reinterpret_cast<char*>(temp), sizeof(float));
        unsigned char t = temp[0];
        temp[0] = temp[3];
        temp[3] = t;
        t = temp[1];
        temp[1] = temp[2];
        temp[2] = t;
        
        f[i] = reinterpret_cast<float&>(temp);
        //cout<<f[i];
        comp[i]=f[i];
        
      }
      int ran = floatto16int(f, c);
      for(int j=0;j<16;j++){
        com[j]=c[j];
        if(j==14){
          curr[0]=c[j];
        }
        if(j==15){
          curr[1]=c[j];
        }
      }
      int choices[2];
      compare(comp,com,choices,ran,index);
      int fi = choices[0];
      int range = choices[1];
      //cout<<k1<<endl;
      //cout<<k2<<endl;
      encoder(c,range,fi,outs,index);
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
  cout<<"Doing Encoding..."<<endl;
  read(in,out);
  cout<<"Encoding Finished!"<<endl;
  return 0;
  
}
