#include "Functions.h"


std::string gbk_to_utf8(const std::string& gbk_string) {
    int utf8_length = ::MultiByteToWideChar(CP_ACP, 0, gbk_string.c_str(), -1, NULL, 0);
    if (utf8_length == 0) {
        // ת��ʧ��
        return "";
    }

    std::wstring utf16_buffer(utf8_length, L'\0');
    ::MultiByteToWideChar(CP_ACP, 0, gbk_string.c_str(), -1, &utf16_buffer[0], utf8_length);

    int utf8_size = ::WideCharToMultiByte(CP_UTF8, 0, utf16_buffer.c_str(), -1, NULL, 0, NULL, NULL);
    if (utf8_size == 0) {
        // ת��ʧ��
        return "";
    }

    std::string utf8_buffer(utf8_size, '\0');
    ::WideCharToMultiByte(CP_UTF8, 0, utf16_buffer.c_str(), -1, &utf8_buffer[0], utf8_size, NULL, NULL);

    return utf8_buffer;
}
bool is_utf8(const std::string& str) {
    for (auto c : str) {
        if ((c & 0xC0) != 0x80) {
            return true;
        }
    }
    return false;
}
// Print the contents of the given collection.
void printCollection(mongocxx::collection& collection)
{
    std::cout << "Collection: " << collection.name() << std::endl;
    // Check if collection is empty.
    if (collection.count_documents({}) == 0)
    {
        std::cout << "Collection is empty." << std::endl;
        return;
    }
    auto cursor = collection.find({});
    for (auto&& doc : cursor)
    {
        std::cout << bsoncxx::to_json(doc) << std::endl;
    }
}

// Find the document with given key-value pair.
void findDocument(mongocxx::collection& collection, const std::string& key, const std::string& value)
{
    std::cout << "Filter:" << key << ": " << value << std::endl;
    try {
        // Create the query filter
        auto filter = bsoncxx::builder::stream::document{} << key << value << bsoncxx::builder::stream::finalize;
        //Add query filter argument in find
        auto cursor = collection.find({ filter });
        std::cout << "result:";
        for (auto&& doc : cursor)
        {
            std::cout << bsoncxx::to_json(doc) << std::endl;
        }
        std::cout << "OK" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
    }
}

// Find the document with given key-value pair.
void findDocument(mongocxx::collection& collection, const std::string& key, int value)
{
    std::cout << "Filter:" << key << ": " << value << std::endl;
    //std::cout << key << std::endl;
    try {
        // Create the query filter
        auto filter = bsoncxx::builder::stream::document{} << key << value << bsoncxx::builder::stream::finalize;
        //Add query filter argument in find
        auto cursor = collection.find(filter.view());
        for (auto&& doc : cursor)
        {
            std::cout << bsoncxx::to_json(doc) << std::endl;
        }
        std::cout << "OK" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "An error occurred!: " << e.what() << std::endl;
    }
}

bsoncxx::document::view getLatestDocument(mongocxx::collection& collection) {
    // ���������ĵ���������ʱ�併������
    bsoncxx::builder::stream::document sort_doc;
    sort_doc << "_id" << -1;

    // ��ѯ�����������ĵ���ȡ��һ���ĵ�
    auto options = mongocxx::options::find{};
    options.sort(sort_doc.view());
    options.limit(1); // ���ƽ��Ϊһ���ĵ�

    auto cursor = collection.find({}, options);

    if (cursor.begin() != cursor.end()) {
        return *cursor.begin();
    }
    else {
        // �����ѯ���Ϊ�գ��򷵻�һ�����ĵ�
        return {};
    }
}

mongocxx::client establishConnection(const std::string& uri_string) {
    // ʹ�ô���� URI �ַ������� mongocxx::uri ����
    mongocxx::uri uri(uri_string);

    // ʹ�� URI ������ MongoDB ����
    mongocxx::client conn(uri);

    return conn;
}

void insertTimeSeriesData(mongocxx::collection& collection, const bsoncxx::document::view& json_data) {
    // ���� BSON �ĵ��԰�װʱ����������
    bsoncxx::builder::stream::document ts_doc_builder{};
    ts_doc_builder << "data" << json_data;
    auto ts_doc = ts_doc_builder << bsoncxx::builder::stream::finalize;

    // ����ʱ����������
    collection.insert_one(ts_doc.view());
}

void insertJsonData(mongocxx::collection& collection, const bsoncxx::document::view& json_data) {
    // �������� JSON ���ݵ��ĵ�
    bsoncxx::builder::stream::document doc_builder{};
    for (const auto& element : json_data) {
        doc_builder << element.key() << element.get_value();
    }
    // ���뵱ǰʱ����Ϊʱ���
    doc_builder << "timestamp" << bsoncxx::types::b_date{ std::chrono::system_clock::now() };
    auto doc = doc_builder << bsoncxx::builder::stream::finalize;

    // �����ĵ���������
    collection.insert_one(doc.view());
}

// �������ݵ�����
void insertData(mongocxx::collection& collection) {
    PROFILE_FUNCTION();
    auto timestamp = bsoncxx::types::b_date(std::chrono::system_clock::now());

    bsoncxx::builder::stream::document doc{};
    // ���ݼ�������ȷ��Ҫ��������ݸ�ʽ
    if (collection.name() == "data0") {
        std::string fixed_string = "example_string";
        int random_number = generateRandomNumber(1, 100); // ���������
        doc << "timestamp" << timestamp << "string_field" << fixed_string << "random_number" << random_number;
    }
    else if (collection.name() == "data1") {
        std::string fixed_string = "another_string";
        int random_number = generateRandomNumber(100, 200); // ���������
        doc << "timestamp" << timestamp << "string_field" << fixed_string << "random_number" << random_number;
        for (int i = 0; i < 3; ++i) {
            std::string additional_string = "additional_string_" + std::to_string(i);
            int additional_random_number = generateRandomNumber(200, 300); // ���������
            doc << "additional_string" << additional_string << "additional_random_number" << additional_random_number;
        }
    }
    else if (collection.name() == "data2") {
        std::string fixed_string = "yet_another_string";
        int random_number = generateRandomNumber(200, 300); // ���������
        doc << "timestamp" << timestamp << "string_field" << fixed_string << "random_number" << random_number;
        for (int i = 0; i < 5; ++i) {
            std::string additional_string = "additional_string_" + std::to_string(i);
            int additional_random_number = generateRandomNumber(300, 400); // ���������
            doc << "additional_string" << additional_string << "additional_random_number" << additional_random_number;
        }
    }
    else if (collection.name() == "data3") {
        // ���ݸ�ʽΪʾ��������������޸�
        std::string fixed_string = "yet_another_string";
        int random_number = generateRandomNumber(200, 300); // ���������
        doc << "timestamp" << timestamp << "string_field" << fixed_string << "random_number" << random_number;
        for (int i = 0; i < 7; ++i) {
            std::string additional_string = "additional_string_" + std::to_string(i);
            int additional_random_number = generateRandomNumber(400, 500); // ���������
            doc << "additional_string" << additional_string << "additional_random_number" << additional_random_number;
        }
    }
    else if (collection.name() == "data4") {
        std::string fixed_string = "another_string";
        int random_number = generateRandomNumber(100, 200); // ���������
        doc << "timestamp" << timestamp << "string_field" << fixed_string << "random_number" << random_number;
        for (int i = 0; i < 3; ++i) {
            std::string additional_string = "additional_string_" + std::to_string(i);
            int additional_random_number = generateRandomNumber(200, 300); // ���������
            doc << "additional_string" << additional_string << "additional_random_number" << additional_random_number;
        }
    }
    else if (collection.name() == "data5") {
        std::string fixed_string = "yet_another_string";
        int random_number = generateRandomNumber(200, 300); // ���������
        doc << "timestamp" << timestamp << "string_field" << fixed_string << "random_number" << random_number;
        for (int i = 0; i < 5; ++i) {
            std::string additional_string = "additional_string_" + std::to_string(i);
            int additional_random_number = generateRandomNumber(300, 400); // ���������
            doc << "additional_string" << additional_string << "additional_random_number" << additional_random_number;
        }
    }
    else if (collection.name() == "data6") {
        // ���ݸ�ʽΪʾ��������������޸�
        std::string fixed_string = "yet_another_string";
        int random_number = generateRandomNumber(200, 300); // ���������
        doc << "timestamp" << timestamp << "string_field" << fixed_string << "random_number" << random_number;
        for (int i = 0; i < 7; ++i) {
            std::string additional_string = "additional_string_" + std::to_string(i);
            int additional_random_number = generateRandomNumber(400, 500); // ���������
            doc << "additional_string" << additional_string << "additional_random_number" << additional_random_number;
        }
    }
    else if (collection.name() == "data7") {
        std::string fixed_string = "another_string";
        int random_number = generateRandomNumber(100, 200); // ���������
        doc << "timestamp" << timestamp << "string_field" << fixed_string << "random_number" << random_number;
        for (int i = 0; i < 3; ++i) {
            std::string additional_string = "additional_string_" + std::to_string(i);
            int additional_random_number = generateRandomNumber(200, 300); // ���������
            doc << "additional_string" << additional_string << "additional_random_number" << additional_random_number;
        }
    }
    else if (collection.name() == "data8") {
        std::string fixed_string = "yet_another_string";
        int random_number = generateRandomNumber(200, 300); // ���������
        doc << "timestamp" << timestamp << "string_field" << fixed_string << "random_number" << random_number;
        for (int i = 0; i < 5; ++i) {
            std::string additional_string = "additional_string_" + std::to_string(i);
            int additional_random_number = generateRandomNumber(300, 400); // ���������
            doc << "additional_string" << additional_string << "additional_random_number" << additional_random_number;
        }
    }
    else if (collection.name() == "data9") {
        // ���ݸ�ʽΪʾ��������������޸�
        std::string fixed_string = "yet_another_string";
        int random_number = generateRandomNumber(200, 300); // ���������
        doc << "timestamp" << timestamp << "string_field" << fixed_string << "random_number" << random_number;
        for (int i = 0; i < 7; ++i) {
            std::string additional_string = "additional_string_" + std::to_string(i);
            int additional_random_number = generateRandomNumber(400, 500); // ���������
            doc << "additional_string" << additional_string << "additional_random_number" << additional_random_number;
        }
    }
    collection.insert_one(doc.view());
}
int generateRandomNumber(int min, int max) {

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}