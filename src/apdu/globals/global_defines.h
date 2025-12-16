#ifndef G_GLOBAL_DEFINES_H
#define G_GLOBAL_DEFINES_H

#define MAX_MEMO_SIZE 256
#define MAX_DATA_SIZE (MAX_MEMO_SIZE)

#define LEGACY_PURPOSE   1105
#define LEGACY_COIN_TYPE 0
#define NEW_PURPOSE      44
#define NEW_COIN_TYPE    919

#define MAX_CDATA_LENGTH 255
/**
 * Key length of (Public Key || Verification Key || Account Key)
 */
#define KEY_LENGTH 32

#define UPDATE_HEADER_LENGTH 28

#define ACCOUNT_TRANSACTION_HEADER_LENGTH 60

#endif  // G_GLOBAL_DEFINES_H