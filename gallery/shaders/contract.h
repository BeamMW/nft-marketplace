#pragma once

namespace Gallery
{
    static const ShaderID s_SID_0 = {0x08,0xb4,0xb7,0x2f,0xb5,0xd7,0x60,0x31,0x29,0x01,0x75,0x82,0x36,0x59,0x86,0xee,0xcc,0x13,0x61,0x35,0x31,0xc4,0x0b,0x28,0xc2,0xc4,0x5b,0x3d,0x37,0x29,0x3c,0xf1};
#pragma pack (push, 1)

    struct Tags
    {
        static const uint8_t s_Artist = 1;
        static const uint8_t s_Payout = 2;
        static const uint8_t s_Masterpiece = 4;
        static const uint8_t s_Impression = 5;
        static const uint8_t s_MasterpieceHeight = 6;
    };

    struct Artist
    {
        struct Key {
            uint8_t m_Tag = Tags::s_Artist;
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
