#include <z3++.h>
#include <vector>
#include <iostream>

using namespace z3;
using namespace std;

int main() {
    context c;
    optimize opt(c);

    int n = 6;

    vector<int> weights = {4, 8, 5, 6, 3, 7};
    vector<int> profits = {6, 12, 8, 7, 4, 11};

    // Boolean decision variables
    vector<expr> x;
    for (int i = 0; i < n; i++) {
        x.push_back(c.bool_const(("x_" + to_string(i+1)).c_str()));
    }

    // Exactly 4 items selected
    expr num_selected = c.int_val(0);
    for (int i = 0; i < n; i++) {
        num_selected = num_selected + ite(x[i], c.int_val(1), c.int_val(0));
    }
    opt.add(num_selected == 4);

    // Weight constraint
    expr total_weight = c.int_val(0);
    for (int i = 0; i < n; i++) {
        total_weight = total_weight + 
            ite(x[i], c.int_val(weights[i]), c.int_val(0));
    }
    opt.add(total_weight <= 20);

    // Profit objective
    expr total_profit = c.int_val(0);
    for (int i = 0; i < n; i++) {
        total_profit = total_profit +
            ite(x[i], c.int_val(profits[i]), c.int_val(0));
    }

    opt.maximize(total_profit);

    // Solve
    if (opt.check() == sat) {
        model m = opt.get_model();

        cout << "Model:\n";
        for (int i = 0; i < n; i++) {
            cout << "x_" << (i+1) << " = "
                 << m.eval(x[i]) << endl;
        }

        cout << "Total Profit = "
             << m.eval(total_profit) << endl;

        cout << "Total Weight = "
             << m.eval(total_weight) << endl;
    } else {
        cout << "No solution found\n";
    }

    return 0;
}
