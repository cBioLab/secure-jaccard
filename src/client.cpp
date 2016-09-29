using namespace std;
#include "sjac.h"
#include "comm.h"
#include <sys/time.h>

extern cybozu::RandomGenerator rg;
const std::string prefix = "../comm/client/";
const std::string prvFile = prefix + "prv.txt";
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
    double start = get_wall_time();
    ROT::SysInit();
    SJAC::Client c;

    int sock;

	int opt;
    int p = 23456;
    std::string query_path;
    std::string hostname;
    std::string parFile;
    //-p port -f DBfile -h hostname
    while((opt = getopt(argc, argv, "h:p:f:k:")) != -1){
        switch(opt){
            case 'h':
                hostname = optarg;
                break;
            case 'p':
                p = atoi(optarg);
                break;
            case 'f':
                query_path = optarg;
                break;
            case 'k':
                parFile = optarg;
                break;
            default:
                fprintf(stderr,"error! \'%c\' \'%c\'\n", opt, optopt);
                return -1;
		}
	}

	sock = prepCSock((char *)hostname.c_str(), p);

    c.setParam(parFile);
    c.readQuery<int>(query_path);
    c.setKey(pubFile, prvFile);
    sendFile(sock, (char *)pubFile.c_str());
    c.hashQuery();
    s_time = get_wall_time();
    c.encQueryHash();
    c.writeEncQueryHash(queEncFile);
    e_time = get_wall_time();
    calc_time += e_time - s_time;
    sendFile(sock, (char *)queEncFile.c_str());
    vector<CipherTextVec> comp_results;
    recvFile(sock, (char *)resultFile.c_str());
    s_time = get_wall_time();
    c.readCompResult(comp_results, resultFile);
    int num_datasets = comp_results.size();
    for(int i = 0; i < num_datasets; i++){
        cerr << "approximate jaccard enc " << (double)c.count(comp_results[i])/c.k << endl;
    }
    e_time = get_wall_time();
    calc_time += e_time - s_time;
    closeSock(sock);
    double end = get_wall_time();
    //cout << end - start << endl;
    //cout << calc_time << endl;
    return 0;
}
