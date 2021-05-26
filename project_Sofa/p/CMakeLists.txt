
set(SAMPLE_NAME "crawler")


dependent libraries
find_package(Poco REQUIRED Foundation Data DataSQLite Net NetSSL Crypto CONFIG)
find_package(easyhttpcpp REQUIRED CONFIG)

add_executable(${SAMPLE_NAME} ${SAMPLE_SRCS} linkqueue.cpp repo_in_memory.cpp indexer.cpp server.cpp) 
target_link_libraries(${SAMPLE_NAME}
                      easyhttpcpp::easyhttp
                      Poco::Foundation
                      Poco::Data Poco::DataSQLite
                      Poco::Net Poco::NetSSL Poco::Crypto
                      ssl crypto
                      )