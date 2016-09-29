#ifndef SJAC_H
#define SJAC_H

#include "rot.h"
//cause multiple definition
//#include <cybozu/siphash.hpp>
#include <set>

namespace SJAC{
    void compare(CipherTextVec vector1_enc, CipherTextVec vector2_enc, CipherTextVec &results);

	class Server : public ROT::Server
	{
	public:
        std::vector<std::set<int> > database;
        std::vector<std::vector<uint64_t> > min_hashes;
        int num_datasets;
        int k;
        std::vector<CipherTextVec > min_hashes_enc;
        std::vector<std::pair<uint64_t, uint64_t> > keys;
		std::string pubk;
        void compare_Query_DB(CipherTextVec vector1_enc, std::vector<CipherTextVec> &comp_results);
		template<class T> void readDB(std::string db_path);
		void setKey(std::string pubkf);
		void setParam(std::string parf);
        void hashDB();
        void encDBHash();
        void readEncQueryHash(CipherTextVec *min_hashes_query_enc, std::string queEncFile);
        void writeCompResult(std::vector<CipherTextVec> results, std::string resultFile);
        Server(){
        }
    };

	class Client : public ROT::Client
	{

	public:
        std::set<int> query;
        std::vector<uint64_t> min_hashes;
        int k;
        CipherTextVec min_hashes_enc;
		std::vector<std::pair<uint64_t, uint64_t> > keys;
        std::string prvk;
		std::string pubk;
        template<class T> void readQuery(std::string query_path);
		void setParam(std::string param_file);
        void setKey(std::string pubkf, std::string prvkf);
        void hashQuery();
        void encQueryHash();
        void writeEncQueryHash(std::string queEncFile);
        void readCompResult(std::vector<CipherTextVec> &results, std::string resultFile);
        int count(CipherTextVec &results);
        Client(){
        }
	};
}


extern cybozu::RandomGenerator rg; 

template<class T>
void SJAC::Server::readDB(std::string db_path){
    std::ifstream ifs(db_path.c_str());
    ifs >> num_datasets;
    database.resize(num_datasets);
    for(int j=0; j<num_datasets; j++){
        int n;
        ifs >> n;
        std::vector<T> db_vector(n);
        for(int i=0; i<n; i++){
            T temp;
            ifs >> temp;
            db_vector[i] = temp;
        }
        database[j] = std::set<T>(db_vector.begin(), db_vector.end());
    }
    std::cerr << "num_datasets = " << num_datasets << std::endl;
}
template<class T>
void SJAC::Client::readQuery(std::string query_path){
    std::ifstream ifs(query_path.c_str());
    int n;
    ifs >> n;
    std::vector<T> query_vector(n);
    for(int i=0; i<n; i++){
        T temp;
        ifs >> temp;
        query_vector[i] = temp;
    }
    query = std::set<T>(query_vector.begin(), query_vector.end());
    std::cerr << "n = " << n << std::endl;
}


#endif
