#pragma once
#include <deque>
#include <algorithm>
#include "search_server.h"

class RequestQueue
{
public:
    explicit RequestQueue(const SearchServer &search_server);
    template <typename DocumentPredicate>
    std::vector<Document> AddFindRequest(const std::string &raw_query, DocumentPredicate document_predicate)
    {
        const auto search_results = searchServer.FindTopDocuments(raw_query, document_predicate);
        AddRequest(search_results, raw_query);
        return search_results;
    }
    std::vector<Document> AddFindRequest(const std::string &raw_query, DocumentStatus status);
    std::vector<Document> AddFindRequest(const std::string &raw_query);
    int GetNoResultRequests() const;

private:
    struct QueryResult
    {
        const std::string &raw_query;
        bool result;
    };
    std::deque<QueryResult> requests_;
    const static int min_in_day_ = 1440;
    const SearchServer &searchServer;
    void AddRequest(std::vector<Document> search_results, const std::string &raw_query);
};