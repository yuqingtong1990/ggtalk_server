
check_user() {
    if [ $(id -u) != "0" ]; then
        echo "Error: You must be root to run this script, please use root to install im"
        exit 1
    fi
}

check_user

apt install uuid-dev

#centos
#sudo yum install e2fsprogs-devel
#sudo yum install uuid-devel
#sudo yum install libuuid-devel
sudo apt-get install aptitude
sudo aptitude install libtool

apt install libmysqlclient-dev

#install mysql-client

cd /tmp
#install protobuf 3.6.1
apt install autoconf
apt install automake
wget https://github.com/protocolbuffers/protobuf/releases/download/v3.6.1/protobuf-cpp-3.6.1.tar.gz
tar -zxvf protobuf-3.6.1.tar.gz
cd protobuf-3.6.1/
./autogen.sh
mkdir build
cd build
cmake ../cmake
make&&make install

#install libevent 2.1.8
cd /tmp
wget https://github.com/libevent/libevent/releases/download/release-2.1.8-stable/libevent-2.1.8-stable.tar.gz
tar -zxvf libevent-2.1.8-stable.tar.gz
cd libevent-2.1.8-stable
./configure --prefix=/usr
make&&make install


#install jsoncpp 1.8.4
cd /tmp
wget https://github.com/open-source-parsers/jsoncpp/archive/1.8.4.tar.gz
tar -zxvf 1.8.4.tar.gz 
cd jsoncpp-1.8.4/
mkdir -p build/debug
cd build/debug
cmake -DCMAKE_BUILD_TYPE=debug -DBUILD_STATIC_LIBS=ON -DBUILD_SHARED_LIBS=OFF -DARCHIVE_INSTALL_DIR=. -G "Unix Makefiles" ../..
make&&make install

#install glog
cd /tmp
wget https://github.com/google/glog/archive/v0.3.5.tar.gz
tar -zxvf v0.3.5.tar.gz
cd glog-0.3.5
./configure --prefix=/usr
make&&make install

#install openssl 1.0.2p
cd /tmp
wget https://github.com/openssl/openssl/archive/OpenSSL_1_0_2p.tar.gz
tar -zxvf OpenSSL_1_0_2p.tar.gz
cd openssl-OpenSSL_1_0_2p/
./config --prefix=/usr/local/openssl
./config -t
make&&make install
cp /usr/local/openssl/include/openssl /usr/local/include/
cp /usr/local/openssl/lib/libcrypto.a  /usr/local/lib
cp /usr/local/openssl/lib/libssl.a /usr/local/lib

#install hredis
cd /tmp
wget https://github.com/redis/hiredis/archive/v0.14.0.tar.gz
tar -zxvf v0.14.0.tar.gz 
cd hiredis-0.14.0/
make&&make install


#in code install evpp 0.7.0
#cd /tmp
#wget https://github.com/Qihoo360/evpp/archive/v0.7.0.tar.gz
#tar -zxvf v0.7.0.tar.gz
#cd evpp-0.7.0/

#mongodb suppot
#first install MongoDB C Driver 
cd /tmp
wget https://github.com/mongodb/mongo-c-driver/archive/1.13.0.tar.gz
tar -zxvf 1.13.0.tar.gz
cd mongo-c-driver-1.13.0
mkdir cmake_build && cd cmake_build
cmake -DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF ..
make&&make install

#then install MongoDB Cxx Driver 
cd /tmp
wget https://github.com/mongodb/mongo-cxx-driver/archive/r3.4.0.tar.gz
tar -zxvf r3.4.0.tar.gz
cd mongo-cxx-driver-r3.4.0/build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local ..
sudo make EP_mnmlstc_core
make&&make install
