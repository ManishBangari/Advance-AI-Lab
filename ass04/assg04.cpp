#include <z3++.h>
#include <vector>
#include <iostream>
#include <cmath>

using namespace z3;
using namespace std;

typedef struct  req
{
    int id;
    int starttime;
    int endtime;
    int chargetime;
};

int main() {
    context c;
    optimize opt(c);

    int k;
    cout<<"Enter number of ports - K\n";
    cin>>k;

    cout<<"Price for ports per time unit\n";
    vector<int> prices(k,0);

    for(int i=0;i<k;i++)    cin>>prices[i];

    cout<<"vehicle requests: id arrival-time departure-time charge-time\n";
    
    vector<int> chargetime(k,0);
    for(int i=0;i<k;i++){
        chargetime[i]=ceil(prices[i]/(i+1));
    }

    // Boolean decision variables
    vector<expr> x;
    for (int i = 0; i < n; i++) {
        x.push_back(c.bool_const(("x_" + to_string(i+1)).c_str()));
    }

    
}


