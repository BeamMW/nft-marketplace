#pragma once

#include <string_view>

namespace Gallery {
    static const ShaderID s_SID_0 = {0xea,0xda,0x0d,0x52,0x6b,0x03,0xc5,0x03,0x23,0x9f,0x16,0xaa,0x67,0x96,0x94,0x01,0xac,0xb3,0x82,0x5c,0x6d,0x14,0xb8,0xe1,0xe6,0x09,0x60,0x05,0x44,0xde,0xf1,0x4e};
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
        struct ID {
            Artwork::Id m_ArtworkID;
            PubKey m_pkUser;
        };

        struct Key {
            Tag tag = Tag::kImpression;
            ID m_ID;
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

        struct ManageModerator {
            static const uint32_t s_iMethod = 16;

            enum class RequestType : uint8_t { kDisable, kEnable } req;
            Gallery::Moderator::Id id;
        };

        struct ManageArtist {
            static const uint32_t s_iMethod = 10;

            enum class RequestType : uint8_t { kSet, kReject, kApprove, kPending } req;
            PubKey signer;
            Gallery::Role role;
            PubKey m_pkArtist;
            uint32_t m_LabelLen;
            uint32_t m_DataLen;
            // followed by label and data without delimiter
        };

        struct ManageCollection {
            static const uint32_t s_iMethod = 15;

            enum class RequestType : uint8_t { kSet, kReject, kApprove, kPending } req;
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

            enum class RequestType : uint8_t { kSet, kReject, kApprove, kPending } req;
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
            Impression::ID m_ID;
            Impression m_Impression;
        };

        struct AddVoteRewards {
            static const uint32_t s_iMethod = 12;
            Amount m_Amount;
        };

        struct AdminDelete {
            static const uint32_t s_iMethod = 13;
            Artwork::Id m_ID;
        };

        struct Transfer {
            static const uint32_t s_iMethod = 14;
            Artwork::Id m_ID;
            PubKey m_pkNewOwner;
        };

    } // namespace Method

#pragma pack (pop)
}
