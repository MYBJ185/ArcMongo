#include "Functions.h"
#define URL "mongodb://useradmin:Xg137839mg@110.41.142.124:27017"


int main() {
    {
        Instrumentor::Get().BeginSession("Read_Profile");
        // ���ÿ���̨����ΪUTF-8
        SetConsoleOutputCP(65001);

        // ���� mongocxx ʵ��
        mongocxx::instance inst{}; // ��Ӧ��ֻ��һ�Ρ�

        try {
            mongocxx::uri uri(URL);
            mongocxx::client conn(uri);

            // Access database
            mongocxx::database db = conn["data21081102"];

            // ����һϵ�еļ���
            std::vector<mongocxx::collection> collections;
            for (int i = 0; i < 10; ++i) {
                std::string collection_name = "data" + std::to_string(i);
                collections.push_back(db[collection_name]);
            }


            // �����߳���ִ�в������ݵĲ���
            int read_loop_count = 0;

            while (true) {
                PROFILE_SCOPE("Read Data Thread Loop");
                int curData = 0; 
                for (auto collection : collections) { // ʹ�� const auto& ������
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
