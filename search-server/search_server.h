#pragma once
#include <set>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <stdexcept>
#include "string_processing.h"
#include "document.h"
#include "log_duration.h"

using namespace std::string_literals;
const double precision = 1e-6;
const int MAX_RESULT_DOCUMENT_COUNT = 5;

class SearchServer
{
public:
    SearchServer();

    template <typename StringContainer>
    SearchServer(const StringContainer &stop_words);

    explicit SearchServer(const std::string &stop_words_text);

    void AddDocument(int document_id, const std::string &document, DocumentStatus status, const std::vector<int> &ratings);

    template <typename DocumentPredicate>
    std::vector<Document> FindTopDocuments(const std::string &raw_query, DocumentPredicate document_predicate) const;
    std::vector<Document> FindTopDocuments(const std::string &raw_query, DocumentStatus status) const;
    std::vector<Document> FindTopDocuments(const std::string &raw_query) const;

    int GetDocumentCount() const;
    const std::map<std::string, double> &GetWordFrequencies(int document_id) const;

    void RemoveDocument(int document_id);
    std::tuple<std::vector<std::string>, DocumentStatus> MatchDocument(const std::string &raw_query, int document_id) const;

    std::set<int>::const_iterator begin() const;
    std::set<int>::const_iterator end() const;

private:
    struct DocumentData
    {
        int rating;
        DocumentStatus status;
    };
    struct QueryWord
    {
        std::string data;
        bool is_minus;
        bool is_stop;
    };
    QueryWord ParseQueryWord(const std::string &text) const;
    struct Query
    {
        std::set<std::string> plus_words;
        std::set<std::string> minus_words;
    };

    const std::set<std::string> stop_words_;
    std::map<std::string, std::map<int, double>> word_to_document_freqs_;
    std::map<int, std::map<std::string, double>> document_to_word_freqs_;
    std::map<int, DocumentData> documents_;
    std::set<int> document_ids_;

    bool IsStopWord(const std::string &word) const;
    static bool IsValidWord(const std::string &word);

    std::vector<std::string> SplitIntoWordsNoStop(const std::string &text) const;
    Query ParseQuery(const std::string &text) const;

    static int ComputeAverageRating(const std::vector<int> &ratings);
    double ComputeWordInverseDocumentFreq(const std::string &word) const;

    template <typename DocumentPredicate>
    std::vector<Document> FindAllDocuments(const Query &query, DocumentPredicate document_predicate) const;
};
