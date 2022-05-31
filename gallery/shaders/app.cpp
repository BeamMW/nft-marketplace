#include "Shaders/common.h"
#include "Shaders/app_common_impl.h"
#include "contract.h"
#include "Shaders/upgradable2/contract.h"
#include "Shaders/upgradable2/app_common_impl.h"

#include <array>
#include <string_view>
#include <type_traits>
#include <vector>
#include <map>

// MANAGER

#define Gallery_manager_view(macro)

#define Gallery_manager_view_params(macro) macro(ContractID, cid)

#define Gallery_manager_view_moderators(macro) \
    macro(ContractID, cid) macro(Height, h0) macro(uint32_t, count)

#define Gallery_manager_set_moderator(macro)                 \
    macro(ContractID, cid) macro(gallery::Moderator::Id, id) \
        macro(uint32_t, enable)

#define Gallery_manager_view_artists_stats(macro) macro(ContractID, cid)

#define Gallery_manager_view_moderators_stats(macro) macro(ContractID, cid)

#define Gallery_manager_view_collections_stats(macro) macro(ContractID, cid)

#define Gallery_manager_view_nfts_stats(macro) macro(ContractID, cid)

#define Gallery_manager_view_nfts(macro) \
    macro(ContractID, cid) macro(Height, h0) macro(Amount, count)

#define Gallery_manager_view_nft_sales(macro) \
    macro(ContractID, cid) macro(gallery::Nft::Id, id)

#define Gallery_manager_view_collections(macro)                     \
    macro(ContractID, cid) macro(Height, h0) macro(uint32_t, count) \
        macro(uint32_t, nfts)

#define Gallery_manager_view_artists(macro)                         \
    macro(ContractID, cid) macro(Height, h0) macro(uint32_t, count) \
        macro(uint32_t, nfts) macro(uint32_t, collections)

#define Gallery_manager_view_balance(macro) macro(ContractID, cid)

#define Gallery_manager_get_id(macro)

#define Gallery_manager_explicit_upgrade(macro) macro(ContractID, cid)

#define Gallery_manager_set_nft_status(macro) macro(ContractID, cid)

#define Gallery_manager_set_collection_status(macro) macro(ContractID, cid)

#define Gallery_manager_set_artist_status(macro) macro(ContractID, cid)

#define Gallery_manager_set_fee_base(macro) \
    macro(ContractID, cid) macro(Amount, amount)

#define GalleryRole_manager(macro)                                            \
    macro(manager, view) macro(manager, view_params) macro(                   \
        manager, view_artists_stats) macro(manager, view_moderators_stats)    \
        macro(manager, view_artists) macro(manager, view_collections)         \
            macro(manager, view_nfts) macro(manager, view_nft_sales)          \
                macro(manager, view_collections_stats) macro(                 \
                    manager, view_nfts_stats) macro(manager, view_balance)    \
                    macro(manager, get_id) macro(manager, explicit_upgrade)   \
                        macro(manager, set_moderator)                         \
                            macro(manager, view_moderators)                   \
                                macro(manager, set_nft_status)                \
                                    macro(manager, set_artist_status)         \
                                        macro(manager, set_collection_status) \
                                            macro(manager, set_fee_base)

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

#define GalleryRole_user(macro)                                           \
    macro(user, set_price) macro(user, transfer) macro(user, buy)         \
        macro(user, view_balance) macro(user, withdraw) macro(user, vote) \
            macro(user, add_rewards)

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

// TODO
const std::string_view StatusToString(const gallery::Status& status);
static inline bool IsOwner(const ContractID& cid, const PubKey& owner,
                           const gallery::Nft::Id& nft_id);
template <class Id>
std::vector<Id> ParseIds(const std::string_view& ids);

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

    Owner(const ContractID& cid, const gallery::Nft::Id& nft_id)
        : cid{cid}, nft_id{nft_id} {
        Env::Memcpy(seed, kOwnerMeta.data(), kOwnerMeta.size());
    }

    PubKey Get() const {
        PubKey pk;
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
struct AppModerator : public gallery::Moderator {
    static inline PubKey id(const ContractID& cid) {
        return key_material::Owner{cid}.Get();
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
struct AppArtist : public gallery::Artist {
    std::array<char, kDataMaxLen + 1> data;
    std::array<char, kLabelMaxLen + 1> label;

    static PubKey id(const ContractID& cid) {
        return key_material::Owner{cid}.Get();
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
        Env::DocAddText("status", StatusToString(status).data());

        uint32_t print_nfts{};
        Env::DocGetNum32("nfts", &print_nfts);
        if (print_nfts)
            PrintNfts_(cid, id);

        uint32_t print_collections{};
        Env::DocGetNum32("collections", &print_collections);
        if (print_collections)
            PrintCollections_(cid, id);
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

private:
    void PrintNfts_(const ContractID& cid, const Id& id) {
        using ACKey =
            gallery::Index<gallery::Tag::kArtistCollectionIdx,
                           gallery::Artist::Id, gallery::Collection>::Key;
        using CAKey =
            gallery::Index<gallery::Tag::kCollectionNftIdx,
                           gallery::Collection::Id, gallery::Nft>::Key;

        Env::Key_T<ACKey> ack0, ack1;
        _POD_(ack0.m_Prefix.m_Cid) = cid;
        _POD_(ack1.m_Prefix.m_Cid) = cid;
        ack0.m_KeyInContract.id = id;
        ack1.m_KeyInContract.id = id;
        ack0.m_KeyInContract.t_id = 0;
        ack1.m_KeyInContract.t_id = static_cast<gallery::Collection::Id>(-1);
        Env::VarReader rac(ack0, ack1);

        bool exists;
        Env::DocArray gr("nfts");
        while (rac.MoveNext_T(ack0, exists)) {
            Env::Key_T<CAKey> cak0, cak1;
            _POD_(cak0.m_Prefix.m_Cid) = cid;
            _POD_(cak1.m_Prefix.m_Cid) = cid;
            cak0.m_KeyInContract.id = ack0.m_KeyInContract.t_id;
            cak1.m_KeyInContract.id = ack0.m_KeyInContract.t_id;
            cak0.m_KeyInContract.t_id = 0;
            cak1.m_KeyInContract.t_id = static_cast<gallery::Nft::Id>(-1);
            Env::VarReader rca(cak0, cak1);

            while (rca.MoveNext_T(cak0, exists)) {
                Env::DocAddNum32("", Utils::FromBE(cak0.m_KeyInContract.t_id));
            }
        }
    }

    void PrintCollections_(const ContractID& cid, const Id& id) {
        using ACKey =
            gallery::Index<gallery::Tag::kArtistCollectionIdx,
                           gallery::Artist::Id, gallery::Collection>::Key;

        Env::Key_T<ACKey> ack0, ack1;
        _POD_(ack0.m_Prefix.m_Cid) = cid;
        _POD_(ack1.m_Prefix.m_Cid) = cid;
        ack0.m_KeyInContract.id = id;
        ack1.m_KeyInContract.id = id;
        ack0.m_KeyInContract.t_id = 0;
        ack1.m_KeyInContract.t_id = static_cast<gallery::Collection::Id>(-1);

        Env::VarReader rac(ack0, ack1);
        Env::DocArray gr("collections");
        bool exists;
        while (rac.MoveNext_T(ack0, exists)) {
            Env::DocAddNum32("", Utils::FromBE(ack0.m_KeyInContract.t_id));
        }
    }
};
#pragma pack(pop)

#pragma pack(push, 0)
struct AppCollection : public gallery::Collection {
    std::array<char, kTotalMaxLen + 2> label_and_data;

    void Print(const ContractID& cid, const Id& id) {
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

        uint32_t print_nfts{};
        Env::DocGetNum32("nfts", &print_nfts);

        if (print_nfts)
            PrintNfts_(cid, id);
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

private:
    void PrintNfts_(const ContractID& cid, const Id& id) {
        using CAKey =
            gallery::Index<gallery::Tag::kCollectionNftIdx,
                           gallery::Collection::Id, gallery::Nft>::Key;

        Env::Key_T<CAKey> cak0, cak1;
        _POD_(cak0.m_Prefix.m_Cid) = cid;
        _POD_(cak1.m_Prefix.m_Cid) = cid;
        cak0.m_KeyInContract.id = Utils::FromBE(id);
        cak1.m_KeyInContract.id = Utils::FromBE(id);
        cak0.m_KeyInContract.t_id = 0;
        cak1.m_KeyInContract.t_id = static_cast<gallery::Nft::Id>(-1);
        Env::VarReader rca(cak0, cak1);

        Env::DocArray gr("nfts");
        bool exists;
        while (rca.MoveNext_T(cak0, exists)) {
            Env::DocAddNum32("", Utils::FromBE(cak0.m_KeyInContract.t_id));
        }
    }
};
#pragma pack(pop)

#pragma pack(push, 0)
struct AppNft : public gallery::Nft {
    std::array<char, kLabelMaxLen + 1> label;
    std::array<char, kDataMaxLen + 1> data;

    void Print(const ContractID& cid, const Id& id) {
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

        Env::Key_T<gallery::Like::Key> like_key;
        like_key.m_Prefix.m_Cid = cid;
        like_key.m_KeyInContract.artist_id = AppArtist::id(cid);
        like_key.m_KeyInContract.nft_id = id;
        uint32_t value{};
        Env::VarReader::Read_T(like_key, value);

        Env::DocAddNum32("likes", likes_number);
        if (value)
            Env::DocAddNum("my_like", value);
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

template <class T, class HeightIdx>
class GalleryObjectPrinter {
public:
    GalleryObjectPrinter(const ContractID& cid) : cid_{cid} {
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

    void Print(const std::string_view& ids) {
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

    void Print(Height h0, uint32_t count) {
        Env::Key_T<typename HeightIdx::Key> k0, k1;
        k0.m_Prefix.m_Cid = cid_;
        k1.m_Prefix.m_Cid = cid_;
        k0.m_KeyInContract.id = Utils::FromBE(h0);
        k1.m_KeyInContract.id = static_cast<Height>(-1);
        _POD_(k0.m_KeyInContract.t_id).SetZero();
        _POD_(k1.m_KeyInContract.t_id).SetObject(0xff);

        Env::VarReader r(k0, k1);

        int cur_cnt = 0;
        Height prev_h = -1, last_printed_h = -1;
        bool exists;
        {
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
        if (last_printed_h != -1)
            Env::DocAddNum("processed_height", last_printed_h);
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
                        key.m_KeyInContract.nft_id))
                break;
        }
        return true;
    }
};

void OnError(const std::string_view& err) {
    Env::DocAddText("error", err.data());
}

template <class Id>
std::vector<Id> ParseIds(const std::string_view& ids) {
    union {
        Id id;
        uint8_t a[sizeof(Id)];
    } blob;

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

gallery::Status StringToStatus(const std::string_view& str_status) {
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

const std::string_view StatusToString(const gallery::Status& status) {
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
                           const gallery::Nft::Id& nft_id) {
    return _POD_(owner) == key_material::Owner{cid, nft_id}.Get();
}

bool my_artist_exists(const ContractID& cid) {
    Env::Key_T<gallery::Artist::Key> k;
    k.m_Prefix.m_Cid = cid;
    k.m_KeyInContract.id = AppArtist::id(cid);
    AppArtist a;
    Env::VarReader r(k, k);
    uint32_t key_size = 0;
    uint32_t val_size = 0;
    return r.MoveNext(nullptr, key_size, nullptr, val_size, 0);
}

gallery::Collection::Id collection_id_by_label(const ContractID& cid,
                                               const std::string_view& label) {
    Env::Key_T<gallery::Collection::LabelKey> lk;
    lk.m_Prefix.m_Cid = cid;
    lk.m_KeyInContract.label_hash = gallery::GetLabelHash(label);
    lk.m_KeyInContract.artist_id = AppArtist::id(cid);
    gallery::Collection::Id collection_id;
    Env::VarReader::Read_T(lk, collection_id);
    return collection_id;
}

gallery::Artist::Id artist_id_by_label(const ContractID& cid,
                                       const std::string_view& label) {
    Env::Key_T<gallery::Artist::LabelKey> lk;
    lk.m_Prefix.m_Cid = cid;
    lk.m_KeyInContract.label_hash = gallery::GetLabelHash(label);
    gallery::Artist::Id artist_id;
    Env::VarReader::Read_T(lk, artist_id);
    return artist_id;
}

ON_METHOD(manager, view) {
    static const ShaderID kSids[] = {
        gallery::s_SID_0,
    };

    ContractID versions[_countof(kSids)];
    Height versions_deploy[_countof(kSids)];

    ManagerUpgadable2::Walker wlk;
    wlk.m_VerInfo.m_Count = _countof(kSids);
    wlk.m_VerInfo.s_pSid = kSids;
    wlk.m_VerInfo.m_pCid = versions;
    wlk.m_VerInfo.m_pHeight = versions_deploy;

    key_material::Admin kid;
    wlk.ViewAll(&kid);
}

ON_METHOD(manager, set_moderator) {
    gallery::method::SetModerator args;
    args.id = id;
    args.approved = enable;
    key_material::Admin akm;
    Env::GenerateKernel(&cid, args.kMethod, &args, sizeof(args), nullptr, 0,
                        &akm, 1, "Set moderator", 131455);
}

ON_METHOD(manager, view_params) {
    StatePlus s;
    if (!s.Init(cid))
        return;

    PubKey admin_id;
    key_material::Admin().get_Pk(admin_id);

    uint32_t is_admin = (_POD_(s.config.admin_id) == admin_id);

    AppModerator moder;
    Env::DocAddNum("is_admin", is_admin);
    Env::DocAddNum32("is_moderator", moder.is_moderator(cid));
    Env::DocAddNum("fee_base", s.fee_base);

    {
        Env::DocGroup gr1("vote_reward");
        Env::DocAddNum("aid", s.config.vote_reward.aid);
        Env::DocAddNum("amount", s.config.vote_reward.amount);
        Env::DocAddNum("balance", s.vote_balance);
    }
}

ON_METHOD(manager, view_artists_stats) {
    StatePlus s;
    s.Init(cid);
    Env::DocAddNum32("total", s.total_artists);
}

ON_METHOD(manager, view_moderators_stats) {
    StatePlus s;
    s.Init(cid);
    Env::DocAddNum32("total", s.total_moderators);
}

ON_METHOD(manager, view_collections_stats) {
    StatePlus s;
    s.Init(cid);
    Env::DocAddNum32("total", s.total_collections);
}

ON_METHOD(manager, view_nfts_stats) {
    StatePlus s;
    s.Init(cid);
    Env::DocAddNum32("total", s.total_nfts);
}

ON_METHOD(manager, view_artists) {
    char buf[1024];
    int buf_len = Env::DocGetText("ids", buf, sizeof(buf));

    using HeightArtistIdx =
        gallery::Index<gallery::Tag::kHeightArtistIdx, Height, gallery::Artist>;

    GalleryObjectPrinter<AppArtist, HeightArtistIdx> a{cid};
    if (count && h0)
        a.Print(h0, count);
    else if (buf_len)
        a.Print(buf);
    else
        a.Print();
}

ON_METHOD(manager, view_moderators) {
    char buf[1024];
    int buf_len = Env::DocGetText("ids", buf, sizeof(buf));

    using HeightModeratorIdx = gallery::Index<gallery::Tag::kHeightModeratorIdx,
                                              Height, gallery::Moderator>;

    GalleryObjectPrinter<AppModerator, HeightModeratorIdx> m{cid};
    if (count && h0)
        m.Print(h0, count);
    else if (buf_len)
        m.Print(buf);
    else
        m.Print();
}

ON_METHOD(manager, view_collections) {
    char buf[256];
    int buf_len = Env::DocGetText("ids", buf, sizeof(buf));

    using HeightCollectionIdx =
        gallery::Index<gallery::Tag::kHeightCollectionIdx, Height,
                       gallery::Collection>;

    GalleryObjectPrinter<AppCollection, HeightCollectionIdx> c{cid};
    if (count && h0)
        c.Print(h0, count);
    else if (buf_len)
        c.Print(buf);
    else
        c.Print();
}

ON_METHOD(artist, set_nft) {
    struct {
        gallery::method::SetNft args;
        char label_and_data[gallery::Nft::kTotalMaxLen];
    } d;

    key_material::Owner km{cid};
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

    uint32_t data_size = Env::DocGetText(
        "data", d.label_and_data + d.args.label_len, gallery::Nft::kDataMaxLen + 1);

    if (data_size < 2) {
        OnError("data must be specified");
        return;
    }
    if (data_size > gallery::Nft::kDataMaxLen + 1) {
        OnError("data is too long");
        return;
    }

    d.args.data_len = data_size - 1;
    uint32_t arg_size = sizeof(d.args) + d.args.label_len + d.args.data_len;

    SigRequest sig;
    sig.m_pID = &km;
    sig.m_nID = sizeof(km);

    uint32_t charge =
        ManagerUpgadable2::get_ChargeInvoke() +
        Env::Cost::LoadVar_For(sizeof(gallery::State)) +
        Env::Cost::SaveVar_For(sizeof(gallery::State)) +
        Env::Cost::LoadVar_For(sizeof(bool)) +
        7 * Env::Cost::SaveVar_For(sizeof(bool)) +
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
    s.Init(cid);

    Env::GenerateKernel(&cid, d.args.kMethod, &d, arg_size, nullptr, 0, &sig, 1,
                        "Upload nft", charge + s.fee_base / 10);
}

ON_METHOD(user, add_rewards) {
    StatePlus s;
    if (!s.Init(cid))
        return;

    gallery::method::AddVoteRewards args;
    args.amount = s.config.vote_reward.amount * amount;

    if (!args.amount) {
        OnError("no rewards");
        return;
    }

    FundsChange fc;
    fc.m_Aid = s.config.vote_reward.aid;
    fc.m_Amount = args.amount;
    fc.m_Consume = 1;

    Env::GenerateKernel(&cid, args.kMethod, &args, sizeof(args), &fc, 1,
                        nullptr, 0, "Add voting rewards", 0);
}

ON_METHOD(manager, get_id) {
    PubKey pk;
    key_material::Admin().get_Pk(pk);
    Env::DocAddBlob_T("id", pk);
}

ON_METHOD(manager, explicit_upgrade) {
    ManagerUpgadable2::MultiSigRitual::Perform_ExplicitUpgrade(cid);
}

ON_METHOD(manager, view_balance) {
    BalanceWalker wlk;
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
    char buf[256];
    Env::DocGetText("ids", buf, sizeof(buf));
    std::vector<gallery::Nft::Id> ids_vec(ParseIds<gallery::Nft::Id>(buf));

    size_t args_size = sizeof(gallery::method::SetNftStatus) +
                       sizeof(gallery::Nft::Id) * ids_vec.size();
    std::unique_ptr<gallery::method::SetNftStatus> args(
        static_cast<gallery::method::SetNftStatus*>(::operator new(args_size)));

    Env::DocGetText("status", buf, sizeof(buf));
    args->status = StringToStatus(buf);
    if (args->status == gallery::Status::kNone)
        return;

    key_material::Admin kid;
    kid.get_Pk(args->signer);

    args->ids_num = ids_vec.size();
    Env::Memcpy(args->ids, ids_vec.data(),
                sizeof(gallery::Nft::Id) * ids_vec.size());

    uint32_t charge =
        ManagerUpgadable2::get_ChargeInvoke() +
        Env::Cost::SaveVar_For(sizeof(gallery::State)) +
        Env::Cost::LoadVar_For(sizeof(gallery::State)) +
        2 * args->ids_num * Env::Cost::SaveVar_For(sizeof(bool)) +
        args->ids_num * Env::Cost::LoadVar_For(sizeof(gallery::Nft)) +
        args->ids_num * Env::Cost::SaveVar_For(sizeof(gallery::Nft)) +
        Env::Cost::LoadVar_For(sizeof(gallery::Moderator)) + Env::Cost::AddSig +
        Env::Cost::Cycle * 300;

    Env::GenerateKernel(&cid, args->kMethod, args.get(), args_size, nullptr, 0,
                        &kid, 1, "Update nfts' status", charge);
}

ON_METHOD(moderator, set_nft_status) {
    char buf[256];
    Env::DocGetText("ids", buf, sizeof(buf));
    std::vector<gallery::Nft::Id> ids_vec(ParseIds<gallery::Nft::Id>(buf));

    size_t args_size = sizeof(gallery::method::SetNftStatus) +
                       sizeof(gallery::Nft::Id) * ids_vec.size();
    std::unique_ptr<gallery::method::SetNftStatus> args(
        static_cast<gallery::method::SetNftStatus*>(::operator new(args_size)));

    Env::DocGetText("status", buf, sizeof(buf));
    args->status = StringToStatus(buf);
    if (args->status == gallery::Status::kNone)
        return;

    key_material::Owner km{cid};
    args->signer = km.Get();

    args->ids_num = ids_vec.size();
    Env::Memcpy(args->ids, ids_vec.data(),
                sizeof(gallery::Nft::Id) * ids_vec.size());

    SigRequest sig;
    sig.m_pID = &km;
    sig.m_nID = sizeof(km);

    uint32_t charge =
        ManagerUpgadable2::get_ChargeInvoke() +
        Env::Cost::SaveVar_For(sizeof(gallery::State)) +
        Env::Cost::LoadVar_For(sizeof(gallery::State)) +
        2 * args->ids_num * Env::Cost::SaveVar_For(sizeof(bool)) +
        args->ids_num * Env::Cost::LoadVar_For(sizeof(gallery::Nft)) +
        args->ids_num * Env::Cost::SaveVar_For(sizeof(gallery::Nft)) +
        Env::Cost::LoadVar_For(sizeof(gallery::Moderator)) + Env::Cost::AddSig +
        Env::Cost::Cycle * 300;

    Env::GenerateKernel(&cid, args->kMethod, args.get(), args_size, nullptr, 0,
                        &sig, 1, "Update nft's status", charge);
}

ON_METHOD(moderator, set_artist_status) {
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
    if (args->status == gallery::Status::kNone)
        return;

    key_material::Owner km{cid};
    args->signer = km.Get();

    args->ids_num = ids_vec.size();
    Env::Memcpy(args->ids, ids_vec.data(),
                sizeof(gallery::Artist::Id) * ids_vec.size());

    SigRequest sig;
    sig.m_pID = &km;
    sig.m_nID = sizeof(km);

    uint32_t charge =
        ManagerUpgadable2::get_ChargeInvoke() +
        Env::Cost::SaveVar_For(sizeof(gallery::State)) +
        Env::Cost::LoadVar_For(sizeof(gallery::State)) +
        2 * args->ids_num * Env::Cost::SaveVar_For(sizeof(bool)) +
        args->ids_num * Env::Cost::LoadVar_For(sizeof(gallery::Artist) +
                                               gallery::Artist::kDataMaxLen) +
        args->ids_num * Env::Cost::SaveVar_For(sizeof(gallery::Artist) +
                                               gallery::Artist::kDataMaxLen) +
        Env::Cost::LoadVar_For(sizeof(gallery::Moderator)) + Env::Cost::AddSig +
        Env::Cost::Cycle * 300;

    Env::GenerateKernel(&cid, args->kMethod, args.get(), args_size, nullptr, 0,
                        &sig, 1, "Update artists' status", charge);
}

ON_METHOD(manager, set_artist_status) {
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
    if (args->status == gallery::Status::kNone)
        return;

    key_material::Admin kid;
    kid.get_Pk(args->signer);

    args->ids_num = ids_vec.size();
    Env::Memcpy(args->ids, ids_vec.data(),
                sizeof(gallery::Artist::Id) * ids_vec.size());

    uint32_t charge =
        ManagerUpgadable2::get_ChargeInvoke() +
        Env::Cost::SaveVar_For(sizeof(gallery::State)) +
        Env::Cost::LoadVar_For(sizeof(gallery::State)) +
        2 * args->ids_num * Env::Cost::SaveVar_For(sizeof(bool)) +
        args->ids_num * Env::Cost::LoadVar_For(sizeof(gallery::Artist) +
                                               gallery::Artist::kDataMaxLen) +
        args->ids_num * Env::Cost::SaveVar_For(sizeof(gallery::Artist) +
                                               gallery::Artist::kDataMaxLen) +
        Env::Cost::LoadVar_For(sizeof(gallery::Moderator)) + Env::Cost::AddSig +
        Env::Cost::Cycle * 300;

    Env::GenerateKernel(&cid, args->kMethod, args.get(), args_size, nullptr, 0,
                        &kid, 1, "Update artist's status", charge);
}

ON_METHOD(manager, set_collection_status) {
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

    key_material::Admin kid;
    kid.get_Pk(args->signer);

    args->ids_num = ids_vec.size();
    Env::Memcpy(args->ids, ids_vec.data(),
                sizeof(gallery::Collection::Id) * ids_vec.size());

    uint32_t charge =
        ManagerUpgadable2::get_ChargeInvoke() +
        Env::Cost::SaveVar_For(sizeof(gallery::State)) +
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
                        &kid, 1, "Update collections' status", charge);
}

ON_METHOD(moderator, set_collection_status) {
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

    key_material::Owner km{cid};
    args->signer = km.Get();

    SigRequest sig;
    sig.m_pID = &km;
    sig.m_nID = sizeof(km);

    args->ids_num = ids_vec.size();
    Env::Memcpy(args->ids, ids_vec.data(),
                sizeof(gallery::Collection::Id) * ids_vec.size());

    uint32_t charge =
        ManagerUpgadable2::get_ChargeInvoke() +
        Env::Cost::SaveVar_For(sizeof(gallery::State)) +
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
                        &sig, 1, "Update collection's status", charge);
}

ON_METHOD(manager, set_fee_base) {
    gallery::method::SetFeeBase args;
    args.amount = amount;
    key_material::Admin kid;
    Env::GenerateKernel(&cid, args.kMethod, &args, sizeof(args), nullptr, 0,
                        &kid, 1, "Set fee base", 0);
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
    bool artist_exists = my_artist_exists(cid);

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

    d.args.data_len = data_size - 1;

    uint32_t args_size = sizeof(d.args) + d.args.label_len + d.args.data_len;

    key_material::Owner km{cid};
    d.args.artist_id = km.Get();

    SigRequest sig;
    sig.m_pID = &km;
    sig.m_nID = sizeof(km);

    std::string_view comment =
        artist_exists ? "Updating artist's info" : "Becoming an artist";

    StatePlus s;
    s.Init(cid);

    uint32_t charge =
        ManagerUpgadable2::get_ChargeInvoke() +
        (!artist_exists ? Env::Cost::LoadVar_For(sizeof(gallery::State)) : 0) +
        (!artist_exists ? Env::Cost::SaveVar_For(sizeof(gallery::State)) : 0) +
        (!artist_exists ? Env::Cost::Log_For(label_size) : 0) +
        (!artist_exists ? Env::Cost::SaveVar_For(sizeof(gallery::Artist::Id))
                        : 0) +
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

    key_material::Owner km{cid};
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

    if (d.args.collection_id && collection_id &&
            collection_id != d.args.collection_id ||
        !d.args.collection_id && collection_id) {
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

    d.args.data_len = data_size - 1;
    uint32_t args_size = sizeof(d.args) + d.args.data_len + d.args.label_len;

    SigRequest sig;
    sig.m_pID = &km;
    sig.m_nID = sizeof(km);

    StatePlus s;
    s.Init(cid);

    uint32_t charge =
        ManagerUpgadable2::get_ChargeInvoke() +
        Env::Cost::LoadVar_For(sizeof(gallery::Collection) +
                               gallery::Collection::kTotalMaxLen) +
        Env::Cost::SaveVar_For(sizeof(gallery::Collection) + data_size +
                               label_size) +
        (!collection_id ? Env::Cost::LoadVar_For(sizeof(gallery::State)) : 0) +
        (!collection_id ? Env::Cost::SaveVar_For(sizeof(gallery::State)) : 0) +
        (!collection_id ? 4 * Env::Cost::SaveVar_For(sizeof(bool)) : 0) +
        (!collection_id ? Env::Cost::LoadVar_For(sizeof(gallery::Artist) +
                                                 gallery::Artist::kDataMaxLen)
                        : 0) +
        (!collection_id ? Env::Cost::SaveVar_For(sizeof(gallery::Artist) +
                                                 gallery::Artist::kDataMaxLen)
                        : 0) +
        2 * Env::Cost::SaveVar_For(sizeof(gallery::Collection::Id)) +
        2 * Env::Cost::SaveVar_For(sizeof(bool)) + Env::Cost::AddSig +
        Env::Cost::MemOpPerByte * (sizeof(gallery::Collection) + label_size + data_size);

    Env::GenerateKernel(&cid, d.args.kMethod, &d.args, args_size, nullptr, 0,
                        &sig, 1, "Set collection",
                        charge + (id ? 0 : s.fee_base / 10));
}

ON_METHOD(artist, get_id) {
    Env::DocAddBlob_T("id", AppArtist::id(cid));
}

ON_METHOD(manager, view_nft_sales) {
    Env::Key_T<gallery::Events::Sell::Key> key;
    _POD_(key.m_Prefix.m_Cid) = cid;
    key.m_KeyInContract.nft_id = id;

    Env::DocArray gr0("sales");

    for (Env::LogReader r(key, key);;) {
        gallery::Events::Sell evt;
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

    GalleryObjectPrinter<AppNft, HeightNftIdx> a{cid};
    if (count && h0)
        a.Print(h0, count);
    else if (buf_len)
        a.Print(buf);
    else
        a.Print();
}

ON_METHOD(user, set_price) {
    AppNft m;
    if (!m.ReadWithoutData(cid, id))
        return;

    key_material::Owner km{cid};
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

    gallery::method::SetPrice args;
    args.nft_id = id;
    args.price.amount = amount;
    args.price.aid = aid;

    SigRequest sig;
    sig.m_pID = &km;
    sig.m_nID = sizeof(km);

    const char* comment =
        args.price.amount ? "Set item price" : "Remove from sale";

    Env::GenerateKernel(&cid, args.kMethod, &args, sizeof(args), nullptr, 0,
                        &sig, 1, comment, 113709);
}

ON_METHOD(user, transfer) {
    AppNft m;
    if (!m.ReadWithoutData(cid, id))
        return;

    gallery::method::Transfer args;
    args.nft_id = id;
    _POD_(args.new_owner) = new_owner;

    key_material::Owner km{cid, id};

    SigRequest sig;
    sig.m_pID = &km;
    sig.m_nID = sizeof(km);

    Env::GenerateKernel(&cid, args.kMethod, &args, sizeof(args), nullptr, 0,
                        &sig, 1, "Transfer item", 113746);
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

    gallery::method::Buy args;
    args.nft_id = id;
    args.has_aid = !!nft.aid;

    key_material::Owner km{cid, id};
    args.user = km.Get();

    SigRequest sig;
    sig.m_pID = &km;
    sig.m_nID = sizeof(km);

    FundsChange fc;
    fc.m_Consume = 1;
    fc.m_Amount = nft.price.amount;
    fc.m_Aid = nft.price.aid;

    Env::GenerateKernel(&cid, args.kMethod, &args, sizeof(args), &fc, 1, &sig,
                        1, "Buy item", 1795645);
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
        gallery::method::Withdraw args;
        _POD_(args.key) = wlk.key.m_KeyInContract;
        args.value = wlk.payout.amount;  // everything

        FundsChange fc;
        fc.m_Consume = 0;
        fc.m_Aid = wlk.key.m_KeyInContract.aid;
        fc.m_Amount = wlk.payout.amount;

        key_material::Owner km{cid, wlk.key.m_KeyInContract.nft_id};

        SigRequest sig;
        sig.m_pID = &km;
        sig.m_nID = sizeof(km);

        Env::GenerateKernel(&cid, args.kMethod, &args, sizeof(args), &fc, 1,
                            &sig, 1, count ? "" : "Withdraw", 0);

        if (max_count == ++count)
            break;
    }
}

ON_METHOD(user, vote) {
    StatePlus s;
    if (!s.Init(cid))
        return;

    gallery::method::Vote args;
    args.nft_id = id;
    args.value = val;
    args.artist_id = AppArtist::id(cid);

    FundsChange fc;
    fc.m_Consume = 0;
    fc.m_Aid = s.config.vote_reward.aid;

    {
        Env::Key_T<gallery::Like::Key> key;
        key.m_Prefix.m_Cid = cid;
        key.m_KeyInContract.nft_id = id;
        key.m_KeyInContract.artist_id = args.artist_id;

        uint32_t like{};
        bool already_voted = Env::VarReader::Read_T(key, like);

        fc.m_Amount = already_voted ? 0 : s.config.vote_reward.amount;
    }

    key_material::Owner km{cid};

    SigRequest sig;
    sig.m_pID = &km;
    sig.m_nID = sizeof(km);

    Env::GenerateKernel(&cid, args.kMethod, &args, sizeof(args), &fc, 1, &sig,
                        1, "Vote", 0);
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
