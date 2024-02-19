#include "Functions.h"
#define URL "mongodb://useradmin:Xg137839mg@110.41.142.124:27017"


int main() {
    {
        Instrumentor::Get().BeginSession("Read_Profile");
        // 设置控制台编码为UTF-8
        SetConsoleOutputCP(65001);

        // 创建 mongocxx 实例
        mongocxx::instance inst{}; // 这应该只做一次。

        try {
            mongocxx::uri uri(URL);
            mongocxx::client conn(uri);

            // Access database
            mongocxx::database db = conn["data21081102"];

            // 创建一系列的集合
            std::vector<mongocxx::collection> collections;
            for (int i = 0; i < 10; ++i) {
                std::string collection_name = "data" + std::to_string(i);
                collections.push_back(db[collection_name]);
            }


            // 在主线程中执行插入数据的操作
            int read_loop_count = 0;

            while (true) {
                PROFILE_SCOPE("Read Data Thread Loop");
                int curData = 0; 
                for (auto collection : collections) { // 使用 const auto& 迭代器
                    PROFILE_SCOPE("Read Data");
                    bsoncxx::document::view latest_doc = getLatestDocument(collection);
                    std::cout << "Read Data"<< curData << ":DONE"<< std::endl;

                }
                std::cout << "Read Data Thread Loop Count: " << read_loop_count << std::endl;
                read_loop_count++;
            }


        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return 1;
        }
        Instrumentor::Get().EndSession();
    }

    return 0;
}
