#pragma once
#include "search_server.h"

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