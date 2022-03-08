#pragma once
#include "search_server.h"

template <typename MapS, typename MapF>
void Print(std::ostream& out, const std::map<MapF, MapS>& container);

template <typename Data>
void Print(std::ostream& out, const Data& container);

template <typename Element>
std::ostream& operator<<(std::ostream& out, const std::vector<Element>& container);

template <typename SetElement>
std::ostream& operator<<(std::ostream& out, const std::set<SetElement>& container);

template <typename MapS, typename MapF>
std::ostream& operator<<(std::ostream& out, const std::map<MapF, MapS>& container);

template <typename Test, typename TestName>
void RunTestImpl(const Test &test, const TestName &func);
#define RUN_TEST(func) RunTestImpl(func, #func)

template <typename T, typename U>
void AssertEqualImpl(const T &t, const U &u, const std::string &t_str, const std::string &u_str, const std::string &file,
                     const std::string &func, unsigned line, const std::string &hint);

#define ASSERT_EQUAL(a, b) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, ""s)

#define ASSERT_EQUAL_HINT(a, b, hint) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, (hint))

void AssertImpl(bool value, const std::string &expr_str, const std::string &file, const std::string &func, unsigned line,
                const std::string &hint);

#define ASSERT(expr) AssertImpl(!!(expr), #expr, __FILE__, __FUNCTION__, __LINE__, ""s)

#define ASSERT_HINT(expr, hint) AssertImpl(!!(expr), #expr, __FILE__, __FUNCTION__, __LINE__, (hint))

void TestExcludeStopWordsFromAddedDocumentContent();
void TestMinusWord();
void TestMatchedDoc();
void TestRaitingCompute();
void TestRelevAndTestSort();
void TestSearchWithDocStatus();
void TestFilterWithPred();
void TestSearchServer();