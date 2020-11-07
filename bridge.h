#pragma once
#include <bits/stdc++.h>
#define vi vector<int>
#define vvi vector< vi >
#define vc vector<char>
#define pii pair<int,int>
#define pci pair<char , int>
#define vpii vector< pii >
#define forn(i,n) for(int (i) = 0 ; (i) < (n) ; (i)++)
#define pb push_back

using namespace std;



class Lan{
public:
	char id;
	int dp ;
	vi conn_bridges , conn_hosts;
	Lan();
	void display();
};
class Fwd_table{
public:
	int host_id;
	Lan fport;
};

class Bridge{
public:
	int id , root , root_distance;
	vc conn_lans ;
	vector<Fwd_table> forwarding_table;
	pci root_port;
	Bridge();
	void display();
};

class Message{
public:
	int root, distance, destination ;
	char lan_id;
	Bridge source;
	Message();
};

class traces{ // this class is used to store the trace objects(Timestamp, bridge activity, action and message)
	public:
	Message m; 
	int timestamp;  // specifies how many unit time intervals have been elapsed
	int bridge; // the bridge id 
	char action; // whether the message has been sent on the port by the bridge or received by the bridge
};

struct messageComp{
	bool operator()(Message const &a , Message const &b){
		if(a.destination >= b.destination){
			return 0;
		}
		return 1;
	}
};

class Datagram{
public:
	int source, destination , curr_bridge;
	char prev_lan_id;

};