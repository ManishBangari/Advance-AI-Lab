Consider an electric vehicle charging station (CS). The CS has k number of charging ports. Each port can charge one vehicle at a given time point. k = 1 is the slowest charging port. Higher value of k indicates a faster charging port. A faster charging port will have a higher cost for charging. Let us assume the price of charging one time unit for different ports are p1 , . . . , pk . An EV sends a charging request to the CS with the following information - time of arrival (ai ), time of departure (di ), and charge time (ci ) when it is connected to the slowest charging port. For k-th port charging time will be ⌈ cki ⌉. Suppose on a given day the CS knows all upcoming requests for the whole day. What will be the most cost effective schedule for charging of vehicles? A vehicle needs to be charged uninterruptedly on a single port only such that it receives desired amount of charge-time before its departure time.

% number of ports - K
K 5
% Price for ports per time unit
P 5 12 17 23 32
% vehicle requests: id arrival-time departure-time charge-time
V 1 10 24 12
V 2 2 22 6

