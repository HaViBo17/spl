#include <bits/stdc++.h>
#include "bridge.h"
#include "bridgesim.h"
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

set<Message,messageComp>  Bridge_Config_msg(Message m, vector<Bridge> b_net, vector<Lan> l_net){
	set<Message,messageComp>  messages;
	int root = m.root;
	int d = m.distance;
	Bridge source = m.source;
	// takes source 
	for(int i=0; i<b_net.size();i++){
		// iterates over the whole bridges network and matches the bridge
		if(source.id == b_net[i].id){
			// check if bridge is same as the source id
			for(int j=0; j<b_net[i].conn_lans.size(); j++){
				// loops over the adjacency or connected list of the lans to the bridge
				for(int k=0; k<l_net.size(); k++){
					// now full looping over the lan network to find the desired lan
					if(b_net[i].conn_lans[j] == l_net[k].id){
						for(int p=0; p<l_net[k].conn_bridges.size();p++){
							// the destination bridges are selected by finding the 
							if(l_net[k].conn_bridges[p] != source.id)
							// bridges in the adjacency list of the lans and choosing those which is not the source itself
							{
								Message ms;
								ms.root = root;
								ms.distance = d;
								ms.source = source;
								ms.destination = l_net[k].conn_bridges[p];
								// the update in the received message list is the destination bridge id along with
								//cout<<"destination "<<ms.destination<<endl;
								ms.lan_id = l_net[k].id;
								// the lan through which both the bridges are connected
								//cout<<"ms.lan"<<ms.lan<<endl;
								messages.insert(ms);
								// insert function is a method of set datatype to enter a element into the set
							}
						}}}}}}
	return messages;
}

Message configuration_update(Message m,vector<Bridge>& b_net){
	// the call by reference method is used on the bridge network
	// call by reference method of passing arguments to a function copies the address of an argument into the formal parameter
	// in simple words the changes to the bridge network inside the function will be automatically applied globally
	Bridge source = m.source;
	int destination = m.destination;
	int root = m.root;
	int bridge_dist = m.distance;
	char lan = m.lan_id;

	Message update_msg;

	for(int i=0; i<b_net.size();i++)
	{
		if(destination == b_net[i].id)
		{
			Bridge b = b_net[i];
			if(root < b.root){
			// rule 1 applied: analysis of the received message 
			// the root in the message is found to be better than the own root id 
			// the first rule of implementing stp is applied
			// bridge acknowledges the other source bridge as the root bridge 
				update_msg.root = root;
				// root update, distance update 
				update_msg.distance = bridge_dist+1;
				update_msg.source=b_net[i];
				b_net[i].root = root;
				b_net[i].root_port = make_pair(lan,source.id);
				b_net[i].root_distance = bridge_dist+1;
				//cout<<"rule 1 "<<endl;
			}
			else if(root == b.root && bridge_dist+1 < b_net[i].root_distance)
			{
				//rule 2 applied: It identifies a root with an equal ID
				update_msg.root = root;
				update_msg.distance = bridge_dist+1;
				// now as to break the tie here we make the use of the distance to the root parameter
				update_msg.source=b_net[i];
				b_net[i].root = root;
				b_net[i].root_port = make_pair(lan,source.id);
				// if distance to the root is found to be shorter than own configured value
				// updates its parameter accordingly
				//cout<<"rule 2 "<<endl;
				b_net[i].root_distance = bridge_dist+1;
			}
			else if (root == b.root && bridge_dist+1 == b_net[i].root_distance && source.id<b_net[i].root_port.second)
			{
				//rule 3 applied: The root ID and distance are equal
				// since both are equal for this case the id of sending bridge is used
				//cout<<"rule 3 "<<endl;
				update_msg.root = root;
				update_msg.distance = bridge_dist+1;
				update_msg.source=b_net[i];
				// ties are breaked by analysing the source id of the sending bridge
				// if its smaller than parameter update takes place
				b_net[i].root = root;				
				b_net[i].root_port= make_pair(lan,source.id);
				b_net[i].root_distance = bridge_dist+1;
			}
			else
			{
				// in all other cases bridge will the bridge stops generating configuration messages on its own and 
				update_msg.root = -1;
				//instead only forwards configuration messages from other bridges,
				// also it adds 1 to the distance field as the distance from the sending bridge to the source bridge is also one hop
                //cout<<"rule 4"<<endl;
				update_msg.distance = bridge_dist+1;
				update_msg.source=b_net[i];
			}
		}
	}

	return update_msg;
}

void runconfig(vector<Bridge> &b_net , vector<Lan> &l_net ,bool trace){
	
	vector<Message> sent_msg,recv_msg;
	// initialising the vector of messages
	// sent_msg abbreviates sent_messages which are advertised by the bridges about the configuration
	// recv_msg abbreviates received configured messages which are sent by the other bridges
	vector<traces> all_traces;
	// stores all the trace objects 
	int timestamp = 0;
	int initial=1;
	// initial = 1 implies that whether the sent messsages are the default initialised one
    // practical implementation of initial stage
    // all bridges assumes themselves as root bridges
	int iter=0;
	while(iter<b_net.size()){
		Message m;
		// the message instance
		// all the bridges configures themselves as the root bridges
		m.root = b_net[iter].id;
		// initialises its own bridge id as the root id
		m.distance=0;
		// and thus takes the distance of the source bridge as zero
		m.source=b_net[iter];
		// the source is the bridge itself as per the initial protocol strategy
		sent_msg.push_back(m);
		iter++;
	}
	while(!sent_msg.empty()){
		if(initial != 1){
			// since for first step, when the values are initialised 
			// the condition remains false
			// on other steps old config messages are deleted 
			// and new are generated as the bridges receive the configuration message from others
			while(!sent_msg.empty()){
				sent_msg.erase(sent_msg.begin());
			}
		}
		// will be true only when there are update message for the configuration to be updated
		// the received message list which includes the parameters by which the bridges compares its configuration
		// by following the rules of protocol each bridge makes a change/updates its configuration
		// finally a hierarchy is form which is the spanning tree/ without any loops 
		while(!recv_msg.empty()){
			Message m = recv_msg.front();
			Message to_be_published = configuration_update(m, b_net);
			if(to_be_published.root != -1){
				sent_msg.push_back(to_be_published);
			}
			// traces of the received messages from the otheer bridges
			traces t;
			t.timestamp = timestamp;
			t.bridge = m.destination;
			t.action = 'r';
			// the action here is r as this time the message is being receieved 
			t.m = m;
			if(trace==1){
			cout<<t.timestamp;
			cout<<" ";
			cout<<t.action;
			cout<<" B";
			cout<<t.bridge;
			cout<<" (B";
			cout<<t.m.root<<" "<<t.m.distance<<" B"<<t.m.source.id<<")";
			cout<<endl;
			} 
			all_traces.push_back(t);
			recv_msg.erase(recv_msg.begin());
			//cout<<"receive trace added"<<endl;
		}
		vector<Message> temp;
		while(!sent_msg.empty()){
			Message m = sent_msg.front();
			// front takes out the first message instance from the sent messages vector
			set<Message,messageComp> recv_msg_by_set = Bridge_Config_msg(m, b_net, l_net);
			// sends the first message instance from the sent_msg queue
			sent_msg.erase(sent_msg.begin());
			// pop_back function removes the end messages from the sent config msg list
			while(!recv_msg_by_set.empty()){
				// recv_msg results are being stored in the recv_msg message queue
				recv_msg.push_back(*recv_msg_by_set.begin());
				recv_msg_by_set.erase(recv_msg_by_set.begin());
			}
			// traces are to implemented and plotted only when the value of trace is one 
			// otherwise only the final output will be printed
			traces t;
			// traces definition in the header file
			t.timestamp = timestamp;
			// timestamp shows the time elapsed in the protocol
			t.bridge = m.source.id;
			// for the trace the bridge activity is stored as the source which generates the msg
			t.action = 's';
			// s denotes the sent config message and r denotes the received config message
			t.m = m;
			all_traces.push_back(t);
			if (trace==1){
		    cout<<t.timestamp<<" "<<t.action<<" B"<<t.bridge<<" (B"<<t.m.root<<" "<<t.m.distance;
			cout<<" B"<<t.m.source.id<<")";
		    cout<<endl;
		}
			temp.push_back(m);
		}
		while(!temp.empty()){
			sent_msg.push_back(temp.front());
			temp.erase(temp.begin());
		}
		timestamp++;
		initial=0;
	}
	iter=0;
	while(iter<l_net.size()){
		// final loop over whole lan network
		int min_val=l_net[iter].conn_bridges[0];
		for(int i=1; i<l_net[iter].conn_bridges.size(); i++){
			if(min_val>l_net[iter].conn_bridges[i]){
				min_val=l_net[iter].conn_bridges[i];
			}
		}
		l_net[iter].dp=min_val;
		// for every iteration value the designated port is selected to the lowest id bridge
		iter++;
	}
	// loop to display the final output 
	iter=0;
	while(iter<b_net.size()){
		// prints the bridge on the line for e.g B1, B2..
		cout<<"B"<<b_net[iter].id<<":";
		// display of output bridges as root/designated or null port
		for(int j=0; j<b_net[iter].conn_lans.size(); j++){
			int flag=0;
			char root_check = b_net[iter].conn_lans[j];
			// this line prints the lan number or character
			cout<<" "<<root_check<<"-";
			// using the root port instance of bridge 
			// the bridge whose root port matches with the root_check variable is the root port
			if(root_check == b_net[iter].root_port.first) {cout<<"RP";flag=1;}
			int iterj=0;
			// for the designated ports denoted as DP
			while(iterj<l_net.size()){
				if(root_check==l_net[iterj].id && b_net[iter].id==l_net[iterj].dp && flag==0)
					{cout<<"DP";
					flag=1;
					break;}
					iterj++;
			}
			// finally the remained ports which are of no use as per the spanning tree protocol are discarded
			// these are called as null ports denoted by NP
			if(!flag) {
				cout<<"NP";
				b_net[iter].conn_lans.erase(remove(b_net[iter].conn_lans.begin(), b_net[iter].conn_lans.end(), root_check), b_net[iter].conn_lans.end());
				j--;
			}
		}
		cout<<endl;
		iter++;
	}
	for(int i=0; i<l_net.size();i++)
	{
		char c = l_net[i].id;
		vector<int> l;
		for(int i=0; i<b_net.size();i++)
			for(int j=0; j<b_net[i].conn_lans.size(); j++)
				if (b_net[i].conn_lans[j] == c) l.push_back(b_net[i].id);
		l_net[i].conn_bridges = l;
	}

}

void transfers(int start , int final , vector<Bridge> &b_net , vector<Lan> &l_net ,bool tr){
	
	Lan starting ;
	//getting the starting (host which sends the message) lan
	
	for(int i=0; i<l_net.size();i++){
		for(int p=0; p<l_net[i].conn_hosts.size(); p++){
			if(l_net[i].conn_hosts[p]==start){
				//starting 
				starting=l_net[i];
			}
		}
	}


	queue<Datagram> sent, received;
	//implementing two queues to simulate the 1 second prapagation delay
	//sent queue contains messages which are just sent
	//receive queue contains the messages which are just received

	for (int i=0; i<starting.conn_bridges.size(); i++){
		Datagram data;
		data.source = start;
		data.destination = final;
		data.prev_lan_id = starting.id;
		data.curr_bridge = starting.conn_bridges[i];
		received.push(data);
		//first all the bridges which are on the starting lan will receive the message
	}

	bool first = true;
	//making sure that the loop runs atleast once

	while(first || !sent.empty()){
		//going through all the messages which are just sent 
		//and forwarding them to the receiving queue
		while(!sent.empty()){
			Datagram current = sent.front();
			sent.pop();
			int sending_bridge_id = current.curr_bridge ;
			int current_lan_id = current.prev_lan_id ;
			//going through the all the lan ports and the one needed
			for(int i = 0 ; i < l_net.size() ; i++){
				if(l_net[i].id == current_lan_id){
					for(int j = 0 ; j < l_net[i].conn_bridges.size() ; j++){
						//making sure that the message is not again sent to the sending bridge
						if(l_net[i].conn_bridges[j] == sending_bridge_id){
							continue;
						}
						//preparing the message to be sent to the bridges
						Datagram send_message;
						send_message.source = start;
						send_message.destination = final;
						send_message.prev_lan_id = current_lan_id ;
						send_message.curr_bridge = l_net[i].conn_bridges[j];
						received.push(send_message);
						if(tr == 1){
							cout << "r "<< l_net[i].id << " on " << send_message.curr_bridge << endl;
						}
					}
				}
			}
		}
		while(!received.empty()){
			Datagram current = received.front();
			received.pop();
			//finding the bridge for which the message is associated
			for(int i = 0 ; i < b_net.size() ;i++){
				if(b_net[i].id == current.curr_bridge){
					//checking if there is a need to update the forwarding table
					bool need_to_update = true;
					for(int j = 0 ; j < b_net[i].forwarding_table.size() ; j++){
						Fwd_table some_entry = b_net[i].forwarding_table[j];
						if(some_entry.host_id == current.source){
							//there exists an entry with the source host so no need to update
							need_to_update = false;
						}
					}
					if(need_to_update){
						//updating the forwarding table
						Fwd_table new_entry; 
						new_entry.host_id = current.source ;
						//finding the correct lan port and then update !
						for(int j = 0 ; j < l_net.size() ; j++){
							if(l_net[j].id == current.prev_lan_id){
								new_entry.fport = l_net[j];
								break;
							}
						}
						b_net[i].forwarding_table.push_back(new_entry);
					}
					//checking if there exists an entry for the destination host
					bool forwarding_entry_present = false;
					for(int j = 0 ; j < b_net[i].forwarding_table.size() ; j++){
						Fwd_table some_entry = b_net[i].forwarding_table[j];
						if(some_entry.host_id == current.destination){
							forwarding_entry_present = true;
							//conditions matched , there exist an entry for the forwarding host
							if(some_entry.fport.id == current.prev_lan_id){
								//making sure that the forwaring port is not the receiving lan
								break;
							}
							//making a new message to be sent
							Datagram send_message;
							send_message.source = start ;
							send_message.destination = final ;
							send_message.prev_lan_id = some_entry.fport.id;
							send_message.curr_bridge = b_net[i].id;
							sent.push(send_message);
							if(tr == 1){
								cout << "s" << b_net[i].conn_lans[j] << "from " << b_net[i].id<< " entry present" <<endl; 
							}
						}
					}
					if(!forwarding_entry_present){
						//no forwarding table entry found so broadcasting on all ports
						for(int j = 0 ; j < b_net[i].conn_lans.size() ; j++){
							//making sure that the broadcast doesn't get sent on the receiving port
							if(b_net[i].conn_lans[j] == current.prev_lan_id){
								continue;
							}
							Datagram send_message ;
							send_message.source = start ;
							send_message.destination = final;
							send_message.prev_lan_id = b_net[i].conn_lans[j];
							send_message.curr_bridge = b_net[i].id;
							sent.push(send_message);
							if(tr == 1){
								cout << "s" << b_net[i].conn_lans[j] << "from " << b_net[i].id << " entry absent" <<endl; 
							}
						}
					}
				}
			}
		}
		
		first = false;
	}
	// displaying the forwarding tables
	for(int i = 0 ; i < b_net.size() ; i++){
		b_net[i].display();
	}
}

