#include "sslwrapper.h"
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
// #pragma comment(lib,"libeay32.lib")
// #pragma comment(lib,"ssleay32.lib")

#define RSA_KEY_LENGTH 2048

using namespace std;

namespace SSLWapper
{
	bool Md5MemCpp(const std::string& strBuffer, std::string& strMd5)
	{
		MD5_CTX ctx;
		unsigned char md[16];
		MD5_Init(&ctx);
		MD5_Update(&ctx, strBuffer.data(), strBuffer.length());
		MD5_Final(md, &ctx);
		char buf[33] = { '\0' };
		char tmp[3] = { '\0' };
		for (int i = 0; i < 16; i++)
		{
			snprintf(tmp, 3, "%02x", md[i]);
			strcat(buf, tmp);
		}
		strMd5.assign(buf, 33);
		return true;
	}

	bool Md5FileCpp(const std::string& filepath, std::string& strMd5)
	{

		FILE *pFile = fopen(filepath.c_str(), "rb");
		if (pFile == NULL)
		{
			return false;
		}

		MD5_CTX ctx;
		unsigned char buffer[1024] = { 0 };
		int len = 0;
		unsigned char md[16];
		MD5_Init(&ctx);
		while ((len = fread(buffer, 1, 1024, pFile)) > 0)
		{
			MD5_Update(&ctx, buffer, len);
		}
		MD5_Final(md, &ctx);
		char buf[33] = { '\0' };
		char tmp[3] = { '\0' };
		for (int i = 0; i < 16; i++)
		{
			snprintf(tmp, 3, "%02x", md[i]);
			strcat(buf, tmp);
		}
		strMd5.assign(buf, 33);
		return true;
	}

	bool Sha1Cpp(const std::string& strBuffer, std::string& strSha1)
	{
		SHA_CTX c;
		unsigned char md[SHA_DIGEST_LENGTH] = { 0 };
		if (!SHA1_Init(&c))
		{
			return false;
		}
		SHA1_Update(&c, strBuffer.c_str(), strBuffer.size());
		SHA1_Final(md, &c);
		OPENSSL_cleanse(&c, sizeof(c));

		char tmp[3] = { '\0' };
		for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
		{
			snprintf(tmp, 3, "%02X", md[i]);
			strSha1 += tmp;
		}
		return true;
	}

	bool Sha224Cpp(const std::string& strBuffer, std::string& sha224)
	{
		SHA256_CTX c;
		unsigned char md[SHA224_DIGEST_LENGTH];
		SHA224((unsigned char *)strBuffer.c_str(), strBuffer.size(), md);

		SHA224_Init(&c);
		SHA224_Update(&c, strBuffer.c_str(), strBuffer.size());
		SHA224_Final(md, &c);
		OPENSSL_cleanse(&c, sizeof(c));
		char tmp[3] = { '\0' };
		for (int i = 0; i < SHA224_DIGEST_LENGTH; i++)
		{
			snprintf(tmp, 3, "%02X", md[i]);
			sha224 += tmp;
		}
		return true;
	}

	bool Sha256Cpp(const std::string& strBuffer, std::string& sha256)
	{
		SHA256_CTX c;
		unsigned char md[SHA256_DIGEST_LENGTH];
		SHA256((unsigned char*)strBuffer.c_str(), strBuffer.size(), md);

		SHA256_Init(&c);
		SHA256_Update(&c, strBuffer.c_str(), strBuffer.size());
		SHA256_Final(md, &c);
		OPENSSL_cleanse(&c, sizeof(c));
		char tmp[3] = { '\0' };
		for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
		{
			snprintf(tmp, 3, "%02X", md[i]);
			sha256 += tmp;
		}
		return true;
	}

	bool Sha384Cpp(const std::string& strBuffer, std::string& sha384)
	{
		SHA512_CTX c;
		unsigned char md[SHA384_DIGEST_LENGTH];
		SHA384((unsigned char*)strBuffer.c_str(), strBuffer.size(), md);

		SHA384_Init(&c);
		SHA384_Update(&c, strBuffer.c_str(), strBuffer.size());
		SHA384_Final(md, &c);
		OPENSSL_cleanse(&c, sizeof(c));
		char tmp[3] = { '\0' };
		for (int i = 0; i < SHA384_DIGEST_LENGTH; i++)
		{
			snprintf(tmp, 3, "%02X", md[i]);
			sha384 += tmp;
		}
		return true;
	}

	bool Sha512Cpp(const std::string& strBuffer, std::string& sha512)
	{
		SHA512_CTX c;
		unsigned char md[SHA512_DIGEST_LENGTH];
		SHA512((unsigned char*)strBuffer.c_str(), strBuffer.size(), md);

		SHA512_Init(&c);
		SHA512_Update(&c, strBuffer.c_str(), strBuffer.size());
		SHA512_Final(md, &c);
		OPENSSL_cleanse(&c, sizeof(c));
		char tmp[3] = { '\0' };

		for (int i = 0; i < SHA512_DIGEST_LENGTH; i++)
		{
			snprintf(tmp, 3, "%02X", md[i]);
			sha512 += tmp;
		}
		return true;
	}

	bool Hmac_Sha1(const std::string& strBuffer, const std::string& SecretKey, std::string& hsha1)
	{
		unsigned int len = 20;
		unsigned char* result = (unsigned char*)malloc(sizeof(char) * len);
		HMAC_CTX* ctx = HMAC_CTX_new();
		HMAC_CTX_reset(ctx);
		HMAC_Init_ex(ctx, SecretKey.c_str(), SecretKey.size(), EVP_sha1(), NULL);
		HMAC_Update(ctx, (unsigned char*)strBuffer.c_str(), strBuffer.size());
		HMAC_Final(ctx, result, &len);
		HMAC_CTX_free(ctx);
		hsha1.assign((char*)result, len);
		free(result);
		return true;
	}

	bool Base64EncodeCpp(const std::string& strBuffer, std::string& strBase64, bool linebreaks /*= false*/)
	{
		BIO *bmem, *b64;
		b64 = BIO_new(BIO_f_base64());
		if (NULL == b64) {
			return false;
		}
		if (!linebreaks)
		{
			BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
		}

		bmem = BIO_new(BIO_s_mem());
		if (NULL == bmem)
		{
			BIO_free_all(b64);
			return false;
		}
		b64 = BIO_push(b64, bmem);
		BUF_MEM* bptr = NULL;
		BIO_write(b64, strBuffer.c_str(), strBuffer.length());
		(void)BIO_flush(b64);
		BIO_get_mem_ptr(b64, &bptr);
		strBase64.assign(bptr->data, bptr->length);
		BIO_free_all(b64);
		return true;
	}

	bool Base64DecodeCpp(const std::string& strBase64, std::string& strBuffer, bool linebreaks /*= false*/)
	{
		BIO *bio, *b64;
		BUF_MEM *bptr = NULL;
		int nMaxLen = (strBase64.size() * 6 + 7) / 8;
		int nMiniLen;
		unsigned char *buf = new unsigned char[nMaxLen];
		b64 = BIO_new(BIO_f_base64());
		if (!linebreaks)
		{
			BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
		}
		bio = BIO_new_mem_buf((void*)strBase64.c_str(), strBase64.size());
		bio = BIO_push(b64, bio);
		nMiniLen = BIO_read(bio, (void*)buf, nMaxLen);
		strBuffer.assign((char*)buf, nMiniLen);
		delete[] buf;
		BIO_free_all(bio);
		return true;
	}

	bool EncodeAESCpp(const std::string& strKey, const std::string& strPlaintext, std::string& strCiphertext)
	{
		AES_KEY aes_key;
		if (AES_set_encrypt_key((const unsigned char*)strKey.c_str(), AES_BLOCK_SIZE * 8, &aes_key) < 0)
		{
			return false;
		}
		std::string data_bak = strPlaintext;
		unsigned int data_length = data_bak.length();
		int padding = 0;
		if (data_bak.length() % AES_BLOCK_SIZE > 0)
		{
			padding = AES_BLOCK_SIZE - data_bak.length() % AES_BLOCK_SIZE;
		}
		data_length += padding;
		while (padding > 0)
		{
			data_bak += '\0';
			padding--;
		}
		for (unsigned int i = 0; i < data_length / AES_BLOCK_SIZE; i++)
		{
			std::string str16 = data_bak.substr(i*AES_BLOCK_SIZE, AES_BLOCK_SIZE);
			unsigned char out[AES_BLOCK_SIZE];
			::memset(out, 0, AES_BLOCK_SIZE);
			AES_encrypt((const unsigned char*)str16.c_str(), out, &aes_key);
			strCiphertext += std::string((const char*)out, AES_BLOCK_SIZE);
		}
		return true;
	}

	bool DecodeAESCpp(const std::string& strKey, const std::string& strPlaintext, std::string& strCiphertext)
	{
		AES_KEY aes_key;
		if (AES_set_decrypt_key((const unsigned char*)strKey.c_str(), AES_BLOCK_SIZE * 8, &aes_key) < 0)
		{
			return false;
		}
		for (unsigned int i = 0; i < strPlaintext.length() / AES_BLOCK_SIZE; i++)
		{
			std::string str16 = strPlaintext.substr(i*AES_BLOCK_SIZE, AES_BLOCK_SIZE);
			unsigned char out[AES_BLOCK_SIZE];
			::memset(out, 0, AES_BLOCK_SIZE);
			AES_decrypt((const unsigned char*)str16.c_str(), out, &aes_key);
			strCiphertext += std::string((const char*)out, AES_BLOCK_SIZE);
		}
		return true;
	}

	std::string AESGenerateKey(int bits)
	{
		if (bits != 128 && bits != 192 && bits != 256)
			return "";

		int len = bits / 8;
		char* pkey = new (std::nothrow)char[len+1];
		memset(pkey,0,len+1);

		for (int i = 0; i<len; i++)
		{
			srand(time(0) + rand());
			pkey[i] = rand()%128;
		}
		std::string strRet(pkey);
		delete[] pkey;
		return strRet;
	}


#define LEN_OF_KEY_DES 24

	bool Encode3DESCpp(const std::string& strKey, const std::string& strPlaintext, std::string& strCiphertext)
	{
		bool bSuccess = false;
		do
		{
			if (strKey.size() > LEN_OF_KEY_DES)
				break;

			unsigned char key[LEN_OF_KEY_DES] = { 0 };
			memcpy(key,strKey.c_str(), strKey.size());

			unsigned char block_key[9] = { 0 };
			DES_key_schedule ks1, ks2, ks3;
			memset(block_key, 0, sizeof(block_key));
			memcpy(block_key, key + 0, 8);
			DES_set_key_unchecked((const_DES_cblock*)block_key, &ks1);
			memcpy(block_key, key + 8, 8);
			DES_set_key_unchecked((const_DES_cblock*)block_key, &ks2);
			memcpy(block_key, key + 16, 8);
			DES_set_key_unchecked((const_DES_cblock*)block_key, &ks3);

			int data_len = strPlaintext.size();
			int data_rest = data_len % 8;
			int len = data_len + (8 - data_rest);
			char ch = 8 - data_rest;

			//PKCS5
			char* src = (char*)malloc(len);
			char* dst = (char*)malloc(len);
			memset(src, 0, len);
			memcpy(src, strPlaintext.c_str(), data_len);
			memset(src + data_len, ch, 8 - data_rest);
			memset(dst, 0, len);

			for (int i = 0; i < len; i += 8)
			{
				DES_ecb3_encrypt((const_DES_cblock*)(src + i), (const_DES_cblock*)(dst + i), &ks1, &ks2, &ks3, DES_ENCRYPT);
			}
			strCiphertext.assign(dst, len);
		} while (0);
		return bSuccess;
	}

	bool Decode3DESCpp(const std::string& strKey, const std::string& strPlaintext, std::string& strCiphertext)
	{
		bool bSuccess = false;
		do
		{
			if (strKey.size() > LEN_OF_KEY_DES)
				break;

			unsigned char key[LEN_OF_KEY_DES] = { 0 };
			memcpy(key,strKey.c_str(), strKey.size());

			unsigned char block_key[9] = { 0 };
			DES_key_schedule ks1, ks2, ks3;
			memset(block_key, 0, sizeof(block_key));
			memcpy(block_key, key + 0, 8);
			DES_set_key_unchecked((const_DES_cblock*)block_key, &ks1);
			memcpy(block_key, key + 8, 8);
			DES_set_key_unchecked((const_DES_cblock*)block_key, &ks2);
			memcpy(block_key, key + 16, 8);
			DES_set_key_unchecked((const_DES_cblock*)block_key, &ks3);

			//密文的大小必须是8的倍树
			int len = strPlaintext.size();
			if (len % 8 != 0)
				break;

			char* src = (char*)malloc(len);
			char* dst = (char*)malloc(len);
			memset(src, 0, len);
			memset(dst, 0, len);
			memcpy(src, strPlaintext.c_str(), strPlaintext.size());

			for (int i = 0; i < len; i += 8)
			{

				DES_ecb3_encrypt((const_DES_cblock *)(src + i), (const_DES_cblock *)(dst + i), &ks1, &ks2, &ks3, DES_DECRYPT);
			}
			//PKCS5:获取最后一个字节来处理多余的字符
			char ch1 = *(dst + len - 1);
			strCiphertext.assign(dst, len - ch1);
		} while (0);
		return bSuccess;
	}

	bool RSAGenerateCpp(std::string& strPublic, std::string& strPrivte)
	{
		RSA *r = RSA_new();
		BIGNUM* e = BN_new();
		BN_set_word(e, 65537);
		int bits = RSA_KEY_LENGTH;
		RSA_generate_key_ex(r,bits,e, nullptr);
		unsigned char* pPublic = NULL;
		unsigned char* pPrivate = NULL;
		int n = i2d_RSAPublicKey(r, &pPublic);
		int m = i2d_RSAPrivateKey(r, &pPrivate);
		string str1((char*)pPublic, n);
		string str2((char*)pPrivate, m);
		Base64EncodeCpp(str1, strPublic);
		Base64EncodeCpp(str2, strPrivte);
		return true;
	}

	bool RSAGenerateCpp(RSA** rPublic, RSA** rPrivte)
	{
		RSA *r = RSA_new();
		BIGNUM* e = BN_new();
		BN_set_word(e, 65537);
		int bits = RSA_KEY_LENGTH;
		RSA_generate_key_ex(r,bits,e, nullptr);
		*rPublic = RSAPublicKey_dup(r);
		*rPrivte = RSAPrivateKey_dup(r);
		return true;
	}

	void AddKeyToString(bool bPublic, std::string& strKey)
	{
		int nPublicKeyLen = strKey.size();
		for (int i = 64; i < nPublicKeyLen; i += 64)
		{
			if (strKey[i] != '\n')
			{
				strKey.insert(i, "\n");
			}
			i++;
		}

		if (bPublic)
		{
			strKey.insert(0, "-----BEGIN PUBLIC KEY-----\n");
			strKey.append("\n-----END PUBLIC KEY-----\n");
		}
		else
		{
			strKey.insert(0, "-----BEGIN PRIVATE KEY-----\n");
			strKey.append("\n-----END PRIVATE KEY-----\n");
		}
	}

	bool EncodeRSAPrivateCpp(RSA* rsaPrivate, const std::string& strPlaintext, std::string& strCiphertext)
	{
		bool bSuccess = false;
		do
		{
			if (NULL == rsaPrivate)
				break;

			int rsa_len = RSA_size(rsaPrivate);
			if (rsa_len % 8 != 0)
				break;

			int flen = strPlaintext.size();
			int nMax = rsa_len - 11;
			int nZone = (flen%nMax == 0 ? flen / nMax : flen / nMax + 1);
			for (int i = 0; i < nZone; i++)
			{
				char* p_en = (char*)malloc(rsa_len);
				memset(p_en, 0, rsa_len);
				if (p_en == NULL)
					break;
				string strTmp = strPlaintext.substr(i*nMax, nMax);
				int nrert = RSA_private_encrypt(strTmp.size(), (const unsigned char*)strTmp.c_str(), (unsigned char*)p_en, rsaPrivate, RSA_PKCS1_PADDING);
				if (nrert < 0)
				{
					free(p_en);
					break;
				}
				strCiphertext.append(p_en, nrert);
				free(p_en);
			}
			bSuccess = true;
		} while (0);

		return bSuccess;
	}

	bool DecodeRSAPublicCpp(RSA* rsaPublic, const std::string& strPlaintext, std::string& strCiphertext)
	{
		bool bSuccess = false;
		do
		{
			if (NULL == rsaPublic)
				break;
			int rsa_len = RSA_size(rsaPublic);
			if (rsa_len % 8 != 0)
				break;
			int flen = strPlaintext.size();
			int nMax = rsa_len - 11;
			if (flen % rsa_len != 0)
				break;
			int nZone = flen / rsa_len;
			for (int i = 0; i < nZone; i++)
			{
				char* p_en = (char*)malloc(rsa_len);
				memset(p_en, 0, rsa_len);
				if (p_en == NULL)
					break;
				string strTmp = strPlaintext.substr(i*rsa_len, rsa_len);
				int nrert = RSA_public_decrypt(strTmp.size(), (const unsigned char*)strTmp.c_str(), (unsigned char*)p_en, rsaPublic, RSA_PKCS1_PADDING);
				if (nrert < 0)
				{
					free(p_en);
					break;
				}
				strCiphertext.append(p_en, nrert);
				free(p_en);
			}
			bSuccess = true;
		} while (0);
		return bSuccess;
	}

	bool EncodeRSAPublicCpp(RSA* rsaPublic, const std::string& strPlaintext, std::string& strCiphertext)
	{
		bool bSuccess = false;
		do
		{
			if (NULL == rsaPublic)
				break;

			int rsa_len = RSA_size(rsaPublic);
			if (rsa_len % 8 != 0)
				break;

			int flen = strPlaintext.size();
			int nMax = rsa_len - 11;
			int nZone = (flen%nMax == 0 ? flen / nMax : flen / nMax + 1);
			for (int i = 0; i < nZone; i++)
			{
				char* p_en = (char*)malloc(rsa_len);
				memset(p_en, 0, rsa_len);
				if (p_en == NULL)
					break;
				string strTmp = strPlaintext.substr(i*nMax, nMax);
				int nrert = RSA_public_encrypt(strTmp.size(), (const unsigned char*)strTmp.c_str(), (unsigned char*)p_en, rsaPublic, RSA_PKCS1_PADDING);
				if (nrert < 0)
				{
					free(p_en);
					break;
				}
				strCiphertext.append(p_en, nrert);
				free(p_en);
			}
			bSuccess = true;
		} while (0);

		return bSuccess;
	}

	bool DecodeRSAPrivateCpp(RSA* rsaPrivate, const std::string& strPlaintext, std::string& strCiphertext)
	{
		bool bSuccess = false;
		do
		{
			if (NULL == rsaPrivate)
				break;
			int rsa_len = RSA_size(rsaPrivate);
			if (rsa_len % 8 != 0)
				break;
			int flen = strPlaintext.size();
			int nMax = rsa_len - 11;
			if (flen % rsa_len != 0)
				break;
			int nZone = flen / rsa_len;
			for (int i = 0; i < nZone; i++)
			{
				char* p_en = (char*)malloc(rsa_len);
				memset(p_en, 0, rsa_len);
				if (p_en == NULL)
					break;
				string strTmp = strPlaintext.substr(i*rsa_len, rsa_len);
				int nrert = RSA_private_decrypt(strTmp.size(), (const unsigned char*)strTmp.c_str(), (unsigned char*)p_en, rsaPrivate, RSA_PKCS1_PADDING);
				if (nrert < 0)
				{
					free(p_en);
					break;
				}
				strCiphertext.append(p_en, nrert);
				free(p_en);
			}
			bSuccess = true;
		} while (0);
		return bSuccess;
	}

	RSA* GetRSAPublicMem(std::string& strmem)
	{
		int nPublicKeyLen = strmem.size();
		for (int i = 64; i < nPublicKeyLen; i += 64)
		{
			if (strmem[i] != '\n')
			{
				strmem.insert(i, "\n");
			}
			i++;
		}
// 		strmem.insert(0, "-----BEGIN PUBLIC KEY-----\n");
// 		strmem.append("\n-----END PUBLIC KEY-----\n");
		//1.0.1g版本修改
		strmem.insert(0, "-----BEGIN RSA PUBLIC KEY-----\n");
		strmem.append("\n-----END RSA PUBLIC KEY-----\n");
		BIO *bio = NULL;
		RSA *rsa = NULL;
		do
		{
			char *chPublicKey = const_cast<char *>(strmem.c_str());
			if ((bio = BIO_new_mem_buf(chPublicKey, -1)) == NULL)       //从字符串读取RSA公钥
				break;
			rsa = PEM_read_bio_RSAPublicKey(bio, NULL, NULL, NULL);   //从bio结构中得到rsa结构
			if (!rsa)
			{
				ERR_load_crypto_strings();
				char errBuf[512] = { 0 };
				ERR_error_string_n(ERR_get_error(), errBuf, sizeof(errBuf));
				std::cout << "load public key failed[" << errBuf << "]" << endl;
			}
		} while (0);
		if (bio != NULL)
		{
			BIO_free_all(bio);
		}
		return rsa;
	}

	RSA* GetRSAPublicFile(std::string& strFilePath)
	{
		BIO *key = NULL;
		RSA *r = NULL;
		key = BIO_new(BIO_s_file());
		BIO_read_filename(key, strFilePath.c_str());
		r = PEM_read_bio_RSAPublicKey(key, NULL, NULL, NULL);
		if (key != NULL)
		{
			BIO_free_all(key);
		}
		return r;
	}

	RSA* GetRSAPrivateMem(std::string& strmem)
	{
		int nPublicKeyLen = strmem.size();
		for (int i = 64; i < nPublicKeyLen; i += 64)
		{
			if (strmem[i] != '\n')
			{
				strmem.insert(i, "\n");
			}
			i++;
		}
		strmem.insert(0, "-----BEGIN RSA PRIVATE KEY-----\n");
		strmem.append("\n-----END RSA PRIVATE KEY-----\n");
		BIO *bio = NULL;
		RSA *rsa = NULL;
		do
		{
			char *chPublicKey = const_cast<char *>(strmem.c_str());
			if ((bio = BIO_new_mem_buf(chPublicKey, -1)) == NULL)       //从字符串读取RSA公钥
				break;
			rsa = PEM_read_bio_RSAPrivateKey(bio, NULL, NULL, NULL);   //从bio结构中得到rsa结构
			if (!rsa)
			{
				ERR_load_crypto_strings();
				char errBuf[512] = { 0 };
				ERR_error_string_n(ERR_get_error(), errBuf, sizeof(errBuf));
				std::cout << "load public key failed[" << errBuf << "]" << endl;
			}
		} while (0);
		if (bio != NULL)
		{
			BIO_free_all(bio);
		}
		return rsa;
	}

	RSA* GetRSAPriveteFile(std::string& strFilePath)
	{
		BIO *key = NULL;
		RSA *r = NULL;
		key = BIO_new(BIO_s_file());
		BIO_read_filename(key, strFilePath.c_str());
		r = PEM_read_bio_RSAPrivateKey(key, NULL, NULL, NULL);
		if (key != NULL)
		{
			BIO_free_all(key);
		}
		return r;
	}
	

	void EncryptTEA(unsigned int *firstChunk, unsigned int *secondChunk, unsigned int* key)
	{
		unsigned int y = *firstChunk;
		unsigned int z = *secondChunk;
		unsigned int sum = 0;

		unsigned int delta = 0x9e3779b9;

		for (int i = 0; i < 8; i++)//8轮运算(需要对应下面的解密核心函数的轮数一样)
		{
			sum += delta;
			y += ((z << 4) + key[0]) ^ (z + sum) ^ ((z >> 5) + key[1]);
			z += ((y << 4) + key[2]) ^ (y + sum) ^ ((y >> 5) + key[3]);
		}

		*firstChunk = y;
		*secondChunk = z;
	}

	void DecryptTEA(unsigned int *firstChunk, unsigned int *secondChunk, unsigned int* key)
	{
		unsigned int  sum = 0;
		unsigned int  y = *firstChunk;
		unsigned int  z = *secondChunk;
		unsigned int  delta = 0x9e3779b9;

		sum = delta << 3; //32轮运算，所以是2的5次方；16轮运算，所以是2的4次方；8轮运算，所以是2的3次方

		for (int i = 0; i < 8; i++) //8轮运算
		{
			z -= (y << 4) + key[2] ^ y + sum ^ (y >> 5) + key[3];
			y -= (z << 4) + key[0] ^ z + sum ^ (z >> 5) + key[1];
			sum -= delta;
		}

		*firstChunk = y;
		*secondChunk = z;
	}

	//buffer：输入的待加密数据buffer，在函数中直接对元数据buffer进行加密；size：buffer长度；key是密钥；
	void EncryptBuffer(char* buffer, int size, unsigned int* key)
	{
		char *p = buffer;

		int leftSize = size;

		while (p < buffer + size &&
			leftSize >= sizeof(unsigned int) * 2)
		{
			EncryptTEA((unsigned int *)p, (unsigned int *)(p + sizeof(unsigned int)), key);
			p += sizeof(unsigned int) * 2;

			leftSize -= sizeof(unsigned int) * 2;
		}
	}

	//buffer：输入的待解密数据buffer，在函数中直接对元数据buffer进行解密；size：buffer长度；key是密钥；
	void DecryptBuffer(char* buffer, int size, unsigned int* key)
	{
		char *p = buffer;

		int leftSize = size;

		while (p < buffer + size &&
			leftSize >= sizeof(unsigned int) * 2)
		{
			DecryptTEA((unsigned int *)p, (unsigned int *)(p + sizeof(unsigned int)), key);
			p += sizeof(unsigned int) * 2;

			leftSize -= sizeof(unsigned int) * 2;
		}
	}
}