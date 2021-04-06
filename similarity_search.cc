#include<bits/stdc++.h>
#include<xapian.h>
#include <sstream>
#include<string>
#include <iostream>
#include<chrono>
using namespace std;

int main(int argc, char **argv) {
    if (argc != 4) {
        cerr << "Usage: <index_name> <top-k> <set_cardinality>"<< endl;
        exit(0);
    }

    string index_name = argv[1];
    int topk = atoi(argv[2]);
    int cardinality = atoi(argv[3]);
    long long duration_total = 0;
    Xapian::Database db(index_name);

    //Building Query
    for(int i = 1; i <= 30; i++) {
        string line;
        string infile_name = argv[3];
        infile_name = infile_name + "_words_sets/"  + to_string(i);
        ifstream infile(infile_name);
        vector<string> terms;

        while (getline(infile, line)) {
            terms.push_back(line);
        }
    
        Xapian::Query query(
            Xapian::Query::OP_OR,
            terms.begin(),
            terms.end()
        );

        auto start = chrono::steady_clock::now();
        //Start the query
        Xapian::Enquire enquire(db);
        enquire.set_query(query);
    
        Xapian::MSet matches = enquire.get_mset(0, topk); 
        printf("mset size is %d\n", matches.size());
        auto end = chrono::steady_clock::now();
        
        cout << "TIME : "
            << chrono::duration_cast<chrono::milliseconds>(end - start).count()
            << "milliseconds" << endl;
        
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        duration_total += duration;
    }

    cout<<"Average time is: "<<duration_total/30<<endl;
}