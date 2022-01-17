void TestExcludeStopWordsFromAddedDocumentContent()
{
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const vector<int> ratings = {1, 2, 3};
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto found_docs = server.FindTopDocuments("in"s);
        ASSERT_EQUAL(found_docs.size(), 1u);
        const Document &doc0 = found_docs[0];
        ASSERT_EQUAL(doc0.id, doc_id);
    }

    {
        SearchServer server;
        server.SetStopWords("in the"s);
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        ASSERT_HINT(server.FindTopDocuments("in"s).empty(), "Stop words must be excluded from documents"s);
    }
}

void TestMinusWord()
{
    const int doc_id = 42;
    const string contentMinus = "cat"s;
    const vector<int> ratings = {1, 2, 3};

    SearchServer server;

    server.AddDocument(doc_id, contentMinus, DocumentStatus::ACTUAL, ratings);
    ASSERT_EQUAL(server.FindTopDocuments("cat"s).empty(), false);
    server.SetStopWords("cat"s);
    ASSERT_HINT(server.FindTopDocuments("cat"s).empty(), "Documents with minus words must be excluded in search results");
}

void TestMatchedDoc()
{
    const int doc_id = 42;
    const string contentMinus = "cat"s;
    const string contentPlus = "dog"s;
    const string contentPlusWithMinus = "cat dog"s;
    const vector<int> ratings = {1, 2, 3};

    SearchServer server;

    server.SetStopWords("cat"s);
    server.AddDocument(doc_id, contentMinus, DocumentStatus::ACTUAL, ratings);
    server.AddDocument(doc_id, contentPlusWithMinus, DocumentStatus::ACTUAL, ratings);
    server.AddDocument(doc_id, contentPlus, DocumentStatus::ACTUAL, ratings);
    string message = "Documents do not match the search query";
    ASSERT_EQUAL_HINT(server.FindTopDocuments("cat"s).size(), 0u, message);
    ASSERT_EQUAL_HINT(server.FindTopDocuments("cat dog"s).size(), 1u, message);
    ASSERT_EQUAL_HINT(server.FindTopDocuments("dog"s).size(), 1u, message);
}

void TestRaitingCompute()
{

    const string contentMinus = "cat dog bad"s;
    const vector<int> ratings = {1, 2, 3};
    const vector<int> zero = {0};
    const vector<int> one = {1, 1, 1};

    SearchServer server;

    server.AddDocument(1, contentMinus, DocumentStatus::ACTUAL, ratings);
    server.AddDocument(5, contentMinus, DocumentStatus::ACTUAL, zero);
    server.AddDocument(9, contentMinus, DocumentStatus::ACTUAL, one);
    string message = "Rating is incorrect";
    ASSERT_EQUAL_HINT(server.FindTopDocuments("cat dog bad")[0].rating, 2, message);
    ASSERT_EQUAL_HINT(server.FindTopDocuments("cat dog")[1].rating, 1, message);
    ASSERT_EQUAL_HINT(server.FindTopDocuments("cat")[2].rating, 0, message);
}

void TestSortRelev()
{
    const int doc_id_one = 1;
    const int doc_id_two = 2;
    const int doc_id_tr = 3;
    const int doc_id_for = 4;
    const string contentOne = "cat dog bad good"s;
    const string contentTwo = "cat"s;
    const string contentThree = "dog bad cat"s;
    const string contentFore = "bad good"s;
    const vector<int> ratings = {1, 2, 3};

    SearchServer server;

    server.AddDocument(doc_id_one, contentOne, DocumentStatus::ACTUAL, ratings);
    server.AddDocument(doc_id_two, contentTwo, DocumentStatus::ACTUAL, ratings);
    server.AddDocument(doc_id_tr, contentThree, DocumentStatus::ACTUAL, ratings);
    server.AddDocument(doc_id_for, contentFore, DocumentStatus::ACTUAL, ratings);
    string message = "Documents are sorted incorrectly";
    ASSERT_EQUAL_HINT(server.FindTopDocuments("cat dog")[0].id, 3, message);
    ASSERT_EQUAL_HINT(server.FindTopDocuments("cat dog")[1].id, 2, message);
    ASSERT_EQUAL_HINT(server.FindTopDocuments("cat dog")[2].id, 1, message);
}

// Функция TestSearchServer является точкой входа для запуска тестов
void TestSearchServer()
{
    RUN_TEST(TestExcludeStopWordsFromAddedDocumentContent);
    RUN_TEST(TestMinusWord);
    RUN_TEST(TestMatchedDoc);
    RUN_TEST(TestRaitingCompute);
    RUN_TEST(TestSortRelev);
}