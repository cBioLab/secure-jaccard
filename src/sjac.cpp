#include "sjac.h"
#include  "comfunc_sjac.h"
extern cybozu::RandomGenerator rg;

void SJAC::compare(CipherTextVec vector1_enc, CipherTextVec vector2_enc, CipherTextVec &results){
    int cnt = 0;
    int n = vector1_enc.size();
    results.resize(n);
    for(int i=0; i<n; i++){ 
        Elgamal::CipherText tmp = vector1_enc[i];
        tmp.neg();
        tmp.add(vector2_enc[i]);
        results[i] = tmp;
    }
    shuffle(results, rg);
}

void SJAC::Server::compare_Query_DB(CipherTextVec vector1_enc, std::vector<CipherTextVec> &comp_results){
    comp_results.resize(num_datasets);
    for(int i=0; i<num_datasets; i++){
        compare(vector1_enc, min_hashes_enc[i], comp_results[i]);
    }
} 

void SJAC::Server::setParam(std::string param_file){
	std::ifstream ifs(param_file.c_str(), std::ios::binary);
    ifs>>k;
    keys.resize(k);
    for(int i=0; i<k; i++){
        uint64_t tmp;
        ifs>>tmp;
        keys[i].first=tmp;
        ifs>>tmp;
        keys[i].second=tmp;
    }
    std::cerr << "server: read Param" << " k=" << k << ", keys" << std::endl;
}

void SJAC::Server::setKey(std::string pubkf){
    //copied from swm.cpp
	std::cerr<<pubkf<<"\n";
	std::cerr<<"read key\n";
	readPubkey(pubkf);
}

void SJAC::Server::hashDB(){
    min_hashes.resize(num_datasets);
    for(int i = 0; i<num_datasets; i++){
        min_hashes[i] = get_min_hashes<int>(database[i], keys);
    }
}
void SJAC::Server::encDBHash(){
    min_hashes_enc.resize(num_datasets);
    for(int j = 0; j < num_datasets; j++){
        min_hashes_enc[j].resize(k); 
        for(int i = 0; i < k; i++){
            pub.enc(min_hashes_enc[j][i], min_hashes[j][i], rg);
        }
    }
}

void SJAC::Server::readEncQueryHash(CipherTextVec *min_hashes_query_enc, std::string queEncFile){
	std::ifstream ifs(queEncFile.c_str(), std::ios::binary);
    for(int i = 0; i < k; i++){
        ifs >> (*min_hashes_query_enc)[i];
    }
}

void SJAC::Server::writeCompResult(std::vector<CipherTextVec> results, std::string resultFile){
	std::ofstream ofs(resultFile.c_str(), std::ios::binary);
    ofs << num_datasets << "\n";
    for(int j = 0; j < num_datasets; j++){
        for(int i = 0; i < k; i++){
            ofs << results[j][i] << " ";
        }
        ofs << "\n";
    }
}

void SJAC::Client::setParam(std::string param_file){
	std::ifstream ifs(param_file.c_str(), std::ios::binary);
    ifs>>k;
    keys.resize(k);
    for(int i=0; i<k; i++){
        uint64_t tmp;
        ifs>>tmp;
        keys[i].first=tmp;
        ifs>>tmp;
        keys[i].second=tmp;
    }
    std::cerr << "client: read Param" << " k=" << k << ", keys" << std::endl;
}
void SJAC::Client::setKey(std::string pubkf, std::string prvkf){
    //copied from rot.cpp
	const mcl::EcParam& para = mcl::ecparam::secp192k1;
	const Fp x0(para.gx);
	const Fp y0(para.gy);
	const Ec P(x0, y0);
	const size_t bitLen = para.bitSize;

	Elgamal::PrivateKey prvt;
	prvt.init(P, bitLen, rg);
	const Elgamal::PublicKey& pubt = prvt.getPublicKey();

	fprintf(stderr,"make privateKey=%s, publicKey=%s\n", prvkf.c_str(), pubkf.c_str());
	ROT::Save(prvkf, prvt);
	ROT::Save(pubkf, pubt);

	ROT::Load(pub, pubkf);
	ROT::Load(prv, prvkf);    
    prv.setCache(0, 1);
}

void SJAC::Client::hashQuery(){
    min_hashes = get_min_hashes<int>(query, keys);
}
void SJAC::Client::encQueryHash(){
    min_hashes_enc.resize(k);
    for(int i = 0; i < k; i++){
        pub.enc(min_hashes_enc[i], min_hashes[i], rg);
    }
}

void SJAC::Client::writeEncQueryHash(std::string queEncFile){
	std::ofstream ofs(queEncFile.c_str(), std::ios::binary);
    for(int i = 0; i < k; i++){
        ofs << min_hashes_enc[i] << "\n";
    }
}

void SJAC::Client::readCompResult(std::vector<CipherTextVec> &results, std::string resultFile){
	std::ifstream ifs(resultFile.c_str(), std::ios::binary);
    int num_datasets;
    ifs >> num_datasets;
    results.resize(num_datasets);
    for(int j = 0; j < num_datasets; j++){
        results[j].resize(k);
        for(int i = 0; i < k; i++){
            ifs >> results[j][i];
        }
    }
}

int SJAC::Client::count(CipherTextVec &results){
    int n = results.size();
    int cnt = 0;
    for(int i=0; i<n; i++){
        bool xc;
        int res;
        res = prv.dec(results[i], &xc);
        if(xc){
            if(res == 0) cnt++;
        }
    }
    return cnt;
}
