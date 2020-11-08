#include "bridge.h"
#include "bridgesim.h"
#include <bits/stdc++.h>
#define vi vector<int>
#define vvi vector< vi >
#define vc vector<char>
#define vs vector<string>
#define pii pair<int,int>
#define vpii vector< pii >
#define forn(i,n) for(int (i) = 0 ; (i) < (n) ; (i)++)
#define mp make_pair
#define pb push_back
using namespace std;

Lan::Lan(){
	id = '?';
	dp = -1 ;
}

Bridge::Bridge(){
	id = -1 ;
	root = id ;
	root_distance = -1 ;
	root_port = mp('?' , -1);
}

Message::Message(){
	root = -1;
	distance = -1;
	destination = -1;
	lan_id = '?';
}

void Lan::display(){
	cout << "=========================" << endl;
	cout << this->id << endl;
	for(auto i : this->conn_bridges){
		cout << i << " ";
	}
	cout << endl;
	cout << this->dp << endl;
}

void Bridge::display(){
	
	cout << "B" << this->id << ":" << endl;
	cout << "HOST ID | FORWARDING PORT" << endl;
	vector< pii > temp;
	for(auto i : this->forwarding_table){
		temp.pb(mp(i.host_id ,  i.fport.id ));
	} 
	sort(temp.begin() , temp.end());
	for(auto i : temp){
		cout << "H" << i.first << " | "  << char(i.second) << endl;
	} 
	
	
}
void Bridge::display_lans(){
	cout << "=========================" << endl;
	cout << "B" << this->id << ": " ;
	for(auto i : conn_lans){
		cout << i << " ";
	}
	cout << endl;
}
