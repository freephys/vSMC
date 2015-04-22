#ifndef VSMC_EXAMPLE_RNG_AES_VALIDATION_HPP
#define VSMC_EXAMPLE_RNG_AES_VALIDATION_HPP

#include <vsmc/rng/aes.hpp>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <string>

static unsigned char plain_text[64] = {0x6B, 0xC1, 0xBE, 0xE2, 0x2E, 0x40,
    0x9F, 0x96, 0xE9, 0x3D, 0x7E, 0x11, 0x73, 0x93, 0x17, 0x2A, 0xAE, 0x2D,
    0x8A, 0x57, 0x1E, 0x03, 0xAC, 0x9C, 0x9E, 0xB7, 0x6F, 0xAC, 0x45, 0xAF,
    0x8E, 0x51, 0x30, 0xC8, 0x1C, 0x46, 0xA3, 0x5C, 0xE4, 0x11, 0xE5, 0xFB,
    0xC1, 0x19, 0x1A, 0x0A, 0x52, 0xEF, 0xF6, 0x9F, 0x24, 0x45, 0xDF, 0x4F,
    0x9B, 0x17, 0xAD, 0x2B, 0x41, 0x7B, 0xE6, 0x6C, 0x37, 0x10};

static unsigned char plain_key128[16] = {0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE,
    0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C};

static unsigned char plain_key192[24] = {0x8E, 0x73, 0xB0, 0xF7, 0xDA, 0x0E,
    0x64, 0x52, 0xC8, 0x10, 0xF3, 0x2B, 0x80, 0x90, 0x79, 0xE5, 0x62, 0xF8,
    0xEA, 0xD2, 0x52, 0x2C, 0x6B, 0x7B};

static unsigned char plain_key256[32] = {0x60, 0x3D, 0xEB, 0x10, 0x15, 0xCA,
    0x71, 0xBE, 0x2B, 0x73, 0xAE, 0xF0, 0x85, 0x7D, 0x77, 0x81, 0x1F, 0x35,
    0x2C, 0x07, 0x3B, 0x61, 0x08, 0xD7, 0x2D, 0x98, 0x10, 0xA3, 0x09, 0x14,
    0xDF, 0xF4};

static unsigned char plain_vector[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05,
    0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};

static unsigned char plain_cipher_cbc128[64] = {0x76, 0x49, 0xAB, 0xAC, 0x81,
    0x19, 0xB2, 0x46, 0xCE, 0xE9, 0x8E, 0x9B, 0x12, 0xE9, 0x19, 0x7D, 0x50,
    0x86, 0xCB, 0x9B, 0x50, 0x72, 0x19, 0xEE, 0x95, 0xDB, 0x11, 0x3A, 0x91,
    0x76, 0x78, 0xB2, 0x73, 0xBE, 0xD6, 0xB8, 0xE3, 0xC1, 0x74, 0x3B, 0x71,
    0x16, 0xE6, 0x9E, 0x22, 0x22, 0x95, 0x16, 0x3F, 0xF1, 0xCA, 0xA1, 0x68,
    0x1F, 0xAC, 0x09, 0x12, 0x0E, 0xCA, 0x30, 0x75, 0x86, 0xE1, 0xA7};

static unsigned char plain_cipher_cbc192[64] = {0x4F, 0x02, 0x1D, 0xB2, 0x43,
    0xBC, 0x63, 0x3D, 0x71, 0x78, 0x18, 0x3A, 0x9F, 0xA0, 0x71, 0xE8, 0xB4,
    0xD9, 0xAD, 0xA9, 0xAD, 0x7D, 0xED, 0xF4, 0xE5, 0xE7, 0x38, 0x76, 0x3F,
    0x69, 0x14, 0x5A, 0x57, 0x1B, 0x24, 0x20, 0x12, 0xFB, 0x7A, 0xE0, 0x7F,
    0xA9, 0xBA, 0xAC, 0x3D, 0xF1, 0x02, 0xE0, 0x08, 0xB0, 0xE2, 0x79, 0x88,
    0x59, 0x88, 0x81, 0xD9, 0x20, 0xA9, 0xE6, 0x4F, 0x56, 0x15, 0xCD};

static unsigned char plain_cipher_cbc256[64] = {0xF5, 0x8C, 0x4C, 0x04, 0xD6,
    0xE5, 0xF1, 0xBA, 0x77, 0x9E, 0xAB, 0xFB, 0x5F, 0x7B, 0xFB, 0xD6, 0x9C,
    0xFC, 0x4E, 0x96, 0x7E, 0xDB, 0x80, 0x8D, 0x67, 0x9F, 0x77, 0x7B, 0xC6,
    0x70, 0x2C, 0x7D, 0x39, 0xF2, 0x33, 0x69, 0xA9, 0xD9, 0xBA, 0xCF, 0xA5,
    0x30, 0xE2, 0x63, 0x04, 0x23, 0x14, 0x61, 0xB2, 0xEB, 0x05, 0xE2, 0xC3,
    0x9B, 0xE9, 0xFC, 0xDA, 0x6C, 0x19, 0x07, 0x8C, 0x6A, 0x9D, 0x1B};

/*****************************************************************************

static unsigned char plain_counter[16] = {
    0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7,
    0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF
};

static unsigned char plain_cipher_ctr128[64] = {
    0x87, 0x4D, 0x61, 0x91, 0xB6, 0x20, 0xE3, 0x26,
    0x1B, 0xEF, 0x68, 0x64, 0x99, 0x0D, 0xB6, 0xCE,
    0x98, 0x06, 0xF6, 0x6B, 0x79, 0x70, 0xFD, 0xFF,
    0x86, 0x17, 0x18, 0x7B, 0xB9, 0xFF, 0xFD, 0xFF,
    0x5A, 0xE4, 0xDF, 0x3E, 0xDB, 0xD5, 0xD3, 0x5E,
    0x5B, 0x4F, 0x09, 0x02, 0x0D, 0xB0, 0x3E, 0xAB,
    0x1E, 0x03, 0x1D, 0xDA, 0x2F, 0xBE, 0x03, 0xD1,
    0x79, 0x21, 0x70, 0xA0, 0xF3, 0x00, 0x9C, 0xEE
};

static unsigned char plain_cipher_ctr192[64] = {
    0x1A, 0xBC, 0x93, 0x24, 0x17, 0x52, 0x1C, 0xA2,
    0x4F, 0x2B, 0x04, 0x59, 0xFE, 0x7E, 0x6E, 0x0B,
    0x09, 0x03, 0x39, 0xEC, 0x0A, 0xA6, 0xFA, 0xEF,
    0xD5, 0xCC, 0xC2, 0xC6, 0xF4, 0xCE, 0x8E, 0x94,
    0x1E, 0x36, 0xB2, 0x6B, 0xD1, 0xEB, 0xC6, 0x70,
    0xD1, 0xBD, 0x1D, 0x66, 0x56, 0x20, 0xAB, 0xF7,
    0x4F, 0x78, 0xA7, 0xF6, 0xD2, 0x98, 0x09, 0x58,
    0x5A, 0x97, 0xDA, 0xEC, 0x58, 0xC6, 0xB0, 0x50
};

static unsigned char plain_cipher_ctr256[64] = {
    0x60, 0x1E, 0xC3, 0x13, 0x77, 0x57, 0x89, 0xA5,
    0xB7, 0xA7, 0xF5, 0x04, 0xBB, 0xF3, 0xD2, 0x28,
    0xF4, 0x43, 0xE3, 0xCA, 0x4D, 0x62, 0xB5, 0x9A,
    0xCA, 0x84, 0xE9, 0x90, 0xCA, 0xCA, 0xF5, 0xC5,
    0x2B, 0x09, 0x30, 0xDA, 0xA2, 0x3D, 0xE9, 0x4C,
    0xE8, 0x70, 0x17, 0xBA, 0x2D, 0x84, 0x98, 0x8D,
    0xDF, 0xC9, 0xC5, 0x8D, 0xB6, 0x7A, 0xAD, 0xA6,
    0x13, 0xC2, 0xDD, 0x08, 0x45, 0x79, 0x41, 0xA6
};

*****************************************************************************/

inline void rng_aes_validation(unsigned bits, const unsigned char *test_cipher,
    const unsigned char *plain_cipher)
{
    bool success = true;
    for (std::size_t i = 0; i != 64; ++i) {
        if (test_cipher[i] != plain_cipher[i]) {
            success = false;
            break;
        }
    }

    if (success) {
        std::cout << "AES" << std::dec << bits << "Engine passed" << std::endl;
        return;
    }

    std::cout << "AES" << std::dec << bits << "Engine failed" << std::endl;
    std::cout << std::string(90, '=') << std::endl;
    std::cout << "vSMC output" << std::endl;
    std::cout << std::string(90, '-') << std::endl;
    for (std::size_t i = 0; i != 64; ++i) {
        std::cout << std::uppercase << std::hex << std::setfill('0')
                  << std::right << std::setw(2)
                  << static_cast<unsigned>(test_cipher[i]) << ' ';
        if (i % 8 == 7)
            std::cout << '\n';
    }
    std::cout << std::string(90, '=') << std::endl;
    std::cout << "Reference output" << std::endl;
    std::cout << std::string(90, '-') << std::endl;
    for (std::size_t i = 0; i != 64; ++i) {
        std::cout << std::uppercase << std::hex << std::setfill('0')
                  << std::right << std::setw(2)
                  << static_cast<unsigned>(plain_cipher[i]) << ' ';
        if (i % 8 == 7)
            std::cout << '\n';
    }
    std::cout << std::string(90, '-') << std::endl;
}

inline void rng_aes_validation_xor(void *a, void *b)
{
    uint64_t *ua = static_cast<uint64_t *>(a);
    uint64_t *ub = static_cast<uint64_t *>(b);
    ua[0] ^= ub[0];
    ua[1] ^= ub[1];
}

template <template <typename, std::size_t> class Eng>
inline void rng_aes_validation_cbc(
    unsigned bits, unsigned char *plain_key, unsigned char plain_cipher[64])
{
    typedef Eng<uint32_t, 1> eng_type;

    typename eng_type::key_type key;
    std::array<typename eng_type::ctr_block_type, 4> text;
    std::array<typename eng_type::buffer_type, 5> cipher;

    std::memcpy(key.data(), plain_key, sizeof(key));
    std::memcpy(text.data(), plain_text, 64);
    std::memcpy(cipher[0].data(), plain_vector, 16);

    eng_type eng(key);
    for (std::size_t i = 0; i != 4; ++i) {
        rng_aes_validation_xor(text[i].data(), cipher[i].data());
        eng(text[i], cipher[i + 1]);
    }
    unsigned char test_cipher[64];
    std::memcpy(test_cipher, cipher[1].data(), 64);
    rng_aes_validation(bits, test_cipher, plain_cipher);
}

inline void rng_aes_validation_cbc()
{
    std::cout << std::string(90, '=') << std::endl;
    std::cout << "Validation of AESNIEngine using CBC mode" << std::endl;
    std::cout << std::string(90, '-') << std::endl;
    rng_aes_validation_cbc<vsmc::AES128Engine>(
        128, plain_key128, plain_cipher_cbc128);
    rng_aes_validation_cbc<vsmc::AES192Engine>(
        192, plain_key192, plain_cipher_cbc192);
    rng_aes_validation_cbc<vsmc::AES256Engine>(
        256, plain_key256, plain_cipher_cbc256);
    std::cout << std::string(90, '=') << std::endl;
}

#endif // VSMC_EXAMPLE_RNG_AES_VALIDATION_HPP
