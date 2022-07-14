#include "Shaders/common.h"
#include "Shaders/app_common_impl.h"
#include "Shaders/upgradable3/app_common_impl.h"
#include "contract.h"

#include <array>
#include <limits>
#include <string_view>
#include <type_traits>
#include <vector>
#include <map>

// MANAGER

#define Gallery_manager_view(macro)

#define Gallery_manager_view_params(macro) macro(ContractID, cid)

#define Gallery_manager_view_moderators(macro) macro(ContractID, cid)

#define Gallery_manager_set_moderator(macro)                 \
    macro(ContractID, cid) macro(gallery::Moderator::Id, id) \
        macro(uint32_t, enable)

#define Gallery_manager_view_artists_stats(macro) macro(ContractID, cid)

#define Gallery_manager_view_moderators_stats(macro) macro(ContractID, cid)

#define Gallery_manager_view_collections_stats(macro) macro(ContractID, cid)

#define Gallery_manager_view_nfts_stats(macro) macro(ContractID, cid)

#define Gallery_manager_view_nfts(macro)                          \
    macro(ContractID, cid) macro(Height, h0) macro(Amount, count) \
        macro(gallery::Nft::Id, id0)

#define Gallery_manager_view_nft_sales(macro) \
    macro(ContractID, cid) macro(gallery::Nft::Id, id)

#define Gallery_manager_view_collections(macro)                     \
    macro(ContractID, cid) macro(Height, h0) macro(uint32_t, count) \
        macro(gallery::Collection::Id, id0)

#define Gallery_manager_view_artists(macro)                         \
    macro(ContractID, cid) macro(Height, h0) macro(uint32_t, count) \
        macro(gallery::Artist::Id, id0)

#define Gallery_manager_view_likes(macro)                                     \
    macro(ContractID, cid) macro(uint32_t, id0) macro(uint32_t, count) macro( \
        gallery::Artist::Id, artist_id) macro(Height, h0) macro(Height, hn)

#define Gallery_manager_view_balance(macro) macro(ContractID, cid)

#define Gallery_manager_create_contract(macro)               \
    macro(ContractID, cid) macro(Amount, vote_reward_amount) \
        macro(AssetID, vote_reward_aid)

#define Gallery_manager_replace_admin(macro) Upgradable3_replace_admin(macro)

#define Gallery_manager_set_min_approvers(macro) \
    Upgradable3_set_min_approvers(macro)

#define Gallery_manager_get_id(macro)

#define Gallery_manager_explicit_upgrade(macro) macro(ContractID, cid)

#define Gallery_manager_schedule_upgrade(macro) \
    macro(ContractID, cid) macro(Height, target_height)

#define Gallery_manager_set_nft_status(macro) macro(ContractID, cid)

#define Gallery_manager_set_collection_status(macro) macro(ContractID, cid)

#define Gallery_manager_set_artist_status(macro) macro(ContractID, cid)

#define Gallery_manager_set_fee_base(macro) \
    macro(ContractID, cid) macro(Amount, amount)

#define Gallery_manager_set_rate_limit(macro) \
    macro(ContractID, cid) macro(Amount, amount)

#define Gallery_manager_migrate_nft(macro)                                    \
    macro(ContractID, cid) macro(gallery::Collection::Id, collection_id)      \
        macro(Amount, amount) macro(AssetID, aid) macro(gallery::Nft::Id, id) \
            macro(gallery::Artist::Id, artist_id)                             \
                macro(gallery::Artist::Id, owner_id)

#define Gallery_manager_migrate_artist(macro) \
    macro(ContractID, cid) macro(gallery::Artist::Id, id)

#define Gallery_manager_migrate_collection(macro)                \
    macro(ContractID, cid) macro(gallery::Artist::Id, artist_id) \
        macro(gallery::Collection::Id, collection_id)

#define Gallery_manager_migrate_sales(macro) \
    macro(ContractID, cid) macro(gallery::Nft::Id, nft_id)

#define GalleryRole_manager(macro)                                             \
    macro(manager, create_contract) macro(manager, replace_admin) macro(       \
        manager, set_min_approvers) macro(manager, view)                       \
        macro(manager, view_params) macro(manager, view_artists_stats) macro(  \
            manager, view_moderators_stats) macro(manager, view_artists)       \
            macro(manager, view_likes) macro(manager, view_collections) macro( \
                manager, view_nfts) macro(manager, view_nft_sales)             \
                macro(manager, view_collections_stats) macro(                  \
                    manager, view_nfts_stats) macro(manager, view_balance)     \
                    macro(manager, get_id) macro(manager, explicit_upgrade)    \
                        macro(manager, schedule_upgrade) macro(manager,        \
                                                               set_moderator)  \
                            macro(manager, view_moderators) macro(             \
                                manager, set_nft_status)                       \
                                macro(manager, set_artist_status) macro(       \
                                    manager, set_collection_status)            \
                                    macro(manager, set_fee_base) macro(        \
                                        manager, set_rate_limit)               \
                                        macro(manager, migrate_nft) macro(     \
                                            manager, migrate_artist)           \
                                            macro(manager, migrate_collection) \
                                                macro(manager, migrate_sales)

// MODERATOR

#define Gallery_moderator_set_nft_status(macro) macro(ContractID, cid)

#define Gallery_moderator_set_collection_status(macro) macro(ContractID, cid)

#define Gallery_moderator_set_artist_status(macro) macro(ContractID, cid)

#define GalleryRole_moderator(macro)                                     \
    macro(moderator, set_nft_status) macro(moderator, set_artist_status) \
        macro(moderator, set_collection_status)

// ARTIST

#define Gallery_artist_get_id(macro) macro(ContractID, cid)

#define Gallery_artist_set_artist(macro) macro(ContractID, cid)

#define Gallery_artist_set_collection(macro) \
    macro(ContractID, cid) macro(gallery::Collection::Id, id)

#define Gallery_artist_set_nft(macro)                                    \
    macro(ContractID, cid) macro(gallery::Collection::Id, collection_id) \
        macro(Amount, amount) macro(AssetID, aid)

#define GalleryRole_artist(macro)                                          \
    macro(artist, get_id) macro(artist, set_nft) macro(artist, set_artist) \
        macro(artist, set_collection)

// USER

#define Gallery_user_set_price(macro)                                        \
    macro(ContractID, cid) macro(gallery::Nft::Id, id) macro(Amount, amount) \
        macro(AssetID, aid)

#define Gallery_user_transfer(macro)                   \
    macro(ContractID, cid) macro(gallery::Nft::Id, id) \
        macro(gallery::Artist::Id, new_owner)

#define Gallery_user_buy(macro) \
    macro(ContractID, cid) macro(gallery::Nft::Id, id)

#define Gallery_user_add_rewards(macro) \
    macro(ContractID, cid) macro(uint32_t, amount)

#define Gallery_user_view_balance(macro) macro(ContractID, cid)

#define Gallery_user_withdraw(macro) \
    macro(ContractID, cid) macro(uint32_t, max_count)

#define Gallery_user_vote(macro) \
    macro(ContractID, cid) macro(gallery::Nft::Id, id) macro(uint32_t, val)

#define Gallery_user_is_my_key(macro)                      \
    macro(ContractID, cid) macro(gallery::Nft::Id, nft_id) \
        macro(gallery::Artist::Id, key)

#define GalleryRole_user(macro)                                           \
    macro(user, set_price) macro(user, transfer) macro(user, buy)         \
        macro(user, view_balance) macro(user, withdraw) macro(user, vote) \
            macro(user, add_rewards) macro(user, is_my_key)

#define GalleryRoles_All(macro) \
    macro(manager) macro(moderator) macro(artist) macro(user)

BEAM_EXPORT void Method_0() {
    // scheme
    Env::DocGroup root("");
    {
        Env::DocGroup gr("roles");
#define THE_FIELD(type, name) Env::DocAddText(#name, #type);
#define THE_METHOD(role, name)             \
    {                                      \
        Env::DocGroup grMethod(#name);     \
        Gallery_##role##_##name(THE_FIELD) \
    }
#define THE_ROLE(name)                 \
    {                                  \
        Env::DocGroup grRole(#name);   \
        GalleryRole_##name(THE_METHOD) \
    }
        GalleryRoles_All(THE_ROLE)
#undef THE_ROLE
#undef THE_METHOD
#undef THE_FIELD
    }
}

#define THE_FIELD(type, name) const type &name,
#define ON_METHOD(role, name) \
    void On_##role##_##name(Gallery_##role##_##name(THE_FIELD) int unused = 0)

/*
 * Constants
 */
// For migration purposes
static const ContractID kOldGalleryCid = {
    0xc0, 0x2d, 0xbd, 0x60, 0x3e, 0xc8, 0x92, 0x51, 0x77, 0xcd, 0x3f,
    0xd5, 0xe2, 0x68, 0x48, 0xe3, 0xb8, 0x2c, 0xd3, 0x69, 0x1b, 0x69,
    0xed, 0xd8, 0xad, 0xd4, 0xcc, 0xfd, 0x40, 0x81, 0xae, 0x9e};
static const ContractID kEmptyCid = {};

const ShaderID kSids[] = {
    gallery::kSid0,
};

const Upgradable3::Manager::VerInfo kVerInfo = {kSids, _countof(kSids)};

/*
 * Forward declarations
 */
const std::string_view StatusToString(gallery::Status status);
static inline bool IsOwner(const ContractID& cid, const PubKey& owner,
                           gallery::Nft::Id nft_id);
template <class Id>
std::vector<Id> ParseIds(std::string_view ids);

/*
 * Structures
 */
namespace key_material {
const std::string_view kAdminMeta{"Gallery-key-admin"};

struct Admin : public Env::KeyID {
    Admin() : Env::KeyID(kAdminMeta.data(), kAdminMeta.size()) {
    }
};

#pragma pack(push, 1)
constexpr std::string_view kOwnerMeta{"Gallery-key-owner"};

struct Owner {
    ContractID cid;
    gallery::Nft::Id nft_id;
    uint8_t seed[kOwnerMeta.size()];

    explicit Owner(const ContractID& cid) : cid{cid}, nft_id{} {
        Env::Memcpy(seed, kOwnerMeta.data(), kOwnerMeta.size());
    }

    Owner(const ContractID& cid, gallery::Nft::Id nft_id)
        : cid{cid}, nft_id{nft_id} {
        Env::Memcpy(seed, kOwnerMeta.data(), kOwnerMeta.size());
    }

    PubKey Get() const {
        PubKey pk{};
        Env::DerivePk(pk, this, sizeof(*this));
        return pk;
    }
};
#pragma pack(pop)
}  // namespace key_material

struct StatePlus : public gallery::State {
    bool Init(const ContractID& cid) {
        Env::Key_T<std::remove_const_t<decltype(kKey)>> key;
        key.m_Prefix.m_Cid = cid;
        key.m_KeyInContract = gallery::State::kKey;
        return Env::VarReader::Read_T(key, Cast::Down<gallery::State>(*this));
    }
};

#pragma pack(push, 0)
struct AppArtist : public gallery::Artist {
    std::array<char, kDataMaxLen + 1> data;
    std::array<char, kLabelMaxLen + 1> label;

    static key_material::Owner key_material(const ContractID& cid,
                                            gallery::Nft::Id id = 0) {
        auto zero_km = key_material::Owner{kEmptyCid, id};
        auto old_km = key_material::Owner{kOldGalleryCid, id};
        auto new_km = key_material::Owner{cid, id};
        if (Exists(cid, zero_km.Get()))
            return zero_km;
        else if (Exists(cid, old_km.Get()))
            return old_km;
        else
            return new_km;
    }

    static PubKey id(const ContractID& cid) {
        return key_material(cid).Get();
    }

    static bool Exists(const ContractID& cid,
                       const gallery::Artist::Id& artist_id) {
        Env::Key_T<gallery::Artist::Key> k;
        k.m_Prefix.m_Cid = cid;
        k.m_KeyInContract.id =
            _POD_(artist_id).IsZero() ? AppArtist::id(cid) : artist_id;
        Env::VarReader r(k, k);
        uint32_t key_size = 0;
        uint32_t val_size = 0;
        return r.MoveNext(nullptr, key_size, nullptr, val_size, 0);
    }

    void Print(const ContractID& cid, const Id& id) {
        Env::DocGroup gr1("");
        Env::DocAddBlob_T("id", id);
        Env::DocAddNum32("updated", updated);
        Env::DocAddText("label", label.data());
        Env::DocAddText("data", data.data());
        Env::DocAddNum("registered", registered);
        Env::DocAddNum("collections_count", collections_num);
        Env::DocAddNum("nfts_count", nfts_num);
        Env::DocAddNum("approved_cnt", approved_cnt);
        Env::DocAddText("status", StatusToString(status).data());
        {
            Env::DocGroup gr_sold("total_sold");
            Env::DocAddNum("count", total_sold);
            Env::DocAddNum("volume", total_sold_price);
            Env::DocAddNum32("aid", 0);
        }
    }

    bool ReadNext(Env::VarReader& r, Env::Key_T<Key>& key) {
        while (true) {
            uint32_t key_size = sizeof(key),
                     val_size = sizeof(Artist) + sizeof(data);
            if (!r.MoveNext(&key, key_size, this, val_size, 0))
                return false;

            if (sizeof(key) != key_size)
                continue;

            val_size -= sizeof(Artist);
            data[std::min(val_size, kDataMaxLen)] = 0;
            break;
        }

        Env::Key_T<gallery::Events::AddArtistLabel::Key> alk;
        alk.m_Prefix.m_Cid = key.m_Prefix.m_Cid;
        alk.m_KeyInContract.id = key.m_KeyInContract.id;

        Env::LogReader alr(alk, alk);
        uint32_t val_size = sizeof(label), key_size = sizeof(alk);

        alr.MoveNext(&alk, key_size, label.data(), val_size, 0);
        label[std::min(val_size, kLabelMaxLen)] = 0;

        return true;
    }
};
#pragma pack(pop)

#pragma pack(push, 0)
struct AppModerator : public gallery::Moderator {
    static inline PubKey id(const ContractID& cid) {
        return AppArtist::id(cid);
    }

    bool is_moderator(const ContractID& cid) {
        Id id = AppModerator::id(cid);
        bool exists = Read(cid, id);
        return exists && approved;
    }

    bool Read(const ContractID& cid, const Id& id) {
        Env::Key_T<Key> k;
        k.m_Prefix.m_Cid = cid;
        k.m_KeyInContract.id = id;
        return Env::VarReader::Read_T(k, Cast::Down<gallery::Moderator>(*this));
    }

    void Print(const ContractID& cid, const Id& id) {
        Env::DocGroup gr1("");
        Env::DocAddBlob_T("id", id);
        Env::DocAddNum32("updated", updated);
        Env::DocAddNum("registered", registered);
        Env::DocAddText("status", approved ? "approved" : "pending");
    }

    bool ReadNext(Env::VarReader& r, Env::Key_T<Key>& k) {
        return r.MoveNext_T(k, Cast::Down<gallery::Moderator>(*this));
    }
};
#pragma pack(pop)

#pragma pack(push, 0)
struct AppCollection : public gallery::Collection {
    std::array<char, kTotalMaxLen + 2> label_and_data;

    void Print(const ContractID& cid, Id id) {
        Env::DocGroup gr1("");
        Env::DocAddNum32("id", id);
        Env::DocAddNum32("updated", updated);
        Env::DocAddNum32("owned", _POD_(author) == AppArtist::id(cid));
        // DocAddText prints char[] until it meets \0 symbol, but
        // m_szLabelData contains label + data without \0 symbol between them
        char tmp = 0;
        std::swap(tmp, label_and_data[label_len]);
        Env::DocAddText("label", label_and_data.data());
        std::swap(tmp, label_and_data[label_len]);
        Env::DocAddText("data", label_and_data.data() + label_len);
        Env::DocAddBlob_T("author", author);
        Env::DocAddNum32("nfts_count", nfts_num);
        Env::DocAddNum32("approved_nfts_count", approved_nfts_num);
        Env::DocAddText("status", StatusToString(status).data());
        {
            Env::DocGroup gr_sold("total_sold");
            Env::DocAddNum("count", total_sold);
            Env::DocAddNum("volume", total_sold_price);
            Env::DocAddNum32("aid", 0);
        }
        {
            Env::DocGroup gr_price("max_price");
            Env::DocAddNum("value", max_sold.price.amount);
            Env::DocAddNum("aid", max_sold.price.aid);
            Env::DocAddNum32("nft_id", max_sold.nft_id);
        }
        {
            Env::DocGroup gr_price("min_price");
            Env::DocAddNum("value", min_sold.price.amount);
            Env::DocAddNum("aid", min_sold.price.aid);
            Env::DocAddNum32("nft_id", min_sold.nft_id);
        }
    }

    bool ReadNext(Env::VarReader& r, Env::Key_T<Key>& key) {
        while (true) {
            uint32_t key_size = sizeof(key),
                     val_size = sizeof(Collection) + sizeof(label_and_data);
            if (!r.MoveNext(&key, key_size, this, val_size, 0))
                return false;

            if (sizeof(key) != key_size)
                continue;

            val_size -= sizeof(Collection);
            label_and_data[std::min(val_size, kTotalMaxLen)] = 0;
            break;
        }
        return true;
    }
};
#pragma pack(pop)

#pragma pack(push, 0)
struct AppNft : public gallery::Nft {
    std::array<char, kLabelMaxLen + 1> label;
    std::array<char, kDataMaxLen + 1> data;

    void Print(const ContractID& cid, Id id) {
        Env::DocGroup gr1("");
        Env::DocAddNum32("id", id);
        Env::DocAddNum32("updated", updated);
        Env::DocAddText("status", StatusToString(status).data());
        Env::DocAddText("label", label.data());
        Env::DocAddText("data", data.data());
        Env::DocAddBlob_T("author", author);
        Env::DocAddNum32("collection", collection_id);
        if (aid)
            Env::DocAddBlob_T("checkout.aid", aid);

        if (!_POD_(owner).IsZero()) {
            uint32_t owned = IsOwner(cid, owner, 0) || IsOwner(cid, owner, id);
            Env::DocAddBlob_T("owner", owner);
            Env::DocAddNum("owned", owned);

            if (owned) {
                Env::Key_T<gallery::Events::Sell::Key> key;
                _POD_(key.m_Prefix.m_Cid) = cid;
                key.m_KeyInContract.nft_id = id;

                Env::LogReader r(key, key);
                gallery::Events::Sell evt;
                if (r.MoveNext_T(key, evt)) {
                    Env::DocGroup gr("first_sale");
                    Env::DocAddNum("height", r.m_Pos.m_Height);
                    Env::DocAddNum("amount", evt.price.amount);
                    Env::DocAddNum("aid", evt.price.aid);
                }
            }

            if (price.amount) {
                Env::DocGroup gr_price("price");
                Env::DocAddNum("aid", price.aid);
                Env::DocAddNum("amount", price.amount);
            }
        }

        // Env::Key_T<gallery::Like::Key> like_key;
        // like_key.m_Prefix.m_Cid = cid;
        // like_key.m_KeyInContract.artist_id = AppArtist::id(cid);
        // like_key.m_KeyInContract.nft_id = id;
        // uint32_t value{};
        // Env::VarReader::Read_T(like_key, value);

        Env::DocAddNum32("likes", likes_number);
        // if (value)
        // Env::DocAddNum("my_like", value);
    }

    bool ReadWithoutData(const ContractID& cid, Id id) {
        Env::Key_T<Key> k;
        k.m_Prefix.m_Cid = cid;
        k.m_KeyInContract.id = Utils::FromBE(id);
        return Env::VarReader::Read_T(k, Cast::Down<Nft>(*this));
    }

    bool ReadNext(Env::VarReader& r, Env::Key_T<Key>& key) {
        uint32_t key_size = sizeof(key), val_size = sizeof(Nft);
        if (!r.MoveNext(&key, key_size, this, val_size, 0))
            return false;

        auto id = Utils::FromBE(key.m_KeyInContract.id);
        auto cid = key.m_Prefix.m_Cid;

        Env::Key_T<gallery::Events::AddNftData::Key> adk;
        adk.m_Prefix.m_Cid = cid;
        adk.m_KeyInContract.nft_id = id;
        adk.m_KeyInContract.artist_id = author;

        Env::LogReader adr(adk, adk);
        key_size = sizeof(adk), val_size = sizeof(data);
        adr.MoveNext(&adk, key_size, data.data(), val_size, 0);
        data[std::min(val_size, kDataMaxLen)] = 0;

        Env::Key_T<gallery::Events::AddNftLabel::Key> alk;
        alk.m_Prefix.m_Cid = cid;
        alk.m_KeyInContract.nft_id = id;
        alk.m_KeyInContract.artist_id = author;

        Env::LogReader alr(alk, alk);
        key_size = sizeof(alk), val_size = sizeof(label);
        alr.MoveNext(&alk, key_size, label.data(), val_size, 0);
        label[std::min(val_size, kLabelMaxLen)] = 0;

        return true;
    }
};
#pragma pack(pop)

template <class T>
class GalleryObjectPrinter {
public:
    explicit GalleryObjectPrinter(const ContractID& cid) : cid_{cid} {
    }

    typename T::Id Print(const typename T::Id& from, size_t count) {
        Env::Key_T<typename T::Key> k0, k1;
        _POD_(k0.m_Prefix.m_Cid) = cid_;
        _POD_(k1.m_Prefix.m_Cid) = cid_;
        if constexpr (std::is_same_v<typename T::Id, PubKey>) {
            _POD_(k0.m_KeyInContract.id) = from;
            _POD_(k1.m_KeyInContract.id).SetObject(0xff);
        } else {
            k0.m_KeyInContract.id = Utils::FromBE(from);
            k1.m_KeyInContract.id =
                Utils::FromBE(static_cast<typename T::Id>(from + count));
        }

        Env::VarReader r(k0, k1);
        Env::DocArray gr0("items");

        size_t printed_cnt = 0;
        typename T::Id last_printed{};
        _POD_(last_printed) = k0.m_KeyInContract.id;
        while (object_.ReadNext(r, k0) && printed_cnt++ < count) {
            if constexpr (std::is_same_v<typename T::Id, PubKey>)
                object_.Print(cid_, k0.m_KeyInContract.id);
            else
                object_.Print(cid_, Utils::FromBE(k0.m_KeyInContract.id));
            last_printed = k0.m_KeyInContract.id;
        }
        typename T::Id not_printed{};
        if constexpr (std::is_same_v<typename T::Id, PubKey>) {
            not_printed = k0.m_KeyInContract.id;
        } else {
            not_printed = Utils::FromBE(k0.m_KeyInContract.id);
            last_printed = Utils::FromBE(last_printed);
        }
        return _POD_(not_printed) != last_printed ? not_printed
                                                  : typename T::Id{};
    }

    void Print() {
        Env::Key_T<typename T::Key> k0, k1;
        _POD_(k0.m_Prefix.m_Cid) = cid_;
        _POD_(k1.m_Prefix.m_Cid) = cid_;
        _POD_(k0.m_KeyInContract.id).SetZero();
        _POD_(k1.m_KeyInContract.id).SetObject(0xff);

        Env::VarReader r(k0, k1);
        Env::DocArray gr0("items");

        while (object_.ReadNext(r, k0)) {
            if constexpr (std::is_same_v<typename T::Id, PubKey>)
                object_.Print(cid_, k0.m_KeyInContract.id);
            else
                object_.Print(cid_, Utils::FromBE(k0.m_KeyInContract.id));
        }
    }

    void Print(std::string_view ids) {
        Env::DocArray gr0("items");

        std::vector<typename T::Id> ids_vec(ParseIds<typename T::Id>(ids));
        for (auto id : ids_vec) {
            Print(id);
        }
    }

    bool Print(const typename T::Id& id) {
        Env::Key_T<typename T::Key> k;
        k.m_Prefix.m_Cid = cid_;
        if constexpr (std::is_same_v<typename T::Id, PubKey>)
            _POD_(k.m_KeyInContract.id) = id;
        else
            _POD_(k.m_KeyInContract.id) = Utils::FromBE(id);

        Env::VarReader r(k, k);
        if (!object_.ReadNext(r, k))
            return false;

        if constexpr (std::is_same_v<typename T::Id, PubKey>)
            object_.Print(cid_, k.m_KeyInContract.id);
        else
            object_.Print(cid_, Utils::FromBE(k.m_KeyInContract.id));
        return true;
    }

    template <class HeightIdx>
    Height Print(Height h0, uint32_t count) {
        Env::Key_T<typename HeightIdx::Key> k0, k1;
        k0.m_Prefix.m_Cid = cid_;
        k1.m_Prefix.m_Cid = cid_;
        k0.m_KeyInContract.id = Utils::FromBE(h0);
        k1.m_KeyInContract.id = kMaxHeight;
        _POD_(k0.m_KeyInContract.t_id).SetZero();
        _POD_(k1.m_KeyInContract.t_id).SetObject(0xff);

        Env::VarReader r(k0, k1);

        Height prev_h = kMaxHeight, last_printed_h = kMaxHeight;
        {
            bool exists;
            uint32_t cur_cnt = 0;
            Env::DocArray gr0("items");
            while (r.MoveNext_T(k0, exists) &&
                   (cur_cnt++ < count || prev_h == k0.m_KeyInContract.id)) {
                if constexpr (std::is_same_v<typename T::Id, PubKey>)
                    Print(k0.m_KeyInContract.t_id);
                else
                    Print(Utils::FromBE(k0.m_KeyInContract.t_id));
                last_printed_h = Utils::FromBE(k0.m_KeyInContract.id);
                prev_h = k0.m_KeyInContract.id;
            }
        }
        return last_printed_h;
    }

private:
    T object_;
    ContractID cid_;
};

struct BalanceWalker {
    Env::VarReaderEx<true> reader;
    Env::Key_T<gallery::Payout::Key> key;
    gallery::Payout payout;
    std::map<AssetID, Amount> totals;

    void Enum(const ContractID& cid) {
        key.m_Prefix.m_Cid = cid;
        _POD_(key.m_KeyInContract).SetZero();

        Env::Key_T<gallery::Payout::Key> k1;
        k1.m_Prefix.m_Cid = cid;
        _POD_(k1.m_KeyInContract).SetObject(0xff);

        reader.Enum_T(key, k1);
    }

    bool MoveNext() {
        return reader.MoveNext_T(key, payout);
    }

    void Print() const {
        Env::DocAddNum("aid", key.m_KeyInContract.aid);
        Env::DocAddNum("amount", payout.amount);
    }

    void AddToTotals() {
        totals[key.m_KeyInContract.aid] += payout.amount;
    }

    void PrintTotals() {
        Env::DocArray gr0("totals");
        for (auto it : totals) {
            Env::DocGroup gr1("");
            Env::DocAddNum("aid", it.first);
            Env::DocAddNum("amount", it.second);
        }
    }
};

struct BalanceWalkerOwner : public BalanceWalker {
    ContractID cid;

    explicit BalanceWalkerOwner(const ContractID& cid) : cid{cid} {
    }

    void Enum(const ContractID& cid) {
        BalanceWalker::Enum(cid);
    }

    bool MoveNext() {
        while (true) {
            if (!BalanceWalker::MoveNext())
                return false;

            if (IsOwner(cid, key.m_KeyInContract.user,
                        key.m_KeyInContract.nft_id) ||
                IsOwner(cid, key.m_KeyInContract.user, 0))
                break;
        }
        return true;
    }
};

/*
 * Functions
 */
void OnError(std::string_view err) {
    Env::DocAddText("error", err.data());
}

template <class Id>
std::vector<Id> ParseIds(std::string_view ids) {
    union {
        Id id;
        uint8_t a[sizeof(Id)];
    } blob;

    _POD_(blob).SetZero();

    std::vector<Id> res;
    int pos = 0;
    for (auto c : ids) {
        if (c == ';') {
            res.push_back(blob.id);
            _POD_(blob.id).SetZero();
            pos = 0;
        } else {
            if constexpr (std::is_same_v<Id, PubKey>) {
                uint8_t val = isdigit(c) ? (c - '0') : (c - 'a' + 10);
                blob.a[pos / 2] |= (val << (4 * !(pos & 1)));
                ++pos;
            } else {
                blob.id = blob.id * 10 + c - '0';
            }
        }
    }
    if (!_POD_(blob.id).IsZero())
        res.push_back(blob.id);
    return res;
}

gallery::Status StringToStatus(std::string_view str_status) {
    gallery::Status ret_status;
    if (str_status == "pending") {
        ret_status = gallery::Status::kPending;
    } else if (str_status == "approved") {
        ret_status = gallery::Status::kApproved;
    } else if (str_status == "rejected") {
        ret_status = gallery::Status::kRejected;
    } else {
        ret_status = gallery::Status::kNone;
    }
    return ret_status;
}

const std::string_view StatusToString(gallery::Status status) {
    if (status == gallery::Status::kPending) {
        return "pending";
    } else if (status == gallery::Status::kApproved) {
        return "approved";
    } else if (status == gallery::Status::kRejected) {
        return "rejected";
    } else {
        return "none";
    }
}

static inline bool IsOwner(const ContractID& cid, const PubKey& owner,
                           gallery::Nft::Id nft_id) {
    auto zero_km = key_material::Owner{kEmptyCid, nft_id};
    auto old_km = key_material::Owner{kOldGalleryCid, nft_id};
    auto new_km = key_material::Owner{cid, nft_id};
    return (_POD_(owner) == zero_km.Get() || _POD_(owner) == old_km.Get() ||
            _POD_(owner) == new_km.Get());
}

gallery::Collection::Id collection_id_by_label(const ContractID& cid,
                                               std::string_view label) {
    Env::Key_T<gallery::Collection::LabelKey> lk{};
    lk.m_Prefix.m_Cid = cid;
    lk.m_KeyInContract.label_hash = gallery::GetLabelHash(label);
    lk.m_KeyInContract.artist_id = AppArtist::id(cid);
    gallery::Collection::Id collection_id{};
    Env::VarReader::Read_T(lk, collection_id);
    return collection_id;
}

gallery::Artist::Id artist_id_by_label(const ContractID& cid,
                                       std::string_view label) {
    Env::Key_T<gallery::Artist::LabelKey> lk{};
    lk.m_Prefix.m_Cid = cid;
    lk.m_KeyInContract.label_hash = gallery::GetLabelHash(label);
    gallery::Artist::Id artist_id{};
    Env::VarReader::Read_T(lk, artist_id);
    return artist_id;
}

void SetNftStatusCommon(const ContractID& cid, const PubKey& signer,
                        const SigRequest* sig) {
    char buf[256];
    Env::DocGetText("ids", buf, sizeof(buf));
    std::vector<gallery::Nft::Id> ids_vec(ParseIds<gallery::Nft::Id>(buf));

    size_t args_size = sizeof(gallery::method::SetNftStatus) +
                       sizeof(gallery::Nft::Id) * ids_vec.size();
    std::unique_ptr<gallery::method::SetNftStatus> args(
        static_cast<gallery::method::SetNftStatus*>(::operator new(args_size)));

    Env::DocGetText("status", buf, sizeof(buf));
    args->status = StringToStatus(buf);
    if (args->status == gallery::Status::kNone) {
        OnError("bad status");
        return;
    }

    args->signer = signer;
    args->ids_num = ids_vec.size();
    Env::Memcpy(args->ids, ids_vec.data(),
                sizeof(gallery::Nft::Id) * ids_vec.size());

    uint32_t charge =
        Env::Cost::LoadVar_For(sizeof(gallery::State)) +
        4 * args->ids_num * Env::Cost::SaveVar_For(sizeof(bool)) +
        args->ids_num * Env::Cost::LoadVar_For(sizeof(gallery::Nft)) +
        args->ids_num * Env::Cost::SaveVar_For(sizeof(gallery::Nft)) +
        args->ids_num *
            Env::Cost::LoadVar_For(sizeof(gallery::Collection) +
                                   gallery::Collection::kTotalMaxLen) +
        args->ids_num *
            Env::Cost::SaveVar_For(sizeof(gallery::Collection) +
                                   gallery::Collection::kTotalMaxLen) +
        Env::Cost::LoadVar_For(sizeof(gallery::Moderator)) + Env::Cost::AddSig +
        Env::Cost::Cycle * 300;

    Env::GenerateKernel(&cid, args->kMethod, args.get(), args_size, nullptr, 0,
                        sig, 1, "Update nfts' status", charge);
}

void SetArtistStatusCommon(const ContractID& cid, const PubKey& signer,
                           const SigRequest* sig) {
    char buf[1024];
    Env::DocGetText("ids", buf, sizeof(buf));
    std::vector<gallery::Artist::Id> ids_vec(
        ParseIds<gallery::Artist::Id>(buf));

    size_t args_size = sizeof(gallery::method::SetArtistStatus) +
                       sizeof(gallery::Artist::Id) * ids_vec.size();
    std::unique_ptr<gallery::method::SetArtistStatus> args(
        static_cast<gallery::method::SetArtistStatus*>(
            ::operator new(args_size)));

    Env::DocGetText("status", buf, sizeof(buf));
    args->status = StringToStatus(buf);
    if (args->status == gallery::Status::kNone) {
        OnError("bad status");
        return;
    }

    args->signer = signer;
    args->ids_num = ids_vec.size();
    Env::Memcpy(args->ids, ids_vec.data(),
                sizeof(gallery::Artist::Id) * ids_vec.size());

    uint32_t charge =
        Env::Cost::LoadVar_For(sizeof(gallery::State)) +
        2 * args->ids_num * Env::Cost::SaveVar_For(sizeof(bool)) +
        args->ids_num * Env::Cost::LoadVar_For(sizeof(gallery::Artist) +
                                               gallery::Artist::kDataMaxLen) +
        args->ids_num * Env::Cost::SaveVar_For(sizeof(gallery::Artist) +
                                               gallery::Artist::kDataMaxLen) +
        Env::Cost::LoadVar_For(sizeof(gallery::Moderator)) + Env::Cost::AddSig +
        Env::Cost::Cycle * 300;

    Env::GenerateKernel(&cid, args->kMethod, args.get(), args_size, nullptr, 0,
                        sig, 1, "Update artists' status", charge);
}

void SetCollectionStatusCommon(const ContractID& cid, const PubKey& signer,
                               const SigRequest* sig) {
    char buf[256];
    Env::DocGetText("ids", buf, sizeof(buf));

    std::vector<gallery::Collection::Id> ids_vec(
        ParseIds<gallery::Collection::Id>(buf));

    size_t args_size = sizeof(gallery::method::SetCollectionStatus) +
                       sizeof(gallery::Collection::Id) * ids_vec.size();
    std::unique_ptr<gallery::method::SetCollectionStatus> args(
        static_cast<gallery::method::SetCollectionStatus*>(
            ::operator new(args_size)));

    Env::DocGetText("status", buf, sizeof(buf));
    args->status = StringToStatus(buf);
    if (args->status == gallery::Status::kNone)
        return;

    args->signer = signer;
    args->ids_num = ids_vec.size();
    Env::Memcpy(args->ids, ids_vec.data(),
                sizeof(gallery::Collection::Id) * ids_vec.size());

    uint32_t charge =
        Env::Cost::LoadVar_For(sizeof(gallery::State)) +
        2 * args->ids_num * Env::Cost::SaveVar_For(sizeof(bool)) +
        args->ids_num *
            Env::Cost::LoadVar_For(sizeof(gallery::Collection) +
                                   gallery::Collection::kTotalMaxLen) +
        args->ids_num *
            Env::Cost::SaveVar_For(sizeof(gallery::Collection) +
                                   gallery::Collection::kTotalMaxLen) +
        Env::Cost::LoadVar_For(sizeof(gallery::Moderator)) + Env::Cost::AddSig +
        Env::Cost::Cycle * 300;

    Env::GenerateKernel(&cid, args->kMethod, args.get(), args_size, nullptr, 0,
                        sig, 1, "Update collections' status", charge);
}

/*
 * On_Methods
 */
ON_METHOD(manager, create_contract) {
    gallery::method::Init args{};
    args.config.vote_reward.aid = vote_reward_aid;
    args.config.vote_reward.amount = vote_reward_amount;
    key_material::Admin().get_Pk(args.config.admin_id);
    if (!kVerInfo.FillDeployArgs(args.settings, &args.config.admin_id)) {
        return;
    }
    Env::GenerateKernel(nullptr, 0, &args, sizeof(args), nullptr, 0, nullptr, 0,
                        "Deploy gallery contract",
                        Upgradable3::Manager::get_ChargeDeploy() * 2);
}

ON_METHOD(manager, schedule_upgrade) {
    key_material::Admin kid{};
    Upgradable3::Manager::MultiSigRitual::Perform_ScheduleUpgrade(
        kVerInfo, cid, kid, target_height);
}

ON_METHOD(manager, explicit_upgrade) {
    key_material::Admin kid{};
    Upgradable3::Manager::MultiSigRitual::Perform_ExplicitUpgrade(cid);
}

ON_METHOD(manager, replace_admin) {
    key_material::Admin kid{};
    Upgradable3::Manager::MultiSigRitual::Perform_ReplaceAdmin(cid, kid, iAdmin,
                                                               pk);
}

ON_METHOD(manager, set_min_approvers) {
    key_material::Admin kid{};
    Upgradable3::Manager::MultiSigRitual::Perform_SetApprovers(cid, kid,
                                                               newVal);
}

ON_METHOD(manager, view) {
    EnumAndDumpContracts(gallery::kSid0);
}

ON_METHOD(manager, set_moderator) {
    gallery::method::SetModerator args{};
    args.id = id;
    args.approved = enable;
    key_material::Admin akm{};
    Env::GenerateKernel(&cid, args.kMethod, &args, sizeof(args), nullptr, 0,
                        &akm, 1, "Set moderator", 0);
}

ON_METHOD(manager, view_params) {
    StatePlus s;
    if (!s.Init(cid)) {
        OnError("no contract with such cid");
        return;
    }

    PubKey admin_id{};
    key_material::Admin().get_Pk(admin_id);

    AppModerator moder;
    Env::DocAddNum32("is_admin", (_POD_(s.config.admin_id) == admin_id));
    Env::DocAddNum32("is_moderator", moder.is_moderator(cid));
    Env::DocAddNum("fee_base", s.fee_base);
    Env::DocAddNum("rate_limit", s.rate_limit);

    {
        Env::DocGroup gr1("vote_reward");
        Env::DocAddNum("aid", s.config.vote_reward.aid);
        Env::DocAddNum("amount", s.config.vote_reward.amount);
        Env::DocAddNum("balance", s.vote_balance);
    }
}

ON_METHOD(manager, view_artists_stats) {
    StatePlus s;
    if (!s.Init(cid)) {
        OnError("no contract with such cid");
        return;
    }
    Env::DocAddNum32("total", s.total_artists);
}

ON_METHOD(manager, view_moderators_stats) {
    StatePlus s;
    if (!s.Init(cid)) {
        OnError("no contract with such cid");
        return;
    }
    Env::DocAddNum32("total", s.total_moderators);
}

ON_METHOD(manager, view_collections_stats) {
    StatePlus s;
    if (!s.Init(cid)) {
        OnError("no contract with such cid");
        return;
    }
    Env::DocAddNum32("total", s.total_collections);
}

ON_METHOD(manager, view_nfts_stats) {
    StatePlus s;
    if (!s.Init(cid)) {
        OnError("no contract with such cid");
        return;
    }
    Env::DocAddNum32("total", s.total_nfts);
}

ON_METHOD(manager, view_artists) {
    char buf[1024];
    int buf_len = Env::DocGetText("ids", buf, sizeof(buf));

    using HeightArtistIdx =
        gallery::Index<gallery::Tag::kHeightArtistIdx, Height, gallery::Artist>;

    GalleryObjectPrinter<AppArtist> a{cid};
    if (count && h0) {
        Height last_printed_h = a.Print<HeightArtistIdx>(h0, count);
        if (last_printed_h != kMaxHeight)
            Env::DocAddNum("processed_height", last_printed_h);
    } else if (count && !_POD_(id0).IsZero()) {
        gallery::Artist::Id not_printed = a.Print(id0, count);
        Env::DocAddBlob_T("next_id", not_printed);
    } else if (buf_len) {
        a.Print(buf);
    } else {
        a.Print();
    }
}

ON_METHOD(manager, view_likes) {
    PubKey artist_id_;
    if (_POD_(artist_id).IsZero()) {
        _POD_(artist_id_) = AppArtist::id(cid);
    } else {
        _POD_(artist_id_) = artist_id;
    }

    if (id0 && count) {
        Env::Key_T<gallery::Like::Key> k0, k1;
        _POD_(k0.m_Prefix.m_Cid) = cid;
        _POD_(k1.m_Prefix.m_Cid) = cid;
        k0.m_KeyInContract.artist_id = artist_id_;
        k1.m_KeyInContract.artist_id = artist_id_;
        k0.m_KeyInContract.nft_id = Utils::FromBE(id0);
        k1.m_KeyInContract.nft_id =
            std::numeric_limits<gallery::Nft::Id>::max();

        Env::VarReader r(k0, k1);
        uint32_t not_printed{};
        {
            Env::DocArray root{"items"};
            uint32_t printed_cnt{};
            uint32_t last_printed = k0.m_KeyInContract.nft_id;
            gallery::Like like{};
            while (r.MoveNext_T(k0, like) && printed_cnt++ < count) {
                Env::DocGroup gr{""};
                Env::DocAddNum("id", Utils::FromBE(k0.m_KeyInContract.nft_id));
                Env::DocAddNum("nft", Utils::FromBE(k0.m_KeyInContract.nft_id));
                Env::DocAddNum("value", like.value);
                Env::DocAddNum("updated", like.updated);
                last_printed = k0.m_KeyInContract.nft_id;
            }
            if (last_printed != k0.m_KeyInContract.nft_id)
                not_printed = Utils::FromBE(k0.m_KeyInContract.nft_id);
        }
        Env::DocAddNum("next_id", not_printed);
    } else {
        Env::Key_T<gallery::Events::Like::Key> k0, k1;
        _POD_(k0.m_Prefix.m_Cid) = cid;
        _POD_(k1.m_Prefix.m_Cid) = cid;
        k0.m_KeyInContract.artist_id = artist_id_;
        k1.m_KeyInContract.artist_id = artist_id_;
        k0.m_KeyInContract.nft_id = 0;
        k1.m_KeyInContract.nft_id =
            std::numeric_limits<gallery::Nft::Id>::max();

        HeightPos pos_min{}, pos_max{};
        pos_min.m_Height = h0;
        pos_max.m_Height = hn ? hn : Env::get_Height();

        Env::LogReader r(k0, k1, &pos_min, &pos_max);
        Env::DocArray root{"items"};
        uint32_t value{};
        uint32_t cur_cnt{};
        while (r.MoveNext_T(k0, value) &&
               (!count || (count && cur_cnt++ < count))) {
            Env::DocGroup gr{""};
            Env::DocAddNum("id", k0.m_KeyInContract.nft_id);
            Env::DocAddNum("nft", k0.m_KeyInContract.nft_id);
            Env::DocAddNum("value", value);
            Env::DocAddNum("updated", r.m_Pos.m_Height);
        }
    }
}

ON_METHOD(manager, view_moderators) {
    char buf[1024];
    int buf_len = Env::DocGetText("ids", buf, sizeof(buf));

    GalleryObjectPrinter<AppModerator> m{cid};
    if (buf_len) {
        m.Print(buf);
    } else {
        m.Print();
    }
}

ON_METHOD(manager, view_collections) {
    char buf[256];
    int buf_len = Env::DocGetText("ids", buf, sizeof(buf));

    using HeightCollectionIdx =
        gallery::Index<gallery::Tag::kHeightCollectionIdx, Height,
                       gallery::Collection>;

    GalleryObjectPrinter<AppCollection> c{cid};
    if (count && h0) {
        Height last_printed_h = c.Print<HeightCollectionIdx>(h0, count);
        if (last_printed_h != kMaxHeight)
            Env::DocAddNum("processed_height", last_printed_h);
    } else if (count && id0) {
        gallery::Collection::Id not_printed = c.Print(id0, count);
        Env::DocAddNum("next_id", not_printed);
    } else if (buf_len) {
        c.Print(buf);
    } else {
        c.Print();
    }
}

ON_METHOD(artist, set_nft) {
    struct {
        gallery::method::SetNft args;
        char label_and_data[gallery::Nft::kTotalMaxLen];
    } d;

    key_material::Owner km{AppArtist::key_material(cid)};
    d.args.artist_id = km.Get();
    d.args.collection_id = collection_id;
    d.args.price.amount = amount;
    d.args.price.aid = aid;

    if (!collection_id) {
        OnError("collection_id must be specified");
        return;
    }

    uint32_t label_size =
        Env::DocGetText("label", d.label_and_data,
                        gallery::Nft::kLabelMaxLen + 1);  // including 0-term

    if (label_size > gallery::Nft::kLabelMaxLen + 1) {
        OnError("label is too long");
        return;
    }
    if (label_size < 2) {
        OnError("label is missing");
        return;
    }

    d.args.label_len = label_size - 1;

    uint32_t data_size =
        Env::DocGetText("data", d.label_and_data + d.args.label_len,
                        gallery::Nft::kDataMaxLen + 1);

    if (data_size < 2) {
        OnError("data must be specified");
        return;
    }
    if (data_size > gallery::Nft::kDataMaxLen + 1) {
        OnError("data is too long");
        return;
    }

    d.args.data_len = data_size ? data_size - 1 : data_size;
    uint32_t arg_size = sizeof(d.args) + d.args.label_len + d.args.data_len;

    SigRequest sig{};
    sig.m_pID = &km;
    sig.m_nID = sizeof(km);

    uint32_t charge =
        Env::Cost::LoadVar_For(sizeof(gallery::State)) +
        Env::Cost::SaveVar_For(sizeof(gallery::State)) +
        6 * Env::Cost::SaveVar_For(sizeof(bool)) +
        Env::Cost::LoadVar_For(sizeof(gallery::Collection) +
                               gallery::Collection::kTotalMaxLen) +
        Env::Cost::SaveVar_For(sizeof(gallery::Collection) +
                               gallery::Collection::kTotalMaxLen) +
        Env::Cost::LoadVar_For(sizeof(gallery::Artist) +
                               gallery::Artist::kDataMaxLen) +
        Env::Cost::SaveVar_For(sizeof(gallery::Artist) +
                               gallery::Artist::kDataMaxLen) +
        Env::Cost::SaveVar_For(sizeof(gallery::Nft)) +
        Env::Cost::Log_For(data_size) + Env::Cost::Log_For(label_size) +
        Env::Cost::AddSig + Env::Cost::Cycle * 1500;

    StatePlus s;
    if (!s.Init(cid)) {
        OnError("no contract with such cid");
        return;
    }

    Env::GenerateKernel(&cid, d.args.kMethod, &d, arg_size, nullptr, 0, &sig, 1,
                        "Upload nft", charge + s.fee_base / 10);
}

ON_METHOD(user, add_rewards) {
    StatePlus s;
    if (!s.Init(cid)) {
        OnError("no contract with such cid");
        return;
    }

    gallery::method::AddVoteRewards args{};
    args.amount = amount;

    if (!args.amount) {
        OnError("no rewards");
        return;
    }

    FundsChange fc{};
    fc.m_Aid = s.config.vote_reward.aid;
    fc.m_Amount = args.amount;
    fc.m_Consume = 1;

    Env::GenerateKernel(&cid, args.kMethod, &args, sizeof(args), &fc, 1,
                        nullptr, 0, "Add voting rewards", 0);
}

ON_METHOD(manager, get_id) {
    PubKey pk{};
    key_material::Admin().get_Pk(pk);
    Env::DocAddBlob_T("id", pk);
}

ON_METHOD(manager, view_balance) {
    BalanceWalker wlk{};
    {
        Env::DocArray gr0("items");
        for (wlk.Enum(cid); wlk.MoveNext();) {
            Env::DocGroup gr1("");
            wlk.Print();
            wlk.AddToTotals();
        }
    }
    wlk.PrintTotals();
}

ON_METHOD(manager, set_nft_status) {
    key_material::Admin kid{};
    PubKey signer{};
    kid.get_Pk(signer);
    SetNftStatusCommon(cid, signer, &kid);
}

ON_METHOD(moderator, set_nft_status) {
    key_material::Owner km{AppArtist::key_material(cid)};
    SigRequest sig{};
    sig.m_pID = &km;
    sig.m_nID = sizeof(km);
    SetNftStatusCommon(cid, km.Get(), &sig);
}

ON_METHOD(moderator, set_artist_status) {
    key_material::Owner km{AppArtist::key_material(cid)};
    SigRequest sig{};
    sig.m_pID = &km;
    sig.m_nID = sizeof(km);
    SetArtistStatusCommon(cid, km.Get(), &sig);
}

ON_METHOD(manager, set_artist_status) {
    key_material::Admin kid{};
    PubKey signer{};
    kid.get_Pk(signer);
    SetArtistStatusCommon(cid, signer, &kid);
}

ON_METHOD(manager, set_collection_status) {
    key_material::Admin kid{};
    PubKey signer{};
    kid.get_Pk(signer);
    SetCollectionStatusCommon(cid, signer, &kid);
}

ON_METHOD(moderator, set_collection_status) {
    key_material::Owner km{AppArtist::key_material(cid)};
    SigRequest sig{};
    sig.m_pID = &km;
    sig.m_nID = sizeof(km);
    SetCollectionStatusCommon(cid, km.Get(), &sig);
}

ON_METHOD(manager, set_fee_base) {
    gallery::method::SetFeeBase args{};
    args.amount = amount;
    key_material::Admin kid{};
    Env::GenerateKernel(&cid, args.kMethod, &args, sizeof(args), nullptr, 0,
                        &kid, 1, "Set fee base", 0);
}

ON_METHOD(manager, set_rate_limit) {
    gallery::method::SetRateLimit args{};
    args.amount = amount;
    key_material::Admin kid{};
    Env::GenerateKernel(&cid, args.kMethod, &args, sizeof(args), nullptr, 0,
                        &kid, 1, "Set rate limit", 0);
}

ON_METHOD(artist, set_artist) {
    struct {
        gallery::method::SetArtist args;
        char label_and_data[gallery::Artist::kTotalMaxLen + 1];
    } d;

    uint32_t label_size =
        Env::DocGetText("label", d.label_and_data,
                        gallery::Artist::kLabelMaxLen + 1);  // including 0-term

    if (label_size > gallery::Artist::kLabelMaxLen + 1) {  // plus \0
        OnError("label is too long");
        return;
    }
    if (label_size < 2) {
        OnError("label is missing");
        return;
    }

    d.args.label_len = label_size - 1;

    std::string_view label(d.label_and_data, d.args.label_len);
    gallery::Artist::Id artist_id = artist_id_by_label(cid, label);
    bool artist_exists = AppArtist::Exists(cid, {});

    if (artist_exists &&
        (_POD_(artist_id).IsZero() || _POD_(artist_id) != AppArtist::id(cid))) {
        OnError("label is immutable");
        return;
    }
    if (!artist_exists && !_POD_(artist_id).IsZero()) {
        OnError("label already exists");
        return;
    }

    uint32_t data_size =
        Env::DocGetText("data", d.label_and_data + d.args.label_len,
                        gallery::Artist::kDataMaxLen + 1);  // including 0-term

    if (data_size > gallery::Artist::kDataMaxLen + 1) {
        OnError("data is too long");
        return;
    }

    d.args.data_len = data_size ? data_size - 1 : data_size;

    uint32_t args_size = sizeof(d.args) + d.args.label_len + d.args.data_len;

    key_material::Owner km{AppArtist::key_material(cid)};
    d.args.artist_id = km.Get();

    SigRequest sig{};
    sig.m_pID = &km;
    sig.m_nID = sizeof(km);

    std::string_view comment =
        artist_exists ? "Updating artist's info" : "Becoming an artist";

    StatePlus s;
    if (!s.Init(cid)) {
        OnError("no contract with such cid");
        return;
    }

    uint32_t charge =
        (!artist_exists ? Env::Cost::LoadVar_For(sizeof(gallery::State)) : 0) +
        (!artist_exists ? Env::Cost::SaveVar_For(sizeof(gallery::State)) : 0) +
        (!artist_exists ? Env::Cost::Log_For(label_size) : 0) +
        Env::Cost::MemOpPerByte * (sizeof(gallery::Artist) + data_size) +
        Env::Cost::LoadVar_For(sizeof(gallery::Artist) +
                               gallery::Artist::kDataMaxLen) +
        Env::Cost::SaveVar_For(sizeof(gallery::Artist) + data_size) +
        2 * Env::Cost::SaveVar_For(sizeof(bool)) + Env::Cost::AddSig;

    Env::GenerateKernel(&cid, d.args.kMethod, &d, args_size, nullptr, 0, &sig,
                        1, comment.data(),
                        charge + (artist_exists ? 0 : s.fee_base / 10));
}

ON_METHOD(artist, set_collection) {
    struct {
        gallery::method::SetCollection args;
        char label_and_data[gallery::Collection::kTotalMaxLen + 2];
    } d;

    key_material::Owner km{AppArtist::key_material(cid)};
    d.args.artist_id = km.Get();
    d.args.collection_id = id;

    uint32_t label_size = Env::DocGetText(
        "label", d.label_and_data,
        gallery::Collection::kLabelMaxLen + 1);  // including 0-term

    if (label_size > gallery::Collection::kLabelMaxLen + 1) {  // plus \0
        OnError("label is too long");
        return;
    }
    if (label_size < 2) {
        OnError("label is missing");
        return;
    }

    d.args.label_len = label_size - 1;

    std::string_view label(d.label_and_data, d.args.label_len);
    gallery::Collection::Id collection_id = collection_id_by_label(cid, label);
    if ((d.args.collection_id && collection_id &&
         collection_id != d.args.collection_id) ||
        (!d.args.collection_id && collection_id)) {
        OnError("label already exists");
        return;
    }

    uint32_t data_size = Env::DocGetText(
        "data", d.label_and_data + d.args.label_len,
        gallery::Collection::kDataMaxLen + 1);  // including 0-term

    if (data_size > gallery::Collection::kDataMaxLen + 1) {
        OnError("data too long");
        return;
    }

    d.args.data_len = data_size ? data_size - 1 : data_size;
    uint32_t args_size = sizeof(d.args) + d.args.data_len + d.args.label_len;

    SigRequest sig{};
    sig.m_pID = &km;
    sig.m_nID = sizeof(km);

    StatePlus s;
    if (!s.Init(cid)) {
        OnError("no contract with such cid");
        return;
    }

    uint32_t charge =
        Env::Cost::LoadVar_For(sizeof(gallery::Collection) +
                               gallery::Collection::kTotalMaxLen) +
        Env::Cost::SaveVar_For(sizeof(gallery::Collection) + data_size +
                               label_size) +
        (!collection_id ? Env::Cost::LoadVar_For(sizeof(gallery::State)) : 0) +
        (!collection_id ? Env::Cost::SaveVar_For(sizeof(gallery::State)) : 0) +
        (!collection_id ? 2 * Env::Cost::SaveVar_For(sizeof(bool)) : 0) +
        (!collection_id ? Env::Cost::LoadVar_For(sizeof(gallery::Artist) +
                                                 gallery::Artist::kDataMaxLen)
                        : 0) +
        (!collection_id ? Env::Cost::SaveVar_For(sizeof(gallery::Artist) +
                                                 gallery::Artist::kDataMaxLen)
                        : 0) +
        2 * Env::Cost::SaveVar_For(sizeof(gallery::Collection::Id)) +
        2 * Env::Cost::SaveVar_For(sizeof(bool)) + Env::Cost::AddSig +
        Env::Cost::MemOpPerByte *
            (sizeof(gallery::Collection) + label_size + data_size);

    Env::GenerateKernel(&cid, d.args.kMethod, &d.args, args_size, nullptr, 0,
                        &sig, 1, "Set collection",
                        charge + (id ? 0 : s.fee_base / 10));
}

ON_METHOD(artist, get_id) {
    Env::DocAddBlob_T("id", AppArtist::id(cid));
}

ON_METHOD(manager, view_nft_sales) {
    Env::Key_T<gallery::Events::Sell::Key> key{};
    _POD_(key.m_Prefix.m_Cid) = cid;
    key.m_KeyInContract.nft_id = id;

    Env::DocArray gr0("sales");

    for (Env::LogReader r(key, key);;) {
        gallery::Events::Sell evt{};
        if (!r.MoveNext_T(key, evt))
            break;

        Env::DocGroup gr("");
        Env::DocAddNum("height", r.m_Pos.m_Height);
        Env::DocAddNum("amount", evt.price.amount);
        Env::DocAddNum("aid", evt.price.aid);
    }
}

ON_METHOD(manager, view_nfts) {
    char buf[256];
    int buf_len = Env::DocGetText("ids", buf, sizeof(buf));

    using HeightNftIdx =
        gallery::Index<gallery::Tag::kHeightNftIdx, Height, gallery::Nft>;

    GalleryObjectPrinter<AppNft> a{cid};
    if (count && h0) {
        Height last_printed_h = a.Print<HeightNftIdx>(h0, count);
        if (last_printed_h != kMaxHeight)
            Env::DocAddNum("processed_height", last_printed_h);
    } else if (count && id0) {
        gallery::Nft::Id not_printed = a.Print(id0, count);
        Env::DocAddNum("next_id", not_printed);
    } else if (buf_len) {
        a.Print(buf);
    } else {
        a.Print();
    }
}

ON_METHOD(user, set_price) {
    AppNft m;
    if (!m.ReadWithoutData(cid, id))
        return;

    key_material::Owner km{AppArtist::key_material(cid)};
    if (!IsOwner(cid, m.owner, id)) {
        if (!IsOwner(cid, m.owner, 0)) {
            OnError("not owned");
            return;
        } else {
            km.nft_id = 0;
        }
    } else {
        km.nft_id = id;
    }

    gallery::method::SetPrice args{};
    args.nft_id = id;
    args.price.amount = amount;
    args.price.aid = aid;

    SigRequest sig{};
    sig.m_pID = &km;
    sig.m_nID = sizeof(km);

    const char* comment =
        args.price.amount ? "Set item price" : "Remove from sale";

    Env::GenerateKernel(&cid, args.kMethod, &args, sizeof(args), nullptr, 0,
                        &sig, 1, comment, 0);
}

ON_METHOD(user, transfer) {
    AppNft m;
    if (!m.ReadWithoutData(cid, id)) {
        OnError("nft not found");
        return;
    }

    gallery::method::Transfer args{};
    args.nft_id = id;
    _POD_(args.new_owner) = new_owner;

    key_material::Owner km_owner{AppArtist::key_material(cid, id)};
    key_material::Owner km_author{AppArtist::key_material(cid)};

    SigRequest sig{};
    if (_POD_(m.owner) == km_author.Get()) {
        sig.m_pID = &km_author;
        sig.m_nID = sizeof(km_author);
    } else {
        sig.m_pID = &km_owner;
        sig.m_nID = sizeof(km_owner);
    }

    Env::GenerateKernel(&cid, args.kMethod, &args, sizeof(args), nullptr, 0,
                        &sig, 1, "Transfer item", 1848900);
}

ON_METHOD(user, buy) {
    AppNft nft;
    if (!nft.ReadWithoutData(cid, id)) {
        OnError("nft not found");
        return;
    }

    if (!nft.price.amount) {
        OnError("not for sale");
        return;
    }

    gallery::method::Buy args{};
    args.nft_id = id;
    args.has_aid = !!nft.aid;

    key_material::Owner km{AppArtist::key_material(cid, id)};
    args.user = km.Get();

    SigRequest sig{};
    sig.m_pID = &km;
    sig.m_nID = sizeof(km);

    FundsChange fc{};
    fc.m_Consume = 1;
    fc.m_Amount = nft.price.amount;
    fc.m_Aid = nft.price.aid;

    Env::GenerateKernel(&cid, args.kMethod, &args, sizeof(args), &fc, 1, &sig,
                        1, "Buy item", 3397731);

    /*
    Height cur_height = Env::get_Height();

    const uint32_t kSlotNonceKey = 0;
    const uint32_t kSlotKrnNonce = 1;
    const uint32_t kSlotKrnBlind = 2;

    PubKey krn_blind;
    PubKey full_nonce;
    Secp::Point p0, p1;
    p0.FromSlot(kSlotKrnBlind);
    p0.Export(krn_blind);

    p0.FromSlot(kSlotKrnNonce);
    p1.FromSlot(kSlotNonceKey);
    p0 += p1;
    p0.Export(full_nonce);

    Secp_scalar_data e;
    Env::GenerateKernelAdvanced(&cid, args.kMethod, &args, sizeof(args), &fc, 1,
                                &args.user, 1, "Buy item", 1895645, cur_height,
                                cur_height + 5, krn_blind, full_nonce, e,
                                kSlotKrnBlind, kSlotKrnNonce, &e);
    Secp::Scalar s;
    s.Import(e);
    Env::KeyID kid(&km, sizeof(km));
    kid.get_Blind(s, s, kSlotNonceKey);
    s.Export(e);

    Env::GenerateKernelAdvanced(&cid, args.kMethod, &args, sizeof(args), &fc, 1,
                                &args.user, 1, "Buy item", 1895645, cur_height,
                                cur_height + 5, krn_blind, full_nonce, e,
                                kSlotKrnBlind, kSlotKrnNonce, nullptr);
                                */
}

ON_METHOD(user, view_balance) {
    BalanceWalkerOwner wlk{cid};
    {
        Env::DocArray gr0("items");
        for (wlk.Enum(cid); wlk.MoveNext();) {
            Env::DocGroup gr1("");
            wlk.Print();
            wlk.AddToTotals();
        }
    }
    wlk.PrintTotals();
}

ON_METHOD(user, withdraw) {
    BalanceWalkerOwner wlk{cid};
    uint32_t count = 0;
    for (wlk.Enum(cid); wlk.MoveNext();) {
        gallery::method::Withdraw args{};
        _POD_(args.key) = wlk.key.m_KeyInContract;
        args.value = wlk.payout.amount;  // everything

        FundsChange fc{};
        fc.m_Consume = 0;
        fc.m_Aid = wlk.key.m_KeyInContract.aid;
        fc.m_Amount = wlk.payout.amount;

        key_material::Owner km_owner{
            AppArtist::key_material(cid, wlk.key.m_KeyInContract.nft_id)};
        key_material::Owner km_author{AppArtist::key_material(cid)};

        SigRequest sig{};
        if (_POD_(wlk.key.m_KeyInContract.user) == km_author.Get()) {
            sig.m_pID = &km_author;
            sig.m_nID = sizeof(km_author);
        } else {
            sig.m_pID = &km_owner;
            sig.m_nID = sizeof(km_owner);
        }

        Env::GenerateKernel(&cid, args.kMethod, &args, sizeof(args), &fc, 1,
                            &sig, 1, count ? "" : "Withdraw", 0);

        if (max_count == ++count)
            break;
    }
}

ON_METHOD(user, is_my_key) {
    Env::DocAddNum32("key", IsOwner(cid, key, 0) || IsOwner(cid, key, nft_id));
}

ON_METHOD(user, vote) {
    StatePlus s;
    if (!s.Init(cid)) {
        OnError("no contract with such cid");
        return;
    }

    gallery::method::Vote args{};
    args.nft_id = id;
    args.value = val;
    args.artist_id = AppArtist::id(cid);

    FundsChange fc{};
    fc.m_Consume = 0;
    fc.m_Aid = s.config.vote_reward.aid;

    {
        Env::Key_T<gallery::Like::Key> key{};
        key.m_Prefix.m_Cid = cid;
        key.m_KeyInContract.nft_id = Utils::FromBE(id);
        key.m_KeyInContract.artist_id = args.artist_id;

        gallery::Like like{};
        bool already_voted = Env::VarReader::Read_T(key, like);

        fc.m_Amount = already_voted ? 0 : s.config.vote_reward.amount;
    }

    key_material::Owner km{AppArtist::key_material(cid)};

    SigRequest sig;
    sig.m_pID = &km;
    sig.m_nID = sizeof(km);

    Env::GenerateKernel(&cid, args.kMethod, &args, sizeof(args), &fc, 1, &sig,
                        1, "Vote", 187200);
}

ON_METHOD(manager, migrate_artist) {
    key_material::Admin kid{};

    struct {
        gallery::method::MigrateArtist args;
        char label_and_data[gallery::Artist::kTotalMaxLen + 1];
    } d;

    uint32_t label_size =
        Env::DocGetText("label", d.label_and_data,
                        gallery::Artist::kLabelMaxLen + 1);  // including 0-term

    if (label_size > gallery::Artist::kLabelMaxLen + 1) {  // plus \0
        OnError("label is too long");
        return;
    }
    if (label_size < 2) {
        OnError("label is missing");
        return;
    }

    d.args.label_len = label_size - 1;

    std::string_view label(d.label_and_data, d.args.label_len);
    gallery::Artist::Id artist_id = artist_id_by_label(cid, label);

    if (!_POD_(artist_id).IsZero()) {
        OnError("label already exists");
        return;
    }

    uint32_t data_size =
        Env::DocGetText("data", d.label_and_data + d.args.label_len,
                        gallery::Artist::kDataMaxLen + 1);  // including 0-term

    if (data_size > gallery::Artist::kDataMaxLen + 1) {
        OnError("data is too long");
        return;
    }

    d.args.data_len = data_size ? data_size - 1 : data_size;

    uint32_t args_size = sizeof(d.args) + d.args.label_len + d.args.data_len;

    d.args.artist_id = id;
    kid.get_Pk(d.args.signer);

    std::string_view comment = "Setting an artist";

    StatePlus s;
    if (!s.Init(cid)) {
        OnError("no contract with such cid");
        return;
    }

    uint32_t charge =
        Env::Cost::LoadVar_For(sizeof(gallery::State)) +
        Env::Cost::SaveVar_For(sizeof(gallery::State)) +
        Env::Cost::Log_For(label_size) +
        Env::Cost::SaveVar_For(sizeof(gallery::Artist::Id)) +
        Env::Cost::MemOpPerByte * (sizeof(gallery::Artist) + data_size) +
        Env::Cost::LoadVar_For(sizeof(gallery::Artist) +
                               gallery::Artist::kDataMaxLen) +
        Env::Cost::SaveVar_For(sizeof(gallery::Artist) + data_size) +
        2 * Env::Cost::SaveVar_For(sizeof(bool)) + Env::Cost::AddSig;

    Env::GenerateKernel(&cid, d.args.kMethod, &d, args_size, nullptr, 0, &kid,
                        1, comment.data(), charge);
}

ON_METHOD(manager, migrate_collection) {
    key_material::Admin kid{};

    struct {
        gallery::method::MigrateCollection args;
        char label_and_data[gallery::Collection::kTotalMaxLen + 2];
    } d;

    d.args.artist_id = artist_id;
    d.args.collection_id = collection_id;
    kid.get_Pk(d.args.signer);

    uint32_t label_size = Env::DocGetText(
        "label", d.label_and_data,
        gallery::Collection::kLabelMaxLen + 1);  // including 0-term

    if (label_size > gallery::Collection::kLabelMaxLen + 1) {  // plus \0
        OnError("label is too long");
        return;
    }
    if (label_size < 2) {
        OnError("label is missing");
        return;
    }

    d.args.label_len = label_size - 1;

    std::string_view label(d.label_and_data, d.args.label_len);
    gallery::Collection::Id collection_id_ = collection_id_by_label(cid, label);
    if (collection_id_) {
        OnError("label already exists");
        return;
    }

    uint32_t data_size = Env::DocGetText(
        "data", d.label_and_data + d.args.label_len,
        gallery::Collection::kDataMaxLen + 1);  // including 0-term

    if (data_size > gallery::Collection::kDataMaxLen + 1) {
        OnError("data too long");
        return;
    }

    d.args.data_len = data_size ? data_size - 1 : 0;
    uint32_t args_size = sizeof(d.args) + d.args.data_len + d.args.label_len;

    StatePlus s;
    if (!s.Init(cid)) {
        OnError("no contract with such cid");
        return;
    }

    uint32_t charge =
        Env::Cost::LoadVar_For(sizeof(gallery::Collection) +
                               gallery::Collection::kTotalMaxLen) +
        Env::Cost::SaveVar_For(sizeof(gallery::Collection) + data_size +
                               label_size) +
        Env::Cost::LoadVar_For(sizeof(gallery::State)) +
        Env::Cost::SaveVar_For(sizeof(gallery::State)) +
        4 * Env::Cost::SaveVar_For(sizeof(bool)) +
        Env::Cost::LoadVar_For(sizeof(gallery::Artist) +
                               gallery::Artist::kDataMaxLen) +
        Env::Cost::SaveVar_For(sizeof(gallery::Artist) +
                               gallery::Artist::kDataMaxLen) +
        2 * Env::Cost::SaveVar_For(sizeof(gallery::Collection::Id)) +
        2 * Env::Cost::SaveVar_For(sizeof(bool)) + Env::Cost::AddSig +
        Env::Cost::MemOpPerByte *
            (sizeof(gallery::Collection) + label_size + data_size);

    Env::GenerateKernel(&cid, d.args.kMethod, &d.args, args_size, nullptr, 0,
                        &kid, 1, "Set collection", charge);
}

ON_METHOD(manager, migrate_nft) {
    key_material::Admin kid;

    struct {
        gallery::method::MigrateNft args;
        char label_and_data[gallery::Nft::kTotalMaxLen];
    } d;

    if (!id) {
        OnError("nft id must be specified");
        return;
    }
    kid.get_Pk(d.args.signer);
    d.args.nft_id = id;
    d.args.artist_id = artist_id;
    d.args.owner_id = owner_id;
    d.args.collection_id = collection_id;
    d.args.price.amount = amount;
    d.args.price.aid = aid;

    if (!collection_id) {
        OnError("collection_id must be specified");
        return;
    }

    uint32_t label_size =
        Env::DocGetText("label", d.label_and_data,
                        gallery::Nft::kLabelMaxLen + 1);  // including 0-term

    if (label_size > gallery::Nft::kLabelMaxLen + 1) {
        OnError("label is too long");
        return;
    }
    if (label_size < 2) {
        OnError("label is missing");
        return;
    }

    d.args.label_len = label_size - 1;

    uint32_t data_size =
        Env::DocGetText("data", d.label_and_data + d.args.label_len,
                        gallery::Nft::kDataMaxLen + 1);

    if (data_size < 2) {
        OnError("data must be specified");
        return;
    }
    if (data_size > gallery::Nft::kDataMaxLen + 1) {
        OnError("data is too long");
        return;
    }

    d.args.data_len = data_size ? data_size - 1 : data_size;
    uint32_t arg_size = sizeof(d.args) + d.args.label_len + d.args.data_len;

    uint32_t charge =
        Env::Cost::LoadVar_For(sizeof(gallery::State)) +
        Env::Cost::SaveVar_For(sizeof(gallery::State)) +
        6 * Env::Cost::SaveVar_For(sizeof(bool)) +
        Env::Cost::LoadVar_For(sizeof(gallery::Collection) +
                               gallery::Collection::kTotalMaxLen) +
        Env::Cost::SaveVar_For(sizeof(gallery::Collection) +
                               gallery::Collection::kTotalMaxLen) +
        Env::Cost::LoadVar_For(sizeof(gallery::Artist) +
                               gallery::Artist::kDataMaxLen) +
        Env::Cost::SaveVar_For(sizeof(gallery::Artist) +
                               gallery::Artist::kDataMaxLen) +
        Env::Cost::SaveVar_For(sizeof(gallery::Nft)) +
        Env::Cost::Log_For(data_size) + Env::Cost::Log_For(label_size) +
        Env::Cost::AddSig + Env::Cost::Cycle * 300;

    StatePlus s;
    if (!s.Init(cid)) {
        OnError("no contract with such cid");
        return;
    }

    Env::GenerateKernel(&cid, d.args.kMethod, &d, arg_size, nullptr, 0, &kid, 1,
                        "Upload nft", charge);
}

ON_METHOD(manager, migrate_sales) {
    Env::Key_T<gallery::Events::Sell::Key> key{};
    _POD_(key.m_Prefix.m_Cid) = cid;
    key.m_KeyInContract.nft_id = nft_id;

    Env::LogReader r(key, key);
    gallery::Events::Sell evt{};
    if (r.MoveNext_T(key, evt)) {
        OnError("sales for the nft has already been migrated");
        return;
    }

    key_material::Admin kid{};

    char buf[256];
    Env::DocGetText("prices", buf, sizeof(buf));
    std::vector<Amount> ids_vec(ParseIds<Amount>(buf));

    size_t args_size =
        sizeof(gallery::method::MigrateSales) + sizeof(Amount) * ids_vec.size();
    std::unique_ptr<gallery::method::MigrateSales> args(
        static_cast<gallery::method::MigrateSales*>(::operator new(args_size)));
    kid.get_Pk(args->signer);
    args->sales_len = ids_vec.size();
    args->nft_id = nft_id;
    for (uint32_t i = 0; i < ids_vec.size(); ++i) {
        args->prices[i] = ids_vec[i];
    }

    uint32_t charge =
        Env::Cost::LoadVar_For(sizeof(gallery::State)) +
        Env::Cost::LoadVar_For(sizeof(bool)) +
        4 * Env::Cost::SaveVar_For(sizeof(bool)) +
        Env::Cost::LoadVar_For(sizeof(gallery::Collection) +
                               gallery::Collection::kTotalMaxLen) +
        Env::Cost::SaveVar_For(sizeof(gallery::Collection) +
                               gallery::Collection::kTotalMaxLen) +
        Env::Cost::LoadVar_For(sizeof(gallery::Artist) +
                               gallery::Artist::kDataMaxLen) +
        Env::Cost::SaveVar_For(sizeof(gallery::Artist) +
                               gallery::Artist::kDataMaxLen) +
        Env::Cost::SaveVar_For(sizeof(gallery::Nft)) +
        Env::Cost::Log_For(ids_vec.size()) * sizeof(gallery::Events::Sell) +
        Env::Cost::AddSig + Env::Cost::Cycle * 300;

    Env::GenerateKernel(&cid, args->kMethod, args.get(), args_size, nullptr, 0,
                        &kid, 1, "Migrate sales", charge);
}

#undef ON_METHOD
#undef THE_FIELD

BEAM_EXPORT void Method_1() {
    Env::DocGroup root("");

    char role[0x10], action[0x20];

    if (!Env::DocGetText("role", role, sizeof(role)))
        return OnError("Role not specified");

    if (!Env::DocGetText("action", action, sizeof(action)))
        return OnError("Action not specified");

#define PAR_READ(type, name) \
    type arg_##name;         \
    Env::DocGet(#name, arg_##name);
#define PAR_PASS(type, name) arg_##name,

#define THE_METHOD(role, name)                                       \
    if (!Env::Strcmp(action, #name)) {                               \
        Gallery_##role##_##name(PAR_READ)                            \
            On_##role##_##name(Gallery_##role##_##name(PAR_PASS) 0); \
        return;                                                      \
    }

#define THE_ROLE(name)                                                   \
    if (!Env::Strcmp(role, #name)) {                                     \
        GalleryRole_##name(THE_METHOD) return OnError("invalid Action"); \
    }

    GalleryRoles_All(THE_ROLE)

#undef THE_ROLE
#undef THE_METHOD
#undef PAR_PASS
#undef PAR_READ

        OnError("unknown Role");
}
