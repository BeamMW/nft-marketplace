#pragma once

namespace Gallery
{
    static const ShaderID s_SID_0 = {0xdf,0xe5,0x0f,0x72,0x5f,0xfa,0x13,0x4f,0x98,0x50,0xd9,0x50,0x25,0x64,0xc1,0x54,0xe8,0xec,0xbc,0x17,0xc8,0x2e,0xf0,0xfc,0xda,0x28,0x14,0xe6,0x8c,0x1a,0x25,0x19};
    static const ShaderID s_SID_1 = {0x08,0xb4,0xb7,0x2f,0xb5,0xd7,0x60,0x31,0x29,0x01,0x75,0x82,0x36,0x59,0x86,0xee,0xcc,0x13,0x61,0x35,0x31,0xc4,0x0b,0x28,0xc2,0xc4,0x5b,0x3d,0x37,0x29,0x3c,0xf1};
    static const ShaderID s_SID_2 = {0xe7,0x45,0x6c,0xe8,0x6f,0x86,0x90,0x6a,0xab,0x0e,0x6a,0x7e,0xe5,0x77,0x79,0x92,0x0a,0xfa,0x32,0xfe,0x9c,0x5c,0x72,0x37,0x0f,0x15,0x47,0xda,0x7d,0x48,0xc4,0x35};
    static const ShaderID s_SID_3 = {0x96,0x03,0xff,0x4c,0x5c,0xa2,0xe9,0xdc,0x1d,0xb7,0x3d,0x01,0x37,0x41,0xde,0x8b,0x70,0xf1,0xe3,0xbe,0x2d,0x3d,0xe0,0x7f,0xd8,0x1e,0x99,0x92,0xd2,0x58,0xd3,0x6b};
    static const ShaderID s_SID_4 = {0x01,0xb5,0xc0,0x6d,0x69,0x4a,0x56,0xe2,0xc5,0xf8,0xea,0xef,0xb5,0xa1,0xf6,0xbd,0xc5,0xc1,0xb2,0x17,0xa5,0x84,0x3d,0xe2,0x4c,0x61,0xa1,0xf7,0x7b,0xc9,0xa9,0xfa};
    static const ShaderID s_SID_5 = {0x62,0xb1,0x94,0x7a,0xcd,0xfc,0x56,0x56,0xee,0x63,0xbd,0x1f,0x39,0x7e,0xcc,0x13,0x0b,0xb8,0x34,0x11,0xb7,0x6b,0xdd,0x95,0x5c,0xe2,0x59,0x2a,0x52,0x1b,0xc8,0x7f};
    static const ShaderID s_SID_6 = {0xb1,0x27,0x40,0x18,0x53,0xc5,0xcc,0xd1,0x31,0xa8,0x19,0x51,0xdc,0x70,0x18,0x42,0x0d,0x54,0x42,0x95,0x37,0x10,0xeb,0xb0,0xaf,0x4c,0xcc,0x31,0xba,0xa1,0x36,0x54};
    static const ShaderID s_SID_7 = {0x02,0xf3,0xe3,0x2b,0x74,0xab,0xac,0xf9,0x73,0x46,0x6d,0xe1,0xdb,0x3d,0x30,0x29,0x01,0xe4,0xda,0xdf,0xe1,0xe0,0xf5,0xcf,0xea,0x0d,0x5c,0xac,0x6a,0xa0,0x0b,0x33};
#pragma pack (push, 1)

    struct Tags
    {
        static const uint8_t s_Artist = 1;
        static const uint8_t s_Payout = 2;
        static const uint8_t s_Masterpiece = 4;
        static const uint8_t s_Impression = 5;
        static const uint8_t s_MasterpieceHeight = 6;
        static const uint8_t s_ArtistHeight = 7;
    };

    struct Artist
    {
        struct FirstStageKey {
            uint8_t m_Tag = Tags::s_ArtistHeight;
            PubKey m_pkUser;
        };

        struct SecondStageKey {
            uint8_t m_Tag = Tags::s_Artist;
            Height h_last_updated;
            PubKey m_pkUser;
        };

        Height m_hRegistered;
        bool is_approved;

        // followed by label
        static const uint32_t s_LabelMaxLen = 100;
    };

    struct AmountWithAsset {
        Amount m_Amount;
        AssetID m_Aid;
    };

    struct Masterpiece
    {
        typedef uint32_t ID; // in big-endian format, for more convenient enumeration sorted by ID

        // By this key masterpiece's height with specified ID is accessed 
        // ID ---FistStageKey---> h_last_updated ---SecondStageKey---> Masterpiece
        // This is done in order to be able to search for masterpieces updated since specified block
        struct FirstStageKey {
            uint8_t m_Tag = Tags::s_MasterpieceHeight;
            ID m_ID;
        };

        struct SecondStageKey {
            uint8_t m_Tag = Tags::s_Masterpiece;
            Height h_last_updated;
            ID m_ID;
        };

        PubKey m_pkOwner;
        AssetID m_Aid; // set when it's taken out of gallery

        AmountWithAsset m_Price;
    };

    struct Impression
    {
        struct ID
        {
            Masterpiece::ID m_MasterpieceID;
            PubKey m_pkUser;
        };

        struct Key
        {
            uint8_t m_Tag = Tags::s_Impression;
            ID m_ID;
        };

        uint32_t m_Value; // 0 = none, 1 = like, etc.
    };

    struct Payout
    {
        struct Key {
            uint8_t m_Tag = Tags::s_Payout;
            PubKey m_pkUser;
            AssetID m_Aid;
            Masterpiece::ID m_ID;
        };

        Amount m_Amount;
    };

    struct Config
    {
        PubKey m_pkAdmin;
        AmountWithAsset m_VoteReward;
    };

    struct State
    {
        static const uint8_t s_Key = 0;

        Config m_Config;
        Masterpiece::ID m_Exhibits;
        Amount m_VoteBalance;
    };

    struct Events
    {
        struct Add {
            struct Key {
                uint8_t m_Prefix = 0;
                Masterpiece::ID m_ID;
                PubKey m_pkArtist;
            };
            // data is the exhibit itself
        };

        struct Sell {
            struct Key {
                uint8_t m_Prefix = 1;
                Masterpiece::ID m_ID;
            };

            AmountWithAsset m_Price;
            uint8_t m_HasAid;
        };
    };

    namespace Method
    {
        struct Init
        {
            static const uint32_t s_iMethod = 0;
            Config m_Config;
        };

        struct ManageArtist
        {
            static const uint32_t s_iMethod = 10;

            enum class RequestType { CREATE, DELETE, APPROVE } req;
            PubKey m_pkArtist;
            uint32_t m_LabelLen; // set Artist::s_LabelMaxLen to remove
            // followed by label
        };

        struct AddExhibit
        {
            static const uint32_t s_iMethod = 3;

            PubKey m_pkArtist;
            uint32_t m_Size;
            // followed by the data
        };

        struct SetPrice
        {
            static const uint32_t s_iMethod = 4;

            Masterpiece::ID m_ID;
            AmountWithAsset m_Price;
        };

        struct Buy
        {
            static const uint32_t s_iMethod = 5;

            Masterpiece::ID m_ID;
            PubKey m_pkUser;
            uint8_t m_HasAid;
            Amount m_PayMax;
        };

        struct Withdraw
        {
            static const uint32_t s_iMethod = 6;

            Payout::Key m_Key;
            Amount m_Value;
        };

        struct CheckPrepare
        {
            static const uint32_t s_iMethod = 7;

            Masterpiece::ID m_ID;
        };

        struct CheckOut
        {
            static const uint32_t s_iMethod = 8;

            Masterpiece::ID m_ID;
        };

        struct CheckIn
        {
            static const uint32_t s_iMethod = 9;

            Masterpiece::ID m_ID;
            PubKey m_pkUser;
        };

        struct Vote
        {
            static const uint32_t s_iMethod = 11;
            Impression::ID m_ID;
            Impression m_Impression;
        };

        struct AddVoteRewards
        {
            static const uint32_t s_iMethod = 12;
            Amount m_Amount;
        };

        struct AdminDelete
        {
            static const uint32_t s_iMethod = 13;
            Masterpiece::ID m_ID;
        };

        struct Transfer
        {
            static const uint32_t s_iMethod = 14;
            Masterpiece::ID m_ID;
            PubKey m_pkNewOwner;
        };

    } // namespace Method

#pragma pack (pop)

}
