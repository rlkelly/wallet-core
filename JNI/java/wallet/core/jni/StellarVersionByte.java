// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

package wallet.core.jni;


public enum StellarVersionByte {
    ACCOUNTID ((short) 0x30),
    SEED ((short) 0xc0),
    PREAUTHTX ((short) 0xc8),
    SHA256HASH ((short) 0x118);

    private final short value;
    StellarVersionByte(short value) {
        this.value = value;
    }
    public short value() { return value; }
}
