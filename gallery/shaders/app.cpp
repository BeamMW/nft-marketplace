#include "Shaders/common.h"
#include "Shaders/app_common_impl.h"
#include "contract.h"
#include "Shaders/upgradable2/contract.h"
#include "Shaders/upgradable2/app_common_impl.h"

#include <string_view>
#include <vector>

// MANAGER

#define Gallery_manager_view(macro)

#define Gallery_manager_view_params(macro) macro(ContractID, cid)

#define Gallery_manager_view_moderators(macro) \
    macro(ContractID, cid) \
    macro(Height, h0) \
    macro(uint32_t, count) \

#define Gallery_manager_set_moderator(macro) \
    macro(ContractID, cid) \
    macro(Gallery::Moderator::Id, id) \
    macro(uint32_t, enable)

#define Gallery_manager_view_artists_stats(macro) \
    macro(ContractID, cid) \

#define Gallery_manager_view_moderators_stats(macro) \
    macro(ContractID, cid) \

#define Gallery_manager_view_collections_stats(macro) \
    macro(ContractID, cid) \

#define Gallery_manager_view_nfts_stats(macro) \
    macro(ContractID, cid) \

#define Gallery_manager_view_nfts(macro) \
    macro(ContractID, cid) \
    macro(Height, h0) \
    macro(Amount, count) \

#define Gallery_manager_view_nft_sales(macro) \
    macro(ContractID, cid) \
    macro(Gallery::Nft::Id, id) \

#define Gallery_manager_view_collections(macro) \
    macro(ContractID, cid) \
    macro(Height, h0) \
    macro(uint32_t, count) \
    macro(uint32_t, nfts) \

#define Gallery_manager_view_artists(macro) \
    macro(ContractID, cid) \
    macro(Height, h0) \
    macro(uint32_t, count) \
    macro(uint32_t, nfts) \
    macro(uint32_t, collections) \

#define Gallery_manager_view_balance(macro)  macro(ContractID, cid)

#define Gallery_manager_get_id(macro)

#define Gallery_manager_explicit_upgrade(macro) macro(ContractID, cid)

#define Gallery_manager_set_nft_status(macro) \
    macro(ContractID, cid) \

#define Gallery_manager_set_collection_status(macro) \
    macro(ContractID, cid) \

#define Gallery_manager_set_artist_status(macro) \
    macro(ContractID, cid) \

#define Gallery_manager_set_fee_base(macro) \
    macro(ContractID, cid) \
    macro(Amount, amount) \

#define GalleryRole_manager(macro) \
    macro(manager, view) \
    macro(manager, view_params) \
    macro(manager, view_artists_stats) \
    macro(manager, view_moderators_stats) \
    macro(manager, view_artists) \
    macro(manager, view_collections) \
    macro(manager, view_nfts) \
    macro(manager, view_nft_sales) \
    macro(manager, view_collections_stats) \
    macro(manager, view_nfts_stats) \
    macro(manager, view_balance) \
    macro(manager, get_id) \
    macro(manager, explicit_upgrade) \
    macro(manager, set_moderator) \
    macro(manager, view_moderators) \
    macro(manager, set_nft_status) \
    macro(manager, set_artist_status) \
    macro(manager, set_collection_status) \
    macro(manager, set_fee_base) \

// MODERATOR

#define Gallery_moderator_set_nft_status(macro) \
    macro(ContractID, cid) \

#define Gallery_moderator_set_collection_status(macro) \
    macro(ContractID, cid) \

#define Gallery_moderator_set_artist_status(macro) \
    macro(ContractID, cid) \

#define GalleryRole_moderator(macro) \
    macro(moderator, set_nft_status) \
    macro(moderator, set_artist_status) \
    macro(moderator, set_collection_status) \

// ARTIST

#define Gallery_artist_get_id(macro) macro(ContractID, cid)

#define Gallery_artist_set_artist(macro) \
    macro(ContractID, cid) \

#define Gallery_artist_set_collection(macro) \
    macro(ContractID, cid) \
    macro(Gallery::Collection::Id, id) \

#define Gallery_artist_set_nft(macro) \
    macro(ContractID, cid) \
    macro(Gallery::Collection::Id, collection_id) \
    macro(Amount, amount) \
    macro(AssetID, aid)

#define GalleryRole_artist(macro) \
    macro(artist, get_id) \
    macro(artist, set_nft) \
    macro(artist, set_artist) \
    macro(artist, set_collection) \

// USER

#define Gallery_user_set_price(macro) \
    macro(ContractID, cid) \
    macro(Gallery::Nft::Id, id) \
    macro(Amount, amount) \
    macro(AssetID, aid)

#define Gallery_user_transfer(macro) \
    macro(ContractID, cid) \
    macro(Gallery::Nft::Id, id) \
    macro(Gallery::Artist::Id, pkNewOwner)

#define Gallery_user_buy(macro) \
    macro(ContractID, cid) \
    macro(Gallery::Nft::Id, id)

#define Gallery_user_add_rewards(macro) \
    macro(ContractID, cid) \
    macro(uint32_t, amount)

#define Gallery_user_view_balance(macro) macro(ContractID, cid)

#define Gallery_user_withdraw(macro) \
    macro(ContractID, cid) \
    macro(uint32_t, nMaxCount) \

#define Gallery_user_vote(macro) \
    macro(ContractID, cid) \
    macro(Gallery::Nft::Id, id) \
    macro(uint32_t, val) \

#define GalleryRole_user(macro) \
    macro(user, set_price) \
    macro(user, transfer) \
    macro(user, buy) \
    macro(user, view_balance) \
    macro(user, withdraw) \
    macro(user, vote) \
    macro(user, add_rewards) \

#define GalleryRoles_All(macro) \
    macro(manager) \
    macro(moderator) \
    macro(artist) \
    macro(user)

BEAM_EXPORT void Method_0() {
    // scheme
    Env::DocGroup root("");

    {
        Env::DocGroup gr("roles");

#define THE_FIELD(type, name) Env::DocAddText(#name, #type);
#define THE_METHOD(role, name) { Env::DocGroup grMethod(#name);  Gallery_##role##_##name(THE_FIELD) }
#define THE_ROLE(name) { Env::DocGroup grRole(#name); GalleryRole_##name(THE_METHOD) }
        
        GalleryRoles_All(THE_ROLE)
#undef THE_ROLE
#undef THE_METHOD
#undef THE_FIELD
    }
}

#define THE_FIELD(type, name) const type& name,
#define ON_METHOD(role, name) void On_##role##_##name(Gallery_##role##_##name(THE_FIELD) int unused = 0)

void OnError(const char* sz) {
    Env::DocAddText("error", sz);
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
            if constexpr(std::is_same_v<Id, PubKey>) {
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

Gallery::Status StringToStatus(const std::string_view& str_status) {
    Gallery::Status ret_status;
    if (str_status == "pending") {
        ret_status = Gallery::Status::kPending;
    } else if (str_status == "approved") {
        ret_status = Gallery::Status::kApproved;
    } else if (str_status == "rejected") {
        ret_status = Gallery::Status::kRejected;
    } else {
        OnError("Invalid status");
        ret_status = Gallery::Status::kNone;
    }
    return ret_status;
}

namespace KeyMaterial {
    const char g_szAdmin[] = "Gallery-key-admin";

    struct MyAdminKey : public Env::KeyID {
        MyAdminKey() : Env::KeyID(g_szAdmin, sizeof(g_szAdmin) - sizeof(char)) {}
    };

#pragma pack (push, 1)

    const char g_szOwner[] = "Gallery-key-owner";

    struct Owner {
        ContractID m_Cid;
        Gallery::Nft::Id m_ID;
        uint8_t m_pSeed[sizeof(g_szOwner) - sizeof(char)];

        Owner() {
            Env::Memcpy(m_pSeed, g_szOwner, sizeof(m_pSeed));
            m_ID = 0;
        }

        void SetCid(const ContractID& cid) {
            _POD_(m_Cid) = cid;
        }

        void Get(PubKey& pk) const {
            Env::DerivePk(pk, this, sizeof(*this));
        }
    };
#pragma pack (pop)

    const Gallery::Nft::Id g_MskImpression = (static_cast<Gallery::Nft::Id>(-1) >> 1) + 1; // hi bit
}

ON_METHOD(manager, view) {
    static const ShaderID s_pSid[] = {
        Gallery::s_SID_0,
    };

    ContractID pVerCid[_countof(s_pSid)];
    Height pVerDeploy[_countof(s_pSid)];

    ManagerUpgadable2::Walker wlk;
    wlk.m_VerInfo.m_Count = _countof(s_pSid);
    wlk.m_VerInfo.s_pSid = s_pSid;
    wlk.m_VerInfo.m_pCid = pVerCid;
    wlk.m_VerInfo.m_pHeight = pVerDeploy;

    KeyMaterial::MyAdminKey kid;
    wlk.ViewAll(&kid);
}

bool ReadItem(const ContractID& cid, Gallery::Nft::Id id, Gallery::Nft& m) {
    Env::Key_T<Gallery::Nft::Key> k;
    k.m_Prefix.m_Cid = cid;
    k.m_KeyInContract.id = Utils::FromBE(id);

    if (!Env::VarReader::Read_T(k, m)) {
        OnError("not found");
        return false;
    } else {
        return true;
    }
}

struct OwnerInfo {
    KeyMaterial::Owner m_km;

    bool DeduceOwner(const ContractID& cid, Gallery::Nft::Id id, const Gallery::Nft& m) {
        return DeduceOwner(cid, id, m.m_pkOwner);
    }

    bool DeduceOwnerRaw(Gallery::Nft::Id id, const PubKey& pkOwner) {
        PubKey pk;
        m_km.m_ID = id;
        m_km.Get(pk);
        return (_POD_(pk) == pkOwner);
    }

    bool DeduceOwner(const ContractID& cid, Gallery::Nft::Id id, const PubKey& pkOwner) {
        m_km.SetCid(cid);
        return DeduceOwnerRaw(id, pkOwner) || // owner
            DeduceOwnerRaw(0, pkOwner); // artist
    }

    bool ReadOwnedItem(const ContractID& cid, Gallery::Nft::Id id, Gallery::Nft& m) {
        if (!ReadItem(cid, id, m))
            return false;

        if (DeduceOwner(cid, id, m))
            return true;

        OnError("not owned");
        return false;
    }
};

struct StatePlus : public Gallery::State {
    bool Init(const ContractID& cid) {
        Env::Key_T<uint8_t> key;
        _POD_(key.m_Prefix.m_Cid) = cid;
        key.m_KeyInContract = Gallery::State::s_Key;

        if (Env::VarReader::Read_T(key, Cast::Down<Gallery::State>(*this)))
            return true;

        OnError("no such a contract");
        return false;
    }
};

ON_METHOD(manager, set_moderator) {
    Gallery::Method::SetModerator args;
    args.id = id;
    args.approved = enable;
    KeyMaterial::MyAdminKey kid;
    Env::GenerateKernel(&cid, args.s_iMethod, &args, sizeof(args), nullptr, 0, &kid, 1, "Set moderator", 500000);
}

struct ImpressionWalker {
    Env::VarReaderEx<true> m_Reader;
    Env::Key_T<Gallery::Impression::Key> m_Key;
    Gallery::Impression m_Value;
    bool m_Valid = false;

    void Enum(const ContractID& cid, Gallery::Nft::Id id) {
        Enum(cid, id, id);
    }

    void Enum(const ContractID& cid, Gallery::Nft::Id id0, Gallery::Nft::Id id1) {
        _POD_(m_Key.m_Prefix.m_Cid) = cid;
        m_Key.m_KeyInContract.m_ID.nft_id = id0;
        _POD_(m_Key.m_KeyInContract.m_ID.m_pkUser).SetZero();

        Env::Key_T<Gallery::Impression::Key> k1;
        _POD_(k1.m_Prefix.m_Cid) = cid;
        k1.m_KeyInContract.m_ID.nft_id = id1;
        _POD_(k1.m_KeyInContract.m_ID.m_pkUser).SetObject(0xff);

        m_Reader.Enum_T(m_Key, k1);
        Move();
    }

    void Move() {
        m_Valid = m_Reader.MoveNext_T(m_Key, m_Value);
    }
};

#pragma pack (push, 0)
struct MyModerator : public Gallery::Moderator {
public:
    static PubKey id(const ContractID& cid) {
        KeyMaterial::Owner km;
        km.SetCid(cid);
        PubKey pk;
        km.Get(pk);
        return pk;
    }

    bool is_moderator(const ContractID& cid) {
        Id id = MyModerator::id(cid);
        bool exists = Read(cid, id);
        return exists && approved;
    }

    bool Read(const ContractID& cid, const Id& id) {
        Env::Key_T<Key> k;
        k.m_Prefix.m_Cid = cid;
        k.m_KeyInContract.id = id;
        if (!Env::VarReader::Read_T(k, Cast::Down<Gallery::Moderator>(*this)))
            return false;
        return true;
    }

    void Print(const ContractID& cid, const Id& id) {
        Env::DocGroup gr1("");
        Env::DocAddBlob_T("id", id);
        Env::DocAddNum32("updated", updated);
        Env::DocAddNum("hReg", registered);
        Env::DocAddText("status", approved ? "approved" : "pending");
    }

    bool ReadNext(Env::VarReader& r, Env::Key_T<Key>& k) {
        return r.MoveNext_T(k, Cast::Down<Gallery::Moderator>(*this));
    }
};
#pragma pack (pop)

#pragma pack (push, 0)
struct MyArtist : public Gallery::Artist {
public:
    char data[s_DataMaxLen];
    Utils::Vector<char> label;

    std::string_view GetLabel() const {
        return std::string_view(label.m_p, label_len);
    }

    static PubKey id(const ContractID& cid) {
        KeyMaterial::Owner km;
        km.SetCid(cid);
        PubKey pk;
        km.Get(pk);
        return pk;
    }

    // TODO: label_exists

    void Print(const ContractID& cid, const Id& id) {
        Env::DocGroup gr1("");
        Env::DocAddBlob_T("id", id);
        Env::DocAddNum32("updated", updated);
        Env::DocAddText("label", label.m_p);
        Env::DocAddText("data", data);
        Env::DocAddNum("hReg", m_hRegistered);
        Env::DocAddText("status", Gallery::status_to_string(status).data());

        uint32_t print_nfts{};
        Env::DocGetNum32("nfts", &print_nfts);

        uint32_t print_collections{};
        Env::DocGetNum32("collections", &print_collections);

        if (!print_nfts && !print_collections)
            return;

        using ACKey = Gallery::Index<Gallery::Tag::kArtistCollectionIdx,
            Gallery::Artist::Id, Gallery::Collection>::Key;

        using CAKey = Gallery::Index<Gallery::Tag::kCollectionNftIdx,
            Gallery::Collection::Id, Gallery::Nft>::Key;

        Env::Key_T<ACKey> ack0, ack1;
        _POD_(ack0.m_Prefix.m_Cid) = cid;
        _POD_(ack1.m_Prefix.m_Cid) = cid;
        ack0.m_KeyInContract.id = id;
        ack1.m_KeyInContract.id = id;
        ack0.m_KeyInContract.t_id = 0;
        ack1.m_KeyInContract.t_id = static_cast<Gallery::Collection::Id>(-1);

        if (print_collections) {
            Env::VarReader rac(ack0, ack1);
            Env::DocArray gr2("collections");
            bool exists;
            while (rac.MoveNext_T(ack0, exists)) {
                Env::DocAddNum32("", Utils::FromBE(ack0.m_KeyInContract.t_id));
            }
        }

        if (print_nfts) {
            ack0.m_KeyInContract.id = id;
            ack0.m_KeyInContract.t_id = 0;
            Env::VarReader rac(ack0, ack1);

            bool exists;
            Env::DocArray gr2("nfts");
            while (rac.MoveNext_T(ack0, exists)) {
                Env::Key_T<CAKey> cak0, cak1;
                _POD_(cak0.m_Prefix.m_Cid) = cid;
                _POD_(cak1.m_Prefix.m_Cid) = cid;
                cak0.m_KeyInContract.id = ack0.m_KeyInContract.t_id;
                cak1.m_KeyInContract.id = ack0.m_KeyInContract.t_id;
                cak0.m_KeyInContract.t_id = 0;
                cak1.m_KeyInContract.t_id = static_cast<Gallery::Nft::Id>(-1);
                Env::VarReader rca(cak0, cak1);

                while (rca.MoveNext_T(cak0, exists)) {
                    Env::DocAddNum32("", Utils::FromBE(cak0.m_KeyInContract.t_id));
                }
            }
        }
    }

    bool ReadNext(Env::VarReader& r, Env::Key_T<Key>& key) {
        label.m_Count = 0;


        while (true) {
            uint32_t nKey = sizeof(key), nVal = sizeof(Artist) + sizeof(data);
            if (!r.MoveNext(&key, nKey, this, nVal, 0))
                return false;

            if (sizeof(key) != nKey)
                continue;

            nVal -= sizeof(Gallery::Artist);
            data[std::min(nVal, s_DataMaxLen)] = 0;
            break;
        }

        auto cid = key.m_Prefix.m_Cid;
        Env::Key_T<Gallery::Events::AddArtistLabel::Key> alk0, alk1;
        _POD_(alk0.m_Prefix.m_Cid) = cid;
        _POD_(alk1.m_Prefix.m_Cid) = cid;
        alk0.m_KeyInContract.id = key.m_KeyInContract.id;
        alk1.m_KeyInContract.id = key.m_KeyInContract.id;

        uint32_t label_count = 0;
        Env::LogReader alr(alk0, alk1);
        for ( ; ; label_count++) {
            uint32_t nData = 0, nKey = sizeof(alk0);
            if (!alr.MoveNext(&alk0, nKey, nullptr, nData, 0))
                break;

            label.Prepare(label.m_Count + nData);
            alr.MoveNext(&alk0, nKey, label.m_p + label.m_Count, nData, 1);
            label.m_Count += nData;
        }
        label.m_p[label.m_Count] = '\0';

        if (!label_count)
            return false;
        return true;
    }
};
#pragma pack (pop)

#pragma pack (push, 0)
struct MyCollection : public Gallery::Collection {
    char m_szLabelData[s_TotalMaxLen + 2];

    void Print(const ContractID& cid, const Id& id) {
        // DocAddText prints char[] until it meets \0 symbol, but
        // m_szLabelData contains label + data without \0 symbol between them
        char label[label_len + 1];
        char data[data_len + 1];
        label[label_len] = 0;
        data[data_len] = 0;
        Env::Memcpy(label, m_szLabelData, label_len);
        Env::Memcpy(data, m_szLabelData + label_len, data_len);

        Env::DocGroup gr1("");
        Env::DocAddNum32("id", id);
        Env::DocAddNum32("updated", updated);
        Env::DocAddNum32("owned", _POD_(m_pkAuthor) == MyArtist::id(cid));
        Env::DocAddText("label", label);
        Env::DocAddText("data", data);
        Env::DocAddBlob_T("author", m_pkAuthor);
        Env::DocAddNum32("nfts_count", nfts_num);
        Env::DocAddText("status", Gallery::status_to_string(status).data());
        {
            Env::DocGroup gr_sold("total_sold");
            Env::DocAddNum("count", total_sold);
            Env::DocAddNum("volume", total_sold_price);
            Env::DocAddNum32("aid", 0);
        }
        {
            Env::DocGroup gr_price("max_price");
            Env::DocAddNum("value", max_sold.price.m_Amount);
            Env::DocAddNum("aid", max_sold.price.m_Aid);
            Env::DocAddNum32("nft_id", max_sold.nft_id);
        }
        {
            Env::DocGroup gr_price("min_price");
            Env::DocAddNum("value", min_sold.price.m_Amount);
            Env::DocAddNum("aid", min_sold.price.m_Aid);
            Env::DocAddNum32("nft_id", min_sold.nft_id);
        }

        uint32_t print_nfts{};
        Env::DocGetNum32("nfts", &print_nfts);

        if (!print_nfts) return;

        using CAKey = Gallery::Index<Gallery::Tag::kCollectionNftIdx,
            Gallery::Collection::Id, Gallery::Nft>::Key;

        Env::Key_T<CAKey> cak0, cak1;
        _POD_(cak0.m_Prefix.m_Cid) = cid;
        _POD_(cak1.m_Prefix.m_Cid) = cid;
        cak0.m_KeyInContract.id = Utils::FromBE(id);
        cak1.m_KeyInContract.id = Utils::FromBE(id);
        cak0.m_KeyInContract.t_id = 0;
        cak1.m_KeyInContract.t_id = static_cast<Gallery::Nft::Id>(-1);
        Env::VarReader rca(cak0, cak1);

        Env::DocArray gr2("nfts");
        bool exists;
        while (rca.MoveNext_T(cak0, exists)) {
            Env::DocAddNum32("", Utils::FromBE(cak0.m_KeyInContract.t_id));
        }
    }

    bool ReadNext(Env::VarReader& r, Env::Key_T<Key>& key) {
        while (true) {
            uint32_t nKey = sizeof(key), nVal = sizeof(Collection) + sizeof(m_szLabelData);
            if (!r.MoveNext(&key, nKey, this, nVal, 0))
                return false;

            if (sizeof(key) != nKey)
                continue;

            nVal -= sizeof(Gallery::Collection);
            m_szLabelData[std::min(nVal, s_TotalMaxLen)] = 0;
            break;
        }
        return true;
    }
};
#pragma pack (pop)

#pragma pack (push, 0)
struct MyNft : public Gallery::Nft {
    Utils::Vector<char> vData;
    Utils::Vector<char> vLabel;

    void Print(const ContractID& cid, const Id& id) {
        ImpressionWalker iwlk;
        iwlk.Enum(cid, id);

        Env::DocGroup gr1("");
        Env::DocAddNum32("id", id);
        Env::DocAddNum32("updated", updated);
        Env::DocAddText("status", Gallery::status_to_string(status).data());
        Env::DocAddText("label", vLabel.m_p);
        Env::DocAddText("data", vData.m_p);
        Env::DocAddBlob_T("author", m_pkAuthor);
        Env::DocAddNum32("collection", collection_id);
        if (m_Aid)
            Env::DocAddBlob_T("checkout.aid", m_Aid);

        OwnerInfo oi;

        if (!_POD_(m_pkOwner).IsZero()) {
            uint32_t owned = !!oi.DeduceOwner(cid, iwlk.m_Key.m_KeyInContract.m_ID.nft_id, m_pkOwner);
            Env::DocAddBlob_T("owner", m_pkOwner);
            Env::DocAddNum("owned", owned);

            if (owned) {
                Env::Key_T<Gallery::Events::Sell::Key> key;
                _POD_(key.m_Prefix.m_Cid) = cid;
                key.m_KeyInContract.m_ID = id;

                Env::LogReader r(key, key);
                Gallery::Events::Sell evt;
                if (r.MoveNext_T(key, evt)) {
                    Env::DocGroup gr("first_sale");
                    Env::DocAddNum("height", r.m_Pos.m_Height);
                    Env::DocAddNum("amount", evt.m_Price.m_Amount);
                    Env::DocAddNum("aid", evt.m_Price.m_Aid);
                }
            }

            if (m_Price.m_Amount) {
                Env::DocGroup gr_price("price");
                Env::DocAddNum("aid", m_Price.m_Aid);
                Env::DocAddNum("amount", m_Price.m_Amount);
            }
        }

        uint32_t nImpressions = 0;
        uint32_t nMyImpression = 0;
        bool bMyImpressionSet = false;
        bool bMyImpressionKey = false;
        PubKey pkMyImpression;

        auto idNorm = iwlk.m_Key.m_KeyInContract.m_ID.nft_id;

        for ( ; iwlk.m_Valid; iwlk.Move()) {
            auto idWlk = iwlk.m_Key.m_KeyInContract.m_ID.nft_id;
            if (idWlk > idNorm)
                continue;
            if (idWlk < idNorm)
                break;

            if (!bMyImpressionSet) {
                if (!bMyImpressionKey) {
                    bMyImpressionKey = true;
                    oi.m_km.SetCid(cid);
                    oi.m_km.m_ID = iwlk.m_Key.m_KeyInContract.m_ID.nft_id | KeyMaterial::g_MskImpression;
                    oi.m_km.Get(pkMyImpression);
                }

                if (_POD_(pkMyImpression) == iwlk.m_Key.m_KeyInContract.m_ID.m_pkUser) {
                    bMyImpressionSet = true;
                    nMyImpression = iwlk.m_Value.m_Value;
                }
            }

            if (iwlk.m_Value.m_Value)
                nImpressions++;
        }

        Env::DocAddNum("impressions", nImpressions);
        if (bMyImpressionSet)
            Env::DocAddNum("my_impression", nMyImpression);
    }

    // TODO: ReadItem

    bool ReadNext(Env::VarReader& r, Env::Key_T<Gallery::Nft::Key>& key) {
        uint32_t nKey = sizeof(key), nVal = sizeof(Gallery::Nft);
        if (!r.MoveNext(&key, nKey, this, nVal, 0))
            return false;

        vData.m_Count = 0;
        vLabel.m_Count = 0;
        auto id = Utils::FromBE(key.m_KeyInContract.id);
        auto cid = key.m_Prefix.m_Cid;

        Env::Key_T<Gallery::Events::AddNftData::Key> adk0, adk1;
        _POD_(adk0.m_Prefix.m_Cid) = cid;
        _POD_(adk1.m_Prefix.m_Cid) = cid;
        adk0.m_KeyInContract.m_ID = id;
        adk1.m_KeyInContract.m_ID = id;
        _POD_(adk0.m_KeyInContract.m_pkArtist).SetZero();
        _POD_(adk1.m_KeyInContract.m_pkArtist).SetObject(0xff);

        Env::Key_T<Gallery::Events::AddNftLabel::Key> alk0, alk1;
        _POD_(alk0.m_Prefix.m_Cid) = cid;
        _POD_(alk1.m_Prefix.m_Cid) = cid;
        alk0.m_KeyInContract.m_ID = id;
        alk1.m_KeyInContract.m_ID = id;
        _POD_(alk0.m_KeyInContract.m_pkArtist).SetZero();
        _POD_(alk1.m_KeyInContract.m_pkArtist).SetObject(0xff);

        uint32_t nDataCount = 0;
        uint32_t nLabelCount = 0;

        Env::LogReader adr(adk0, adk1);
        for ( ; ; nDataCount++) {
            uint32_t nData = 0, nKey = sizeof(adk0);
            if (!adr.MoveNext(&adk0, nKey, nullptr, nData, 0))
                break;

            vData.Prepare(vData.m_Count + nData);
            adr.MoveNext(&adk0, nKey, vData.m_p + vData.m_Count, nData, 1);
            vData.m_Count += nData;
        }
        vData.m_p[vData.m_Count] = '\0';

        Env::LogReader alr(alk0, alk1);
        for ( ; ; nLabelCount++) {
            uint32_t nData = 0, nKey = sizeof(alk0);
            if (!alr.MoveNext(&alk0, nKey, nullptr, nData, 0))
                break;

            vLabel.Prepare(vLabel.m_Count + nData);
            alr.MoveNext(&alk0, nKey, vLabel.m_p + vLabel.m_Count, nData, 1);
            vLabel.m_Count += nData;
        }
        vLabel.m_p[vLabel.m_Count] = '\0';

        if (!nDataCount)
            return false;

        return true;
    }
};
#pragma pack (pop)

template <class T, class HeightIdx>
class GalleryObjectPrinter {
public:
    GalleryObjectPrinter(const ContractID& cid) : cid_{cid} {}

    void Print() {
        Env::Key_T<typename T::Key> k0, k1;
        _POD_(k0.m_Prefix.m_Cid) = cid_;
        _POD_(k1.m_Prefix.m_Cid) = cid_;
        _POD_(k0.m_KeyInContract.id).SetZero();
        _POD_(k1.m_KeyInContract.id).SetObject(0xff);

        Env::VarReader r(k0, k1);
        Env::DocArray gr0("items");

        while (object_.ReadNext(r, k0)) {
            if constexpr(std::is_same_v<typename T::Id, PubKey>)
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
        _POD_(k.m_Prefix.m_Cid) = cid_;
        if constexpr(std::is_same_v<typename T::Id, PubKey>)
            _POD_(k.m_KeyInContract.id) = id;
        else 
            _POD_(k.m_KeyInContract.id) = Utils::FromBE(id);
 
        Env::VarReader r(k, k);
        if (!object_.ReadNext(r, k))
            return false;

        if constexpr(std::is_same_v<typename T::Id, PubKey>)
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
            while (r.MoveNext_T(k0, exists) && (cur_cnt++ < count || prev_h == k0.m_KeyInContract.id)) {
                if constexpr(std::is_same_v<typename T::Id, PubKey>)
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

ON_METHOD(manager, view_params) {
    StatePlus s;
    if (!s.Init(cid))
        return;

    PubKey admin_pk;
    KeyMaterial::MyAdminKey().get_Pk(admin_pk);

    uint32_t bIsAdmin = (_POD_(s.m_Config.m_pkAdmin) == admin_pk);

    MyModerator moder;
    Env::DocAddNum("is_admin", bIsAdmin);
    Env::DocAddNum32("is_moderator", moder.is_moderator(cid));
    Env::DocAddNum("fee_base", s.fee_base);

    {
        Env::DocGroup gr1("vote_reward");
        Env::DocAddNum("aid", s.m_Config.m_VoteReward.m_Aid);
        Env::DocAddNum("amount", s.m_Config.m_VoteReward.m_Amount);
        Env::DocAddNum("balance", s.m_VoteBalance);
    }
}

bool artist_label_exists(const ContractID& cid, const std::string_view& label, bool& artist_exists) {
    artist_exists = false;

    Env::Key_T<Gallery::Artist::Key> k0, k1;
    k0.m_Prefix.m_Cid = cid;
    k1.m_Prefix.m_Cid = cid;
    _POD_(k0.m_KeyInContract.id).SetZero();
    _POD_(k1.m_KeyInContract.id).SetObject(0xff);

    Env::VarReader r(k0, k1);
    MyArtist a;
    PubKey my_key = a.id(cid);

    while (true) {
        if (!a.ReadNext(r, k0))
            break;

        if (!Env::Memcmp(&k0.m_KeyInContract.id, &my_key, sizeof(PubKey))) {
            artist_exists = true;
            continue;
        }

        if (a.GetLabel() == label)
            return true;
    }
    return false;
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
    char buf[256];
    int buf_len = Env::DocGetText("ids", buf, sizeof(buf));

    using HeightArtistIdx = 
        Gallery::Index<Gallery::Tag::kHeightArtistIdx, Height, Gallery::Artist>;

    GalleryObjectPrinter<MyArtist, HeightArtistIdx> a{cid};
    if (count && h0)
        a.Print(h0, count);
    else if (buf_len)
        a.Print(buf);
    else
        a.Print();
}

ON_METHOD(manager, view_moderators) {
    char buf[256];
    int buf_len = Env::DocGetText("ids", buf, sizeof(buf));

    using HeightModeratorIdx = 
        Gallery::Index<Gallery::Tag::kHeightModeratorIdx, Height, Gallery::Moderator>;

    GalleryObjectPrinter<MyModerator, HeightModeratorIdx> m{cid};
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
        Gallery::Index<Gallery::Tag::kHeightCollectionIdx, Height, Gallery::Collection>;

    GalleryObjectPrinter<MyCollection, HeightCollectionIdx> c{cid};
    if (count && h0)
        c.Print(h0, count);
    else if (buf_len)
        c.Print(buf);
    else
        c.Print();
}

ON_METHOD(artist, set_nft) {
    struct {
        Gallery::Method::SetNft args;
        char m_szLabelData[Gallery::Nft::s_TotalMaxLen];
    } d;

    KeyMaterial::Owner km;
    km.SetCid(cid);
    PubKey pkArtist;
    km.Get(d.args.m_pkArtist);

    d.args.collection_id = collection_id;
    d.args.m_Price.m_Amount = amount;
    d.args.m_Price.m_Aid = aid;

    if (!collection_id) {
        OnError("collection_id must be specified");
        return;
    }

    uint32_t nArgSize = sizeof(d.args);
    uint32_t nLabelSize = Env::DocGetText("label", d.m_szLabelData, Gallery::Nft::s_LabelMaxLen + 1); // including 0-term

    if (nLabelSize > Gallery::Nft::s_LabelMaxLen + 1) {
        OnError("label is too long");
        return;
    }

    d.args.label_len = (nLabelSize ? nLabelSize - 1 : 0);
    nArgSize += d.args.label_len;

    uint32_t nDataSize = Env::DocGetText("data", d.m_szLabelData + d.args.label_len, Gallery::Nft::s_DataMaxLen);

    if (!nDataSize) {
        OnError("data must be specified");
        return;
    }

    if (nDataSize > Gallery::Artist::s_DataMaxLen + 1) {
        OnError("data is too long");
        return;
    }

    d.args.data_len = nDataSize;
    nArgSize += d.args.data_len;

    SigRequest sig;
    sig.m_pID = &km;
    sig.m_nID = sizeof(km);

    // estimate charge, it may be non-negligible.
    // - Patent verification: LoadVar + SaveVar, 25K units
    // - Artist verification: LoadVar, 5K units
    // - Global state: LoadVar + SaveVar, 25K units
    // - Nft creation: SaveVar, 20K units
    // - AddSig by admin, 10K units
    // - event for the data. This is 100 units per byte, plus 5K units per call, which is repeated for each 8K of data
    // - add some extra for other stuff

    uint32_t nCharge =
        ManagerUpgadable2::get_ChargeInvoke() +
        Env::Cost::LoadVar_For(sizeof(Gallery::State)) +
        Env::Cost::SaveVar_For(sizeof(Gallery::State)) +
        Env::Cost::LoadVar_For(sizeof(Gallery::Artist)) +
        Env::Cost::LoadVar_For(sizeof(Gallery::Artist::Key)) +
        Env::Cost::SaveVar_For(sizeof(Gallery::Nft)) +
        Env::Cost::SaveVar_For(sizeof(Height)) +
        Env::Cost::AddSig +
        Env::Cost::Cycle * 200;

    const uint32_t nSizeEvt = 0x100000;
    uint32_t nFullCycles = nDataSize / nSizeEvt;

    nCharge += (Env::Cost::Log_For(nSizeEvt) + Env::Cost::Cycle * 50) * nFullCycles;

    nDataSize %= nSizeEvt;
    if (nDataSize)
        nCharge += Env::Cost::Log_For(nDataSize) + Env::Cost::Cycle * 50;

    StatePlus s;
    s.Init(cid);

    Env::GenerateKernel(&cid, d.args.s_iMethod, &d, nArgSize, nullptr, 0, &sig, 1, "Upload nft", nCharge + 25000000 + s.fee_base / 10);
}

ON_METHOD(user, add_rewards) {
    StatePlus s;
    if (!s.Init(cid))
        return;

    Gallery::Method::AddVoteRewards args;
    args.m_Amount = s.m_Config.m_VoteReward.m_Amount * amount;

    if (!args.m_Amount) {
        OnError("no rewards");
        return;
    }

    FundsChange fc;
    fc.m_Aid = s.m_Config.m_VoteReward.m_Aid;
    fc.m_Amount = args.m_Amount;
    fc.m_Consume = 1;

    Env::GenerateKernel(&cid, args.s_iMethod, &args, sizeof(args), &fc, 1, nullptr, 0, "Add voting rewards", 0);
}

ON_METHOD(manager, get_id) {
    PubKey pk;
    KeyMaterial::MyAdminKey().get_Pk(pk);
    Env::DocAddBlob_T("get_id", pk);
}

ON_METHOD(manager, explicit_upgrade) {
    ManagerUpgadable2::MultiSigRitual::Perform_ExplicitUpgrade(cid);
}

struct BalanceWalker {
    Env::VarReaderEx<true> m_Reader;
    Env::Key_T<Gallery::Payout::Key> m_Key;
    Gallery::Payout m_Data;

    // TODO: add tree (map) class
    Utils::Vector<Gallery::AmountWithAsset> m_Totals;

    void Enum(const ContractID& cid) {
        _POD_(m_Key.m_Prefix.m_Cid) = cid;
        _POD_(m_Key.m_KeyInContract).SetZero();

        Env::Key_T<Gallery::Payout::Key> k1;
        _POD_(k1.m_Prefix.m_Cid) = cid;
        _POD_(k1.m_KeyInContract).SetObject(0xff);

        m_Reader.Enum_T(m_Key, k1);
    }

    bool MoveNext() {
        return m_Reader.MoveNext_T(m_Key, m_Data);
    }

    void Print() const {
        Env::DocAddNum("aid", m_Key.m_KeyInContract.m_Aid);
        Env::DocAddNum("amount", m_Data.m_Amount);
    }

    void AddToTotals() {
        auto aid = m_Key.m_KeyInContract.m_Aid;

        uint32_t iIdx = 0;
        for (; ; iIdx++) {
            if (iIdx == m_Totals.m_Count) {
                auto& x = m_Totals.emplace_back();
                x.m_Aid = aid;
                x.m_Amount = m_Data.m_Amount;
                break;
            }

            if (m_Totals.m_p[iIdx].m_Aid == aid) {
                m_Totals.m_p[iIdx].m_Amount += m_Data.m_Amount; // don't care if overflows
                break;
            }
        }
    }

    void PrintTotals() {
        Env::DocArray gr0("totals");

        for (uint32_t i = 0; i < m_Totals.m_Count; i++) {
            Env::DocGroup gr1("");

            auto& x = m_Totals.m_p[i];
            Env::DocAddNum("aid", x.m_Aid);
            Env::DocAddNum("amount", x.m_Amount);
        }
    }
};

ON_METHOD(manager, view_balance) {
    BalanceWalker wlk;
    {
        Env::DocArray gr0("items");
        for (wlk.Enum(cid); wlk.MoveNext(); ) {
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
    std::vector<Gallery::Nft::Id> ids_vec(ParseIds<Gallery::Nft::Id>(buf));

    size_t args_size = sizeof(Gallery::Method::SetNftStatus) + sizeof(Gallery::Nft::Id) * ids_vec.size();
    std::unique_ptr<Gallery::Method::SetNftStatus> args(
            static_cast<Gallery::Method::SetNftStatus*>(::operator new(args_size)));

    Env::DocGetText("status", buf, sizeof(buf));
    args->status = StringToStatus(buf);
    if (args->status == Gallery::Status::kNone) return;

    KeyMaterial::MyAdminKey kid;
    kid.get_Pk(args->signer);

    args->ids_num = ids_vec.size();
    Env::Memcpy(args->ids, ids_vec.data(), sizeof(Gallery::Nft::Id) * ids_vec.size());

    Env::GenerateKernel(&cid, args->s_iMethod, args.get(), args_size, nullptr, 0, &kid, 1, "Update nft's status", 2500000);
}

ON_METHOD(moderator, set_nft_status) {
    char buf[256];
    Env::DocGetText("ids", buf, sizeof(buf));
    std::vector<Gallery::Nft::Id> ids_vec(ParseIds<Gallery::Nft::Id>(buf));

    size_t args_size = sizeof(Gallery::Method::SetNftStatus) + sizeof(Gallery::Nft::Id) * ids_vec.size();
    std::unique_ptr<Gallery::Method::SetNftStatus> args(
            static_cast<Gallery::Method::SetNftStatus*>(::operator new(args_size)));

    Env::DocGetText("status", buf, sizeof(buf));
    args->status = StringToStatus(buf);
    if (args->status == Gallery::Status::kNone) return;

    KeyMaterial::Owner km;
    km.SetCid(cid);
    km.Get(args->signer);

    args->ids_num = ids_vec.size();
    Env::Memcpy(args->ids, ids_vec.data(), sizeof(Gallery::Nft::Id) * ids_vec.size());

    SigRequest sig;
    sig.m_pID = &km;
    sig.m_nID = sizeof(km);
    
    Env::GenerateKernel(&cid, args->s_iMethod, args.get(), args_size, nullptr, 0, &sig, 1, "Update nft's status", 2500000);
}

ON_METHOD(moderator, set_artist_status) {
    char buf[256];
    Env::DocGetText("ids", buf, sizeof(buf));
    std::vector<Gallery::Artist::Id> ids_vec(ParseIds<Gallery::Artist::Id>(buf));

    size_t args_size = sizeof(Gallery::Method::SetArtistStatus) + sizeof(Gallery::Artist::Id) * ids_vec.size();
    std::unique_ptr<Gallery::Method::SetArtistStatus> args(
            static_cast<Gallery::Method::SetArtistStatus*>(::operator new(args_size)));

    Env::DocGetText("status", buf, sizeof(buf));
    args->status = StringToStatus(buf);
    if (args->status == Gallery::Status::kNone) return;

    KeyMaterial::Owner km;
    km.SetCid(cid);
    km.Get(args->signer);

    args->ids_num = ids_vec.size();
    Env::Memcpy(args->ids, ids_vec.data(), sizeof(Gallery::Artist::Id) * ids_vec.size());

    SigRequest sig;
    sig.m_pID = &km;
    sig.m_nID = sizeof(km);
    
    Env::GenerateKernel(&cid, args->s_iMethod, args.get(), args_size, nullptr, 0, &sig, 1, "Update artist's status", 2500000);
}

ON_METHOD(manager, set_artist_status) {
    char buf[256];
    Env::DocGetText("ids", buf, sizeof(buf));
    std::vector<Gallery::Artist::Id> ids_vec(ParseIds<Gallery::Artist::Id>(buf));

    size_t args_size = sizeof(Gallery::Method::SetArtistStatus) + sizeof(Gallery::Artist::Id) * ids_vec.size();
    std::unique_ptr<Gallery::Method::SetArtistStatus> args(
            static_cast<Gallery::Method::SetArtistStatus*>(::operator new(args_size)));

    Env::DocGetText("status", buf, sizeof(buf));
    args->status = StringToStatus(buf);
    if (args->status == Gallery::Status::kNone) return;

    KeyMaterial::MyAdminKey kid;
    kid.get_Pk(args->signer);

    args->ids_num = ids_vec.size();
    Env::Memcpy(args->ids, ids_vec.data(), sizeof(Gallery::Artist::Id) * ids_vec.size());

    Env::GenerateKernel(&cid, args->s_iMethod, args.get(), args_size, nullptr, 0, &kid, 1, "Update artist's status", 2500000);
}

ON_METHOD(manager, set_collection_status) {
    char buf[256];
    Env::DocGetText("ids", buf, sizeof(buf));
    std::vector<Gallery::Collection::Id> ids_vec(ParseIds<Gallery::Collection::Id>(buf));

    size_t args_size = sizeof(Gallery::Method::SetCollectionStatus) + sizeof(Gallery::Collection::Id) * ids_vec.size();
    std::unique_ptr<Gallery::Method::SetCollectionStatus> args(
            static_cast<Gallery::Method::SetCollectionStatus*>(::operator new(args_size)));

    Env::DocGetText("status", buf, sizeof(buf));
    args->status = StringToStatus(buf);
    if (args->status == Gallery::Status::kNone) return;

    KeyMaterial::MyAdminKey kid;
    kid.get_Pk(args->signer);

    args->ids_num = ids_vec.size();
    Env::Memcpy(args->ids, ids_vec.data(), sizeof(Gallery::Collection::Id) * ids_vec.size());

    Env::GenerateKernel(&cid, args->s_iMethod, args.get(), args_size, nullptr, 0, &kid, 1, "Update collection's status", 2500000);
}

ON_METHOD(moderator, set_collection_status) {
    char buf[256];
    Env::DocGetText("ids", buf, sizeof(buf));
    std::vector<Gallery::Collection::Id> ids_vec(ParseIds<Gallery::Collection::Id>(buf));

    size_t args_size = sizeof(Gallery::Method::SetCollectionStatus) + sizeof(Gallery::Collection::Id) * ids_vec.size();
    std::unique_ptr<Gallery::Method::SetCollectionStatus> args(
            static_cast<Gallery::Method::SetCollectionStatus*>(::operator new(args_size)));

    Env::DocGetText("status", buf, sizeof(buf));
    args->status = StringToStatus(buf);
    if (args->status == Gallery::Status::kNone) return;

    KeyMaterial::Owner km;
    km.SetCid(cid);
    km.Get(args->signer);

    SigRequest sig;
    sig.m_pID = &km;
    sig.m_nID = sizeof(km);

    args->ids_num = ids_vec.size();
    Env::Memcpy(args->ids, ids_vec.data(), sizeof(Gallery::Collection::Id) * ids_vec.size());

    Env::GenerateKernel(&cid, args->s_iMethod, args.get(), args_size, nullptr, 0, &sig, 1, "Update collection's status", 2500000);
}

ON_METHOD(manager, set_fee_base) {
    Gallery::Method::SetFeeBase args;
    args.amount = amount;
    KeyMaterial::MyAdminKey kid;
    Env::GenerateKernel(&cid, args.s_iMethod, &args, sizeof(args), nullptr, 0, &kid, 1, "Set fee base", 0);
}

ON_METHOD(artist, set_artist) {
    struct {
        Gallery::Method::SetArtist args;
        char m_szLabelData[Gallery::Artist::s_TotalMaxLen + 1];
    } d;

    KeyMaterial::Owner km;
    km.SetCid(cid);
    km.Get(d.args.m_pkArtist);

    uint32_t nArgSize = sizeof(d.args);
    uint32_t nLabelSize = Env::DocGetText("label", d.m_szLabelData, Gallery::Artist::s_LabelMaxLen + 1); // including 0-term

    if (nLabelSize > Gallery::Artist::s_LabelMaxLen + 1) { // plus \0
        OnError("label is too long");
        return;
    }

    if (nLabelSize < 2) {
        OnError("label is missing");
        return;
    }

    d.args.m_LabelLen = (nLabelSize ? nLabelSize - 1 : 0);
    nArgSize += d.args.m_LabelLen;

    bool artist_exists = false;
    if (artist_label_exists(cid, d.m_szLabelData, artist_exists)) {
        OnError("label already exists");
        return;
    }

    uint32_t nDataSize = Env::DocGetText("data", d.m_szLabelData + d.args.m_LabelLen, Gallery::Artist::s_DataMaxLen + 1); // including 0-term

    if (nDataSize > Gallery::Artist::s_DataMaxLen + 1) {
        OnError("data is too long");
        return;
    }

    d.args.m_DataLen = (nDataSize ? nDataSize - 1 : 0);
    nArgSize += d.args.m_DataLen;

    SigRequest sig;
    sig.m_pID = &km;
    sig.m_nID = sizeof(km);

    const char* comment = artist_exists ? "Updating artist's info" : "Becoming an artist";

    StatePlus s;
    s.Init(cid);

    Env::GenerateKernel(&cid, d.args.s_iMethod, &d, nArgSize, nullptr, 0, &sig, 1, comment, 25000000 + (artist_exists ? 0 : s.fee_base / 10));
}

ON_METHOD(artist, set_collection) {
    struct {
        Gallery::Method::SetCollection args;
        char m_szLabelData[Gallery::Collection::s_TotalMaxLen + 2];
    } d;

    KeyMaterial::Owner km;
    km.SetCid(cid);
    km.Get(d.args.m_pkArtist);

    d.args.collection_id = id;

    uint32_t nArgSize = sizeof(d.args);

    uint32_t nLabelSize = Env::DocGetText("label", d.m_szLabelData, Gallery::Collection::s_LabelMaxLen + 1); // including 0-term

    if (nLabelSize > Gallery::Collection::s_LabelMaxLen + 1) { // plus \0
        OnError("label is too long");
        return;
    }

    if (nLabelSize < 2) {
        OnError("label is missing");
        return;
    }

    d.args.m_LabelLen = (nLabelSize ? nLabelSize - 1 : 0);

    nArgSize += d.args.m_LabelLen;

    uint32_t nDataSize = Env::DocGetText("data", d.m_szLabelData + d.args.m_LabelLen, Gallery::Collection::s_DataMaxLen + 1); // including 0-term

    if (nDataSize > Gallery::Collection::s_DataMaxLen + 1) {
        OnError("data too long");
        return;
    }

    d.args.m_DataLen = (nDataSize ? nDataSize - 1 : 0);
    nArgSize += d.args.m_DataLen;

    SigRequest sig;
    sig.m_pID = &km;
    sig.m_nID = sizeof(km);

    StatePlus s;
    s.Init(cid);

    Env::GenerateKernel(&cid, d.args.s_iMethod, &d.args, nArgSize, nullptr, 0, &sig, 1, "Set collection", 25000000 + (id ? 0 : s.fee_base / 10));
}

ON_METHOD(artist, get_id) {
    Env::DocAddBlob_T("id", MyArtist::id(cid));
}

ON_METHOD(manager, view_nft_sales) {
    Env::Key_T<Gallery::Events::Sell::Key> key;
    _POD_(key.m_Prefix.m_Cid) = cid;
    key.m_KeyInContract.m_ID = id;

    Env::DocArray gr0("sales");

    for (Env::LogReader r(key, key); ; ) {
        Gallery::Events::Sell evt;
        if (!r.MoveNext_T(key, evt))
            break;

        Env::DocGroup gr("");
        Env::DocAddNum("height", r.m_Pos.m_Height);
        Env::DocAddNum("amount", evt.m_Price.m_Amount);
        Env::DocAddNum("aid", evt.m_Price.m_Aid);
    }
}

ON_METHOD(manager, view_nfts) {
    char buf[256];
    int buf_len = Env::DocGetText("ids", buf, sizeof(buf));

    using HeightNftIdx = 
        Gallery::Index<Gallery::Tag::kHeightNftIdx, Height, Gallery::Nft>;

    GalleryObjectPrinter<MyNft, HeightNftIdx> a{cid};
    if (count && h0)
        a.Print(h0, count);
    else if (buf_len)
        a.Print(buf);
    else
        a.Print();
}

ON_METHOD(user, set_price) {
    Gallery::Nft m;
    OwnerInfo oi;
    if (!oi.ReadOwnedItem(cid, id, m))
        return;

    Gallery::Method::SetPrice args;
    args.m_ID = id;
    args.m_Price.m_Amount = amount;
    args.m_Price.m_Aid = aid;

    SigRequest sig;
    sig.m_pID = &oi.m_km;
    sig.m_nID = sizeof(oi.m_km);

    const char* comment = args.m_Price.m_Amount ? "Set item price" : "Remove from sale";

    Env::GenerateKernel(&cid, args.s_iMethod, &args, sizeof(args), nullptr, 0, &sig, 1, comment, 2000000);
}

ON_METHOD(user, transfer) {
    Gallery::Nft m;
    OwnerInfo oi;
    if (!oi.ReadOwnedItem(cid, id, m))
        return;

    Gallery::Method::Transfer args;
    args.m_ID = id;
    _POD_(args.m_pkNewOwner) = pkNewOwner;

    SigRequest sig;
    sig.m_pID = &oi.m_km;
    sig.m_nID = sizeof(oi.m_km);

    Env::GenerateKernel(&cid, args.s_iMethod, &args, sizeof(args), nullptr, 0, &sig, 1, "Transfer item", 0);
}

ON_METHOD(user, buy) {
    Gallery::Nft m;
    if (!ReadItem(cid, id, m))
        return;

    if (!m.m_Price.m_Amount) {
        OnError("not for sale");
        return;
    }

    Gallery::Method::Buy args;
    args.m_ID = id;
    args.m_HasAid = !!m.m_Aid;
    args.m_PayMax = m.m_Price.m_Amount;

    KeyMaterial::Owner km;
    km.SetCid(cid);
    km.m_ID = id;
    km.Get(args.m_pkUser);

    FundsChange fc;
    fc.m_Consume = 1;
    fc.m_Amount = m.m_Price.m_Amount;
    fc.m_Aid = m.m_Price.m_Aid;

    Env::GenerateKernel(&cid, args.s_iMethod, &args, sizeof(args), &fc, 1, nullptr, 0, "Buy item", 2500000);
}

struct BalanceWalkerOwner : public BalanceWalker {
    OwnerInfo m_Oi;

    void Enum(const ContractID& cid) {
        BalanceWalker::Enum(cid);
    }

    bool MoveNext() {
        while (true) {
            if (!BalanceWalker::MoveNext())
                return false;

            if (m_Oi.DeduceOwner(m_Key.m_Prefix.m_Cid, m_Key.m_KeyInContract.m_ID, m_Key.m_KeyInContract.m_pkUser))
                break;
        }

        return true;
    }
};

ON_METHOD(user, view_balance) {
    BalanceWalkerOwner wlk;

    {
        Env::DocArray gr0("items");
        for (wlk.Enum(cid); wlk.MoveNext(); ) {
            Env::DocGroup gr1("");
            wlk.Print();
            wlk.AddToTotals();
        }
    }
    wlk.PrintTotals();
}

ON_METHOD(user, withdraw) {
    BalanceWalkerOwner wlk;
    uint32_t nCount = 0;
    for (wlk.Enum(cid); wlk.MoveNext(); ) {
        Gallery::Method::Withdraw args;
        _POD_(args.m_Key) = wlk.m_Key.m_KeyInContract;
        args.m_Value = wlk.m_Data.m_Amount; // everything

        FundsChange fc;
        fc.m_Consume = 0;
        fc.m_Aid = wlk.m_Key.m_KeyInContract.m_Aid;
        fc.m_Amount = wlk.m_Data.m_Amount;

        SigRequest sig;
        sig.m_pID = &wlk.m_Oi.m_km;
        sig.m_nID = sizeof(wlk.m_Oi.m_km);

        Env::GenerateKernel(&cid, args.s_iMethod, &args, sizeof(args), &fc, 1, &sig, 1, nCount ? "" : "Withdraw", 0);

        if (nMaxCount == ++nCount)
            break;
    }
}

ON_METHOD(user, vote) {
    StatePlus s;
    if (!s.Init(cid))
        return;

    Gallery::Method::Vote args;
    args.m_Impression.m_Value = val;
    args.m_ID.nft_id = id;

    KeyMaterial::Owner km;
    km.SetCid(cid);
    km.m_ID = id | KeyMaterial::g_MskImpression;
    km.Get(args.m_ID.m_pkUser);

    FundsChange fc;
    fc.m_Consume = 0;
    fc.m_Aid = s.m_Config.m_VoteReward.m_Aid;

    {
        Env::Key_T<Gallery::Impression::Key> key;
        _POD_(key.m_Prefix.m_Cid) = cid;
        _POD_(key.m_KeyInContract.m_ID) = args.m_ID;

        Gallery::Impression imp;
        bool bAlreadyVoted = Env::VarReader::Read_T(key, imp);

        fc.m_Amount = bAlreadyVoted ? 0 : s.m_Config.m_VoteReward.m_Amount;
    }

    SigRequest sig;
    sig.m_pID = &km;
    sig.m_nID = sizeof(km);

    Env::GenerateKernel(&cid, args.s_iMethod, &args, sizeof(args), &fc, 1, &sig, 1, "Vote", 1000000);
}

#undef ON_METHOD
#undef THE_FIELD

BEAM_EXPORT void Method_1() {
    Env::DocGroup root("");

    char szRole[0x10], szAction[0x20];

    if (!Env::DocGetText("role", szRole, sizeof(szRole)))
        return OnError("Role not specified");

    if (!Env::DocGetText("action", szAction, sizeof(szAction)))
        return OnError("Action not specified");

#define PAR_READ(type, name) type arg_##name; Env::DocGet(#name, arg_##name);
#define PAR_PASS(type, name) arg_##name,

#define THE_METHOD(role, name) \
        if (!Env::Strcmp(szAction, #name)) { \
            Gallery_##role##_##name(PAR_READ) \
            On_##role##_##name(Gallery_##role##_##name(PAR_PASS) 0); \
            return; \
        }

#define THE_ROLE(name) \
    if (!Env::Strcmp(szRole, #name)) { \
        GalleryRole_##name(THE_METHOD) \
        return OnError("invalid Action"); \
    }

    GalleryRoles_All(THE_ROLE)

#undef THE_ROLE
#undef THE_METHOD
#undef PAR_PASS
#undef PAR_READ

    OnError("unknown Role");
}
