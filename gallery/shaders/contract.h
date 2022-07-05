#pragma once

#include "Shaders/upgradable3/contract.h"

#include <limits>
#include <string_view>

const Height kMaxHeight = std::numeric_limits<Height>::max();

namespace gallery {
static const ShaderID kSid0 = {0xc0, 0x06, 0x7f, 0xf6, 0x40, 0xc8, 0xa5, 0xaf,
                               0x98, 0xed, 0x82, 0x7e, 0x2c, 0x11, 0x57, 0x8d,
                               0xae, 0xbb, 0xca, 0xc4, 0x90, 0x7a, 0xce, 0x5f,
                               0xa9, 0x6c, 0x8d, 0x40, 0x0b, 0xe2, 0xe8, 0x01};
#pragma pack(push, 1)

using Hash256 = Opaque<32>;

inline Hash256 GetLabelHash(std::string_view label) {
    Hash256 res{};
    HashProcessor::Sha256 hp{};
    hp.Write(label.data(), label.size());
    hp >> res;
    return res;
}

enum class Tag : uint8_t {
    kArtist = 1,
    kPayout = 2,
    kNft = 3,
    kModerator = 4,
    kLike = 5,
    kCollection = 6,
    kHeightArtistIdx = 7,
    kHeightNftIdx = 8,
    kHeightCollectionIdx = 9,
    kArtistLabelHash = 10,
    kCollectionLabelHash = 11,
};

enum class Status : uint8_t {
    kPending,
    kApproved,
    kRejected,
    kNone,
};

struct AmountWithAsset {
    Amount amount;
    AssetID aid;
};

struct GalleryObject {
    template <class T>
    static bool Load(T& object, const typename T::Id& id,
                     size_t object_size = sizeof(T)) {
        typename T::Key k{InitKey_<T>(id)};
        return Env::LoadVar(&k, sizeof(k), &object, object_size,
                            KeyTag::Internal);
    }

    template <class T>
    static bool Save(T& object, const typename T::Id& id,
                     size_t object_size = sizeof(T)) {
        typename T::Key k{InitKey_<T>(id)};
        return Env::SaveVar(&k, sizeof(k), &object, object_size,
                            KeyTag::Internal);
    }

    template <class T>
    static void Delete(const typename T::Id& id) {
        Env::DelVar_T(InitKey_<T>(id));
    }

private:
    template <class T>
    static typename T::Key InitKey_(const typename T::Id& id) {
        if constexpr (std::is_same_v<typename T::Id, PubKey>)
            return typename T::Key{id};
        else
            return typename T::Key{Utils::FromBE(id)};
    }
};

struct Moderator {
    using Id = PubKey;
    struct Key {
        // The order is crucial: tag must be the first
        Tag tag = Tag::kModerator;
        Id id;
        explicit Key(const Id& id) : id{id} {
        }
        Key() : id{} {
        }
    };

    Height registered;
    Height updated;
    bool approved;
};

struct Artist {
    using Id = PubKey;
    struct Key {
        // The order is crucial: tag must be the first
        Tag tag = Tag::kArtist;
        Id id;
        explicit Key(const Id& id) : id{id} {
        }
        Key() : id{} {
        }
    };

    struct LabelKey {
        // The order is crucial: tag must be the first
        Tag tag = Tag::kArtistLabelHash;
        Hash256 label_hash;
        explicit LabelKey(const Hash256& hash) : label_hash{hash} {
        }
        LabelKey() : label_hash{} {
        }
    };

    Height registered;
    Height updated;
    Height last_created_object;
    Status status;
    uint32_t total_sold;
    Amount total_sold_price;
    uint32_t data_len;
    uint32_t collections_num;
    uint32_t nfts_num;
    uint32_t approved_cnt;

    // followed by label and data without delimiter
    static const uint32_t kLabelMaxLen = 200;
    static const uint32_t kDataMaxLen = 10 * 1024;
    static const uint32_t kTotalMaxLen = kLabelMaxLen + kDataMaxLen;
};

struct Collection {
    using Id = uint32_t;
    struct Key {
        // The order is crucial: tag must be the first
        Tag tag = Tag::kCollection;
        Id id;
        explicit Key(Id id) : id{id} {
        }
        Key() : id{} {
        }
    };

    struct LabelKey {
        // The order is crucial: tag must be the first
        Tag tag = Tag::kCollectionLabelHash;
        Artist::Id artist_id;
        Hash256 label_hash;
        explicit LabelKey(const Artist::Id& artist_id, const Hash256& hash)
            : artist_id{artist_id}, label_hash{hash} {
        }
        LabelKey() : label_hash{} {
        }
    };

    Status status;
    Height updated;
    uint32_t nfts_num;
    uint32_t approved_nfts_num;
    uint32_t data_len;
    uint32_t label_len;
    Artist::Id author;
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
    static const uint32_t kLabelMaxLen = 200;
    static const uint32_t kDataMaxLen = 10 * 1024;
    static const uint32_t kTotalMaxLen = kLabelMaxLen + kDataMaxLen;
    static const uint32_t kMaxNfts = 500;
};

struct Nft {
    using Id = uint32_t;
    struct Key {
        // The order is crucial: tag must be the first
        Tag tag = Tag::kNft;
        Id id;
        explicit Key(Id id) : id{id} {
        }
        Key() : id{} {
        }
    };

    Id id;
    Height updated;
    Artist::Id author;
    Artist::Id owner;
    AssetID aid;  // set when it's taken out of gallery
    Collection::Id collection_id;
    Status status;
    uint32_t likes_number;
    AmountWithAsset price;

    static const uint32_t kLabelMaxLen = 200;
    static const uint32_t kDataMaxLen = 10 * 1024;
    static const uint32_t kTotalMaxLen = kLabelMaxLen + kDataMaxLen;
};

struct Like {
    struct Key {
        // The order is crucial: tag must be the first
        Tag tag = Tag::kLike;
        Nft::Id nft_id;
        Artist::Id artist_id;
    };
    uint32_t value;
};

struct Payout {
    struct Key {
        // The order is crucial: tag must be the first
        Tag tag = Tag::kPayout;
        Artist::Id user;
        AssetID aid;
        Nft::Id nft_id;
    };
    Amount amount;
};

struct Config {
    PubKey admin_id;
    AmountWithAsset vote_reward;
};

struct State {
    static const uint8_t kKey = 0;
    Config config;
    uint32_t total_nfts;
    uint32_t total_collections;
    uint32_t total_artists;
    uint32_t total_moderators;
    Height rate_limit;
    Amount fee_base;
    Amount vote_balance;
};

struct Events {
    struct AddNftData {
        struct Key {
            // The order is crucial: prefix must be the first
            const uint8_t kPrefix = 0;
            Nft::Id nft_id;
            Artist::Id artist_id;
        };
        // data is the exhibit itself
    };

    struct AddNftLabel {
        struct Key {
            // The order is crucial: prefix must be the first
            const uint8_t prefix = 1;
            Nft::Id nft_id;
            Artist::Id artist_id;
        };
        // data is the exhibit itself
    };

    struct AddArtistLabel {
        struct Key {
            // The order is crucial: prefix must be the first
            uint8_t prefix = 2;
            Artist::Id id;
            explicit Key(const Artist::Id& id) : id{id} {
            }
            Key() : id{} {
            }
        };
    };

    struct Sell {
        struct Key {
            // The order is crucial: prefix must be the first
            uint8_t prefix = 3;
            Nft::Id nft_id;
        };
        AmountWithAsset price;
        uint8_t has_aid;
    };
};

template <Tag tg, class Idx, class T>
struct Index {
    struct Key {
        // The order is crucial
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

    static bool Update(const Idx& old_id, const Idx& new_id,
                       const typename T::Id& t_id) {
        Delete(old_id, t_id);
        return Save(new_id, t_id);
    }

private:
    static Key InitKey_(const Idx& id, const typename T::Id& t_id) {
        Key k;
        if constexpr (std::is_same_v<Idx, PubKey>)
            k.id = id;
        else
            k.id = Utils::FromBE(id);

        if constexpr (std::is_same_v<typename T::Id, PubKey>)
            k.t_id = t_id;
        else
            k.t_id = Utils::FromBE(t_id);
        return k;
    }
};

namespace method {
struct Init {
    static const uint32_t kMethod = 0;
    Config config;
    Upgradable3::Settings settings;
};

struct SetFeeBase {
    static const uint32_t kMethod = 3;
    Amount amount;
};

struct SetModerator {
    static const uint32_t kMethod = 4;
    bool approved;
    Moderator::Id id;
};

struct SetArtist {
    static const uint32_t kMethod = 5;
    Artist::Id artist_id;
    uint32_t label_len;
    uint32_t data_len;
    // followed by label and data
};

struct SetArtistStatus {
    static const uint32_t kMethod = 6;
    Status status;
    PubKey signer;
    uint32_t ids_num;
    Artist::Id ids[];
};

struct SetCollectionStatus {
    static const uint32_t kMethod = 7;
    Status status;
    PubKey signer;
    uint32_t ids_num;
    Collection::Id ids[];
};

struct SetCollection {
    static const uint32_t kMethod = 8;
    Artist::Id artist_id;
    Collection::Id collection_id;
    uint32_t label_len;
    uint32_t data_len;
    // followed by label and data without delimiter
};

struct SetNft {
    static const uint32_t kMethod = 9;
    Artist::Id artist_id;
    uint32_t data_len;
    uint32_t label_len;
    Collection::Id collection_id;
    AmountWithAsset price;
    // followed by the data and label
};

struct SetNftStatus {
    static const uint32_t kMethod = 10;
    PubKey signer;
    Status status;
    uint32_t ids_num;
    Nft::Id ids[];
};

struct SetPrice {
    static const uint32_t kMethod = 11;
    Nft::Id nft_id;
    AmountWithAsset price;
};

struct Buy {
    static const uint32_t kMethod = 12;
    Nft::Id nft_id;
    Artist::Id user;
    uint8_t has_aid;
};

struct Withdraw {
    static const uint32_t kMethod = 13;
    Payout::Key key;
    Amount value;
};

struct CheckPrepare {
    static const uint32_t kMethod = 14;
    Nft::Id nft_id;
};

struct CheckOut {
    static const uint32_t kMethod = 15;
    Nft::Id nft_id;
};

struct CheckIn {
    static const uint32_t kMethod = 16;
    Nft::Id nft_id;
    Artist::Id user;
};

struct Vote {
    static const uint32_t kMethod = 17;
    Artist::Id artist_id;
    Nft::Id nft_id;
    uint32_t value;
};

struct AddVoteRewards {
    static const uint32_t kMethod = 18;
    Amount amount;
};

struct Transfer {
    static const uint32_t kMethod = 19;
    Nft::Id nft_id;
    Artist::Id new_owner;
};

struct SetRateLimit {
    static const uint32_t kMethod = 20;
    Amount amount;
};

struct MigrateArtist {
    static const uint32_t kMethod = 21;
    Artist::Id artist_id;
    PubKey signer;
    uint32_t label_len;
    uint32_t data_len;
    // followed by label and data
};

struct MigrateCollection {
    static const uint32_t kMethod = 22;
    Artist::Id artist_id;
    Collection::Id collection_id;
    PubKey signer;
    uint32_t label_len;
    uint32_t data_len;
    // followed by label and data without delimiter
};

struct MigrateNft {
    static const uint32_t kMethod = 23;
    Artist::Id artist_id;
    Artist::Id owner_id;
    Nft::Id nft_id;
    PubKey signer;
    uint32_t data_len;
    uint32_t label_len;
    Collection::Id collection_id;
    AmountWithAsset price;
    // followed by the data and label
};

struct MigrateSales {
    static const uint32_t kMethod = 24;
    Nft::Id nft_id;
    PubKey signer;
    uint32_t sales_len;
    Amount prices[];
};
}  // namespace method
#pragma pack(pop)
}  // namespace gallery
