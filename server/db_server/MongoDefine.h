//
// Created by yqt on 18-11-8.
//

#ifndef ALL_MONGODEFINE_H
#define ALL_MONGODEFINE_H

enum MongoCode {
    E_SUCCESS = 0,
    E_CONNECTION_FAILS,
    E_INVALID_ARGS,
    E_INVALID_NAMESPACE,
    E_AUTH_FAILS,
    E_INVALID_RESULT,
    E_DB_EXCEPTION,
    E_EXIST,
    E_TIMEOUT
};

enum DBOpertionCode
{
    OPERTION_UPDATE = 0,
    OPERTION_INSERT,
    OPERTION_REMOVE,
    OPERTION_REMOVEALL
};

enum ReadMode
{
    k_primary = 0,
    k_primary_preferred,
    k_secondary,
    k_secondary_preferred,
    k_nearest
};


#define DB_MAX_QUEUE 1024
#endif //ALL_MONGODEFINE_H
