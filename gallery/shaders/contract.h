#pragma once

#include <string_view>

namespace Gallery {
    static const ShaderID s_SID_0 = {0x57,0x38,0x20,0x47,0x22,0xf8,0x34,0x3f,0x61,0x7a,0x69,0x63,0x02,0xda,0x7c,0x4a,0x68,0x88,0x8d,0x3e,0x00,0x81,0x14,0x9f,0xd8,0x5b,0xd8,0xf7,0xbe,0xf6,0xb9,0xbd};
#pragma pack (push, 1)

    enum class Tag : uint8_t {
        kArtist = 1,
        kPayout = 2,
        kNft = 3,
        kModerator = 4,
        kImpression = 5,
        kCollection = 6,
        kArtistCollectionIdx = 7,
        kCollectionNftIdx = 8,
        kHeightArtistIdx = 9,
        kHeightModeratorIdx = 10,
        kHeightNftIdx = 11,
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
        kNone,
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
        uint32_t nfts_num;

        // followed by label and data without delimiter
        static const uint32_t s_LabelMaxLen = 200;
        static const uint32_t s_DataMaxLen = 10 * 1024;
        static const uint32_t s_TotalMaxLen = s_LabelMaxLen + s_DataMaxLen;
    };

    struct Collection : public GalleryObject<Collection, uint32_t> {
        struct Key {
            Tag tag = Tag::kCollection;
            Id id;
        };

        Status status;
        Height updated;
        uint32_t nfts_num;
        uint32_t data_len;
        uint32_t label_len;
        PubKey m_pkAuthor;
        uint32_t total_sold;
        Amount total_sold_price;
        struct {
            AmountWithAsset price;
            uint32_t nft_id;
        } max_sold;

        struct {
            AmountWithAsset price;
            uint32_t nft_id;
        } min_sold;
        
        // followed by label and data without delimiter
        static const uint32_t s_LabelMaxLen = 200;
        static const uint32_t s_DataMaxLen = 10 * 1024;
        static const uint32_t s_TotalMaxLen = s_LabelMaxLen + s_DataMaxLen;
        static const uint32_t s_MaxNfts = 500;
    };

    struct Nft : public GalleryObject<Nft, uint32_t> {
        struct Key {
            Tag tag = Tag::kNft;
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

        static const uint32_t s_LabelMaxLen = 200;
        static const uint32_t s_DataMaxLen = 10 * 1024;
        static const uint32_t s_TotalMaxLen = s_LabelMaxLen + s_DataMaxLen;
    };

    struct Impression {
        struct Id {
            Nft::Id nft_id;
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
            Nft::Id m_ID;
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
        uint32_t total_nfts;
        uint32_t total_collections;
        uint32_t total_artists;
        uint32_t total_moderators;
        Amount fee_base;
        Amount m_VoteBalance;
    };

    struct Events {
        struct AddNftData {
            struct Key {
                uint8_t m_Prefix = 0;
                Nft::Id m_ID;
                PubKey m_pkArtist;
            };
            // data is the exhibit itself
        };

        struct AddNftLabel {
            struct Key {
                uint8_t m_Prefix = 1;
                Nft::Id m_ID;
                PubKey m_pkArtist;
            };
            // data is the exhibit itself
        };

        struct AddArtistLabel {
            struct Key {
                uint8_t m_Prefix = 2;
                Artist::Id id;
            };
        };

        struct Sell {
            struct Key {
                uint8_t m_Prefix = 3;
                Nft::Id m_ID;
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
            static const uint32_t s_iMethod = 3;
            Amount amount;
        };

        struct SetModerator {
            static const uint32_t s_iMethod = 4;
            bool approved;
            Moderator::Id id;
        };

        struct SetArtist {
            static const uint32_t s_iMethod = 5;
            PubKey m_pkArtist;
            uint32_t m_LabelLen;
            uint32_t m_DataLen;
            // followed by label and data without delimiter
        };

        struct SetArtistStatus {
            static const uint32_t s_iMethod = 6;
            Status status;
            PubKey signer;
            uint32_t ids_num;
            Artist::Id ids[];
            static const uint32_t kMaxIds = 128;
        };

        struct SetCollectionStatus {
            static const uint32_t s_iMethod = 7;
            Status status;
            PubKey signer;
            uint32_t ids_num;
            Collection::Id ids[];
            static const uint32_t kMaxIds = 128;
        };

        struct SetCollection {
            static const uint32_t s_iMethod = 8;
            PubKey m_pkArtist;
            Collection::Id collection_id;
            uint32_t m_LabelLen;
            uint32_t m_DataLen;
            // followed by label and data without delimiter
        };

        struct SetNft {
            static const uint32_t s_iMethod = 9;
            PubKey m_pkArtist;
            uint32_t data_len;
            uint32_t label_len;
            uint32_t collection_id;
            AmountWithAsset m_Price;
            // followed by the data and label
        };

        struct SetNftStatus {
            static const uint32_t s_iMethod = 10;
            PubKey signer;
            Status status;
            uint32_t ids_num;
            Nft::Id ids[];
            static const uint32_t kMaxIds = 128;
        };

        struct SetPrice {
            static const uint32_t s_iMethod = 11;
            Nft::Id m_ID;
            AmountWithAsset m_Price;
        };

        struct Buy {
            static const uint32_t s_iMethod = 12;
            Nft::Id m_ID;
            PubKey m_pkUser;
            uint8_t m_HasAid;
            Amount m_PayMax;
        };

        struct Withdraw {
            static const uint32_t s_iMethod = 13;
            Payout::Key m_Key;
            Amount m_Value;
        };

        struct CheckPrepare {
            static const uint32_t s_iMethod = 14;
            Nft::Id m_ID;
        };

        struct CheckOut {
            static const uint32_t s_iMethod = 15;
            Nft::Id m_ID;
        };

        struct CheckIn {
            static const uint32_t s_iMethod = 16;
            Nft::Id m_ID;
            PubKey m_pkUser;
        };

        struct Vote {
            static const uint32_t s_iMethod = 17;
            Impression::Id m_ID;
            Impression m_Impression;
        };

        struct AddVoteRewards {
            static const uint32_t s_iMethod = 18;
            Amount m_Amount;
        };

        struct Transfer {
            static const uint32_t s_iMethod = 19;
            Nft::Id m_ID;
            PubKey m_pkNewOwner;
        };
    } // namespace Method
#pragma pack (pop)
}
