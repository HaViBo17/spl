#pragma once
#include "bridge.h"
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


void runconfig(vector<Bridge> &b_net , vector<Lan> &l_net ,bool );

void transfers(int start , int final , vector<Bridge> &b_net , vector<Lan> &l_net,bool );
