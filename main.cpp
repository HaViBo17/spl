// Name : Shlok Nemani and Harsh Borkar
//reference : https://github.com/ani8897/Spanning-Tree-Protocol

#include <bits/stdc++.h>
#define vi vector<int>
#define vvi vector< vi >
#define vc vector<char>
#define pii pair<int,int>
#define vpii vector< pii >
#define forn(i,n) for(int (i) = 0 ; (i) < (n) ; (i)++)
#define pb push_back

using namespace std;
#include "bridge.h"
#include "bridgesim.h"

int nxt();

// splits a string on the basis of delimiter key
void split(string a, char key , vector<string> &b);

// converts (string) H45 -> (int) 45
int hosttoint(string a);

int main(){
	freopen("input.txt", "r", stdin);
	int trace_flag = nxt();
	int n = nxt();
	vector<Bridge>b_net;
	set<char>lan_names;
	//input for spl
	forn(i,n){
		string a ;
		getline(cin , a);
		vector<string> temp ;
		split(a,' ',temp);
		Bridge b;
		b.id = hosttoint(temp[0].substr(0,temp[0].size() -1));
		b.root = b.id;
		forn(j, temp.size()){
			if(j == 0 )continue;
			b.conn_lans.pb(temp[j][0]);
			lan_names.insert(temp[j][0]);
		}
		sort(b.conn_lans.begin() , b.conn_lans.end());
		b_net.pb(b);
	}
	//input for spl ends

	// for(auto i: b_net){
	// 	i.display();
	// }

	vector<Lan>l_net;
	//adding the connected bridges to the lan
	for(auto lan_id : lan_names){
		Lan temp;
		temp.id = lan_id;
		for(auto some_bridge : b_net ){
			for( auto some_conn_lan : some_bridge.conn_lans){
				if(some_conn_lan == lan_id){
					temp.conn_bridges.pb(some_bridge.id);
				}
			}
		}
		l_net.pb(temp);
	}

	// for(auto i: l_net){
	// 	i.display();
	// }

	//runs the spanning tree protocol
	runconfig(b_net,l_net);
	
	for(int k  = 0 ; k < l_net.size() ; k++){
		string a;
		getline(cin , a);
		vector<string> temp;
		split(a , ' ' , temp);
		for(int i = 0 ; i < temp.size() ; i++){
			if(i == 0)continue;
			l_net[k].conn_hosts.pb(hosttoint(temp[i]));
		}
	}

	int transfersa = nxt();
	cout << transfersa << endl;
	forn(i,transfersa){
		
		string a;
		getline(cin ,a);
		vector<string> temp;
		split(a , ' ' , temp);
		int start = hosttoint(temp[0]);
		int final = hosttoint(temp[1]);
		cout << start << " " << final << endl;

		//running the simulation
		transfers(start,final , b_net , l_net);

	}
	
	
}

int nxt(){
	stringstream inn;
	string inpp;
	getline(cin , inpp);
	inn << inpp;
	int a;
	inn >> a;
	return a;
}

void split(string a, char key , vector<string> &b){
	b.clear();
	int n = a.size();
	string temp ="";
	forn(i,n){
		if(a[i] == key){
			if(temp.size() != 0){
				b.pb(temp);
				temp.clear();
			}
		}else{
			temp = temp + a[i];	
		}
	}
	if(temp.size() != 0){
		b.pb(temp);
		temp.clear();
	}
}

int hosttoint(string a){
	stringstream X ;
	X << a.substr(1, a.size()-1);
	int b = 0;
	X >> b;
	return b;
}
