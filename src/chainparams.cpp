// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Copyright (c) 2017-2019 The Raven Core developers
// Copyright (c) 2019 The Taona Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"
#include "arith_uint256.h"

#include <assert.h>

#include "chainparamsseeds.h"

//TODO: Take these out
extern double algoHashTotal[16];
extern int algoHashHits[16];


static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << CScriptNum(0) << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "Taona 6/8/2019 Polish Cryptocurrency Exchange Shuts Down Overnight";
    const CScript genesisOutputScript = CScript() << ParseHex("04cc7be1db572423631279b3703bb07e4928441e6d8208b8eecbcf868f63692e0f22d98f617096b4fa74e9a9ebed028236f597a57992ae40a09988159691fb2887") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

void CChainParams::UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    consensus.vDeployments[d].nStartTime = nStartTime;
    consensus.vDeployments[d].nTimeout = nTimeout;
}

void CChainParams::TurnOffSegwit() {
	consensus.nSegwitEnabled = false;
}

void CChainParams::TurnOffCSV() {
	consensus.nCSVEnabled = false;
}

void CChainParams::TurnOffBIP34() {
	consensus.nBIP34Enabled = false;
}

void CChainParams::TurnOffBIP65() {
	consensus.nBIP65Enabled = false;
}

void CChainParams::TurnOffBIP66() {
	consensus.nBIP66Enabled = false;
}

bool CChainParams::BIP34() {
	return consensus.nBIP34Enabled;
}

bool CChainParams::BIP65() {
	return consensus.nBIP34Enabled;
}

bool CChainParams::BIP66() {
	return consensus.nBIP34Enabled;
}

bool CChainParams::CSVEnabled() const{
	return consensus.nCSVEnabled;
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

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 2100000;
        consensus.nBIP34Enabled = true;
        consensus.nBIP65Enabled = true;
        consensus.nBIP66Enabled = true;
        consensus.nSegwitEnabled = true;
        consensus.nCSVEnabled = true;
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 2016 * 60;
        consensus.nPowTargetSpacing = 60;
		consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1814; // Approx 90% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].bit = 6;  //Assets (RIP2)
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nStartTime = 1558476000;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nTimeout = 1672480000;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0x51;
        pchMessageStart[1] = 0x40;
        pchMessageStart[2] = 0x55;
        pchMessageStart[3] = 0x4d;
        nDefaultPort = 8757;
        nPruneAfterHeight = 100000;
/*
        uint32_t nGenesisTime = 1560052800;
        arith_uint256 test;
        bool fNegative;
        bool fOverflow;
        test.SetCompact(0x1e00ffff, &fNegative, &fOverflow);
        std::cout << "Test threshold: " << test.GetHex() << "\n\n";
        int genesisNonce = 0;
        uint256 TempHashHolding = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000");
        uint256 BestBlockHash = uint256S("0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        for (int i=0;i<40000000;i++) {
            genesis = CreateGenesisBlock(nGenesisTime, i, 0x1e00ffff, 2, 5000 * COIN);
            //genesis.hashPrevBlock = TempHashHolding;
            consensus.hashGenesisBlock = genesis.GetHash();
            arith_uint256 BestBlockHashArith = UintToArith256(BestBlockHash);
            if (UintToArith256(consensus.hashGenesisBlock) < BestBlockHashArith) {
                BestBlockHash = consensus.hashGenesisBlock;
                std::cout << BestBlockHash.GetHex() << " Nonce: " << i << "\n";
                std::cout << "   PrevBlockHash: " << genesis.hashPrevBlock.GetHex() << "\n";
        	std::cout << "hashGenesisBlock to 0x" << BestBlockHash.GetHex() << std::endl;
        	std::cout << "Genesis Nonce to " << genesisNonce << std::endl;
        	std::cout << "Genesis Merkle " << genesis.hashMerkleRoot.GetHex() << std::endl;
            }
            TempHashHolding = consensus.hashGenesisBlock;
            if (BestBlockHashArith < test) {
                genesisNonce = i - 1;
                break;
            }
            //std::cout << consensus.hashGenesisBlock.GetHex() << "\n";
        }
        std::cout << "\n";
        std::cout << "\n";
        std::cout << "\n";
        std::cout << "hashGenesisBlock to 0x" << BestBlockHash.GetHex() << std::endl;
        std::cout << "Genesis Nonce to " << genesisNonce << std::endl;
        std::cout << "Genesis Merkle " << genesis.hashMerkleRoot.GetHex() << std::endl;
        std::cout << "\n";
        return;
*/
        genesis = CreateGenesisBlock( 1560052800, 1628672, 0x1e00ffff, 2, 5000 * COIN);

        consensus.hashGenesisBlock = genesis.GetHash();

        assert(consensus.hashGenesisBlock == uint256S("0x00000032b7fe96ff6fdf52f92c4f0a7ea25dc90f597945ecba8dbb7d5ffe751e"));
        assert(genesis.hashMerkleRoot == uint256S("432d892f8d8efb4a6750781f248c42911d8ea1c8b77ee460afb43bc8b717b978"));

        vSeeds.emplace_back("seed1.taonacoin.com", false);
        vSeeds.emplace_back("community1.taonacoin.com", false);
	vSeeds.emplace_back("community2.taonacoin.com", false);
	vSeeds.emplace_back("community3.taonacoin.com", false);

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,66);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,125);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,128);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x41, 0x38, 0x2B, 0x1D};
        base58Prefixes[EXT_SECRET_KEY] = {0x4C, 0xA8, 0x5A, 0x3E};

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fMiningRequiresPeers = true;

        checkpointData = (CCheckpointData) {
            {
                { 50000, uint256S("0x0000000000ea511e464ce9bbf3cae4854d3bf82493517606ec3fbf0863111a9c")},
                { 100000, uint256S("0x000000000292860d435c34a4132253ba94911d1d7e6f1999657a0e89255d072a")},
                { 150000, uint256S("0000000006aded85cf996ca1fe44fbfaa22b450acb4aee0009d43edb5c33b73f")},
                { 200000, uint256S("000000000b17eaad441cdbd530550ece9afd5ec7eb529508411ca92a04926221")},
                { 300000, uint256S("000000000036f0b25189183e3d3c84532ec971d7f056803ce5e069cdbb4e6213")},
                { 440000, uint256S("d02392c8b4af09008d4a2d3ac64865d264d4aa4b66776873d4d22bc8d908f299")},

            }
        };

        chainTxData = ChainTxData{
            // Update as we know more about the contents of the Taona chain
            0, // * UNIX timestamp of last known number of transactions
            0,    // * total number of transactions between genesis and that timestamp
                        //   (the tx=... number in the SetBestChain debug.log lines)
            0         // * estimated number of transactions per second after that timestamp
        };

        /** RVN Start **/
        // Burn Amounts
        nIssueAssetBurnAmount = 500 * COIN;
        nReissueAssetBurnAmount = 100 * COIN;
        nIssueSubAssetBurnAmount = 100 * COIN;
        nIssueUniqueAssetBurnAmount = 5 * COIN;

        // Burn Addresses
        strIssueAssetBurnAddress = "TZissueAssetXXXXXXXXXXXXXXXXZBvh9N";
        strReissueAssetBurnAddress = "TZreissueAssetXXXXXXXXXXXXXXW94PRc";
        strIssueSubAssetBurnAddress = "TZissueSubAssetXXXXXXXXXXXXXbCtq41";
        strIssueUniqueAssetBurnAddress = "TZissueUniqueAssetXXXXXXXXXXRe4bEG";

        //Global Burn Address
        strGlobalBurnAddress = "TZburnXXXXXXXXXXXXXXXXXXXXXXZ1hfPj";

        // DGW Activation
        nDGWActivationBlock = 1;

        nMaxReorganizationDepth = 60; // 60 at 1 minute block timespan is +/- 60 minutes.
        nMinReorganizationPeers = 4;
        nMinReorganizationAge = 60 * 60 * 12; // 12 hours
        /** RVN End **/
    }
};

/**
 * Testnet (v6)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 2100000;
        consensus.nBIP34Enabled = true;
        consensus.nBIP65Enabled = true;
        consensus.nBIP66Enabled = true;
        consensus.nSegwitEnabled = true;
        consensus.nCSVEnabled = true;
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 2016 * 60; // 1.4 days
        consensus.nPowTargetSpacing = 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1310; // Approx 65% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].bit = 5;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nStartTime = 1458231200;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nTimeout = 1678351999;


        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");


        pchMessageStart[0] = 0x50;
        pchMessageStart[1] = 0x54;
        pchMessageStart[2] = 0x4C;
        pchMessageStart[3] = 0x52;
        nDefaultPort = 18757;
        nPruneAfterHeight = 1000;
/*
        uint32_t nGenesisTime = 1560052900;
        arith_uint256 test;
        bool fNegative;
        bool fOverflow;
        test.SetCompact(0x1e00ffff, &fNegative, &fOverflow);
        std::cout << "Test threshold: " << test.GetHex() << "\n\n";
        int genesisNonce = 0;
        uint256 TempHashHolding = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000");
        uint256 BestBlockHash = uint256S("0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        for (int i=0;i<40000000;i++) {
            genesis = CreateGenesisBlock(nGenesisTime, i, 0x1e00ffff, 2, 5000 * COIN);
            //genesis.hashPrevBlock = TempHashHolding;
            consensus.hashGenesisBlock = genesis.GetHash();
            arith_uint256 BestBlockHashArith = UintToArith256(BestBlockHash);
            if (UintToArith256(consensus.hashGenesisBlock) < BestBlockHashArith) {
                BestBlockHash = consensus.hashGenesisBlock;
                std::cout << BestBlockHash.GetHex() << " Nonce: " << i << "\n";
                std::cout << "   PrevBlockHash: " << genesis.hashPrevBlock.GetHex() << "\n";
        	std::cout << "hashGenesisBlock to 0x" << BestBlockHash.GetHex() << std::endl;
        	std::cout << "Genesis Nonce to " << genesisNonce << std::endl;
        	std::cout << "Genesis Merkle " << genesis.hashMerkleRoot.GetHex() << std::endl;
            }
            TempHashHolding = consensus.hashGenesisBlock;
            if (BestBlockHashArith < test) {
                genesisNonce = i - 1;
                break;
            }
            //std::cout << consensus.hashGenesisBlock.GetHex() << "\n";
        }
        std::cout << "\n";
        std::cout << "\n";
        std::cout << "\n";
        std::cout << "hashGenesisBlock to 0x" << BestBlockHash.GetHex() << std::endl;
        std::cout << "Genesis Nonce to " << genesisNonce << std::endl;
        std::cout << "Genesis Merkle " << genesis.hashMerkleRoot.GetHex() << std::endl;
        std::cout << "\n";
        return;
*/
        genesis = CreateGenesisBlock( 1560052900, 4821953, 0x1e00ffff, 2, 5000 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();

        //Test MerkleRoot and GenesisBlock
        assert(consensus.hashGenesisBlock == uint256S("0x000000275fa88402c5724d72505c1b10b9f1dfe0d243be659e23c11d6a2fb2da"));
        assert(genesis.hashMerkleRoot == uint256S("432d892f8d8efb4a6750781f248c42911d8ea1c8b77ee460afb43bc8b717b978"));

        vFixedSeeds.clear();
        vSeeds.clear();

//        vSeeds.emplace_back("testseed1.taonacoin.com", false);
//        vSeeds.emplace_back("testseed2.taonacoin.com", false);
//        vSeeds.emplace_back("testseed3.taonacoin.com", false);

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,75);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,128);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,137);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fMiningRequiresPeers = true;

        checkpointData = (CCheckpointData) {
            {
            }
        };

        chainTxData = ChainTxData{
            // Update as we know more about the contents of the Taona chain
            0, // * UNIX timestamp of last known number of transactions
            0,     // * total number of transactions between genesis and that timestamp
                        //   (the tx=... number in the SetBestChain debug.log lines)
            0        // * estimated number of transactions per second after that timestamp
        };

        /** RVN Start **/
        // Burn Amounts
        nIssueAssetBurnAmount = 500 * COIN;
        nReissueAssetBurnAmount = 100 * COIN;
        nIssueSubAssetBurnAmount = 100 * COIN;
        nIssueUniqueAssetBurnAmount = 5 * COIN;

        // Burn Addresses
        strIssueAssetBurnAddress = "XZissueAssetXXXXXXXXXXXXXXXXZHwwrJ";
        strReissueAssetBurnAddress = "XZreissueAssetXXXXXXXXXXXXXXauye4M";
        strIssueSubAssetBurnAddress = "XZissueSubAssetXXXXXXXXXXXXXbhpdEv";
        strIssueUniqueAssetBurnAddress = "XZissueUniqueAssetXXXXXXXXXXWCcBY1";

        // Global Burn Address
        strGlobalBurnAddress = "XZburnXXXXXXXXXXXXXXXXXXXXXXTgqvdh";

        // DGW Activation
        nDGWActivationBlock = 1;

        nMaxReorganizationDepth = 60; // 60 at 1 minute block timespan is +/- 60 minutes.
        nMinReorganizationPeers = 4;
        nMinReorganizationAge = 60 * 60 * 12; // 12 hours
        /** RVN End **/

    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nBIP34Enabled = true;
        consensus.nBIP65Enabled = true;
        consensus.nBIP66Enabled = true;
        consensus.nSegwitEnabled = true;
        consensus.nCSVEnabled = true;
        consensus.nSubsidyHalvingInterval = 150;
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 2016 * 60; // 1.4 days
        consensus.nPowTargetSpacing = 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].bit = 6;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0x44;
        pchMessageStart[1] = 0x53;
        pchMessageStart[2] = 0x4E;
        pchMessageStart[3] = 0x58;
        nDefaultPort = 18257;
        nPruneAfterHeight = 1000;
/*
        uint32_t nGenesisTime = 1560053000;
        arith_uint256 test;
        bool fNegative;
        bool fOverflow;
        test.SetCompact(0x1e00ffff, &fNegative, &fOverflow);
        std::cout << "Test threshold: " << test.GetHex() << "\n\n";
        int genesisNonce = 0;
        uint256 TempHashHolding = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000");
        uint256 BestBlockHash = uint256S("0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        for (int i=0;i<40000000;i++) {
            genesis = CreateGenesisBlock(nGenesisTime, i, 0x1e00ffff, 2, 5000 * COIN);
            //genesis.hashPrevBlock = TempHashHolding;
            consensus.hashGenesisBlock = genesis.GetHash();
            arith_uint256 BestBlockHashArith = UintToArith256(BestBlockHash);
            if (UintToArith256(consensus.hashGenesisBlock) < BestBlockHashArith) {
                BestBlockHash = consensus.hashGenesisBlock;
                std::cout << BestBlockHash.GetHex() << " Nonce: " << i << "\n";
                std::cout << "   PrevBlockHash: " << genesis.hashPrevBlock.GetHex() << "\n";
        	std::cout << "hashGenesisBlock to 0x" << BestBlockHash.GetHex() << std::endl;
        	std::cout << "Genesis Nonce to " << genesisNonce << std::endl;
        	std::cout << "Genesis Merkle " << genesis.hashMerkleRoot.GetHex() << std::endl;
            }
            TempHashHolding = consensus.hashGenesisBlock;
            if (BestBlockHashArith < test) {
                genesisNonce = i - 1;
                break;
            }
            //std::cout << consensus.hashGenesisBlock.GetHex() << "\n";
        }
        std::cout << "\n";
        std::cout << "\n";
        std::cout << "\n";
        std::cout << "hashGenesisBlock to 0x" << BestBlockHash.GetHex() << std::endl;
        std::cout << "Genesis Nonce to " << genesisNonce << std::endl;
        std::cout << "Genesis Merkle " << genesis.hashMerkleRoot.GetHex() << std::endl;
        std::cout << "\n";
        return;
*/
        genesis = CreateGenesisBlock( 1560053000, 17168820, 0x1e00ffff, 2, 5000 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();

        assert(consensus.hashGenesisBlock == uint256S("0x00000022e57b435122ea07ecefffe1f9b820391e1a5507b1e0580b2b64d64c41"));
        assert(genesis.hashMerkleRoot == uint256S("432d892f8d8efb4a6750781f248c42911d8ea1c8b77ee460afb43bc8b717b978"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        checkpointData = (CCheckpointData) {
            {
            }
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,80);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,137);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,142);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};


        /** RVN Start **/
        // Burn Amounts
        nIssueAssetBurnAmount = 500 * COIN;
        nReissueAssetBurnAmount = 100 * COIN;
        nIssueSubAssetBurnAmount = 100 * COIN;
        nIssueUniqueAssetBurnAmount = 5 * COIN;

        // Burn Addresses
        strIssueAssetBurnAddress = "ZZissueAssetXXXXXXXXXXXXXXXXWUY5Ut";
        strReissueAssetBurnAddress = "ZZreissueAssetXXXXXXXXXXXXXXXAm5AW";
        strIssueSubAssetBurnAddress = "ZZissuesubAssetXXXXXXXXXXXXXZGzYnF";
        strIssueUniqueAssetBurnAddress = "ZZissueUniqueAssetXXXXXXXXXXVEs7jo";

        // Global Burn Address
        strGlobalBurnAddress = "ZZburnXXXXXXXXXXXXXXXXXXXXXXX1tYsh";

        // DGW Activation
        nDGWActivationBlock = 1;

        nMaxReorganizationDepth = 60; // 60 at 1 minute block timespan is +/- 60 minutes.
        nMinReorganizationPeers = 4;
        nMinReorganizationAge = 60 * 60 * 12; // 12 hours
        /** RVN End **/
    }
};

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams());
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}

void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    globalChainParams->UpdateVersionBitsParameters(d, nStartTime, nTimeout);
}

void TurnOffSegwit(){
	globalChainParams->TurnOffSegwit();
}

void TurnOffCSV() {
	globalChainParams->TurnOffCSV();
}

void TurnOffBIP34() {
	globalChainParams->TurnOffBIP34();
}

void TurnOffBIP65() {
	globalChainParams->TurnOffBIP65();
}

void TurnOffBIP66() {
	globalChainParams->TurnOffBIP66();
}
