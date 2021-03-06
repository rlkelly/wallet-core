// Copyright © 2017 Pieter Wuille
// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Bech32Address.h"

#include "../Bech32.h"
#include <TrustWalletCore/TWHRP.h>
#include <TrezorCrypto/ecdsa.h>

using namespace TW::Bitcoin;
typedef std::vector<uint8_t> Data;

bool Bech32Address::isValid(const std::string& addr) {
    auto dec = Bech32::decode(addr);
    if (dec.second.empty()) {
        return false;
    }

    Data conv;
    if (!Bech32::convertBits<5, 8, false>(conv, Data(dec.second.begin() + 1, dec.second.end())) ||
        conv.size() < 2 || conv.size() > 40 || dec.second[0] > 16 || (dec.second[0] == 0 &&
        conv.size() != 20 && conv.size() != 32)) {
        return false;
    }

    return true;
}

Bech32Address::Bech32Address(const PublicKey& publicKey, int witver, const std::string& hrp) : hrp(hrp), witnessVersion(witver), witnessProgram() {
    witnessProgram.resize(20);
    ecdsa_get_pubkeyhash(publicKey.compressed().bytes.data(), HASHER_SHA2_RIPEMD, witnessProgram.data());
}

std::pair<Bech32Address, bool> Bech32Address::decode(const std::string& addr) {
    auto dec = Bech32::decode(addr);
    if (dec.second.empty()) {
        return std::make_pair(Bech32Address(), false);
    }

    Data conv;
    if (!Bech32::convertBits<5, 8, false>(conv, Data(dec.second.begin() + 1, dec.second.end())) ||
        conv.size() < 2 || conv.size() > 40 || dec.second[0] > 16 || (dec.second[0] == 0 &&
        conv.size() != 20 && conv.size() != 32)) {
        return std::make_pair(Bech32Address(), false);
    }

    return std::make_pair(Bech32Address(dec.first, dec.second[0], conv), true);
}

std::string Bech32Address::encode() const {
    Data enc;
    enc.push_back(witnessVersion);
    Bech32::convertBits<8, 5, true>(enc, witnessProgram);
    std::string result = Bech32::encode(hrp, enc);
    if (!decode(result).second) {
        return {};
    }
    return result;
}

std::pair<Bech32Address, bool> Bech32Address::fromRaw(const std::string& hrp, const std::vector<uint8_t>& data) {
    Data conv;
    if (!Bech32::convertBits<5, 8, false>(conv, Data(data.begin() + 1, data.end())) ||
        conv.size() < 2 || conv.size() > 40 || data[0] > 16 || (data[0] == 0 &&
        conv.size() != 20 && conv.size() != 32)) {
        return std::make_pair(Bech32Address(), false);
    }

    return std::make_pair(Bech32Address(hrp, data[0], conv), true);
}
