#pragma once

#include <string_view>

namespace Gallery {
    static const ShaderID s_SID_0 = {0xc9,0xdd,0xb6,0x91,0xa0,0xa2,0xfd,0xc8,0x51,0x8c,0x20,0xc0,0x02,0x6d,0x82,0x7e,0x56,0x19,0xe7,0x8a,0x78,0x31,0xb3,0x27,0x13,0x34,0x00,0xcf,0x6a,0xc5,0x5e,0x80};
#pragma pack (push, 1)

    enum class Tag : uint8_t {
        kArtist = 1,
        kPayout = 2,
        kArtwork = 3,
        kModerator = 4,
        kImpression = 5,
        kCollection = 6,
        kArtistCollectionIdx = 7,
        kCollectionArtworkIdx = 8,
        kHeightArtistIdx = 9,
        kHeightModeratorIdx = 10,
        kHeightArtworkIdx = 11,
        kHeightCollectionIdx = 12,
    };

    enum class Role : uint8_t {
        kManager,
        kModerator,
        kArtist,
        kUser,
    };

    enum class Status : uint8_t {
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

    template <class T, class ID>
    struct GalleryObject {
        using Id = ID;

        bool Load(const Id& id, size_t object_size = sizeof(T)) {
            typename T::Key k;
            if constexpr(std::is_same_v<Id, PubKey>)
                k.id = id;
            else
                k.id = Utils::FromBE(id); 

            return Env::LoadVar(&k, sizeof(k), this, object_size, KeyTag::Internal);
        }

        bool Save(const Id& id, size_t object_size = sizeof(T)) {
            typename T::Key k;
            if constexpr(std::is_same_v<Id, PubKey>)
                k.id = id;
            else
                k.id = Utils::FromBE(id); 
            return Env::SaveVar(&k, sizeof(k), this, object_size, KeyTag::Internal);
        }

        void Delete(const Id& id) {
            typename T::Key k;
            if constexpr(std::is_same_v<Id, PubKey>)
                k.id = id; 
            else
                k.id = Utils::FromBE(id); 
            Env::DelVar_T(k);
        }
    };

    struct Moderator : public GalleryObject<Moderator, PubKey> {
        struct Key {
            Tag tag = Tag::kModerator;
            Id id;
        };

        Height registered;
        Height updated;
        bool approved;
    };

    struct Artist : public GalleryObject<Artist, PubKey> {
        struct Key {
            Tag tag = Tag::kArtist;
            Id id;
        };

        Height m_hRegistered;
        Height updated;
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
        struct Key {
            Tag tag = Tag::kCollection;
            Id id;
        };

        Status status;
        Height updated;
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

        struct {
            AmountWithAsset price;
            uint32_t artwork_id;
        } min_sold;
        
        //Id id;

        // followed by label and data without delimiter
        static const uint32_t s_LabelMaxLen = 120;
        static const uint32_t s_DataMaxLen = 1024;
        static const uint32_t s_TotalMaxLen = s_LabelMaxLen + s_DataMaxLen;
        static const uint32_t s_MaxArtworks = 500;
    };

    struct Artwork : public GalleryObject<Artwork, uint32_t> {
        struct Key {
            Tag tag = Tag::kArtwork;
            Id id;
        };

        Id id;
        Height updated;
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

    struct Impression {
        struct Id {
            Artwork::Id m_ArtworkID;
            PubKey m_pkUser;
        };

        struct Key {
            Tag tag = Tag::kImpression;
            Id m_ID;
        };

        uint32_t m_Value; // 0 = none, 1 = like, etc.
    };

    struct Payout {
        struct Key {
            Tag tag = Tag::kPayout;
            PubKey m_pkUser;
            AssetID m_Aid;
            Artwork::Id m_ID;
        };

        Amount m_Amount;
    };

    struct Config {
        PubKey m_pkAdmin;
        AmountWithAsset m_VoteReward;
    };

    struct State {
        static const uint8_t s_Key = 0;

        Config m_Config;

        uint32_t total_artworks;
        uint32_t total_collections;
        uint32_t total_artists;
        uint32_t total_moderators;
        Amount fee_base;

        Amount m_VoteBalance;
    };

    struct Events {
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

    template <Tag tg, class Idx, class T>
    struct Index {
        struct Key {
            Tag tag = tg;
            Idx id;
            typename T::Id t_id;
        };

        static bool Save(const Idx& id, const typename T::Id& t_id) {
            return Env::SaveVar_T(InitKey_(id, t_id), true);
        }

        static bool Load(const Idx& id, const typename T::Id& t_id) {
            bool placeholder;
            return Env::LoadVar_T(InitKey_(id, t_id), placeholder);
        }

        static void Delete(const Idx& id, const typename T::Id& t_id) {
            Env::DelVar_T(InitKey_(id, t_id));
        }

        static bool Update(const Idx& old_id, const Idx& new_id, const typename T::Id& t_id) {
            Delete(old_id, t_id);
            return Save(new_id, t_id);
        }

        bool placeholder;
    private:
        static Key InitKey_(const Idx& id, const typename T::Id& t_id) {
            Key k;
            if constexpr(std::is_same_v<Idx, PubKey>)
                k.id = id;
            else
                k.id = Utils::FromBE(id); 

            if constexpr(std::is_same_v<typename T::Id, PubKey>)
                k.t_id = t_id;
            else
                k.t_id = Utils::FromBE(t_id); 
            return k;
        }
    };

    namespace Method {
        struct Init {
            static const uint32_t s_iMethod = 0;
            Config m_Config;
        };

        struct SetFeeBase {
            static const uint32_t s_iMethod = 13;
            Amount amount;
            PubKey signer;
        };

        struct ManageModerator {
            static const uint32_t s_iMethod = 16;

            enum class RequestType : uint8_t { kDisable, kEnable } req;
            Gallery::Moderator::Id id;
        };

        struct ManageArtist {
            static const uint32_t s_iMethod = 10;

            enum class RequestType : uint8_t { kSet, kSetStatus } req;
            Gallery::Status status;
            PubKey signer;
            Gallery::Role role;
            PubKey m_pkArtist;
            uint32_t m_LabelLen;
            uint32_t m_DataLen;
            // followed by label and data without delimiter
        };

        struct ManageCollection {
            static const uint32_t s_iMethod = 15;

            enum class RequestType : uint8_t { kSet, kSetStatus } req;
            Gallery::Status status;
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

            enum class RequestType : uint8_t { kSet, kSetStatus } req;
            Gallery::Status status;
            Gallery::Role role;
            PubKey m_pkArtist;
            PubKey signer;
            uint32_t data_len;
            uint32_t label_len;
            uint32_t collection_id;
            uint32_t artwork_id;
            uint32_t impressions_num;
            AmountWithAsset m_Price;
            // followed by the data and label
        };

        struct SetPrice {
            static const uint32_t s_iMethod = 4;

            Artwork::Id m_ID;
            AmountWithAsset m_Price;
        };

        struct Buy {
            static const uint32_t s_iMethod = 5;

            Artwork::Id m_ID;
            PubKey m_pkUser;
            uint8_t m_HasAid;
            Amount m_PayMax;
        };

        struct Withdraw {
            static const uint32_t s_iMethod = 6;

            Payout::Key m_Key;
            Amount m_Value;
        };

        struct CheckPrepare {
            static const uint32_t s_iMethod = 7;

            Artwork::Id m_ID;
        };

        struct CheckOut {
            static const uint32_t s_iMethod = 8;

            Artwork::Id m_ID;
        };

        struct CheckIn {
            static const uint32_t s_iMethod = 9;

            Artwork::Id m_ID;
            PubKey m_pkUser;
        };

        struct Vote {
            static const uint32_t s_iMethod = 11;
            Impression::Id m_ID;
            Impression m_Impression;
        };

        struct AddVoteRewards {
            static const uint32_t s_iMethod = 12;
            Amount m_Amount;
        };

        /*struct AdminDelete {
            static const uint32_t s_iMethod = 13;
            Artwork::Id m_ID;
        };
        */

        struct Transfer {
            static const uint32_t s_iMethod = 14;
            Artwork::Id m_ID;
            PubKey m_pkNewOwner;
        };

    } // namespace Method

#pragma pack (pop)
}
