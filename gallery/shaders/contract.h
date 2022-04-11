#pragma once

namespace Gallery
{
    static const ShaderID s_SID_0 = {0x70,0xa4,0x51,0x3d,0x85,0x1e,0x47,0xdb,0x5c,0x6c,0x21,0x36,0x3b,0x87,0x87,0x2e,0xdb,0x51,0xc3,0x10,0xf9,0x9a,0x4e,0x5e,0x82,0x4b,0x73,0x64,0x81,0x33,0x1a,0x9f};
#pragma pack (push, 1)

    struct Tags
    {
        static const uint8_t s_Artist = 1;
        static const uint8_t s_Payout = 2;
        static const uint8_t s_Artwork = 4;
        static const uint8_t s_Impression = 5;
        static const uint8_t s_ArtworkHeight = 6;
        static const uint8_t s_ArtistHeight = 7;
        static const uint8_t s_CollectionHeight = 8;
        static const uint8_t s_Collection = 9;
        static const uint8_t s_ArtistCollection = 10;
        static const uint8_t s_CollectionArtwork = 11;
    };

    enum class Role {
        MANAGER,
        ARTIST,
        USER,
    };

    struct ArtistCollectionKey {
        uint8_t m_Tag = Tags::s_ArtistCollection;
        PubKey pkArtist;
        uint32_t collection_id;
    };

    struct CollectionArtworkKey {
        uint8_t m_Tag = Tags::s_CollectionArtwork;
        uint32_t collection_id;
        uint32_t artwork_id;
    };

    template <class T, class ID>
    struct GalleryObject {
        using Id = ID;

        bool Load(const Id& id, size_t object_size = sizeof(T)) {
            typename T::FirstStageKey fsk;
            fsk.id = id; 
            typename T::SecondStageKey ssk;
            ssk.id = id;
            if (!Env::LoadVar_T(fsk, ssk))
                return false;
            return Env::LoadVar(&ssk, sizeof(ssk), this, object_size, KeyTag::Internal);
        }

        bool Save(const Id& id, Height h = Utils::FromBE(Env::get_Height()), size_t object_size = sizeof(T)) {
            typename T::FirstStageKey fsk;
            fsk.id = id; 
            typename T::SecondStageKey ssk;
            ssk.id = id;
            ssk.h_updated = h;
            Env::SaveVar_T(fsk, ssk);
            return Env::SaveVar(&ssk, sizeof(ssk), this, object_size, KeyTag::Internal);
        }

        bool TakeOut(const Id& id, size_t object_size = sizeof(T)) {
            typename T::FirstStageKey fsk;
            fsk.id = id; 
            typename T::SecondStageKey ssk;
            ssk.id = id;
            if (!Env::LoadVar_T(fsk, ssk))
                return false;
            if (!Env::LoadVar(&ssk, sizeof(ssk), this, object_size, KeyTag::Internal))
                return false;
            Env::DelVar_T(ssk);
            return true;
        }

        void Delete(const Id& id) {
            typename T::FirstStageKey fsk;
            fsk.id = id; 
            typename T::SecondStageKey ssk;
            ssk.id = id;
            Env::LoadVar_T(fsk, ssk.h_updated);
            Env::DelVar_T(ssk);
            Env::DelVar_T(fsk);
        }

        bool Exists(const Id& id) {
            typename T::FirstStageKey fsk;
            fsk.id = id; 
            typename T::SecondStageKey ssk;
            return Env::LoadVar_T(fsk, ssk);
        }
    };

    struct Artist : public GalleryObject<Artist, PubKey> {
        struct FirstStageKey {
            uint8_t m_Tag = Tags::s_ArtistHeight;
            Id id;
        };

        struct SecondStageKey {
            uint8_t m_Tag = Tags::s_Artist;
            Id id;
            Height h_updated;
        };

        Height m_hRegistered;
        bool is_approved;
        uint32_t data_len;
        uint32_t label_len;

        // followed by label and data without delimiter
        static const uint32_t s_LabelMaxLen = 120;
        static const uint32_t s_DataMaxLen = 1024;
        static const uint32_t s_TotalMaxLen = s_LabelMaxLen + s_DataMaxLen;
    };

    struct Collection : public GalleryObject<Collection, uint32_t> {
        struct FirstStageKey {
            uint8_t m_Tag = Tags::s_CollectionHeight;
            Id id;
        };

        struct SecondStageKey {
            uint8_t m_Tag = Tags::s_Collection;
            Id id;
            Height h_updated;
        };

        bool is_approved;
        bool is_default;
        uint32_t artworks_num;
        uint32_t data_len;
        uint32_t label_len;
        PubKey m_pkAuthor;
        //Id id;

        // followed by label and data without delimiter
        static const uint32_t s_LabelMaxLen = 120;
        static const uint32_t s_DataMaxLen = 1024;
        static const uint32_t s_TotalMaxLen = s_LabelMaxLen + s_DataMaxLen;
    };

    struct AmountWithAsset {
        Amount m_Amount;
        AssetID m_Aid;
    };

    struct Artwork : public GalleryObject<Artwork, uint32_t> {
        // By this key masterpiece's height with specified ID is accessed 
        // ID ---FistStageKey---> h_updated ---SecondStageKey---> Artwork
        // This is done in order to be able to search for masterpieces updated since specified block
        struct FirstStageKey {
            uint8_t m_Tag = Tags::s_ArtworkHeight;
            Id id;
        };

        struct SecondStageKey {
            uint8_t m_Tag = Tags::s_Artwork;
            Id id;
            Height h_updated;
        };

        PubKey m_pkAuthor;
        PubKey m_pkOwner;
        AssetID m_Aid; // set when it's taken out of gallery
        Collection::Id collection_id;

        AmountWithAsset m_Price;
        static const uint32_t s_LabelMaxLen = 120;
        static const uint32_t s_DataMaxLen = 1024;
        static const uint32_t s_TotalMaxLen = s_LabelMaxLen + s_DataMaxLen;
    };

    struct Impression
    {
        struct ID
        {
            Artwork::Id m_ArtworkID;
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
            Artwork::Id m_ID;
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

        struct {
            uint32_t total;
            uint32_t approved;
        } artists_stats;

        struct {
            uint32_t total;
            uint32_t approved;
            uint32_t free_id;
        } artworks_stats;

        struct {
            uint32_t total;
            uint32_t approved;
            uint32_t free_id;
        } collections_stats;

        Amount m_VoteBalance;
    };

    struct Events
    {
        struct AddArtworkData {
            struct Key {
                uint8_t m_Prefix = 0;
                Artwork::Id m_ID;
                PubKey m_pkArtist;
            };
            // data is the exhibit itself
        };

        struct AddArtworkLabel {
            struct Key {
                uint8_t m_Prefix = 1;
                Artwork::Id m_ID;
                PubKey m_pkArtist;
            };
            // data is the exhibit itself
        };

        struct Sell {
            struct Key {
                uint8_t m_Prefix = 1;
                Artwork::Id m_ID;
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

            enum class RequestType { SET, DISABLE, ENABLE, DELETE } req;
            Gallery::Role role;
            PubKey m_pkArtist;
            uint32_t m_LabelLen; // set Artist::s_LabelMaxLen to remove
            uint32_t m_DataLen;
            // followed by label and data without delimiter
        };

        struct ManageCollection
        {
            static const uint32_t s_iMethod = 15;

            enum class RequestType { SET, DISABLE, ENABLE } req;
            Gallery::Role role;
            PubKey m_pkArtist;
            Collection::Id collection_id;
            uint32_t m_LabelLen;
            uint32_t m_DataLen;
            // followed by label and data without delimiter
        };

        struct AddExhibit
        {
            static const uint32_t s_iMethod = 3;

            enum class RequestType { SET, DISABLE, ENABLE } req;
            Gallery::Role role;
            PubKey m_pkArtist;
            uint32_t data_len;
            uint32_t label_len;
            uint32_t collection_id;
            // followed by the data and label
        };

        struct SetPrice
        {
            static const uint32_t s_iMethod = 4;

            Artwork::Id m_ID;
            AmountWithAsset m_Price;
        };

        struct Buy
        {
            static const uint32_t s_iMethod = 5;

            Artwork::Id m_ID;
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

            Artwork::Id m_ID;
        };

        struct CheckOut
        {
            static const uint32_t s_iMethod = 8;

            Artwork::Id m_ID;
        };

        struct CheckIn
        {
            static const uint32_t s_iMethod = 9;

            Artwork::Id m_ID;
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
            Artwork::Id m_ID;
        };

        struct Transfer
        {
            static const uint32_t s_iMethod = 14;
            Artwork::Id m_ID;
            PubKey m_pkNewOwner;
        };

    } // namespace Method

#pragma pack (pop)
}
