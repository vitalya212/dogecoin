// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2013 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_CHAIN_PARAMS_H
#define BITCOIN_CHAIN_PARAMS_H

#include "core.h"
#include "protocol.h"
#include "uint256.h"

#include <vector>

typedef unsigned char MessageStartChars[MESSAGE_START_SIZE];

struct CDNSSeedData {
    std::string name, host;
    CDNSSeedData(const std::string &strName, const std::string &strHost) : name(strName), host(strHost) {}
};

/**
 * CChainParams defines various tweakable parameters of a given instance of the
 * Bitcoin system. There are three: the main network on which people trade goods
 * and services, the public test network which gets reset from time to time and
 * a regression test mode which is intended for private networks only. It has
 * minimal difficulty to ensure that blocks can be found instantly.
 */
class CChainParams
{
public:
    enum Network {
        MAIN,
        TESTNET,
        REGTEST,

        MAX_NETWORK_TYPES
    };

    enum Base58Type {
        PUBKEY_ADDRESS,
        SCRIPT_ADDRESS,
        SECRET_KEY,
        EXT_PUBLIC_KEY,
        EXT_SECRET_KEY,

        MAX_BASE58_TYPES
    };

    const uint256& HashGenesisBlock() const { return hashGenesisBlock; }
    const MessageStartChars& MessageStart() const { return pchMessageStart; }
    const std::vector<unsigned char>& AlertKey() const { return vAlertPubKey; }
    int GetDefaultPort() const { return nDefaultPort; }
    const uint256& ProofOfWorkLimit() const { return bnProofOfWorkLimit; }
    int SubsidyHalvingInterval() const { return nSubsidyHalvingInterval; }

    /* Used to check majorities for block version upgrade */
    int EnforceBlockUpgradeMajority() const { return nEnforceBlockUpgradeMajority; }
    int RejectBlockOutdatedMajority() const { return nRejectBlockOutdatedMajority; }
    int ToCheckBlockUpgradeMajority() const { return nToCheckBlockUpgradeMajority; }

    /* Used if GenerateBitcoins is called with a negative number of threads */
    int DefaultMinerThreads() const { return nMinerThreads; }

    const CBlock& GenesisBlock() const { return genesis; }

    bool RequireRPCPassword() const { return fRequireRPCPassword; }
    /* Make miner wait to have peers to avoid wasting work */
    bool MiningRequiresPeers() const { return fMiningRequiresPeers; }
    /* Default value for -checkmempool argument */
    bool DefaultCheckMemPool() const { return fDefaultCheckMemPool; }
    /* Allow mining of a min-difficulty block */
    bool AllowMinDifficultyBlocks() const { return fAllowMinDifficultyBlocks; }
    /* Make standard checks */
    bool RequireStandard() const { return fRequireStandard; }
    /* RPC network identity, to be depreciated */
    bool RPCisTestNet() const { return fRPCisTestNet; }

    const std::string& DataDir() const { return strDataDir; }

    int64_t TargetTimespan() const { return nTargetTimespan; }
    int64_t TargetSpacing() const { return nTargetSpacing; }
    int64_t Interval() const { return nTargetTimespan / nTargetSpacing; }

    /* Make miner stop after a block is found. In RPC, don't return
     * until nGenProcLimit blocks are generated */
    bool MineBlocksOnDemand() const { return fMineBlocksOnDemand; }

    Network NetworkID() const { return networkID; }

    const std::vector<CDNSSeedData>& DNSSeeds() const { return vSeeds; }

    const std::vector<unsigned char>& Base58Prefix(Base58Type type) const { return base58Prefixes[type]; }
    const std::vector<CAddress>& FixedSeeds() const { return vFixedSeeds; }
    int RPCPort() const { return nRPCPort; }

    // Dogecoin specific properties
    bool SimplifiedRewards() const { return fSimplifiedRewards; }

    // AUXPOW
    /* The block number from where AuxPow starts */
    int GetAuxPowStartBlock() const { return nAuxPowStartBlock; }
    /* whether we allow ourself to be the auxpow parent chain */
    bool AllowSelfAuxParent() const { return fAllowSelfAuxParent; }

    // DIGISHIELD
    /* The block number where digishield starts */
    int GetDigiShieldForkBlock() const { return nDigiShieldForkBlock; }
    /* The TargetTimespan with DigiShield */
    int64_t DigiShieldTargetTimespan() const { return nDigiShieldTargetTimespan; }
    /* Retarget interval with DigiShield */
    int64_t DigiShieldInterval() const { return nDigiShieldTargetTimespan / nTargetSpacing; }

    // TESTNET FORK: Allow post-digishield min difficulty at 157500
    /* The minimum difficulty at which we allow post-DigiShield minimum difficulty blocks */
    int GetMinDifficultyAllowedStartBlock() const { return nMinDifficultyAllowedStartBlock; }

protected:
    CChainParams() {}

    uint256 hashGenesisBlock;
    MessageStartChars pchMessageStart;
    // Raw pub key bytes for the broadcast alert signing key.
    std::vector<unsigned char> vAlertPubKey;
    int nDefaultPort;
    int nRPCPort;
    uint256 bnProofOfWorkLimit;
    int nSubsidyHalvingInterval;
    int nEnforceBlockUpgradeMajority;
    int nRejectBlockOutdatedMajority;
    int nToCheckBlockUpgradeMajority;
    std::string strDataDir;
    int64_t nTargetTimespan;
    int64_t nTargetSpacing;
    int nMinerThreads;
    std::vector<CDNSSeedData> vSeeds;
    std::vector<unsigned char> base58Prefixes[MAX_BASE58_TYPES];
    Network networkID;
    CBlock genesis;
    std::vector<CAddress> vFixedSeeds;
    bool fRequireRPCPassword;
    bool fMiningRequiresPeers;
    bool fDefaultCheckMemPool;
    bool fAllowMinDifficultyBlocks;
    bool fRequireStandard;
    bool fRPCisTestNet;
    bool fMineBlocksOnDemand;

    // Dogecoin specific properties
    bool fSimplifiedRewards;

    int nAuxPowStartBlock;
    bool fAllowSelfAuxParent;

    int nDigiShieldForkBlock;
    int64_t nDigiShieldTargetTimespan;

    int nMinDifficultyAllowedStartBlock;

};

/**
 * Return the currently selected parameters. This won't change after app startup
 * outside of the unit tests.
 */
const CChainParams &Params();

/** Sets the params returned by Params() to those for the given network. */
void SelectParams(CChainParams::Network network);

/**
 * Looks for -regtest or -testnet and then calls SelectParams as appropriate.
 * Returns false if an invalid combination is given.
 */
bool SelectParamsFromCommandLine();

#endif
