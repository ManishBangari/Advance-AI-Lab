#include <bits/stdc++.h>
using namespace std;

int main() {
    // variables declared to max range
    int maxPorts = 5;
    int maxVehicles = 10;
    int maxPrice = 50;
    int maxArrival = 50;
    int maxDepartureGap = 50;
    int maxCharge = 30;

    // rand is used to add randomness
    srand(time(0));

    int K = rand() % maxPorts + 1;
    cout << "% number of ports - K\n";
    cout << "K " << K << "\n";

    cout << "% Price for ports per time unit\n";
    cout << "P ";
    for (int i = 1; i <= K; i++) {
        int price = rand() % maxPrice + 1;
        cout << price << (i==K?'\n':' ');
    }

    int V = rand() % maxVehicles + 1;
    cout << "% vehicle requests: id arrival-time departure-time charge-time\n";

    for (int i = 1; i <= V; i++) {
        int a = rand() % maxArrival;
        int d = a + rand() % maxDepartureGap + 1;
        int c = rand() % maxCharge + 1;
        cout << "V " << i << " " << a << " " << d << " " << c << "\n";
    }

    return 0;
}