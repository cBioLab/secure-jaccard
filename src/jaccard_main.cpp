using namespace std;
/*
#include <cybozu/option.hpp>
*/
#include "sjac.h"

extern cybozu::RandomGenerator rg;
const std::string pubFile = "pub.txt";
const std::string prvFile = "prv.txt";
const std::string parFile = "par.txt";
const std::string db_path = "db.dat";
const std::string query_path = "que.dat";

/*
void inner_product(Elgamal::CipherText &ans, CipherTextVec &v1, vector<int> &v2, Elgamal::PublicKey &pub){
    pub.enc(ans, 0, rg);
    if(v1.size() != v2.size()) exit(EXIT_FAILURE);
    int i = 0;
    for(vector<int>::iterator it = v2.begin(); it != v2.end(); it++, i++){
        Elgamal::CipherText tmp = v1[i];
        tmp.mul(*it);
        ans.add(tmp);
    }
}

void inner_product(CipherTextVec &ans, CipherTextVec &v, vector< vector<int> > &mat, Elgamal::PublicKey &pub){
    int m = mat.size();
    for(int i = 0; i < m; i++){
        inner_product(ans[i], v, mat[i],pub);
    }
}
*/

int main(int argc, char **argv){
    ROT::SysInit();
    SJAC::Server s;
    SJAC::Client c;

    //server
    s.setParam(parFile);
    s.readDB<int>(db_path);
    s.hashDB();
    //client
    c.readQuery<int>(query_path);
    c.setKey(prvFile, pubFile);
    //server
    s.setKey(pubFile);
    s.encDBHash();
    std::cerr << "setParam@server done" << std::endl;
    //client
    c.setParam(parFile);
    c.hashQuery();
    c.encQueryHash();
    std::cerr << "setParam@client done" << std::endl;

    //server
    std::vector<CipherTextVec> comp_results;
    s.compare_Query_DB(c.min_hashes_enc, comp_results);
    //client
    int num_datasets = comp_results.size();
    for(int i=0; i<num_datasets; i++){
        cout << "approximate jaccard enc " << (double)c.count(comp_results[i])/c.k << endl;
    }

    return 0;
}
