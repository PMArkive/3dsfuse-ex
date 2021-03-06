#include <algorithm>
#include <functional>
#include "aes_key.h"

AESKey Lrot128(const AESKey& in, u32 rot) {
    AESKey out(16);
    rot %= 128;
    const u32 byte_shift = rot / 8;
    const u32 bit_shift = rot % 8;

    for (u32 i = 0; i < 16; i++) {
        const u32 wrap_index_a = (i + byte_shift) % 16;
        const u32 wrap_index_b = (i + byte_shift + 1) % 16;
        out[i] = (in[wrap_index_a] << bit_shift) | (in[wrap_index_b] >> (8 - bit_shift));
    }
    return out;
}

AESKey Add128(const AESKey& a, const AESKey& b) {
    AESKey out(16);
    u32 carry = 0;
    u32 sum = 0;

    for (int i = 15; i >= 0; i--) {
        sum = (u32)a[i] + (u32)b[i] + carry;
        carry = sum >> 8;
        out[i] = byte{(u8)(sum & 0xFF)};
    }

    return out;
}

AESKey Xor128(const AESKey& a, const AESKey& b) {
    AESKey out(16);
    std::transform(a.cbegin(), a.cend(), b.cbegin(), out.begin(), std::bit_xor<>());
    return out;
}

AESKey ScrambleKey(const AESKey& x, const AESKey& y, const AESKey& c) {
    return Lrot128(Add128(Xor128(Lrot128(x, 2), y), c), 87);
}
