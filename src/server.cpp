using namespace std;
#include "sjac.h"
#include "comm.h"
#include <sys/time.h>

extern cybozu::RandomGenerator rg;
const std::string prefix = "../comm/server/";
const std::string pubFile = prefix + "pub.txt";
const std::string queEncFile = prefix + "queEnc.dat";
const std::string resultFile = prefix + "result.dat";

double get_wall_time(){
    struct timeval time;
    if (gettimeofday(&time, NULL)){
        return 0;
    }
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}

int main(int argc, char **argv){
	double s_time,e_time,calc_time=0;

    ROT::SysInit();
    SJAC::Server s;

    int sock;

	int opt;
    int p = 23456;
    std::string parFile;
    std::string db_path;
    //-p port -f DBfile -k param
    while((opt = getopt(argc, argv, "p:f:k:")) != -1){
        switch(opt){
            case 'p':
                p = atoi(optarg);
                break;
            case 'f':
                db_path = optarg;
                break;
			case 'k':
                parFile = optarg;
                break;
            default:
                fprintf(stderr,"error! \'%c\' \'%c\'\n", opt, optopt);
                return -1;
		}
	}

	sock = prepSSock(p);
    sock = acceptSSock(sock);

    s.setParam(parFile);
    s.readDB<int>(db_path);
    s.hashDB();
    recvFile(sock, (char *)pubFile.c_str());
    s_time = get_wall_time();
    s.setKey(pubFile);
    s.encDBHash();
    e_time = get_wall_time();
    calc_time += e_time - s_time;
    recvFile(sock, (char *)queEncFile.c_str());
    s_time = get_wall_time();
    CipherTextVec min_hashes_query_enc;
    min_hashes_query_enc.resize(s.k);
    s.readEncQueryHash(&min_hashes_query_enc, queEncFile);
    vector<CipherTextVec> comp_results;
    s.compare_Query_DB(min_hashes_query_enc, comp_results);
    s.writeCompResult(comp_results, resultFile);
    e_time = get_wall_time();
    sendFile(sock, (char *)resultFile.c_str());
    calc_time += e_time - s_time;
    //cout << calc_time << endl;
    closeSock(sock);
    return 0;
}
