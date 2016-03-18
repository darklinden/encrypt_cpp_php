
#include "des.h"
#include "aes.h"
#include <string>

// 使用‘0’补全/截取字符串到指定长度
std::string matchPading(const std::string &src,
                        const long size)
{
    std::string des(src);

    while (des.length() < size) {
        des += "0";
    }

    if (des.length() > size) {
        des = des.substr(0, size);
    }

    return des;
}

// 使用‘0’补全字符串到指定长度倍数
std::string multiplePading(const std::string &src,
                           const long size)
{
    std::string des(src);

    auto deslen = des.length() - (des.length() % size) + size;

    while (des.length() < deslen) {
        des += "0";
    }

    return des;
}

std::string base64(unsigned char *buff, const long len)
{
    static const char base64EncodingTable[64] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
        'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
        'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
        'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
    };

    unsigned long ixtext, lentext;
    long ctremaining;
    unsigned char input[3], output[4];
    short i, charsonline = 0, ctcopy;
    const unsigned char *raw;
    std::string result;

    lentext = len;
    if (lentext < 1) {
        return "";
    }

    raw = buff;
    ixtext = 0;

    while (true) {
        ctremaining = lentext - ixtext;
        if (ctremaining <= 0) {
            break;
        }
        for (i = 0; i < 3; i++) {
            unsigned long ix = ixtext + i;
            if (ix < lentext) {
                input[i] = raw[ix];
            }
            else {
                input[i] = 0;
            }
        }
        output[0] = (input[0] & 0xFC) >> 2;
        output[1] = ((input[0] & 0x03) << 4) | ((input[1] & 0xF0) >> 4);
        output[2] = ((input[1] & 0x0F) << 2) | ((input[2] & 0xC0) >> 6);
        output[3] = input[2] & 0x3F;
        ctcopy = 4;
        switch (ctremaining) {
            case 1:
                ctcopy = 2;
                break;
            case 2:
                ctcopy = 3;
                break;
        }

        for (i = 0; i < ctcopy; i++) {
            result += base64EncodingTable[output[i]];
        }

        for (i = ctcopy; i < 4; i++) {
            result += "=";
        }

        ixtext += 3;
        charsonline += 4;

        if ((len > 0) && (charsonline >= len)) {
            charsonline = 0;
        }
    }
    return result;
}

std::string encrypt(const std::string &inputs,
                    const std::string &keys,
                    const std::string &ivs)
{
    const int size = 16;

    auto padinput = multiplePading(inputs, size);
    auto padkey = matchPading(keys, size);
    auto padiv = matchPading(ivs, size);

    printf("str: %s\n", padinput.c_str());
    printf("key: %s\n", padkey.c_str());
    printf("iv: %s\n", padiv.c_str());

    auto inputlen = padinput.length();

    unsigned char *cinput = (unsigned char *)malloc(sizeof(unsigned char) * (inputlen + 1));
    memset(cinput, 0, inputlen + 1);
    memcpy(cinput, padinput.c_str(), padinput.length());

    unsigned char* ckey = (unsigned char *)malloc(sizeof(unsigned char) * (size + 1));
    memset(ckey, 0, size + 1);
    memcpy(ckey, padkey.c_str(), padkey.length());

    unsigned char* civ = (unsigned char *)malloc(sizeof(unsigned char) * (size + 1));
    memset(civ, 0, size + 1);
    memcpy(civ, padiv.c_str(), padiv.length());

    unsigned char *coutput = (unsigned char *)malloc(sizeof(unsigned char) * (inputlen + 1));
    memset(coutput, 0, inputlen + 1);

    AES_KEY key;
    int v = AES_set_encrypt_key(ckey, 128, &key);
    printf("AES_set_encrypt_key %d\n", v);

    AES_cbc_encrypt(cinput, coutput, inputlen, &key, civ, AES_ENCRYPT);

    auto ret = base64(coutput, inputlen);

    free(cinput);
    free(coutput);
    free(ckey);
    free(civ);

    return ret;
}
