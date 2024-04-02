#include<bits/stdc++.h>
using namespace std;

pair<int,int> lcFindMin(vector<vector<int>>mainTable,int demandLimit[],int supplyLimit[],int r,int c){
    int x=INT_MAX;
    pair<int,int> p={-1,-1};
    for(int i=0;i<r;i++){
        for(int j=0;j<c;j++){
            if(mainTable[i][j]<x && demandLimit[j]!=-1 && supplyLimit[i]!=-1){
                x=mainTable[i][j];
                p.first=i; p.second=j;
            }
        }
    }
    return p;
}
int lcClearDemand(int demandLimit[],int supplyLimit[],pair<int,int> p){
    if(demandLimit[p.second]<supplyLimit[p.first]){
    	int k=demandLimit[p.second];
        supplyLimit[p.first]-=demandLimit[p.second];
        demandLimit[p.second]=-1;
        return k;
    }
    else if(demandLimit[p.second]>supplyLimit[p.first]){
    	int k=supplyLimit[p.first];
        demandLimit[p.second]-=supplyLimit[p.first];
        supplyLimit[p.first]=-1;
        return k;
    }
    else{
    	int k=supplyLimit[p.first];
        demandLimit[p.second]=0;
        supplyLimit[p.first]=-1;
        return k;
    }
}
void lcMethod(vector<vector<int>>mainTable,int r,int c,int demandLimit[],int supplyLimit[],vector<pair<int,pair<int,int>>>&v,int &cost){
    pair<int,int>p=lcFindMin(mainTable,demandLimit,supplyLimit,r,c);
    if(p.first==-1) return ;
    v.push_back({0,p});
    int k=lcClearDemand(demandLimit,supplyLimit,p);
    cost+=mainTable[p.first][p.second]*k;
    v.back().first=k;
    lcMethod(mainTable,r,c,demandLimit,supplyLimit,v,cost);
}
pair<int,int> findPenality(vector<vector<int>>mainTable,int supplyLimit[],int demandLimit[],int r,int c){
    vector<pair<int,int>>vr,vc;
    vector<int>v;
    pair<int,int>pr{-1,-1},pc{-1,-1};
    for(int i=0;i<r;i++){
        v.clear();
        if(supplyLimit[i]!=-1){
            for(int j=0;j<c;j++){
                if(demandLimit[j]!=-1) v.push_back(mainTable[i][j]);
            }
	        int x1=*min_element(v.begin(),v.end());
	        //v.erase(find(v.begin(),v.end(),x1));
	        v.erase(remove(v.begin(), v.end(),x1),v.end());//remove all occurences of x1
	        
	        int x2=*min_element(v.begin(),v.end());
	        vr.push_back({x2-x1,(-1*i)});
    	}
    	
    }
    
    for(int i=0;i<c;i++){
        v.clear();
        if(demandLimit[i]!=-1){
            for(int j=0;j<r;j++){
                if(supplyLimit[j]!=-1) v.push_back(mainTable[j][i]);
            }
            int x1=*min_element(v.begin(),v.end());
	       // v.erase(find(v.begin(),v.end(),x1));
	        v.erase(remove(v.begin(), v.end(),x1),v.end()); //remove all occurences of x1
	        int x2=*min_element(v.begin(),v.end());
	        vc.push_back({x2-x1,(-1*i)});
        }
    }
    pair<int,int>p1=*max_element(vr.begin(),vr.end());
    pair<int,int>p2=*max_element(vc.begin(),vc.end());
    if(p1.first>p2.first) return {(-1*p1.second),0};
    else return {(-1*p2.second),1};
}
void VAM(vector<vector<int>>mainTable,int r,int c,int demandLimit[],int supplyLimit[],vector<pair<int,pair<int,int>>>&v,int &cost){	
    int count=0;
    for(int i=0;i<c;i++) if(demandLimit[i]!=-1) count++;
    if(count==1){
        lcMethod(mainTable,r,c,demandLimit,supplyLimit,v,cost);
        return;
    }
    for(int i=0;i<r;i++) if(supplyLimit[i]!=-1) count++;
    if(count==1){
        lcMethod(mainTable,r,c,demandLimit,supplyLimit,v,cost);
        return;
    }
    pair<int,int>p=findPenality(mainTable,supplyLimit,demandLimit,r,c); //second-> 0:r ; 1:c
    int min=INT_MAX,minIndex=0;
    if(!p.second){
        for(int i=0;i<c;i++){
            if(min>mainTable[p.first][i] && demandLimit[i]!=-1 && supplyLimit[p.first]!=-1){
                minIndex=i;
                min=mainTable[p.first][i];
            }
        }
        v.push_back({0,{p.first,minIndex}});
    }
    else{
        for(int i=0;i<r;i++){
            if(min>mainTable[i][p.first] && demandLimit[p.first]!=-1 && supplyLimit[i]!=-1){
                minIndex=i;
                min=mainTable[i][p.first];
            }
        }
        v.push_back({0,{minIndex,p.first}});
    }
    int k=lcClearDemand(demandLimit,supplyLimit,v.back().second);
    
    cost+=mainTable[v.back().second.first][v.back().second.second]*k;
    v.back().first=k;
    cout<<v.back().first<<" "<<v.back().second.first+1<<" "<<v.back().second.second+1<<" "<<cost<<endl;
    
    VAM(mainTable,r,c,demandLimit,supplyLimit,v,cost);
}
int main(){
    int r,c;
    cout<<"Enter no.of sources : ";
    cin>>r;
    cout<<"Enter no.of destinations : ";
    cin>>c;
    vector<vector<int>>mainTable(r,vector<int>(c,0));
	int supplyLimit[r],demandLimit[c];
    cout<<"Enter supply-limits : ";
    for(int i=0;i<r;i++) cin>>supplyLimit[i];
    cout<<"Enter demand-limits : ";
    for(int i=0;i<c;i++) cin>>demandLimit[i];
    cout<<"Enter unit transportation costs row wise :"<<endl;
    for(int i=0;i<r;i++){
        for(int j=0;j<c;j++) cin>>mainTable[i][j];
    }
    vector<pair<int,pair<int,int>>>v;
    int cost=0;
    VAM(mainTable,r,c,demandLimit,supplyLimit,v,cost);
    map<pair<int,int>,int>mp;
    for(auto k:v)
    {
    	mp[{k.second.first,k.second.second}]=k.first+1;
	}
    cout<<"solution table using AVAM :"<<endl;
    for(int i=0;i<r;i++)
    {
    	for(int j=0;j<c;j++)
    	{
    		if(mp[{i,j}]>=1)
    		{
    			cout<<mp[{i,j}]-1<<"\t";
			}
			else
			{
				cout<<"__\t";
			}
		}
		cout<<endl<<endl;
	}
    
    cout<<"AVAM cost is:"<<cost<<endl;
    cout<<"\nEnd of Code"<<endl;
    return 0;
}
/*
3
4
10 25 15
10 15 10 15
10 2 20 11
12 7 9 20
4 14 16 18
cost-475

3
4
50 75 25
20 20 50 60
6 8 10 9
5 8 11 5
6 9 12 5
cost-1085

4
5
100 80 70 90
60 40 100 50 90
4 4 9 8 13
7 9 8 10 4
9 3 7 10 6
11 4 8 3 9
cost-1670

5
5
40 70 35 90 85
80 55 60 80 45
8 8 2 10 2
11 4 10 9 4
5 2 2 11 10
10 6 6 5 2
8 11 8 6 4
cost-1475

*/
