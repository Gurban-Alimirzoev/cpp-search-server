#include "remove_duplicates.h"

using namespace std;

void RemoveDuplicates(SearchServer &search_server)
{
    set<set<string>> all_words;
    vector<int> dubl_doc;
    int compareDocId = 0;
    for (const int document_id : search_server)
    {
        auto tmp = search_server.GetWordFrequencies(document_id);
        set<string> words;
        for (auto word_freqs : tmp)
        {
            words.insert(word_freqs.first);
        }
        if (all_words.count(words) == 0)
        {
            all_words.insert(words);
            compareDocId = document_id;
        }
        else
        {
            if (document_id > compareDocId)
            {
                dubl_doc.push_back(document_id);
            }
            else {
                dubl_doc.push_back(compareDocId);
            }
        }
    }
    for (int id_doc : dubl_doc)
    {
        cout << "Found duplicate document id " << id_doc << endl;
        search_server.RemoveDocument(id_doc);
    }
}