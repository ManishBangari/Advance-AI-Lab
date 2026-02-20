#include <z3++.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <sstream>

using namespace z3;
using namespace std;

// make a vehicle adt which will have all the required variables needed
struct Vehicle {
    int id;
    int a, d, c; 
};

int main() {
    int K;
    vector<int> price;
    vector<Vehicle> V;

    // Reading input
    string line;
    while(getline(cin,line)){
        if(line.size()==0 || line[0]=='%') continue;

        stringstream ss(line);
        char type;
        ss>>type;

        if(type=='K'){
            ss>>K;
        }
        else if(type=='P'){
            int x;
            while(ss>>x) price.push_back(x);
        }
        else if(type=='V'){
            Vehicle v;
            ss>>v.id>>v.a>>v.d>>v.c;
            V.push_back(v);
        }
    }

    context c;
    optimize opt(c);

    int n = V.size();

    vector<expr> port, start, time, cost;

    for(int i=0;i<n;i++) {
        port.push_back(c.int_const(("port "+to_string(i)).c_str()));
        start.push_back(c.int_const(("start "+to_string(i)).c_str()));
        time.push_back(c.int_const(("time  ,"+to_string(i)).c_str()));
        
        // Decision variable
        /*
        x(i,k)={1, if vehicle i is assigned to port k
                0,  otherwise
        }
        */

        // Port range from 1 to K
        opt.add(port[i] >= 1 && port[i] <= K);

        // Charging time for k-th port charging time will be = ceil(c[i] / k) as given in the problem
        // f(i)=s(i)+sum(from 1 to K)x(i,k)*(c[i]/k)

        int ci = V[i].c;
        expr k = port[i];
        opt.add(time[i] == (ci + k - 1) / k);

        // Arrival/departure constraint
        //  finish time will always be less than or equal to departure time. f(i)<=d(i)
        // and start time will always be more than or equal to arrival time. a(i)<=s(i)
        opt.add(start[i] >= V[i].a);
        opt.add(start[i] + time[i] <= V[i].d);

        // Cost
        // t(i,k)=ceil(c[i] / k)
        // cost(i,k)=p(k)*t(i,k)
        expr p = c.int_val(0);
        for(int j=0;j<K;j++) {
            p = ite(port[i] == j+1,
                    c.int_val(price[j]) * time[i],
                    p);
        }
        cost.push_back(p);
    }

    // No overlap if same port
    // The CSP formulation would be x(i,k)+x(j,k)<=1

    for(int i=0;i<n;i++){
        for(int j=i+1;j<n;j++){
            opt.add(
                (port[i] != port[j]) ||
                (start[i] + time[i] <= start[j]) ||
                (start[j] + time[j] <= start[i])
            );
        }
    }

    expr total_cost = c.int_val(0);
    for(auto &x : cost) total_cost = total_cost + x;

    // minimize the sum(for all i), sum(for all k)x(i,k)*p(k)*ceil(c[i] / k)
    opt.minimize(total_cost);

    if(opt.check() == sat){
        model m = opt.get_model();
        cout << "There is an optimal Schedule :)\n";
        cout << "Total Cost = " << m.eval(total_cost) << "\n\n";

        for(int i=0;i<n;i++){
            cout << "Vehicle " << V[i].id
                 << " at Port " << m.eval(port[i])
                 << " with starting time " << m.eval(start[i])
                 << " and finish time is " << m.eval(time[i]) << "\n";
        }
    } else {
        cout << "No valid schedule ;(\n";
    }
}
