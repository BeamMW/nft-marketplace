#pragma once

#include <string_view>

namespace Gallery
{
    static const ShaderID s_SID_0 = {0xf1,0x54,0x7b,0x92,0xd1,0x24,0x80,0x6c,0xf4,0x88,0xfc,0xae,0x7b,0x8c,0xa1,0xc1,0x3f,0xfc,0x6b,0x7f,0xa5,0xaa,0xee,0x06,0xa4,0xd4,0x8b,0x14,0x6e,0xd8,0x81,0xb6};
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
        static const uint8_t s_Moderator = 12;
        static const uint8_t s_ModeratorHeight = 13;
    };

    enum class Role {
        kManager,
        kModerator,
        kArtist,
        kUser,
    };

    enum class Status {
        kPending,
        kApproved,
        kRejected,

    };

    const std::string_view status_to_string(const Status& status) {
        switch (status) {
        case Status::kPending:
            return "pending";
        case Status::kApproved:
            return "approved";
        case Status::kRejected:
            return "rejected";
        default:
            return "";
        };
    }

    struct AmountWithAsset {
        Amount m_Amount;
        AssetID m_Aid;
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
            if constexpr(std::is_same_v<Id, PubKey>)
                fsk.id = id;
            else
                fsk.id = Utils::FromBE(id); 

            typename T::SecondStageKey ssk;
            if (!Env::LoadVar_T(fsk, ssk))
                return false;
            return Env::LoadVar(&ssk, sizeof(ssk), this, object_size, KeyTag::Internal);
        }

        bool Save(const Id& id, Height h = Env::get_Height(), size_t object_size = sizeof(T)) {
            typename T::FirstStageKey fsk;
            typename T::SecondStageKey ssk;
            if constexpr(std::is_same_v<Id, PubKey>) {
                ssk.id = id;
                fsk.id = id;
            } else {
                fsk.id = Utils::FromBE(id); 
                ssk.id = Utils::FromBE(id); 
            }
            ssk.h_updated = Utils::FromBE(h);
            Env::SaveVar_T(fsk, ssk);
            return Env::SaveVar(&ssk, sizeof(ssk), this, object_size, KeyTag::Internal);
        }

        bool TakeOut(const Id& id, size_t object_size = sizeof(T)) {
            typename T::FirstStageKey fsk;
            if constexpr(std::is_same_v<Id, PubKey>)
                fsk.id = id; 
            else
                fsk.id = Utils::FromBE(id); 
            typename T::SecondStageKey ssk;
            if (!Env::LoadVar_T(fsk, ssk))
                return false;
            if (!Env::LoadVar(&ssk, sizeof(ssk), this, object_size, KeyTag::Internal))
                return false;
            Env::DelVar_T(ssk);
            return true;
        }

        void Delete(const Id& id) {
            typename T::FirstStageKey fsk;
            if constexpr(std::is_same_v<Id, PubKey>)
                fsk.id = id; 
            else
                fsk.id = Utils::FromBE(id); 
            typename T::SecondStageKey ssk;
            Env::LoadVar_T(fsk, ssk);
            Env::DelVar_T(ssk);
            Env::DelVar_T(fsk);
        }

        bool Exists(const Id& id) {
            typename T::FirstStageKey fsk;
            if constexpr(std::is_same_v<Id, PubKey>)
                fsk.id = id; 
            else
                fsk.id = Utils::FromBE(id); 
            typename T::SecondStageKey ssk;
            return Env::LoadVar_T(fsk, ssk);
        }
    };

    struct Moderator : GalleryObject<Moderator, PubKey> {
        struct FirstStageKey {
            uint8_t m_Tag = Tags::s_ModeratorHeight;
            Id id;
        };

        struct SecondStageKey {
            uint8_t m_Tag = Tags::s_Moderator;
            Height h_updated;
            Id id;
        };

        Height registered;
        bool approved;
    };

    struct Artist : public GalleryObject<Artist, PubKey> {
        struct FirstStageKey {
            uint8_t m_Tag = Tags::s_ArtistHeight;
            Id id;
        };

        struct SecondStageKey {
            uint8_t m_Tag = Tags::s_Artist;
            Height h_updated;
            Id id;
        };

        Height m_hRegistered;
        Status status;
        uint32_t data_len;
        uint32_t label_len;
        uint32_t collections_num;
        uint32_t artworks_num;

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
            Height h_updated;
            Id id;
        };

        Status status;
        bool is_default;
        uint32_t artworks_num;
        uint32_t data_len;
        uint32_t label_len;
        PubKey m_pkAuthor;
        uint32_t total_sold;
        Amount total_sold_price;
        struct {
            AmountWithAsset price;
            uint32_t artwork_id;
        } max_sold;
        
        //Id id;

        // followed by label and data without delimiter
        static const uint32_t s_LabelMaxLen = 120;
        static const uint32_t s_DataMaxLen = 1024;
        static const uint32_t s_TotalMaxLen = s_LabelMaxLen + s_DataMaxLen;
        static const uint32_t s_MaxArtworks = 500;
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
            Height h_updated;
            Id id;
        };

        PubKey m_pkAuthor;
        PubKey m_pkOwner;
        AssetID m_Aid; // set when it's taken out of gallery
        Collection::Id collection_id;
        Status status;
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

        struct Stats {
            uint32_t total;
            uint32_t approved;
            uint32_t pending;
        };

        struct : public Stats {
            uint32_t free_id;
        } artworks_stats;

        struct : public Stats {
            uint32_t free_id;
        } collections_stats;

        struct : public Stats {
        } artists_stats;

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

        struct ManageModerator
        {
            static const uint32_t s_iMethod = 16;

            enum class RequestType { kDisable, kEnable } req;
            Gallery::Moderator::Id id;
        };

        struct ManageArtist {
            static const uint32_t s_iMethod = 10;

            enum class RequestType { kSet, kReject, kApprove, kPending } req;
            PubKey signer;
            Gallery::Role role;
            PubKey m_pkArtist;
            uint32_t m_LabelLen;
            uint32_t m_DataLen;
            // followed by label and data without delimiter
        };

        struct ManageCollection
        {
            static const uint32_t s_iMethod = 15;

            enum class RequestType { kSet, kReject, kApprove, kPending } req;
            PubKey signer;
            Gallery::Role role;
            PubKey m_pkArtist;
            Collection::Id collection_id;
            uint32_t m_LabelLen;
            uint32_t m_DataLen;
            // followed by label and data without delimiter
        };

        struct ManageArtwork {
            static const uint32_t s_iMethod = 3;

            enum class RequestType { kSet, kReject, kApprove, kPending } req;
            Gallery::Role role;
            PubKey m_pkArtist;
            PubKey signer;
            uint32_t data_len;
            uint32_t label_len;
            uint32_t collection_id;
            uint32_t artwork_id;
            AmountWithAsset m_Price;
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
