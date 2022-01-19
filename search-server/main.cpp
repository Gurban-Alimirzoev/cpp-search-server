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
    const int doc_id_one = 1;
    const int doc_id_two = 2;
    const int doc_id_tr = 3;
    const string doc_1 = "cat"s;
    const string doc_2 = "cat dog"s;
    const string doc_3 = "dog"s;
    const vector<int> ratings = {1, 2, 3};

    SearchServer server;
    server.AddDocument(doc_id_one, doc_1, DocumentStatus::ACTUAL, ratings);
    server.AddDocument(doc_id_two, doc_2, DocumentStatus::ACTUAL, ratings);
    server.AddDocument(doc_id_tr, doc_3, DocumentStatus::ACTUAL, ratings);

    string message = "Documents with minus words must be excluded in search results";
    ASSERT_EQUAL(server.FindTopDocuments("cat"s).empty(), false);
    vector<Document> test = server.FindTopDocuments("dog -cat"s);
    ASSERT_EQUAL_HINT(test.size(), 1u, message);
    ASSERT_EQUAL_HINT(test[0].id, doc_id_tr, message);
}

void TestMatchedDoc()
{
    const int doc_id_one = 1;
    const string contentPlusWithMinus = "cat dog good bad"s;
    const vector<int> ratings = {1, 2, 3};

    SearchServer server;
    //server.SetStopWords("cat dog"s);
    server.AddDocument(doc_id_one, contentPlusWithMinus, DocumentStatus::ACTUAL, ratings);
    string message = "Documents do not match the search query";

    vector<string> vectorPlusWordInMatchDoc, vectorMinusWordInMatchDoc;
    DocumentStatus TestDocumentStatus;
    tie(vectorPlusWordInMatchDoc, TestDocumentStatus) = server.MatchDocument("cat dog good", doc_id_one);
    tie(vectorMinusWordInMatchDoc, TestDocumentStatus) = server.MatchDocument("-cat dog good bad", doc_id_one);
    vector<string> testVector = {"cat", "dog", "good"};
    ASSERT_EQUAL_HINT(vectorPlusWordInMatchDoc, testVector, message);
    ASSERT_EQUAL_HINT(vectorMinusWordInMatchDoc.size(), 0u, message);
}

void TestRaitingCompute()
{
    const string contentMinus = "cat dog bad"s;
    const vector<int> ratings = {1, 2, 3};
    const vector<int> one = {1, 1, 1};
    const vector<int> zero = {0};
    const int doc_id_one = 1;
    const int doc_id_two = 5;
    const int doc_id_tr = 9;

    SearchServer server;
    server.AddDocument(doc_id_one, contentMinus, DocumentStatus::ACTUAL, ratings);
    server.AddDocument(doc_id_two, contentMinus, DocumentStatus::ACTUAL, one);
    server.AddDocument(doc_id_tr, contentMinus, DocumentStatus::ACTUAL, zero);
    string message = "Rating is incorrect";
    vector<Document> test = server.FindTopDocuments(contentMinus);
    ASSERT_EQUAL(test[0].id, doc_id_one);
    ASSERT_EQUAL_HINT(test[0].rating, 2, message);

    ASSERT_EQUAL(test[1].id, doc_id_two);
    ASSERT_EQUAL_HINT(test[1].rating, 1, message);

    ASSERT_EQUAL(test[2].id, doc_id_tr);
    ASSERT_EQUAL_HINT(test[2].rating, 0, message);
}

void TestRelevAndTestSort()
{
    const int doc_id_one = 1;
    const int doc_id_two = 2;
    const int doc_id_tr = 3;
    const int doc_id_for = 4;
    const string contentOne = "cat dog bad good"s;
    const string contentTwo = "cat"s;
    const string contentThree = "dog bad cat"s;
    const vector<int> ratings = {1, 2, 3};
    const vector<int> ratingsForSort = {1};

    SearchServer server;
    server.AddDocument(doc_id_one, contentOne, DocumentStatus::ACTUAL, ratings);
    server.AddDocument(doc_id_two, contentTwo, DocumentStatus::ACTUAL, ratings);
    server.AddDocument(doc_id_tr, contentThree, DocumentStatus::ACTUAL, ratings);
    server.AddDocument(doc_id_for, contentOne, DocumentStatus::ACTUAL, ratingsForSort);

    string message = "Documents are sorted incorrectly";
    vector<Document> test = server.FindTopDocuments("cat dog");
    ASSERT_EQUAL(test[0].id, 3);
    ASSERT_EQUAL_HINT(test[0].relevance, 0.095894024150593615, message);

    ASSERT_EQUAL(test[1].id, 1);
    ASSERT_EQUAL_HINT(test[1].relevance, 0.071920518112945211, message);

    ASSERT_EQUAL(test[2].id, 4);
    ASSERT_EQUAL_HINT(test[2].relevance, 0.071920518112945211, message);
    ASSERT_EQUAL_HINT(test[2].rating, 1, message);

    ASSERT_EQUAL(test[3].id, 2);
    ASSERT_EQUAL_HINT(test[3].relevance, 0, message);
    ASSERT_EQUAL_HINT(test[3].rating, 2, message);
}

void TestSearchWithDocStatus()
{
    const int doc_id_one = 1;
    const int doc_id_two = 2;
    const int doc_id_tr = 3;
    const int doc_id_for = 4;
    const string contentOne = "cat dog bad good"s;
    const string contentTwo = "cat god"s;
    const string contentThree = "dog bad cat"s;
    const string contentFore = "dog bad cat god"s;
    const string testContent = "cat";
    const vector<int> ratings = {1, 2, 3};

    SearchServer server;
    server.AddDocument(doc_id_one, contentOne, DocumentStatus::ACTUAL, ratings);
    server.AddDocument(doc_id_two, contentTwo, DocumentStatus::BANNED, ratings);
    server.AddDocument(doc_id_tr, contentThree, DocumentStatus::IRRELEVANT, ratings);
    server.AddDocument(doc_id_for, contentFore, DocumentStatus::REMOVED, ratings);

    string message = "Documents search incorrectly";
    vector<Document> testActual = server.FindTopDocuments(testContent, DocumentStatus::ACTUAL);
    ASSERT_EQUAL(testActual.size(), 1u);
    ASSERT_EQUAL_HINT(testActual[0].id, doc_id_one, message);

    vector<Document> testBanned = server.FindTopDocuments(testContent, DocumentStatus::BANNED);
    ASSERT_EQUAL(testBanned.size(), 1u);
    ASSERT_EQUAL_HINT(testBanned[0].id, doc_id_two, message);

    vector<Document> testIrrelevant = server.FindTopDocuments(testContent, DocumentStatus::IRRELEVANT);
    ASSERT_EQUAL(testIrrelevant.size(), 1u);
    ASSERT_EQUAL_HINT(testIrrelevant[0].id, doc_id_tr, message);

    vector<Document> testRemoved = server.FindTopDocuments(testContent, DocumentStatus::REMOVED);
    ASSERT_EQUAL(testRemoved.size(), 1u);
    ASSERT_EQUAL_HINT(testRemoved[0].id, doc_id_for, message);
}

void TestFilterWithPred()
{
    const int doc_id_one = 1;
    const int doc_id_two = 2;
    const int doc_id_tr = 3;
    const int doc_id_for = 4;
    const string contentOne = "cat dog bad good"s;
    const string contentTwo = "cat"s;
    const string contentThree = "dog bad cat"s;
    const string contentFore = "dog cat"s;
    const int ratingOne = 1;
    const int ratingTwo = 2;
    const vector<int> ratingsOne = {ratingOne};
    const vector<int> ratingsTwo = {ratingTwo};

    SearchServer server;
    server.AddDocument(doc_id_one, contentOne, DocumentStatus::ACTUAL, ratingsOne);
    server.AddDocument(doc_id_two, contentTwo, DocumentStatus::BANNED, ratingsTwo);
    server.AddDocument(doc_id_tr, contentThree, DocumentStatus::IRRELEVANT, ratingsOne);
    server.AddDocument(doc_id_for, contentFore, DocumentStatus::REMOVED, ratingsTwo);
    string message = "Predicat filter worked incorrectly";

    vector<Document> testStatus = server.FindTopDocuments(contentTwo, [](int document_id, DocumentStatus status, int rating)
                                                          { return status == DocumentStatus::ACTUAL; });
    ASSERT_EQUAL(testStatus.size(), 1u);
    ASSERT_EQUAL_HINT(testStatus[0].id, doc_id_one, message);

    vector<Document> testRating = server.FindTopDocuments(contentTwo, [](int document_id, DocumentStatus status, int rating)
                                                          { return rating == 2; });
    ASSERT_EQUAL(testRating.size(), 2u);
    ASSERT_EQUAL_HINT(testRating[0].id, doc_id_two, message);
    ASSERT_EQUAL_HINT(testRating[1].id, doc_id_for, message);

    vector<Document> testId = server.FindTopDocuments(contentTwo, [](int document_id, DocumentStatus status, int rating)
                                                      { return document_id == 1; });
    ASSERT_EQUAL(testId.size(), 1u);
    ASSERT_EQUAL_HINT(testId[0].id, doc_id_one, message);
}

void TestSearchServer()
{
    RUN_TEST(TestExcludeStopWordsFromAddedDocumentContent);
    RUN_TEST(TestMinusWord);
    RUN_TEST(TestMatchedDoc);
    RUN_TEST(TestRaitingCompute);
    RUN_TEST(TestRelevAndTestSort);
    RUN_TEST(TestSearchWithDocStatus);
    RUN_TEST(TestFilterWithPred);
}