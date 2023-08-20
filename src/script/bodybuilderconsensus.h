// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2020 The Bitcoin Core developers
// Copyright (c) 2023 The Bodybuilder Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BODYBUILDER_SCRIPT_BODYBUILDERCONSENSUS_H
#define BODYBUILDER_SCRIPT_BODYBUILDERCONSENSUS_H

#include <stdint.h>

#if defined(BUILD_BODYBUILDER_INTERNAL) && defined(HAVE_CONFIG_H)
#include <config/bodybuilder-config.h>
  #if defined(_WIN32)
    #if defined(HAVE_DLLEXPORT_ATTRIBUTE)
      #define EXPORT_SYMBOL __declspec(dllexport)
    #else
      #define EXPORT_SYMBOL
    #endif
  #elif defined(HAVE_DEFAULT_VISIBILITY_ATTRIBUTE)
    #define EXPORT_SYMBOL __attribute__ ((visibility ("default")))
  #endif
#elif defined(MSC_VER) && !defined(STATIC_LIBBODYBUILDERCONSENSUS)
  #define EXPORT_SYMBOL __declspec(dllimport)
#endif

#ifndef EXPORT_SYMBOL
  #define EXPORT_SYMBOL
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define BODYBUILDERCONSENSUS_API_VER 1

typedef enum bodybuilderconsensus_error_t
{
    bodybuilderconsensus_ERR_OK = 0,
    bodybuilderconsensus_ERR_TX_INDEX,
    bodybuilderconsensus_ERR_TX_SIZE_MISMATCH,
    bodybuilderconsensus_ERR_TX_DESERIALIZE,
    bodybuilderconsensus_ERR_AMOUNT_REQUIRED,
    bodybuilderconsensus_ERR_INVALID_FLAGS,
} bodybuilderconsensus_error;

/** Script verification flags */
enum
{
    bodybuilderconsensus_SCRIPT_FLAGS_VERIFY_NONE                = 0,
    bodybuilderconsensus_SCRIPT_FLAGS_VERIFY_P2SH                = (1U << 0), // evaluate P2SH (BIP16) subscripts
    bodybuilderconsensus_SCRIPT_FLAGS_VERIFY_DERSIG              = (1U << 2), // enforce strict DER (BIP66) compliance
    bodybuilderconsensus_SCRIPT_FLAGS_VERIFY_NULLDUMMY           = (1U << 4), // enforce NULLDUMMY (BIP147)
    bodybuilderconsensus_SCRIPT_FLAGS_VERIFY_CHECKLOCKTIMEVERIFY = (1U << 9), // enable CHECKLOCKTIMEVERIFY (BIP65)
    bodybuilderconsensus_SCRIPT_FLAGS_VERIFY_CHECKSEQUENCEVERIFY = (1U << 10), // enable CHECKSEQUENCEVERIFY (BIP112)
    bodybuilderconsensus_SCRIPT_FLAGS_VERIFY_WITNESS             = (1U << 11), // enable WITNESS (BIP141)
    bodybuilderconsensus_SCRIPT_FLAGS_VERIFY_ALL                 = bodybuilderconsensus_SCRIPT_FLAGS_VERIFY_P2SH | bodybuilderconsensus_SCRIPT_FLAGS_VERIFY_DERSIG |
                                                               bodybuilderconsensus_SCRIPT_FLAGS_VERIFY_NULLDUMMY | bodybuilderconsensus_SCRIPT_FLAGS_VERIFY_CHECKLOCKTIMEVERIFY |
                                                               bodybuilderconsensus_SCRIPT_FLAGS_VERIFY_CHECKSEQUENCEVERIFY | bodybuilderconsensus_SCRIPT_FLAGS_VERIFY_WITNESS
};

/// Returns 1 if the input nIn of the serialized transaction pointed to by
/// txTo correctly spends the scriptPubKey pointed to by scriptPubKey under
/// the additional constraints specified by flags.
/// If not nullptr, err will contain an error/success code for the operation
EXPORT_SYMBOL int bodybuilderconsensus_verify_script(const unsigned char *scriptPubKey, unsigned int scriptPubKeyLen,
                                                 const unsigned char *txTo        , unsigned int txToLen,
                                                 unsigned int nIn, unsigned int flags, bodybuilderconsensus_error* err);

EXPORT_SYMBOL int bodybuilderconsensus_verify_script_with_amount(const unsigned char *scriptPubKey, unsigned int scriptPubKeyLen, int64_t amount,
                                    const unsigned char *txTo        , unsigned int txToLen,
                                    unsigned int nIn, unsigned int flags, bodybuilderconsensus_error* err);

EXPORT_SYMBOL unsigned int bodybuilderconsensus_version();

#ifdef __cplusplus
} // extern "C"
#endif

#undef EXPORT_SYMBOL

#endif // BODYBUILDER_SCRIPT_BODYBUILDERCONSENSUS_H
