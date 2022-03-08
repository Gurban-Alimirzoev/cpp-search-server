#include <string>
#include <iostream>
#include "remove_duplicates.h"

using namespace std;

void RemoveDuplicates(SearchServer& search_server) {
    set<set<string>> all_words;
    vector<int> dubl_doc;
    for (const int document_id : search_server){
        auto tmp = search_server.GetWordFrequencies(document_id);
        set<string> words;
        for(auto word_freqs : tmp){
            words.insert(word_freqs.first);
        }
        if(all_words.count(words) == 0){
            all_words.insert(words);
        }else {
        dubl_doc.push_back(document_id);
            cout << "Found duplicate document id " << document_id << endl;
        }
 
    }
    for (int i : dubl_doc) {
        search_server.RemoveDocument(i);
    }
}