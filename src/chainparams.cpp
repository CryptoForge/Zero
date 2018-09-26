// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "main.h"
#include "crypto/equihash.h"

#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

#include "base58.h"

#include "chainparamsseeds.h"

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, const uint256& nNonce, const std::vector<unsigned char>& nSolution, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    // To create a genesis block for a new chain which is Overwintered:
    //   txNew.nVersion = 3
    //   txNew.fOverwintered = true
    //   txNew.nVersionGroupId = OVERWINTER_VERSION_GROUP_ID
    //   txNew.nExpiryHeight = <default value>
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 520617983 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nSolution = nSolution;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(txNew);
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = genesis.BuildMerkleTree();
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database (and is in any case of zero value).
 *
 * >>> from pyblake2 import blake2s
 * >>> 'Zcash' + blake2s(b'The Economist 2016-10-29 Known unknown: Another crypto-currency is born. BTC#436254 0000000000000000044f321997f336d2908cf8c8d6893e88dbf067e2d949487d ETH#2521903 483039a6b6bd8bd05f0584f9a078d075e454925eb71c1f13eaff59b405a721bb DJIA close on 27 Oct 2016: 18,169.68').hexdigest()
 *
 * CBlock(hash=00040fe8, ver=4, hashPrevBlock=00000000000000, hashMerkleRoot=c4eaa5, nTime=1477641360, nBits=1f07ffff, nNonce=4695, vtx=1)
 *   CTransaction(hash=c4eaa5, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff071f0104455a6361736830623963346565663862376363343137656535303031653335303039383462366665613335363833613763616331343161303433633432303634383335643334)
 *     CTxOut(nValue=0.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: c4eaa5
 */
static CBlock CreateGenesisBlock(uint32_t nTime, const uint256& nNonce, const std::vector<unsigned char>& nSolution, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "Zcash0b9c4eef8b7cc417ee5001e3500984b6fea35683a7cac141a043c42064835d34";
    const CScript genesisOutputScript = CScript() << ParseHex("04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nSolution, nBits, nVersion, genesisReward);
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

const arith_uint256 maxUint = UintToArith256(uint256S("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"));

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        strCurrencyUnits = "ZER";
        consensus.fCoinbaseMustBeProtected = true;
        consensus.nFeeStartBlockHeight = 412300;
        consensus.nSubsidyHalvingInterval = 800000;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 4000;
        consensus.powLimit = uint256S("0AB1Efffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowAveragingWindow = 17;
        assert(maxUint/UintToArith256(consensus.powLimit) >= consensus.nPowAveragingWindow);
        consensus.nPowMaxAdjustDown = 30; // 30% adjustment down
        consensus.nPowMaxAdjustUp = 10; // 10% adjustment up
        consensus.nPowTargetSpacing = 2 * 60;
        consensus.vUpgrades[Consensus::BASE_SPROUT].nProtocolVersion = 170002;
        consensus.vUpgrades[Consensus::BASE_SPROUT].nActivationHeight =
            Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nProtocolVersion = 170002;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nActivationHeight =
            Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_OVERWINTER].nProtocolVersion = 170004;
        consensus.vUpgrades[Consensus::UPGRADE_OVERWINTER].nActivationHeight =
            Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;

        /**
         * The message start string should be awesome!
         */
        pchMessageStart[0] = 0x5A; // Z
        pchMessageStart[1] = 0x45; // E
        pchMessageStart[2] = 0x52; // R
        pchMessageStart[3] = 0x4F; // O
        vAlertPubKey = ParseHex("73B0");
        nDefaultPort = 23801;
//        nMinerThreads = 0;
/*
        pchMessageStart[0] = 0x24;
        pchMessageStart[1] = 0xe9;
        pchMessageStart[2] = 0x27;
        pchMessageStart[3] = 0x64;
        vAlertPubKey = ParseHex("04b7ecf0baa90495ceb4e4090f6b2fd37eec1e9c85fac68a487f3ce11589692e4a317479316ee814e066638e1db54e37a10689b70286e6315b1087b6615d179264");
        nDefaultPort = 8233;
*/
        nMaxTipAge = 24 * 60 * 60;
        nPruneAfterHeight = 100000;
        const size_t N = 192, K = 7;
        BOOST_STATIC_ASSERT(equihash_parameters_acceptable(N, K));
        nEquihashN = N;
        nEquihashK = K;


        /**
         * Build the genesis block. Note that the output of its generation
         * transaction cannot be spent since it did not originally exist in the
         * database (and is in any case of zero value).
         *
         * >>> from pyblake2 import blake2s
         * >>> 'ZERO' + blake2s(b'ZERO is born. BTC #453749 - 00000000000000000252b2fb7e477185e56228a4f5c31ff9e8b5604b88adbbbe').hexdigest()
         */
        const char* pszTimestamp = "ZEROe374a91d7fcdfe63b1c4662de6c76a5eb1d16f50a4f52ad6c71a541f550f83bb";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 537534368 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 0;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock.SetNull();
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 4;
        genesis.nTime    = 1487500000;
        genesis.nBits    = 0x200A1FA<<4;
        genesis.nNonce   = uint256S("4c697665206c6f6e6720616e642070726f7370657221014592005a64336e336b");
        genesis.nSolution = ParseHex("06aa402279cac1b6f8d0b364d09deee9f578ba95ac97dd02ce337b1e39a095efecab9f3572c41b6b3a3d4521e2ef4b278f8a16110778dd580c26c13673eaf4fb6ce4e0c725e60219c8372017226f74aa16932498ce82f0e2c61f0b7b8936ca528bd3e81223d5256c02556156c61a94c323a4618cb43d4596422ae65cdf37ae61d6d7965150dd9f7833166f705e804d4a2490b37fcbc528fa8660c0c52610a87db8b3a33bef0b51d3f23e6d7327643c2d2fe3363b1dea511b7c84a1e919f25f830a6eb6bdc9bcf07f080138765e8ec6d4081e6f5b824df3bcbc30fa1efa4df797160c4417ee94b7908bca17333c350622b333c0377dbcb7c28445164ad6290ca41f066e99d596024a703f78b5352ed6157d6f8f64379173b2fb27ef0e77b49cd6f218063ec846336f27a827fc0a181feb63b09786aa76cc5585e8bc57ad9cf9bb031d15387e119bdd629ef11b3e4af13b9ff1abf4ccfc98208ee823f3d0d7155c57d85088135250fb6befd2423fd169390129ffdb1dfead37a38bcc0f5c161c972cef5b58a88d6554877e33887d98b61b");

        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x068cbb5db6bc11be5b93479ea4df41fa7e012e92ca8603c315f9b1a2202205c6"));
        assert(genesis.hashMerkleRoot == uint256S("0x094ef7f8882f3ec07edf16aa707c9511562b0e6211a8ed9db36332134bfe5357"));

        vFixedSeeds.clear();
        vSeeds.clear();
        vSeeds.push_back(CDNSSeedData("CryptoForge", "zeroseed.cryptoforge.cc")); // @cryptoforge
        vSeeds.push_back(CDNSSeedData("prophetalgorithms", "zerocurrency.prophetalgorithms.com")); // @MoodyDreamer
        vSeeds.push_back(CDNSSeedData("cryptonode1", "zseed1.cryptonode.cloud")); // @Kryptostuff
        vSeeds.push_back(CDNSSeedData("cryptonode2", "zseed2.cryptonode.cloud")); // @bKRyptostuff
        vSeeds.push_back(CDNSSeedData("zerocurrency", "insight.zerocurrency.io")); // @R3DEY3

        // guarantees the first 2 characters, when base58 encoded, are "t1"
        base58Prefixes[PUBKEY_ADDRESS]     = {0x1C,0xB8};
        // guarantees the first 2 characters, when base58 encoded, are "t3"
        base58Prefixes[SCRIPT_ADDRESS]     = {0x1C,0xBD};
        // the first character, when base58 encoded, is "5" or "K" or "L" (as in Bitcoin)
        base58Prefixes[SECRET_KEY]         = {0x80};
        // do not rely on these BIP32 prefixes; they are not specified and may change
        base58Prefixes[EXT_PUBLIC_KEY]     = {0x04,0x88,0xB2,0x1E};
        base58Prefixes[EXT_SECRET_KEY]     = {0x04,0x88,0xAD,0xE4};
        // guarantees the first 2 characters, when base58 encoded, are "zc"
        base58Prefixes[ZCPAYMENT_ADDRRESS] = {0x16,0x9A};
        // guarantees the first 4 characters, when base58 encoded, are "ZiVK"
        base58Prefixes[ZCVIEWING_KEY]      = {0xA8,0xAB,0xD3};
        // guarantees the first 2 characters, when base58 encoded, are "SK"
        base58Prefixes[ZCSPENDING_KEY]     = {0xAB,0x36};

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            ( 0, consensus.hashGenesisBlock),
            genesis.nTime,
            0,
            0
        };

        // Founders reward script expects a vector of 2-of-3 multisig addresses
        vFoundersRewardAddress = {
          "t3hmg6WApjqVFw9oPWTDy4JLEqXcUWthg5v", /* main-index: 0*/
          "t3hrh5M7eaGA5zXCitPXz2pbe146GkVPWHs", /* main-index: 1*/
          "t3aWmHqBGS7watoKQLa7uykeTaYHoYqM361", /* main-index: 2*/
          "t3hsi89hPsZzmnbs3pny6cfAxMxV5TJLErj", /* main-index: 3*/
          "t3TdGxPVUdMXd6qDrDCEuJETLadZ9Ki3s9r", /* main-index: 4*/
          "t3cb5ZjKmbGbqDaYk97Auam9kXXikGQBmyY", /* main-index: 5*/
          "t3V1YovGUPW9WSBoAHS48FDdUfUTo6LDpZR", /* main-index: 6*/
          "t3KB9n28MVg31oo856t1tQGfJuYq8usTvSi", /* main-index: 7*/
          "t3dqSV4YGj5V3WjQhqFGrKTMUf9Tgc6xnJM", /* main-index: 8*/
          "t3aJkYT1i6tyytq8J6khPaDNtgZsBSXgfBf", /* main-index: 9*/

        };
        assert(vFoundersRewardAddress.size() <= consensus.GetLastFoundersRewardBlockHeight());
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        strCurrencyUnits = "ZET";
        consensus.nFeeStartBlockHeight = 2800;
        consensus.nSubsidyHalvingInterval = 3000;
        consensus.nMajorityEnforceBlockUpgrade = 51;
        consensus.nMajorityRejectBlockOutdated = 75;
        consensus.nMajorityWindow = 400;
        consensus.powLimit = uint256S("0effffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
// "0AB1Efffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowAveragingWindow = 17;
        assert(maxUint/UintToArith256(consensus.powLimit) >= consensus.nPowAveragingWindow);
        consensus.nPowMaxAdjustDown = 30; // 30% adjustment down
        consensus.nPowMaxAdjustUp = 10; // 10% adjustment up
        consensus.nPowTargetSpacing = 2 * 60;
        consensus.vUpgrades[Consensus::BASE_SPROUT].nProtocolVersion = 170002;
        consensus.vUpgrades[Consensus::BASE_SPROUT].nActivationHeight =
            Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nProtocolVersion = 170002;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nActivationHeight =
            Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_OVERWINTER].nProtocolVersion = 170003;
        consensus.vUpgrades[Consensus::UPGRADE_OVERWINTER].nActivationHeight = 207500;
        //consensus.fPowAllowMinDifficultyBlocks = true;
        pchMessageStart[0] = 0x5B; // Z+1
        pchMessageStart[1] = 0x46; // E+1
        pchMessageStart[2] = 0x53; // R+1
        pchMessageStart[3] = 0x50; // O+1
        vAlertPubKey = ParseHex("73B0");
        nDefaultPort = 23832;
        nMaxTipAge = 1000000000; // accept all - 0x7fffffff; 24 * 60 * 60;
//        nMinerThreads = 0;

        nPruneAfterHeight = 1000;
        consensus.hashGenesisBlock = genesis.GetHash();
        const size_t N = 192, K = 7;
        BOOST_STATIC_ASSERT(equihash_parameters_acceptable(N, K));


        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        //genesis.nTime = 1487500000;
        genesis.nTime = 1531037937;
        genesis.nBits    = 0x200A1FA<<4;
        genesis.nNonce   = uint256S("0000000000000000000000000000000000000000000000000000000000000005"); // 192,7
        genesis.nSolution = ParseHex("003da4d81cb7cb368659ac0b42dcd585b2b4fd8522c928df1d6f0ee2cf0c7369152ddd60ef4b1f823797936ef8390dfacc41189e91f89b05c9ef262fc0403593dbceb6cff9a075174f730021efbac9576bd66da83faad58823270ebe561f30d9efdd4b9804021dda83320519522a57b670f46b8bb6fa50f52acbe9b8c80d517fbaac930cfb8fd7b39db20c6014bb41699e83a7faa1ca1872034c9208e68ac5d813c793f24aa5eee4d9b675ad05763135100bee8d40aa35375915d037e9d0d5dcee3336ba77c5ccf00182b1564d318bad31998ec007a8f49c658b56c5eb45ecd38d3d101d2801e91a17fd14b91636d1a6870e62ce970bd5b2d86827074aeaeac90a8c928f47ba136d836a8f3736bf9b6fdbf95f2ce372a99a19cc4077d5bb81055dfea6dfb662139a337cfe8d06dd80e25d8b2ea7f49c7a07f4c159175665018dbd68f581e7108ef5adcdc5331f2a1c227a546a1c232f94a2f88993fe71ab0a3d7d5008042ea7c07c514bc49a0366c41dda632379c7fbef13ae3cec6706567fff13147ec866c16d42cb1ef6cd8fcc38a7");
        consensus.hashGenesisBlock = genesis.GetHash();

        LogPrintf("TESTNET\n");
        LogPrintf("consensus.hashGenesisBlock=%s\n", consensus.hashGenesisBlock.GetHex());
        LogPrintf("genesis.hashMerkleRoot=%s\n", genesis.hashMerkleRoot.GetHex());
        LogPrintf("genesis.nTime=%is\n", genesis.nTime);
        //   Might be GetDec orsomething


        assert(consensus.hashGenesisBlock == uint256S("07d3146fbe5aff11eb071207b2fa90574907154944ea0e94be8433b2c5d16ae7"));
        vFixedSeeds.clear();
        vSeeds.clear();
        vSeeds.push_back(CDNSSeedData("cryptoforge", "testseed.cryptoforge.cc")); // CryptoForge

        // guarantees the first 2 characters, when base58 encoded, are "tm"
        base58Prefixes[PUBKEY_ADDRESS]     = {0x1D,0x25};
        // guarantees the first 2 characters, when base58 encoded, are "t2"
        base58Prefixes[SCRIPT_ADDRESS]     = {0x1C,0xBA};
        // the first character, when base58 encoded, is "9" or "c" (as in Bitcoin)
        base58Prefixes[SECRET_KEY]         = {0xEF};
        // do not rely on these BIP32 prefixes; they are not specified and may change
        base58Prefixes[EXT_PUBLIC_KEY]     = {0x04,0x35,0x87,0xCF};
        base58Prefixes[EXT_SECRET_KEY]     = {0x04,0x35,0x83,0x94};
        // guarantees the first 2 characters, when base58 encoded, are "zt"
        base58Prefixes[ZCPAYMENT_ADDRRESS] = {0x16,0xB6};
        // guarantees the first 4 characters, when base58 encoded, are "ZiVt"
        base58Prefixes[ZCVIEWING_KEY]      = {0xA8,0xAC,0x0C};
        // guarantees the first 2 characters, when base58 encoded, are "ST"
        base58Prefixes[ZCSPENDING_KEY]     = {0xAC,0x08};

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            (0, consensus.hashGenesisBlock),
            genesis.nTime,  // * UNIX timestamp of last checkpoint block
            0,       // * total number of transactions between genesis and last checkpoint
                     //   (the tx=... number in the SetBestChain debug.log lines)
            0        //   total number of tx / (checkpoint block height / (24 * 24))
          };

        // Founders reward script expects a vector of 2-of-3 multisig addresses
        vFoundersRewardAddress = {
          "t2BEnZwurNtPyhyWdZ82zTdS93rKyoUpgMJ",
          "t2AwNRubry4rQrEvHwAdpYve4Gz5cSmjGXA",
          "t2FM2EZqQZANA18rbhBGLWbxKcpGk9soAvS",
          "t2TBZfnXRCmFPc3nSrQcNtnR6AGWNzF9hzz",
          "t2QMyS1C3jXJ5hcR7zJTPSbWFvMWQEyjVTE",
          "t2SxA3E34aZJDDxnUxC2gZXBVrXWuoH5v7W",
          "t2FhR8STLvuKTUPgM586T1LMUG4JAHV2XSt",
          "t2NScRwr3FHPN8fNSgMKn8ngZG2kgfC8JNt",
          "t2LYZj3LiCDaCGUXQNXH1ZFPmHiC46wB9se",
          "t28azskSALVTtgbW63CqDwzXgtWQ56GVVXa"
        };
        assert(vFoundersRewardAddress.size() <= consensus.GetLastFoundersRewardBlockHeight());
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        strCurrencyUnits = "REG";
        consensus.fCoinbaseMustBeProtected = false;
        consensus.nFeeStartBlockHeight = 5000;
        consensus.nSubsidyHalvingInterval = 10000;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.powLimit = uint256S("0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f0f");
        consensus.nPowAveragingWindow = 17;
        assert(maxUint/UintToArith256(consensus.powLimit) >= consensus.nPowAveragingWindow);
        consensus.nPowMaxAdjustDown = 0; // Turn off adjustment down
        consensus.nPowMaxAdjustUp = 0; // Turn off adjustment up
        consensus.nPowTargetSpacing = 2 * 60;
        consensus.vUpgrades[Consensus::BASE_SPROUT].nProtocolVersion = 170002;
        consensus.vUpgrades[Consensus::BASE_SPROUT].nActivationHeight =
            Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nProtocolVersion = 170002;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nActivationHeight =
            Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_OVERWINTER].nProtocolVersion = 170003;
        consensus.vUpgrades[Consensus::UPGRADE_OVERWINTER].nActivationHeight =
            Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;

        pchMessageStart[0] = 0xaa;
        pchMessageStart[1] = 0xe8;
        pchMessageStart[2] = 0x3f;
        pchMessageStart[3] = 0x5f;
        nDefaultPort = 18344;
        nMaxTipAge = 24 * 60 * 60;
        nPruneAfterHeight = 1000;
        const size_t N = 48, K = 5;
        BOOST_STATIC_ASSERT(equihash_parameters_acceptable(N, K));
        nEquihashN = N;
        nEquihashK = K;
        genesis.nTime = 1531037936;
        genesis.nBits = 0x200f0f0f;
        genesis.nNonce = uint256S("0000000000000000000000000000000000000000000000000000000000000001");
        genesis.nSolution = ParseHex("09354815a3ad96efa233c6edbff6d3a245490c12d71971cf2969791411cd11132fcec3e8");
        consensus.hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 23802;
        assert(consensus.hashGenesisBlock == uint256S("0275863eebea76f824674494b7f6a3770ac46c732aaa62b07328feaa9d79798b"));
        nPruneAfterHeight = 1000;

        vFixedSeeds.clear(); //! Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;

        checkpointData = (CCheckpointData){
            boost::assign::map_list_of
            ( 0, consensus.hashGenesisBlock),
            genesis.nTime,
            0,
            0
        };
        // These prefixes are the same as the testnet prefixes
        base58Prefixes[PUBKEY_ADDRESS]     = {0x1D,0x25};
        base58Prefixes[SCRIPT_ADDRESS]     = {0x1C,0xBA};
        base58Prefixes[SECRET_KEY]         = {0xEF};
        // do not rely on these BIP32 prefixes; they are not specified and may change
        base58Prefixes[EXT_PUBLIC_KEY]     = {0x04,0x35,0x87,0xCF};
        base58Prefixes[EXT_SECRET_KEY]     = {0x04,0x35,0x83,0x94};
        base58Prefixes[ZCPAYMENT_ADDRRESS] = {0x16,0xB6};
        base58Prefixes[ZCVIEWING_KEY]      = {0xA8,0xAC,0x0C};
        base58Prefixes[ZCSPENDING_KEY]     = {0xAC,0x08};

        // guarantees the first 2 characters, when base58 encoded, are "tm"
        base58Prefixes[PUBKEY_ADDRESS]     = {0x1D,0x25};
        // guarantees the first 2 characters, when base58 encoded, are "t2"
        base58Prefixes[SCRIPT_ADDRESS]     = {0x1C,0xBA};
        // the first character, when base58 encoded, is "9" or "c" (as in Bitcoin)
        base58Prefixes[SECRET_KEY]         = {0xEF};
        // do not rely on these BIP32 prefixes; they are not specified and may change
        base58Prefixes[EXT_PUBLIC_KEY]     = {0x04,0x35,0x87,0xCF};
        base58Prefixes[EXT_SECRET_KEY]     = {0x04,0x35,0x83,0x94};
        // guarantees the first 2 characters, when base58 encoded, are "zt"
        base58Prefixes[ZCPAYMENT_ADDRRESS] = {0x16,0xB6};
        // guarantees the first 4 characters, when base58 encoded, are "ZiVt"
        base58Prefixes[ZCVIEWING_KEY]      = {0xA8,0xAC,0x0C};
        // guarantees the first 2 characters, when base58 encoded, are "ST"
        base58Prefixes[ZCSPENDING_KEY]     = {0xAC,0x08};

        // Founders reward script expects a vector of 2-of-3 multisig addresses
        vFoundersRewardAddress = { "t2FwcEhFdNXuFMv1tcYwaBJtYVtMj8b1uTg" };
        assert(vFoundersRewardAddress.size() <= consensus.GetLastFoundersRewardBlockHeight());
    }

    void UpdateNetworkUpgradeParameters(Consensus::UpgradeIndex idx, int nActivationHeight)
    {
        assert(idx > Consensus::BASE_SPROUT && idx < Consensus::MAX_NETWORK_UPGRADES);
        consensus.vUpgrades[idx].nActivationHeight = nActivationHeight;
    }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams &Params(CBaseChainParams::Network network) {
    switch (network) {
        case CBaseChainParams::MAIN:
            return mainParams;
        case CBaseChainParams::TESTNET:
            return testNetParams;
        case CBaseChainParams::REGTEST:
            return regTestParams;
        default:
            assert(false && "Unimplemented network");
            return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network) {
    SelectBaseParams(network);
    pCurrentParams = &Params(network);

    // Some python qa rpc tests need to enforce the coinbase consensus rule
    if (network == CBaseChainParams::REGTEST && mapArgs.count("-regtestprotectcoinbase")) {
        regTestParams.SetRegTestCoinbaseMustBeProtected();
    }
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}


// Block height must be >0 and <=last founders reward block height
// Index variable i ranges from 0 - (vFoundersRewardAddress.size()-1)
std::string CChainParams::GetFoundersRewardAddressAtHeight(int nHeight) const {
    int maxHeight = consensus.GetLastFoundersRewardBlockHeight();
    assert(nHeight >= consensus.nFeeStartBlockHeight && nHeight <= maxHeight);

    size_t addressChangeInterval = (maxHeight + vFoundersRewardAddress.size()) / vFoundersRewardAddress.size();
    size_t i = nHeight / addressChangeInterval;
    return vFoundersRewardAddress[i];
}

// Block height must be >0 and <=last founders reward block height
// The founders reward address is expected to be a multisig (P2SH) address
CScript CChainParams::GetFoundersRewardScriptAtHeight(int nHeight) const {
    assert(nHeight >= consensus.nFeeStartBlockHeight && nHeight <= consensus.GetLastFoundersRewardBlockHeight());

    CBitcoinAddress address(GetFoundersRewardAddressAtHeight(nHeight).c_str());
    assert(address.IsValid());
    assert(address.IsScript());
    CScriptID scriptID = boost::get<CScriptID>(address.Get()); // Get() returns a boost variant
    CScript script = CScript() << OP_HASH160 << ToByteVector(scriptID) << OP_EQUAL;
    return script;
}

std::string CChainParams::GetFoundersRewardAddressAtIndex(int i) const {
    assert(i >= 0 && i < vFoundersRewardAddress.size());
    return vFoundersRewardAddress[i];
}

void UpdateNetworkUpgradeParameters(Consensus::UpgradeIndex idx, int nActivationHeight)
{
    regTestParams.UpdateNetworkUpgradeParameters(idx, nActivationHeight);
}
