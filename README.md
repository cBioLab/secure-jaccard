# **Secure-jaccard**

C++ implementation of approximating jaccard similarity coefficient between a server's set and a client's set based on MinHash and additively homomorphic encryption

# Installation Requirements

Create a working directory (e.g., work) and clone the following repositories.

       mkdir work
       cd work
       git clone git://github.com/cBioLab/secure-jaccard.git
       git clone git://github.com/iskana/PBWT-sec.git
       git clone git://github.com/herumi/xbyak.git
       git clone git://github.com/herumi/mcl.git
       git clone git://github.com/herumi/cybozulib.git
       git clone git://github.com/herumi/cybozulib_ext.git

* Xbyak is a prerequisite for optimizing the operations in the finite field on Intel CPUs.
* OpenSSL and libgmp-dev are available via apt-get (or other similar commands).

# Installation
      cd secure-jaccard
      cd src
      make
      
* use tcmalloc (optimal) for Linux; sudo apt-get install libgoogle-perftools-dev

# Usage
server side:

    cd secure-jaccard
    cd bin
    ./server -f "database_file" -p "port_number" -k "parameter_file"
    
client side:

    cd secure-jaccard
    cd bin
    ./client -f "query_file" -h "hostname" -p "port_number" -k "parameter_file"

# Example
server side:

    ./server -f ../data/server/test.db -p 4444 -k ../data/server/4000.par

client side:

    ./client -f ../data/client/test.que -p 4444 -k ../data/client/4000.par -h localhost

# Database file format
    1st line  : The number of datasets
    2nd line  : The length of the 1st dataset
    3rd line  : 1st dataset (space-separated integers)
    4th line  : The length of the next dataset
    5th line  : 2nd dataset (space-separated integers)
    ...

# Query file format
    1st line  : The length of the query
    2nd line  : Query (space-separated integers)


# Parameter file format
Parameter file must be shared between a client and a server. 

    1st line  : The number of keys of hash functions
    2nd line  : 1st key (64bit random number)
    ...

# Prerequisite Files and Libraries for Running Your Application
  * OpenSSL
  * GMP (libgmp-dev)

# Copyright Notice

Copyright (C) 2016, Hiroki Sudo, Masanobu Jimbo, 
All rights reserved.

# License

secure-jaccard (files in this repository) is distributed under the [BSD 3-Clause License] (http://opensource.org/licenses/BSD-3-Clause "The BSD 3-Clause License").

# Licenses of External Libraries

Licenses of external libraries are listed as follows.

* cybozulib: BSD-3-Clause
* mcl: BSD-3-Clause
* Xbyak: BSD-3-Clause
* MPIR: LGPL2
* OpenSSL: Apache License Version 1.0 + SSLeay License
* PBWT-sec: BSD 3-Clause

Software including any of those libraries is allowed to be used for commercial purposes without releasing its source code as long as the regarding copyright statements described as follows are included in the software.

* This product includes software that was developed by an OpenSSL project in order to use OpenSSL toolkit.
* This product includes cybozulib, mcl, and Xbyak.
* This product includes MPIR.

# Contact Information

* Hiroki Sudo (hsudo108@ruri.waseda.jp)
* Masanobu Jimbo (jimwase@asagi.waseda.jp)

# History

2016/June/16; initial version
