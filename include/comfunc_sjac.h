#ifndef COMFUNC_SJAC_H
#define COMFUNC_SJAC_H

#include "sjac.h"
#include <cybozu/siphash.hpp>
#include <set>

namespace SJAC{
    template<class T>
    std::vector<uint64_t> get_n_hash(std::set<T> src, std::pair<uint64_t, uint64_t> key);
    template<class T>
    std::vector<uint64_t> get_min_hashes(std::set<T> src, std::vector<std::pair<uint64_t, uint64_t> >keys);
}

template<class T>
std::vector<uint64_t> SJAC::get_n_hash(std::set<T> src, std::pair<uint64_t, uint64_t> key){
    int n = src.size();
    std::vector<uint64_t> hashes(n);
    typename std::set<T>::iterator it = src.begin();
    int i = 0;
    while( it != src.end()){
        uint64_t hash =  cybozu::siphash24(&(*it), sizeof(*it), key.first, key.second);
        hashes[i] = hash;
        it++;
        i++;
    }
    return hashes;
}

template<class T>
std::vector<uint64_t> SJAC::get_min_hashes(std::set<T> src, std::vector<std::pair<uint64_t, uint64_t> >keys){
    int n = keys.size();
    int len = src.size();
    std::vector<uint64_t> min_hashes(n);
    for(int j=0; j<n; j++){
        std::vector<uint64_t> hashes(len);
        hashes = get_n_hash<T>(src, keys[j]);
        uint64_t min = *min_element(hashes.begin(), hashes.end());
        min_hashes[j] = min;
    }
	return min_hashes;
}
 
#endif
