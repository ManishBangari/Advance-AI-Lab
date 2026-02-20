#include <z3++.h>
#include <iostream>
#include <vector>
#include <cmath>

using namespace z3;
using namespace std;

struct Vehicle {
    int id;
    int a, d, c; // arrival, departure, slow-port charge time
};

int main() {
    context c;
    optimize opt(c);

    int K = 5;
    vector<int> price = {5,12,17,23,32};

    vector<Vehicle> V = {
        {1,10,24,12},
        {2,2,22,6}
    };

    int n = V.size();

    vector<expr> port, start, time, cost;

    for(int i=0;i<n;i++) {
        port.push_back(c.int_const(("port_"+to_string(i)).c_str()));
        start.push_back(c.int_const(("start_"+to_string(i)).c_str()));
        time.push_back(c.int_const(("time_"+to_string(i)).c_str()));

        // Port range
        opt.add(port[i] >= 1 && port[i] <= K);

        // Charging time = ceil(ci / k)
        int ci = V[i].c;
        expr k = port[i];
        opt.add(time[i] == (ci + k - 1) / k);

        // Arrival/departure constraint
        opt.add(start[i] >= V[i].a);
        opt.add(start[i] + time[i] <= V[i].d);

        // Cost
        expr p = c.int_val(0);
        for(int j=0;j<K;j++) {
            p = ite(port[i] == j+1,
                    c.int_val(price[j]) * time[i],
                    p);
        }
        cost.push_back(p);
    }

    // No overlap if same port
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

    opt.minimize(total_cost);

    if(opt.check() == sat){
        model m = opt.get_model();
        cout << "Optimal Schedule\n";
        cout << "Total Cost = " << m.eval(total_cost) << "\n\n";

        for(int i=0;i<n;i++){
            cout << "Vehicle " << V[i].id
                 << " Port " << m.eval(port[i])
                 << " Start " << m.eval(start[i])
                 << " Time " << m.eval(time[i]) << "\n";
        }
    } else {
        cout << "No feasible schedule\n";
    }
}
