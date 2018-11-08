#pragma once
/*
*@fuc��openssl C++��װ����
*@author��yuqingtong
*/
#include <string>
#include <iostream>
#include "openssl/md5.h"
#include "openssl/bio.h"
#include "openssl/sha.h"
#include "openssl/pem.h"
#include "openssl/aes.h"
#include "openssl/rsa.h"
#include "openssl/err.h"
#include "openssl/hmac.h"
#include "openssl/des.h" 

namespace SSLWapper
{
	bool Md5MemCpp(const std::string& strBuffer, std::string& strMd5);
	bool Md5FileCpp(const std::string& filepath,std::string& strMd5);

	bool Sha1Cpp(const std::string& strBuffer, std::string& strSha1);
	bool Sha224Cpp(const std::string& strBuffer, std::string& sha224);
	bool Sha256Cpp(const std::string& strBuffer, std::string& sha256);
	bool Sha384Cpp(const std::string& strBuffer, std::string& sha384);
	bool Sha512Cpp(const std::string& strBuffer, std::string& sha512);
	bool Hmac_Sha1(const std::string& strBuffer, const std::string& SecretKey,std::string& hsha1);

	bool Base64EncodeCpp(const std::string& strBuffer, std::string& strBase64, bool linebreaks = false);
	bool Base64DecodeCpp(const std::string& strBase64, std::string& strBuffer, bool linebreaks = false);

	bool EncodeAESCpp(const std::string& strKey, const std::string& strPlaintext, std::string& strCiphertext);
	bool DecodeAESCpp(const std::string& strKey, const std::string& strPlaintext, std::string& strCiphertext);

	//�������AES key bits ��128, 192��256
	std::string AESGenerateKey(int bits);


	//3des-ecb���ܷ�ʽ
	bool Encode3DESCpp(const std::string& strKey, const std::string& strPlaintext, std::string& strCiphertext);
	bool Decode3DESCpp(const std::string& strKey, const std::string& strPlaintext, std::string& strCiphertext);

	//RSA�㷨
	//���ɹ�˽Կ
	bool RSAGenerateCpp(std::string& strPublic, std::string& strPrivte);
	bool RSAGenerateCpp(RSA** rPublic, RSA** rPrivte);

	//�������Կ���м���
	bool EncodeRSAPrivateCpp(RSA* rsaPrivate, const std::string& strPlaintext, std::string& strCiphertext);
	bool DecodeRSAPublicCpp(RSA* rsaPublic, const std::string& strPlaintext, std::string& strCiphertext);
	bool EncodeRSAPublicCpp(RSA* rsaPublic, const std::string& strPlaintext, std::string& strCiphertext);
	bool DecodeRSAPrivateCpp(RSA* rsaPrivate, const std::string& strPlaintext, std::string& strCiphertext);

	//���ڴ�����ȡ��Կ
	RSA* GetRSAPublicMem(std::string& strmem);
	RSA* GetRSAPublicFile(std::string& strFilePath);
	RSA* GetRSAPrivateMem(std::string& strmem);
	RSA* GetRSAPriveteFile(std::string& strFilePath);
}


