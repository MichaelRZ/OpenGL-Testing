#include <iostream>
#include <string>
#include <cmath>
#include <algorithm>
#include <unistd.h>
#include <chrono>
using namespace std;

struct guess{
  int gs[5];
  int distance;
  guess(){
    distance = 0;
  }
  guess(int vals[]) {
    for(int i = 0; i < 5; i++)
      gs[i] = vals[i];
    // distance = (vals[0] - 5)*(vals[0] - 5) + (vals[1] - 5)*(vals[1] - 5) + (vals[2] - 5)*(vals[2] - 5) + (vals[3] - 5)*(vals[3] - 5) + (vals[4] - 5)*(vals[4] - 5);
    int targ = 5;
    distance = pow(vals[0] - targ,2) + pow(vals[1] - targ,2) + pow(vals[2] - targ,2) + pow(vals[3] - targ,2) + pow(vals[4] - targ,2);
    // cout << distance << endl;
  }
  bool operator< (const guess &other) const {
    return distance < other.distance;
  }
  bool operator> (const guess &other) const {
    return distance > other.distance;
  }
};

struct howClose{
  guess distances[32];
  howClose(){}
  howClose(int lo[], int hi[]){
    int vals[5];
    for(int i = 0; i < 5; i++)
      vals[i] = lo[i];
    distances[0] = guess(vals);
    vals[4] = hi[4]; distances[1] = guess(vals); //next 2
    vals[3] = hi[3]; distances[2] = guess(vals); //next 4
    vals[4] = lo[4]; distances[3] = guess(vals); //2
    vals[2] = hi[2]; distances[4] = guess(vals); //next 8
    vals[4] = hi[4]; distances[5] = guess(vals); //2
    vals[3] = lo[3]; distances[6] = guess(vals); //4
    vals[4] = lo[4]; distances[7] = guess(vals); //2
    vals[1] = hi[1]; distances[8] = guess(vals); //next 16
    vals[4] = hi[4]; distances[9] = guess(vals); //2
    vals[3] = hi[3]; distances[10] = guess(vals); //4
    vals[4] = lo[4]; distances[11] = guess(vals); //2
    vals[2] = lo[2]; distances[12] = guess(vals); //8
    vals[4] = hi[4]; distances[13] = guess(vals); //2
    vals[3] = lo[3]; distances[14] = guess(vals); //4
    vals[4] = lo[4]; distances[15] = guess(vals); //2
    vals[0] = hi[0]; distances[16] = guess(vals); //rest of guesses
    vals[4] = hi[4]; distances[17] = guess(vals); //2
    vals[3] = hi[3]; distances[18] = guess(vals); //4
    vals[4] = lo[4]; distances[19] = guess(vals); //2
    vals[2] = hi[2]; distances[20] = guess(vals); //8
    vals[4] = hi[4]; distances[21] = guess(vals); //2
    vals[3] = lo[3]; distances[22] = guess(vals); //4
    vals[4] = lo[4]; distances[23] = guess(vals); //2
    vals[1] = lo[1]; distances[24] = guess(vals); //16
    vals[4] = hi[4]; distances[25] = guess(vals); //2
    vals[3] = hi[3]; distances[26] = guess(vals); //4
    vals[4] = lo[4]; distances[27] = guess(vals); //2
    vals[2] = lo[2]; distances[28] = guess(vals); //8
    vals[4] = hi[4]; distances[29] = guess(vals); //2
    vals[3] = lo[3]; distances[30] = guess(vals); //4
    vals[4] = lo[4]; distances[31] = guess(vals); //2
    if (distances[0] > distances[1]){swap(distances[0],distances[1]);}
    if (distances[2] > distances[3]){swap(distances[2],distances[3]);}
    if (distances[0] > distances[2]){swap(distances[0],distances[2]);}
    if (distances[1] > distances[3]){swap(distances[1],distances[3]);}
    if (distances[1] > distances[2]){swap(distances[1],distances[2]);}
    if (distances[4] > distances[5]){swap(distances[4],distances[5]);}
    if (distances[6] > distances[7]){swap(distances[6],distances[7]);}
    if (distances[4] > distances[6]){swap(distances[4],distances[6]);}
    if (distances[5] > distances[7]){swap(distances[5],distances[7]);}
    if (distances[5] > distances[6]){swap(distances[5],distances[6]);}
    if (distances[0] > distances[4]){swap(distances[0],distances[4]);}
    if (distances[2] > distances[6]){swap(distances[2],distances[6]);}
    if (distances[2] > distances[4]){swap(distances[2],distances[4]);}
    if (distances[1] > distances[5]){swap(distances[1],distances[5]);}
    if (distances[3] > distances[7]){swap(distances[3],distances[7]);}
    if (distances[3] > distances[5]){swap(distances[3],distances[5]);}
    if (distances[1] > distances[2]){swap(distances[1],distances[2]);}
    if (distances[3] > distances[4]){swap(distances[3],distances[4]);}
    if (distances[5] > distances[6]){swap(distances[5],distances[6]);}
    if (distances[8] > distances[9]){swap(distances[8],distances[9]);}
    if (distances[10] > distances[11]){swap(distances[10],distances[11]);}
    if (distances[8] > distances[10]){swap(distances[8],distances[10]);}
    if (distances[9] > distances[11]){swap(distances[9],distances[11]);}
    if (distances[9] > distances[10]){swap(distances[9],distances[10]);}
    if (distances[12] > distances[13]){swap(distances[12],distances[13]);}
    if (distances[14] > distances[15]){swap(distances[14],distances[15]);}
    if (distances[12] > distances[14]){swap(distances[12],distances[14]);}
    if (distances[13] > distances[15]){swap(distances[13],distances[15]);}
    if (distances[13] > distances[14]){swap(distances[13],distances[14]);}
    if (distances[8] > distances[12]){swap(distances[8],distances[12]);}
    if (distances[10] > distances[14]){swap(distances[10],distances[14]);}
    if (distances[10] > distances[12]){swap(distances[10],distances[12]);}
    if (distances[9] > distances[13]){swap(distances[9],distances[13]);}
    if (distances[11] > distances[15]){swap(distances[11],distances[15]);}
    if (distances[11] > distances[13]){swap(distances[11],distances[13]);}
    if (distances[9] > distances[10]){swap(distances[9],distances[10]);}
    if (distances[11] > distances[12]){swap(distances[11],distances[12]);}
    if (distances[13] > distances[14]){swap(distances[13],distances[14]);}
    if (distances[0] > distances[8]){swap(distances[0],distances[8]);}
    if (distances[4] > distances[12]){swap(distances[4],distances[12]);}
    if (distances[4] > distances[8]){swap(distances[4],distances[8]);}
    if (distances[2] > distances[10]){swap(distances[2],distances[10]);}
    if (distances[6] > distances[14]){swap(distances[6],distances[14]);}
    if (distances[6] > distances[10]){swap(distances[6],distances[10]);}
    if (distances[2] > distances[4]){swap(distances[2],distances[4]);}
    if (distances[6] > distances[8]){swap(distances[6],distances[8]);}
    if (distances[10] > distances[12]){swap(distances[10],distances[12]);}
    if (distances[1] > distances[9]){swap(distances[1],distances[9]);}
    if (distances[5] > distances[13]){swap(distances[5],distances[13]);}
    if (distances[5] > distances[9]){swap(distances[5],distances[9]);}
    if (distances[3] > distances[11]){swap(distances[3],distances[11]);}
    if (distances[7] > distances[15]){swap(distances[7],distances[15]);}
    if (distances[7] > distances[11]){swap(distances[7],distances[11]);}
    if (distances[3] > distances[5]){swap(distances[3],distances[5]);}
    if (distances[7] > distances[9]){swap(distances[7],distances[9]);}
    if (distances[11] > distances[13]){swap(distances[11],distances[13]);}
    if (distances[1] > distances[2]){swap(distances[1],distances[2]);}
    if (distances[3] > distances[4]){swap(distances[3],distances[4]);}
    if (distances[5] > distances[6]){swap(distances[5],distances[6]);}
    if (distances[7] > distances[8]){swap(distances[7],distances[8]);}
    if (distances[9] > distances[10]){swap(distances[9],distances[10]);}
    if (distances[11] > distances[12]){swap(distances[11],distances[12]);}
    if (distances[13] > distances[14]){swap(distances[13],distances[14]);}
    if (distances[16] > distances[17]){swap(distances[16],distances[17]);}
    if (distances[18] > distances[19]){swap(distances[18],distances[19]);}
    if (distances[16] > distances[18]){swap(distances[16],distances[18]);}
    if (distances[17] > distances[19]){swap(distances[17],distances[19]);}
    if (distances[17] > distances[18]){swap(distances[17],distances[18]);}
    if (distances[20] > distances[21]){swap(distances[20],distances[21]);}
    if (distances[22] > distances[23]){swap(distances[22],distances[23]);}
    if (distances[20] > distances[22]){swap(distances[20],distances[22]);}
    if (distances[21] > distances[23]){swap(distances[21],distances[23]);}
    if (distances[21] > distances[22]){swap(distances[21],distances[22]);}
    if (distances[16] > distances[20]){swap(distances[16],distances[20]);}
    if (distances[18] > distances[22]){swap(distances[18],distances[22]);}
    if (distances[18] > distances[20]){swap(distances[18],distances[20]);}
    if (distances[17] > distances[21]){swap(distances[17],distances[21]);}
    if (distances[19] > distances[23]){swap(distances[19],distances[23]);}
    if (distances[19] > distances[21]){swap(distances[19],distances[21]);}
    if (distances[17] > distances[18]){swap(distances[17],distances[18]);}
    if (distances[19] > distances[20]){swap(distances[19],distances[20]);}
    if (distances[21] > distances[22]){swap(distances[21],distances[22]);}
    if (distances[24] > distances[25]){swap(distances[24],distances[25]);}
    if (distances[26] > distances[27]){swap(distances[26],distances[27]);}
    if (distances[24] > distances[26]){swap(distances[24],distances[26]);}
    if (distances[25] > distances[27]){swap(distances[25],distances[27]);}
    if (distances[25] > distances[26]){swap(distances[25],distances[26]);}
    if (distances[28] > distances[29]){swap(distances[28],distances[29]);}
    if (distances[30] > distances[31]){swap(distances[30],distances[31]);}
    if (distances[28] > distances[30]){swap(distances[28],distances[30]);}
    if (distances[29] > distances[31]){swap(distances[29],distances[31]);}
    if (distances[29] > distances[30]){swap(distances[29],distances[30]);}
    if (distances[24] > distances[28]){swap(distances[24],distances[28]);}
    if (distances[26] > distances[30]){swap(distances[26],distances[30]);}
    if (distances[26] > distances[28]){swap(distances[26],distances[28]);}
    if (distances[25] > distances[29]){swap(distances[25],distances[29]);}
    if (distances[27] > distances[31]){swap(distances[27],distances[31]);}
    if (distances[27] > distances[29]){swap(distances[27],distances[29]);}
    if (distances[25] > distances[26]){swap(distances[25],distances[26]);}
    if (distances[27] > distances[28]){swap(distances[27],distances[28]);}
    if (distances[29] > distances[30]){swap(distances[29],distances[30]);}
    if (distances[16] > distances[24]){swap(distances[16],distances[24]);}
    if (distances[20] > distances[28]){swap(distances[20],distances[28]);}
    if (distances[20] > distances[24]){swap(distances[20],distances[24]);}
    if (distances[18] > distances[26]){swap(distances[18],distances[26]);}
    if (distances[22] > distances[30]){swap(distances[22],distances[30]);}
    if (distances[22] > distances[26]){swap(distances[22],distances[26]);}
    if (distances[18] > distances[20]){swap(distances[18],distances[20]);}
    if (distances[22] > distances[24]){swap(distances[22],distances[24]);}
    if (distances[26] > distances[28]){swap(distances[26],distances[28]);}
    if (distances[17] > distances[25]){swap(distances[17],distances[25]);}
    if (distances[21] > distances[29]){swap(distances[21],distances[29]);}
    if (distances[21] > distances[25]){swap(distances[21],distances[25]);}
    if (distances[19] > distances[27]){swap(distances[19],distances[27]);}
    if (distances[23] > distances[31]){swap(distances[23],distances[31]);}
    if (distances[23] > distances[27]){swap(distances[23],distances[27]);}
    if (distances[19] > distances[21]){swap(distances[19],distances[21]);}
    if (distances[23] > distances[25]){swap(distances[23],distances[25]);}
    if (distances[27] > distances[29]){swap(distances[27],distances[29]);}
    if (distances[17] > distances[18]){swap(distances[17],distances[18]);}
    if (distances[19] > distances[20]){swap(distances[19],distances[20]);}
    if (distances[21] > distances[22]){swap(distances[21],distances[22]);}
    if (distances[23] > distances[24]){swap(distances[23],distances[24]);}
    if (distances[25] > distances[26]){swap(distances[25],distances[26]);}
    if (distances[27] > distances[28]){swap(distances[27],distances[28]);}
    if (distances[29] > distances[30]){swap(distances[29],distances[30]);}
    if (distances[0] > distances[16]){swap(distances[0],distances[16]);}
    if (distances[8] > distances[24]){swap(distances[8],distances[24]);}
    if (distances[8] > distances[16]){swap(distances[8],distances[16]);}
    if (distances[4] > distances[20]){swap(distances[4],distances[20]);}
    if (distances[12] > distances[28]){swap(distances[12],distances[28]);}
    if (distances[12] > distances[20]){swap(distances[12],distances[20]);}
    if (distances[4] > distances[8]){swap(distances[4],distances[8]);}
    if (distances[12] > distances[16]){swap(distances[12],distances[16]);}
    if (distances[20] > distances[24]){swap(distances[20],distances[24]);}
    if (distances[2] > distances[18]){swap(distances[2],distances[18]);}
    if (distances[10] > distances[26]){swap(distances[10],distances[26]);}
    if (distances[10] > distances[18]){swap(distances[10],distances[18]);}
    if (distances[6] > distances[22]){swap(distances[6],distances[22]);}
    if (distances[14] > distances[30]){swap(distances[14],distances[30]);}
    if (distances[14] > distances[22]){swap(distances[14],distances[22]);}
    if (distances[6] > distances[10]){swap(distances[6],distances[10]);}
    if (distances[14] > distances[18]){swap(distances[14],distances[18]);}
    if (distances[22] > distances[26]){swap(distances[22],distances[26]);}
    if (distances[2] > distances[4]){swap(distances[2],distances[4]);}
    if (distances[6] > distances[8]){swap(distances[6],distances[8]);}
    if (distances[10] > distances[12]){swap(distances[10],distances[12]);}
    if (distances[14] > distances[16]){swap(distances[14],distances[16]);}
    if (distances[18] > distances[20]){swap(distances[18],distances[20]);}
    if (distances[22] > distances[24]){swap(distances[22],distances[24]);}
    if (distances[26] > distances[28]){swap(distances[26],distances[28]);}
    if (distances[1] > distances[17]){swap(distances[1],distances[17]);}
    if (distances[9] > distances[25]){swap(distances[9],distances[25]);}
    if (distances[9] > distances[17]){swap(distances[9],distances[17]);}
    if (distances[5] > distances[21]){swap(distances[5],distances[21]);}
    if (distances[13] > distances[29]){swap(distances[13],distances[29]);}
    if (distances[13] > distances[21]){swap(distances[13],distances[21]);}
    if (distances[5] > distances[9]){swap(distances[5],distances[9]);}
    if (distances[13] > distances[17]){swap(distances[13],distances[17]);}
    if (distances[21] > distances[25]){swap(distances[21],distances[25]);}
    if (distances[3] > distances[19]){swap(distances[3],distances[19]);}
    if (distances[11] > distances[27]){swap(distances[11],distances[27]);}
    if (distances[11] > distances[19]){swap(distances[11],distances[19]);}
    if (distances[7] > distances[23]){swap(distances[7],distances[23]);}
    if (distances[15] > distances[31]){swap(distances[15],distances[31]);}
    if (distances[15] > distances[23]){swap(distances[15],distances[23]);}
    if (distances[7] > distances[11]){swap(distances[7],distances[11]);}
    if (distances[15] > distances[19]){swap(distances[15],distances[19]);}
    if (distances[23] > distances[27]){swap(distances[23],distances[27]);}
    if (distances[3] > distances[5]){swap(distances[3],distances[5]);}
    if (distances[7] > distances[9]){swap(distances[7],distances[9]);}
    if (distances[11] > distances[13]){swap(distances[11],distances[13]);}
    if (distances[15] > distances[17]){swap(distances[15],distances[17]);}
    if (distances[19] > distances[21]){swap(distances[19],distances[21]);}
    if (distances[23] > distances[25]){swap(distances[23],distances[25]);}
    if (distances[27] > distances[29]){swap(distances[27],distances[29]);}
    if (distances[1] > distances[2]){swap(distances[1],distances[2]);}
    if (distances[3] > distances[4]){swap(distances[3],distances[4]);}
    if (distances[5] > distances[6]){swap(distances[5],distances[6]);}
    if (distances[7] > distances[8]){swap(distances[7],distances[8]);}
    if (distances[9] > distances[10]){swap(distances[9],distances[10]);}
    if (distances[11] > distances[12]){swap(distances[11],distances[12]);}
    if (distances[13] > distances[14]){swap(distances[13],distances[14]);}
    if (distances[15] > distances[16]){swap(distances[15],distances[16]);}
    if (distances[17] > distances[18]){swap(distances[17],distances[18]);}
    if (distances[19] > distances[20]){swap(distances[19],distances[20]);}
    if (distances[21] > distances[22]){swap(distances[21],distances[22]);}
    if (distances[23] > distances[24]){swap(distances[23],distances[24]);}
    if (distances[25] > distances[26]){swap(distances[25],distances[26]);}
    if (distances[27] > distances[28]){swap(distances[27],distances[28]);}
    if (distances[29] > distances[30]){swap(distances[29],distances[30]);}
  }
};

string arrToString(int arr[], int n, int pad){
  string res = "[";
  for(int i = 0; i < n-1; i++){
    res += to_string(arr[i]) + ", ";
  }
  res += to_string(arr[n-1]) + "]";
  res.append(pad - res.length(), ' ');
  return res;
}      

string numToString(int &num, int pad){
  string res = to_string(num);
  res.append(pad - res.length(), ' ');
  return res;
}

int main(){
  auto t1 = chrono::high_resolution_clock::now();
  int minDist = 2147483647;
  int correct[5] = {0};
  int correctSequence[8] = {0};
  // int maxm[8] = {32,32,32,32,32,32,32,32};
  int maxm[8] = {1,1,5,5,5,5,5,5};
  int mid[8][5] = {0};
  for(int i = 0; i < 5; i++){
    mid[0][i] = 127;
  }
  int ss[8] = {64,32,16,8,4,2,1,0};
  int ch[8] = {65,33, 17, 9, 5, 3, 2, 1};
  int a[8] = {0};
  int d = -1;
  int lims[8] = {1};
  howClose guesses[8];
  // distances = [[]]*8
  int hi[8][5] = {0};
  int lo[8][5] = {0};
  int lim = 1;
  for(int i = 1; i < 8; i++){
    lims[i] = maxm[i] * lims[i-1];
    lim += lims[i];
  }
  cout << "\nBranches at each depth: [";
  for(int j = 0; j < 7; j++){
    cout << maxm[j] << ", ";
  }
  cout << maxm[7] << "]\n";
  cout << "Total number of iterations: " << lim << endl;
  cout << "\nmid                        depth stsz  floor                      ceiling                    seq                        guess" << endl;
  for(int i = 0; i < lim; i++){
    if ((d < 7) && (!a[d+1])){
      d += 1;
    }else{
      cout << "\x1b[1A\x1b[2K";
    }

    bool movedBack = false; 
    while (a[d] == maxm[d]){
      d -= 1;
      cout << "\x1b[1A\x1b[2K";
      movedBack = true;
    }
    if (movedBack && (d >= 0)){
      for(int j = d+1; j < 8; j++){
        a[j] = 0;
      }
    }
    a[d] += 1;

    if (a[d] == 1){
      for(int j = 0; j < 5; j++){
        hi[d][j] = mid[d][j] + ch[d];
        lo[d][j] = mid[d][j] - ss[d];
      }
      // 32 guesses here, gray code
      guesses[d] = howClose(lo[d], hi[d]);
    }

    if (d < 7){
      string outLine = arrToString(mid[d], 5, 27);
      outLine += numToString(d, 6);
      outLine += numToString(ss[d], 6);
      outLine += arrToString(lo[d], 5, 27);
      outLine += arrToString(hi[d], 5, 27);
      outLine += arrToString(a, 8, 27);
      cout << outLine << "\n";
      // pr(f'{str(mid[d]):<27}{d:<6}{ss[d]:<6}{str(lo[d]):<27}{str(hi[d]):<27}{str(a):<27}')
      for(int j = 0; j < 5; j++){
        if(guesses[d].distances[a[d]-1].gs[j] == hi[d][j]){
          mid[d+1][j] = mid[d][j] + ss[d];
        }else{
          mid[d+1][j] = mid[d][j] - ss[d];
        }
      }
      // mid[d+1] = [mid[d][i] + ss[d] if distances[d][a[d]-1][1][i] == hi[d][i] else mid[d][i] - ss[d] for i in range(5)]
    }
    else{
      string outLine = arrToString(mid[d], 5, 27);
      outLine += numToString(d, 6);
      outLine += numToString(ss[d], 6);
      outLine += arrToString(lo[d], 5, 27);
      outLine += arrToString(hi[d], 5, 27);
      outLine += arrToString(a, 8, 27);
      outLine += arrToString(guesses[d].distances[a[d]-1].gs, 5, 27);
      cout << outLine << "\n";
      // pr(f'{str(mid[d]):<27}{d:<6}{ss[d]:<6}{str(lo[d]):<27}{str(hi[d]):<27}{str(a):<27}{str(distances[d][a[d]-1][1]):<27}')
    }
    if(guesses[d].distances[a[d]-1].distance <= minDist){
      minDist = guesses[d].distances[a[d]-1].distance;
      for(int j = 0; j < 5; j++)
        correct[j] = guesses[d].distances[a[d]-1].gs[j];
      for(int j = 0; j < 8; j++)
        correctSequence[j] = a[j];
    }
    // sleep(1);
  }
  chrono::duration<double> s_double = chrono::high_resolution_clock::now() - t1;
  cout << "\nMost correct guess: [";
      for(int j = 0; j < 4; j++){
        cout << correct[j] << ", ";
      }
      cout << correct[4] << "] ";
  cout << "with a distance of " << minDist << ", sequence [";
  for(int j = 0; j < 7; j++){
    cout << correctSequence[j] << ", ";
  }
  cout << correctSequence[7] << "] \n" << endl;
  cout << s_double.count() << " seconds\n" << endl;
  return 0;
}
