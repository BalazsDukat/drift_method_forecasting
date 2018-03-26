#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <cctype>

#include "get_type.h"

using namespace std;

int main(int argc, char* argv[])
{string file_name, line, temp;
vector<double> TS;
double d;
if(argc < 2)
{cout <<
"Please enter the name of the time-series file \n" <<
"(ideally should be a csv or tsv file) \nthat you want to continue:\n";
cin >> file_name;
}
else
{file_name = argv[1];
//cout << argv[1] << '\t' << argv[0];
}

fstream file;
file.open(file_name.c_str());
char c;

  while((file >> noskipws >> c))
  {if((isspace(c)) || (c == ',') || (c == '|')) //looking for some sort of a delimiter
    {if(temp != "" && (get_type(temp) == 'd' || get_type(temp) == 'i'))
      {d = atof(temp.c_str());
      TS.push_back(d);  //cout << temp << " " << d << ", ";
      } //else ;//leave it, temp is empty or wrong type
    temp = ""; //there was a delimiter, so no matter what it has, flush temp.
    }
  else
    {temp += c; //not a delimter, so let's chuck it on top and continue until we hit a delimiter
    }
  }
file.close(); //finished with the file, close it for now.
int period = 1; /*this has o be signed as well as position has to be signed,
so position can go below 0, which is a condition*/
unsigned int steps = 1;

cout << "\nPlease enter the number of steps one period contains." <<
"\n(For example, if your data has 1 number for every hour of the day,\n and the length of the period " <<
"you need to consider is 1 day, then he number of \"steps\" your period contains will be 24)\n";
cin >> period;

cout << "\nPlease enter the number of steps (data points) you want to predict.\n";
cin >> steps;

for(unsigned int s = 0; s < TS.size(); s++) cout << " " << TS[s]; //just to show the vector's contents.

int position = TS.size() - period;
cout << "\nTS has " << TS.size() << " elements\nwe are at: " << position;
double result = 0, collector = 0;
unsigned int counter = 0;
while((position - period) >= 0)
  {collector = collector + (TS[position] - TS[position-period]);
  position = position - period;
  counter++;
  }

double drift = (collector / counter);

file.open(file_name.c_str(),std::ostream::app); //re-open the file for writing.
file << "\n";
for(unsigned int i = 0; i < steps; i++)
{result = TS[TS.size() - period] + drift;
TS.push_back(result);
cout << "\ndrift: "<< drift << "\tresult: " << result << "\n";
file << result << ",";
}

file << "\0";
file.close();
return 0;
}
