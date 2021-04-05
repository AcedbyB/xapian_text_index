#include<bits/stdc++.h>
#include<xapian.h>
#include <sstream>
#include<string>
#include <iostream>
using namespace std;
set<string> needed_words;

string highlight(string s) {
    string ret = "";
    string temp = "";

    // Filter words and check if it is matched
    for(std::string::size_type i = 0; i < s.size(); i++)
    {
        if(isalnum(s[i])){
            temp+=s[i];
        }
        else
        {   
            if(temp != "" && needed_words.find(temp)!=needed_words.end())
            {
                ret = ret + "==" + temp + "==";
            }
            else 
            {
                ret = ret + temp;
            }
            ret = ret + " ";
            temp = "";
        }
    }
    
    return ret;
}

int main(int argc, char **argv) {
    if (argc < 3) {
        cerr << "Usage: <index_name> <top-k> <keyword1> <keyword2> ..."  << endl;
        exit(0);
    }

    string index_name = argv[1];
    int topk = atoi(argv[2]);
    Xapian::Database db(index_name);

    //Building Query
    vector<string> or_terms, and_terms;
    for(int i = 3; i < argc; i++) {
        char sign = argv[i][0];
        string cur = argv[i];
        if(sign == '+') {
            cur = cur.substr(1, (int)cur.size());
            and_terms.push_back(cur);
        }
        else or_terms.push_back(cur);

        needed_words.insert(cur);
    }
    
    Xapian::Query or_query(
        Xapian::Query::OP_OR,
        or_terms.begin(),
        or_terms.end()
    );
    Xapian::Query and_query(
        Xapian::Query::OP_AND,
        and_terms.begin(),
        and_terms.end()
    );

    //Combine 2 query with an AND
    Xapian::Query query(
        Xapian::Query::OP_AND,
        and_query,
        or_query
    );	

    //Start the query
    Xapian::Enquire enquire(db);
    enquire.set_query(query);
 
    Xapian::MSet matches = enquire.get_mset(0, topk); 
    printf("mset size is %d\n", matches.size());
    
    for(Xapian::MSetIterator match = matches.begin();  match != matches.end(); match ++) {
        Xapian::Document doc = match.get_document();
        cout << highlight(doc.get_value(1)) << endl << highlight(doc.get_value(0)) <<endl;
    }
}