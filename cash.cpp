#include <bits/stdc++.h>
using namespace std;

class bank{
    public:
    string name;
    int netAmount;
    set<string> types;
}; 

int getMinIndex_bank(bank listOfNetAmounts[],int numBanks){
    int min=INT_MAX, minIndex=-1;
    for(int i=0;i<numBanks;i++){
        if(listOfNetAmounts[i].netAmount == 0) continue;
        
        if(listOfNetAmounts[i].netAmount < min){
            minIndex = i;
            min = listOfNetAmounts[i].netAmount;
        }
    }
    return minIndex;
}

int getSimpleMaxIndex(bank listOfNetAmounts[],int numBanks){
    int max=INT_MIN, maxIndex=-1;
    for(int i=0;i<numBanks;i++){
        if(listOfNetAmounts[i].netAmount == 0) continue;
        
        if(listOfNetAmounts[i].netAmount > max){
            maxIndex = i;
            max = listOfNetAmounts[i].netAmount;
        }
    }
    return maxIndex;
}

pair<int,string> getMaxIndex(bank listOfNetAmounts[],int numBanks,int minIndex,bank input[],int maxNumTypes){
    int max=INT_MIN;
    int maxIndex=-1;
    string matchingType;
    
    for(int i=0;i<numBanks;i++){
        if(listOfNetAmounts[i].netAmount == 0) continue;
        
        if(listOfNetAmounts[i].netAmount < 0) continue;
        
        //TODO 
        //see complexity of intersection
        
        vector<string> v(maxNumTypes);
        vector<string>::iterator ls=set_intersection(listOfNetAmounts[minIndex].types.begin(),listOfNetAmounts[minIndex].types.end(), listOfNetAmounts[i].types.begin(),listOfNetAmounts[i].types.end(), v.begin());
        
        if((ls-v.begin())!=0 && max<listOfNetAmounts[i].netAmount ){
            max=listOfNetAmounts[i].netAmount;
            maxIndex=i;
            matchingType = *(v.begin());
        } 
    }
    
    //if there is NO such max which has a common type with any remaining banks then maxIndex has -1
    // also return the common payment type
    return make_pair(maxIndex,matchingType);
}

void printAns(vector<vector<pair<int,string>>> ansGraph, int numBanks,bank input[]){
    
    cout<<"\nThe transactions for minimum cash flow are as follows : \n\n";
    for(int i=0;i<numBanks;i++){
        for(int j=0;j<numBanks;j++){
            
            if(i==j) continue;
            
            if(ansGraph[i][j].first != 0 && ansGraph[j][i].first != 0){
                
                if(ansGraph[i][j].first == ansGraph[j][i].first){
                    ansGraph[i][j].first=0;
                    ansGraph[j][i].first=0;
                }
                else if(ansGraph[i][j].first > ansGraph[j][i].first){
                    ansGraph[i][j].first -= ansGraph[j][i].first; 
                    ansGraph[j][i].first =0;
                    
                    cout<<input[i].name<<" pays Rs" << ansGraph[i][j].first<< "to "<<input[j].name<<" via "<<ansGraph[i][j].second<<endl;
                }
                else{
                    ansGraph[j][i].first -= ansGraph[i][j].first;
                    ansGraph[i][j].first = 0;
                    
                    cout<<input[j].name<<" pays Rs "<< ansGraph[j][i].first<<" to "<<input[i].name<<" via "<<ansGraph[j][i].second<<endl;
                    
                }
            }
            else if(ansGraph[i][j].first != 0){
                cout<<input[i].name<<" pays Rs "<<ansGraph[i][j].first<<" to "<<input[j].name<<" via "<<ansGraph[i][j].second<<endl;
                
            }
            else if(ansGraph[j][i].first != 0){
                cout<<input[j].name<<" pays Rs "<<ansGraph[j][i].first<<" to "<<input[i].name<<" via "<<ansGraph[j][i].second<<endl;
                
            }
            
            ansGraph[i][j].first = 0;
            ansGraph[j][i].first = 0;
        }
    }
    cout<<"\n";
}

void minimizeCashFlow_Bank(int numBanks,bank input[],unordered_map<string,int>& indexOf,int numTransactions,vector<vector<int>>& graph,int maxNumTypes){
    
    //Find net amount of each bank has
    bank listOfNetAmounts[numBanks];
    
    for(int b=0;b<numBanks;b++){
        listOfNetAmounts[b].name = input[b].name;
        listOfNetAmounts[b].types = input[b].types;
        
        int amount = 0;
        //incoming edges
        //column travers
        for(int i=0;i<numBanks;i++){
            amount += (graph[i][b]);
        }
        
        //outgoing edges
        //row traverse
        for(int j=0;j<numBanks;j++){
            amount += ((-1) * graph[b][j]);
        }
        
        listOfNetAmounts[b].netAmount = amount;
    }
    
    vector<vector<pair<int,string>>> ansGraph(numBanks,vector<pair<int,string>>(numBanks,{0,""}));//adjacency matrix
    
    
    //find min and max net amount
    int numZeroNetAmounts=0;
    
    for(int i=0;i<numBanks;i++){
        if(listOfNetAmounts[i].netAmount == 0) numZeroNetAmounts++;
    }
    while(numZeroNetAmounts!=numBanks){
        
        int minIndex=getMinIndex_bank(listOfNetAmounts, numBanks);
        pair<int,string> maxAns = getMaxIndex(listOfNetAmounts, numBanks, minIndex,input,maxNumTypes);
        
        int maxIndex = maxAns.first;
        
        if(maxIndex == -1){
            
            (ansGraph[minIndex][0].first) += abs(listOfNetAmounts[minIndex].netAmount);
            (ansGraph[minIndex][0].second) = *(input[minIndex].types.begin());
            
            int simpleMaxIndex = getSimpleMaxIndex(listOfNetAmounts, numBanks);
            (ansGraph[0][simpleMaxIndex].first) += abs(listOfNetAmounts[minIndex].netAmount);
            (ansGraph[0][simpleMaxIndex].second) = *(input[simpleMaxIndex].types.begin());
            
            listOfNetAmounts[simpleMaxIndex].netAmount += listOfNetAmounts[minIndex].netAmount;
            listOfNetAmounts[minIndex].netAmount = 0;
            
            if(listOfNetAmounts[minIndex].netAmount == 0) numZeroNetAmounts++;
            
            if(listOfNetAmounts[simpleMaxIndex].netAmount == 0) numZeroNetAmounts++;
            
        }
        else{
            int transactionAmount = min(abs(listOfNetAmounts[minIndex].netAmount), listOfNetAmounts[maxIndex].netAmount);
            
            (ansGraph[minIndex][maxIndex].first) += (transactionAmount);
            (ansGraph[minIndex][maxIndex].second) = maxAns.second;
            
            listOfNetAmounts[minIndex].netAmount += transactionAmount;
            listOfNetAmounts[maxIndex].netAmount -= transactionAmount;
            
            if(listOfNetAmounts[minIndex].netAmount == 0) numZeroNetAmounts++;
            
            if(listOfNetAmounts[maxIndex].netAmount == 0) numZeroNetAmounts++;
        }
        
    }
    
    printAns(ansGraph,numBanks,input);

}


//////////////////////////
//////////////////////////
//////People's Cash///////
//////////////////////////
//////////////////////////


int getMinIndex_personal(vector<int>& netAmount) {
    int minIndex = -1, minValue = INT_MAX;
    for (int i = 0; i < netAmount.size(); i++) {
        if (netAmount[i] < minValue) {
            minValue = netAmount[i];
            minIndex = i;
        }
    }
    return minIndex;
}

int getMaxIndex_personal(vector<int>& netAmount) {
    int maxIndex = -1, maxValue = INT_MIN;
    for (int i = 0; i < netAmount.size(); i++) {
        if (netAmount[i] > maxValue) {
            maxValue = netAmount[i];
            maxIndex = i;
        }
    }
    return maxIndex;
}

void minimizeCashFlow_personal(vector<int>& netAmount, vector<string>& people) {
    vector<tuple<string, string, int>> result;
    
    while (true) {
        int maxCreditIndex = getMaxIndex_personal(netAmount);
        int maxDebitIndex = getMinIndex_personal(netAmount);
        
        if (netAmount[maxCreditIndex] == 0 && netAmount[maxDebitIndex] == 0)
            break;

        int minValue = min(-netAmount[maxDebitIndex], netAmount[maxCreditIndex]);
        netAmount[maxCreditIndex] -= minValue;
        netAmount[maxDebitIndex] += minValue;

        result.push_back(make_tuple(people[maxDebitIndex], people[maxCreditIndex], minValue));
    }
    
    // Output minimized transactions
    cout << "\nMinimized transactions are:\n";
    if(result.size()==0) cout<<"No Transactions required.\n****Thank You****\n";
    for (auto& transaction : result) {
        cout << get<0>(transaction) << " has to pay " << get<2>(transaction) << " amount to " << get<1>(transaction) << endl;
    }
}

int main() {

    system("cls");
    cout<<"\n\t\t\t\t*********************** Welcome to CASH FLOW MINIMIZER SYSTEM ***********************\n\n";
    int problem;
    cout<<"***Choose the problem type***\n"
        <<"1. Personal Transaction:\n\t-->(It is to settle cash flow between people. Every person has access to all modes of payment)\n"
        <<"2. Bank Transaction:\n\t-->(Each Bank only supports a set of modes of payments and can make or receive payments only via those. "
        <<"Only World Bank suppports all modes of payments.)\n"
        <<"here: ";
    cin>>problem;
    if(problem==1){
        cout<<"\n\t\t\t\t****************************** People's Transaction *********************************\n\n";
    int numPersons;
    cout << "Enter the number of persons involved: ";
    cin >> numPersons;

    vector<string> people(numPersons);
    cout << "Enter the names of the persons involved:\n";
    for (int i = 0; i < numPersons; i++) {
        cin >> people[i];
    }

    int numTransactions;
    cout << "Enter the number of transactions: ";
    cin >> numTransactions;

    vector<int> netAmount(numPersons, 0);
    cout << "Enter each transaction: [person(i) person(j) amount] (i.e Person(i) has pay to Person(j) the amount):\n";
    for (int i = 0; i < numTransactions; i++) {
        string debtor, creditor;
        int amount;
        cin >> debtor >> creditor >> amount;
        int debtorIndex = find(people.begin(), people.end(), debtor) - people.begin();
        int creditorIndex = find(people.begin(), people.end(), creditor) - people.begin();
        netAmount[debtorIndex] -= amount;
        netAmount[creditorIndex] += amount;
    }

    minimizeCashFlow_personal(netAmount, people);
    }
    /////////////////////problem 2/////////////////////////
    if(problem==2){
                cout<<"\n\t\t\t\t******************************* Bank's Transaction **********************************\n\n";
        cout<<"This system minimizes the number of transactions among multiple banks in the different corners of the world that use different modes of payment.There is one world bank (with all payment modes) to act as an intermediary between banks that have no common mode of payment. \n\n";
        cout<<"Enter the number of banks participating in the transactions.\n";
        int numBanks;cin>>numBanks;
        
        bank input[numBanks];
        unordered_map<string,int> indexOf;//stores index of a bank
        
        cout<<"Enter the details of the banks and transactions as stated:\n";
        cout<<"[Bank name, Number of payment modes it supports, The payment modes]\n";
        cout<<"Enter space separated inputs (Example : SwissBank 2 Paypal Fampay )\n";
        
        int maxNumTypes;
        for(int i=0; i<numBanks;i++){
            if(i==0){
                cout<<"World Bank Name (the bank that supports all payment methods):";
            }
            else{
                cout<<"Bank "<<i+1<<" : ";
            }
            cin>>input[i].name;
            indexOf[input[i].name] = i;
            int numTypes;
            cin>>numTypes;
            
            if(i==0) maxNumTypes = numTypes;
            
            string type;
            while(numTypes--){
                cin>>type;
                
                input[i].types.insert(type);
            }   
            
        }
        
        cout<<"Enter number of transactions.\n";
        int numTransactions;
        cin>>numTransactions;
        
        vector<vector<int>> graph(numBanks,vector<int>(numBanks,0));//adjacency matrix
        
        cout<<"Enter the details of each transaction as stated:";
        cout<<"Debtor Bank , creditor Bank and amount\n";
        cout<<"The transactions can be in any order\n";
        for(int i=0;i<numTransactions;i++){
            cout<<(i+1)<<" th transaction : ";
            string s1,s2;
            int amount;
            cin >> s1>>s2>>amount;
            
            graph[indexOf[s1]][indexOf[s2]] = amount;
        }
        
        //settle
        minimizeCashFlow_Bank(numBanks,input,indexOf,numTransactions,graph,maxNumTypes);
    }
    cout<<"\nChoose\n"
        <<"1. Try Again\n"
        <<"2. Exit\n"
        <<"here->";
    int run;
    cin>>run;
    if(run==1) main();

    cout<<"\n\t\t\t\t********************* Thank You ***********************\n\n\n";
    return 0;
}