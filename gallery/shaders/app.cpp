#include "Shaders/common.h"
#include "Shaders/app_common_impl.h"
#include "contract.h"
#include "Shaders/upgradable2/contract.h"
#include "Shaders/upgradable2/app_common_impl.h"

#include <string_view>

#define Gallery_manager_view(macro)
#define Gallery_manager_view_params(macro) macro(ContractID, cid)
#define Gallery_manager_view_artists_stats(macro) \
    macro(ContractID, cid) \

#define Gallery_manager_view_collections_stats(macro) \
    macro(ContractID, cid) \

#define Gallery_manager_view_artworks_stats(macro) \
    macro(ContractID, cid) \

#define Gallery_manager_view_artworks(macro) \
    macro(ContractID, cid) \
    macro(Height, h0) \
    macro(Amount, count) \
    macro(uint32_t, idx0) \
    macro(PubKey, artist_id) \
    macro(uint32_t, collection_id) \

#define Gallery_manager_view_collections(macro) \
    macro(ContractID, cid) \
    macro(Height, h0) \
    macro(uint32_t, idx0) \
    macro(uint32_t, count) \
    macro(PubKey, artist_id) \
    macro(uint32_t, print_artworks) \

#define Gallery_manager_view_artists(macro) \
    macro(ContractID, cid) \
    macro(Height, h0) \
    macro(uint32_t, idx0) \
    macro(uint32_t, count) \

//#define Gallery_manager_manage_artist(macro) \
    macro(ContractID, cid) \
    macro(PubKey, pkArtist) \
    macro(uint32_t, bEnable)

#define Gallery_manager_view_balance(macro)  macro(ContractID, cid)

#define Gallery_manager_add_rewards(macro) \
    macro(ContractID, cid) \
    macro(uint32_t, num)

#define Gallery_manager_my_admin_key(macro)

#define Gallery_manager_explicit_upgrade(macro) macro(ContractID, cid)

#define Gallery_manager_admin_delete(macro) \
    macro(ContractID, cid) \
    macro(Gallery::Artwork::Id, id)

#define GalleryRole_manager(macro) \
    macro(manager, view) \
    macro(manager, view_params) \
    macro(manager, view_artists_stats) \
    macro(manager, view_artists) \
    macro(manager, view_collections) \
    macro(manager, view_artworks) \
    macro(manager, view_collections_stats) \
    macro(manager, view_artworks_stats) \
    macro(manager, view_balance) \
    macro(manager, add_rewards) \
    macro(manager, my_admin_key) \
    macro(manager, explicit_upgrade) \
    macro(manager, admin_delete) \
    //macro(manager, manage_artist) \

#define Gallery_artist_view(macro) macro(ContractID, cid)
#define Gallery_artist_get_id(macro) macro(ContractID, cid)

#define Gallery_artist_set_artist(macro) \
    macro(ContractID, cid) \

#define Gallery_artist_set_collection(macro) \
    macro(ContractID, cid) \
    macro(Gallery::Collection::Id, collection_id) \

#define Gallery_artist_set_artwork(macro) \
    macro(ContractID, cid) \
    macro(uint32_t, collection_id) \

#define GalleryRole_artist(macro) \
    macro(artist, view) \
    macro(artist, get_id) \
    macro(artist, set_artwork) \
    macro(artist, set_artist) \
    macro(artist, set_collection) \

//#define Gallery_user_view_item(macro) \
    macro(ContractID, cid) \
    macro(Gallery::Artwork::Id, id)

#define Gallery_user_view_artworks(macro) \
    macro(ContractID, cid) \
    macro(Height, h0) \
    macro(Amount, count) \
    macro(uint32_t, idx0) \
    macro(PubKey, artist_id) \
    macro(uint32_t, collection_id) \

#define Gallery_user_view_collections(macro) \
    macro(ContractID, cid) \
    macro(Height, h0) \
    macro(uint32_t, idx0) \
    macro(uint32_t, count) \
    macro(PubKey, artist_id) \
    macro(uint32_t, print_artworks) \

#define Gallery_user_view_artists(macro) \
    macro(ContractID, cid) \
    macro(Height, h0) \
    macro(uint32_t, idx0) \
    macro(uint32_t, count) \

#define Gallery_user_download(macro) \
    macro(ContractID, cid) \
    macro(Gallery::Artwork::Id, id)

#define Gallery_user_set_price(macro) \
    macro(ContractID, cid) \
    macro(Gallery::Artwork::Id, id) \
    macro(Amount, amount) \
    macro(AssetID, aid)

#define Gallery_user_transfer(macro) \
    macro(ContractID, cid) \
    macro(Gallery::Artwork::Id, id) \
    macro(PubKey, pkNewOwner)

#define Gallery_user_buy(macro) \
    macro(ContractID, cid) \
    macro(Gallery::Artwork::Id, id)

#define Gallery_user_view_balance(macro) macro(ContractID, cid)

#define Gallery_user_withdraw(macro) \
    macro(ContractID, cid) \
    macro(uint32_t, nMaxCount) \

#define Gallery_user_vote(macro) \
    macro(ContractID, cid) \
    macro(Gallery::Artwork::Id, id) \
    macro(uint32_t, val) \

#define Gallery_user_view_artists_stats(macro) \
    macro(ContractID, cid) \

#define Gallery_user_view_collections_stats(macro) \
    macro(ContractID, cid) \

#define Gallery_user_view_artworks_stats(macro) \
    macro(ContractID, cid) \

#define GalleryRole_user(macro) \
    macro(user, view_artists_stats) \
    macro(user, view_artists) \
    macro(user, view_collections) \
    macro(user, view_collections_stats) \
    macro(user, view_artworks_stats) \
    macro(user, view_artworks) \
    macro(user, download) \
    macro(user, set_price) \
    macro(user, transfer) \
    macro(user, buy) \
    macro(user, view_balance) \
    macro(user, withdraw) \
    macro(user, vote) \
    //macro(user, view_item) \

#define GalleryRoles_All(macro) \
    macro(manager) \
    macro(artist) \
    macro(user)

BEAM_EXPORT void Method_0()
{
    // scheme
    Env::DocGroup root("");

    {   Env::DocGroup gr("roles");

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

void OnError(const char* sz)
{
    Env::DocAddText("error", sz);
}

namespace KeyMaterial
{
    const char g_szAdmin[] = "Gallery-key-admin";

    struct MyAdminKey :public Env::KeyID {
        MyAdminKey() :Env::KeyID(g_szAdmin, sizeof(g_szAdmin) - sizeof(char)) {}
    };

#pragma pack (push, 1)

    const char g_szOwner[] = "Gallery-key-owner";

    struct Owner
    {
        ContractID m_Cid;
        Gallery::Artwork::Id m_ID;
        uint8_t m_pSeed[sizeof(g_szOwner) - sizeof(char)];

        Owner()
        {
            Env::Memcpy(m_pSeed, g_szOwner, sizeof(m_pSeed));
            m_ID = 0;
        }

        void SetCid(const ContractID& cid)
        {
            _POD_(m_Cid) = cid;
        }

        void SetCid()
        {
            _POD_(m_Cid).SetZero();
        }

        void Get(PubKey& pk) const {
            Env::DerivePk(pk, this, sizeof(*this));
        }
    };
#pragma pack (pop)

    const Gallery::Artwork::Id g_MskImpression = Utils::FromBE((static_cast<Gallery::Artwork::Id>(-1) >> 1) + 1); // hi bit
}

ON_METHOD(manager, view)
{
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

bool ReadItem(const ContractID& cid, Gallery::Artwork::Id id, Gallery::Artwork& m)
{
    Env::Key_T<Gallery::Artwork::FirstStageKey> fskey;
    Env::Key_T<Gallery::Artwork::SecondStageKey> sskey;
    _POD_(fskey.m_Prefix.m_Cid) = cid;
    _POD_(sskey.m_Prefix.m_Cid) = cid;
    fskey.m_KeyInContract.id = id;
    sskey.m_KeyInContract.id = id;

    if (!Env::VarReader::Read_T(fskey, sskey.m_KeyInContract.h_updated)) {
        OnError("not found");
        return false;
    }

    Env::VarReader::Read_T(sskey, m);
    return true;
}

struct OwnerInfo
{
    KeyMaterial::Owner m_km;

    bool DeduceOwner(const ContractID& cid, Gallery::Artwork::Id id, const Gallery::Artwork& m)
    {
        return DeduceOwner(cid, id, m.m_pkOwner);
    }

    bool DeduceOwnerRaw(Gallery::Artwork::Id id, const PubKey& pkOwner)
    {
        PubKey pk;

        m_km.m_ID = id;
        m_km.Get(pk);
        return (_POD_(pk) == pkOwner);
    }

    bool DeduceOwner(const ContractID& cid, Gallery::Artwork::Id id, const PubKey& pkOwner)
    {
        m_km.SetCid(cid);
        if (DeduceOwnerRaw(id, pkOwner) || // owner
            DeduceOwnerRaw(0, pkOwner)) // artist
            return true;

        m_km.SetCid();
        return DeduceOwnerRaw(0, pkOwner); // artist, older key gen
    }

    bool ReadOwnedItem(const ContractID& cid, Gallery::Artwork::Id id, Gallery::Artwork& m)
    {
        if (!ReadItem(cid, id, m))
            return false;

        if (DeduceOwner(cid, id, m))
            return true;

        OnError("not owned");
        return false;
    }
};

struct StatePlus
    :public Gallery::State
{
    bool Init(const ContractID& cid)
    {
        Env::Key_T<uint8_t> key;
        _POD_(key.m_Prefix.m_Cid) = cid;
        key.m_KeyInContract = Gallery::State::s_Key;

        if (Env::VarReader::Read_T(key, Cast::Down<Gallery::State>(*this)))
            return true;

        OnError("no such a contract");
        return false;
    }
};

ON_METHOD(manager, view_params)
{
    StatePlus s;
    if (!s.Init(cid))
        return;

    PubKey pk;
    KeyMaterial::MyAdminKey().get_Pk(pk);

    uint32_t bIsAdmin = (_POD_(s.m_Config.m_pkAdmin) == pk);

    Env::DocAddNum("Admin", bIsAdmin);
    Env::DocAddNum("Exhibits", s.artworks_stats.total);
    Env::DocAddNum("voteReward.aid", s.m_Config.m_VoteReward.m_Aid);
    Env::DocAddNum("voteReward.amount", s.m_Config.m_VoteReward.m_Amount);
    Env::DocAddNum("voteReward_balance", s.m_VoteBalance);
}

struct ImpressionWalker
{
    Env::VarReaderEx<true> m_Reader;
    Env::Key_T<Gallery::Impression::Key> m_Key;
    Gallery::Impression m_Value;
    bool m_Valid = false;

    void Enum(const ContractID& cid, Gallery::Artwork::Id id) {
        Enum(cid, id, id);
    }

    void Enum(const ContractID& cid, Gallery::Artwork::Id id0, Gallery::Artwork::Id id1)
    {
        _POD_(m_Key.m_Prefix.m_Cid) = cid;
        m_Key.m_KeyInContract.m_ID.m_ArtworkID = id0;
        _POD_(m_Key.m_KeyInContract.m_ID.m_pkUser).SetZero();

        Env::Key_T<Gallery::Impression::Key> k1;
        _POD_(k1.m_Prefix.m_Cid) = cid;
        k1.m_KeyInContract.m_ID.m_ArtworkID = id1;
        _POD_(k1.m_KeyInContract.m_ID.m_pkUser).SetObject(0xff);

        m_Reader.Enum_T(m_Key, k1);
        Move();
    }

    void Move()
    {
        m_Valid = m_Reader.MoveNext_T(m_Key, m_Value);
    }
};

#pragma pack (push, 0)
struct MyArtist : public Gallery::Artist {
public:
    char m_szLabelData[s_TotalMaxLen + 2];

    std::string_view GetData() {
        return std::string_view(m_szLabelData + label_len, data_len);
    }

    std::string_view GetLabel() {
        return std::string_view(m_szLabelData, label_len);
    }

    void Print(const ContractID& cid) {
        Env::Key_T<Gallery::Artist::SecondStageKey> k0, k1;
        _POD_(k0.m_Prefix.m_Cid) = cid;
        _POD_(k1.m_Prefix.m_Cid) = cid;
        _POD_(k0.m_KeyInContract.id).SetZero();
        _POD_(k1.m_KeyInContract.id).SetObject(0xff);
        k0.m_KeyInContract.h_updated = 0;
        k1.m_KeyInContract.h_updated = static_cast<Height>(-1);

        Env::VarReader r(k0, k1);
        Env::DocArray gr0("artists");

        while (ReadNext(r, k0)) {
            Print(k0.m_KeyInContract.id, Utils::FromBE(k0.m_KeyInContract.h_updated));
        }
    }

    void Print(const ContractID& cid, const std::string_view& ids) {
        int cur_pos = 0;
        int next_pos;
        Env::DocArray gr0("artists");
        while ((next_pos = ids.find(';', cur_pos)) != ids.npos) {
            union {
                PubKey pk;
                uint8_t a[33];
            } blob; 

            Env::Memset(blob.a, 0, 33);

            for (int i = cur_pos; i < next_pos; ++i) {
                if (ids[i] >= '0' && ids[i] <= '9') {
                    blob.a[(i - cur_pos) / 2] |= ((ids[i] - '0') << (4 * !((i - cur_pos) & 1)));
                } else {
                    blob.a[(i - cur_pos) / 2] |= ((ids[i] - 'a' + 10) << (4 * !((i - cur_pos) & 1)));
                }
            }
            cur_pos = next_pos + 1;

            Print(cid, blob.pk);
        }
    }

    void Print(const ContractID& cid, const PubKey& pk) {
        Gallery::Artist::SecondStageKey ssak;
        Env::Key_T<Gallery::Artist::FirstStageKey> k2;
        k2.m_Prefix.m_Cid = cid;
        _POD_(k2.m_KeyInContract.id) = pk;
        Env::VarReader::Read_T(k2, ssak);

        Env::Key_T<Gallery::Artist::SecondStageKey> k0;
        _POD_(k0.m_Prefix.m_Cid) = cid;
        _POD_(k0.m_KeyInContract.id) = pk;
        k0.m_KeyInContract.h_updated = ssak.h_updated;

        Env::VarReader r(k0, k0);
        if (!ReadNext(r, k0))
            return;

        Print(k0.m_KeyInContract.id, Utils::FromBE(k0.m_KeyInContract.h_updated));
    }

    void Print(const ContractID& cid, uint32_t idx0, uint32_t count) {
        Env::Key_T<Gallery::Artist::FirstStageKey> fsk0, fsk1;
        _POD_(fsk0.m_Prefix.m_Cid) = cid;
        _POD_(fsk1.m_Prefix.m_Cid) = cid;
        _POD_(fsk0.m_KeyInContract.id).SetZero();
        _POD_(fsk1.m_KeyInContract.id).SetObject(0xff);

        Env::VarReader r(fsk0, fsk1);
        Gallery::Artist::SecondStageKey ssak;
        Env::DocArray gr0("artists");

        int cur_idx = 1, cur_cnt = 0;
        while (cur_cnt < count && r.MoveNext_T(fsk0, ssak)) {
            if (cur_idx >= idx0) {
                Env::Key_T<Gallery::Artist::SecondStageKey> ssk;
                _POD_(ssk.m_Prefix.m_Cid) = cid;
                _POD_(ssk.m_KeyInContract.id) = ssak.id;
                ssk.m_KeyInContract.h_updated = ssak.h_updated;

                Env::VarReader r(ssk, ssk);
                if (!ReadNext(r, ssk))
                    break;

                Print(ssk.m_KeyInContract.id, Utils::FromBE(ssk.m_KeyInContract.h_updated));
                ++cur_cnt;
            }
            ++cur_idx;
        }
    }

    void Print(const ContractID& cid, Height h0, uint32_t count) {
        Env::Key_T<Gallery::Artist::SecondStageKey> k0, k1;
        _POD_(k0.m_Prefix.m_Cid) = cid;
        _POD_(k1.m_Prefix.m_Cid) = cid;
        _POD_(k0.m_KeyInContract.id).SetZero();
        _POD_(k1.m_KeyInContract.id).SetObject(0xff);
        k0.m_KeyInContract.h_updated = Utils::FromBE(h0);
        k1.m_KeyInContract.h_updated = static_cast<Height>(-1);

        Env::VarReader r(k0, k1);
        Env::DocArray gr0("artists");

        int cur_cnt = 0;
        Height prev_h = -1;
        while (ReadNext(r, k0) && (cur_cnt++ < count || prev_h == k0.m_KeyInContract.h_updated)) {
            Print(k0.m_KeyInContract.id, Utils::FromBE(k0.m_KeyInContract.h_updated));
            prev_h = k0.m_KeyInContract.h_updated;
        }
    }

    void Print(const Id& id, Height updated) {
        // DocAddText prints char[] until it meets \0 symbol, but
        // m_szLabelData contains label + data without \0 symbol between them
        char label[label_len + 1];
        char data[data_len + 1];
        label[label_len] = 0;
        data[data_len] = 0;
        Env::Memcpy(label, m_szLabelData, label_len);
        Env::Memcpy(data, m_szLabelData + label_len, data_len);

        Env::DocGroup gr1("");
        Env::DocAddBlob_T("id", id);
        Env::DocAddNum32("updated", updated);
        Env::DocAddText("label", label);
        Env::DocAddText("data", data);
        Env::DocAddNum("hReg", m_hRegistered);
        Env::DocAddNum32("approved", is_approved);
    }

    bool ReadNext(Env::VarReader& r, Env::Key_T<Gallery::Artist::SecondStageKey>& key) {
        while (true) {
            uint32_t nKey = sizeof(key), nVal = sizeof(*this);
            if (!r.MoveNext(&key, nKey, this, nVal, 0))
                return false;

            if (sizeof(key) != nKey)
                continue;

            nVal -= sizeof(Gallery::Artist);
            m_szLabelData[std::min(nVal, s_TotalMaxLen)] = 0;
            break;
        }
        return true;
    }
};
#pragma pack (pop)

#pragma pack (push, 0)
struct MyCollection
    :public Gallery::Collection
{
    char m_szLabelData[s_TotalMaxLen + 2];

    void Print()
    {
        // DocAddText prints char[] until it meets \0 symbol, but
        // m_szLabelData contains label + data without \0 symbol between them
        char label[label_len + 1];
        char data[data_len + 1];
        label[label_len] = 0;
        data[data_len] = 0;
        Env::Memcpy(label, m_szLabelData, label_len);
        Env::Memcpy(data, m_szLabelData + label_len, data_len);

        Env::DocAddText("label", label);
        Env::DocAddText("data", data);
        Env::DocAddBlob_T("author", m_pkAuthor);
        Env::DocAddNum32("artworks_count", artworks_num);
        Env::DocAddNum32("approved", is_approved);
        Env::DocAddNum32("default", is_default);
    }

    std::string_view GetData()
    {
        return std::string_view(m_szLabelData + label_len, data_len);
    }

    std::string_view GetLabel()
    {
        return std::string_view(m_szLabelData, label_len);
    }

    bool ReadNext(Env::VarReader& r, Env::Key_T<Gallery::Collection::SecondStageKey>& key)
    {
        while (true)
        {
            uint32_t nKey = sizeof(key), nVal = sizeof(*this);
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
struct MyArtwork
    :public Gallery::Artwork
{
    Utils::Vector<uint8_t> vData;
    Utils::Vector<char> vLabel;

    void Print(ImpressionWalker& iwlk)
    {
        Env::DocAddText("label", vLabel.m_p);
        Env::DocAddBlob("data", vData.m_p, vData.m_Count);
        Env::DocAddBlob_T("author", m_pkAuthor);
        Env::DocAddNum32("collection", collection_id);
        if (m_Aid)
            Env::DocAddBlob_T("checkout.aid", m_Aid);

        const ContractID& cid = iwlk.m_Key.m_Prefix.m_Cid;
        OwnerInfo oi;

        if (!_POD_(m_pkOwner).IsZero())
        {
            Env::DocAddBlob_T("owner", m_pkOwner);
            Env::DocAddNum("owned", (uint32_t) !!oi.DeduceOwner(cid, iwlk.m_Key.m_KeyInContract.m_ID.m_ArtworkID, *this));

            if (m_Price.m_Amount)
            {
                Env::DocAddNum("price.aid", m_Price.m_Aid);
                Env::DocAddNum("price.amount", m_Price.m_Amount);
            }
        }

        uint32_t nImpressions = 0;
        uint32_t nMyImpression = 0;
        bool bMyImpressionSet = false;
        bool bMyImpressionKey = false;
        PubKey pkMyImpression;

        auto idNorm = Utils::FromBE(iwlk.m_Key.m_KeyInContract.m_ID.m_ArtworkID);

        for ( ; iwlk.m_Valid; iwlk.Move())
        {
            auto idWlk = Utils::FromBE(iwlk.m_Key.m_KeyInContract.m_ID.m_ArtworkID);
            if (idWlk < idNorm)
                continue;
            if (idWlk > idNorm)
                break;

            if (!bMyImpressionSet)
            {
                if (!bMyImpressionKey)
                {
                    bMyImpressionKey = true;
                    oi.m_km.SetCid(cid);
                    oi.m_km.m_ID = iwlk.m_Key.m_KeyInContract.m_ID.m_ArtworkID | KeyMaterial::g_MskImpression;
                    oi.m_km.Get(pkMyImpression);
                }

                if (_POD_(pkMyImpression) == iwlk.m_Key.m_KeyInContract.m_ID.m_pkUser)
                {
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

    bool ReadNext(Env::VarReader& r, Env::Key_T<Gallery::Artwork::SecondStageKey>& key)
    {
        uint32_t nKey = sizeof(key), nVal = sizeof(Gallery::Artwork);
        if (!r.MoveNext(&key, nKey, this, nVal, 0))
            return false;

        vData.m_Count = 0;
        vLabel.m_Count = 0;
        auto id_ = key.m_KeyInContract.id;
        auto cid = key.m_Prefix.m_Cid;

        Env::Key_T<Gallery::Events::AddArtworkData::Key> adk0, adk1;
        _POD_(adk0.m_Prefix.m_Cid) = cid;
        _POD_(adk1.m_Prefix.m_Cid) = cid;
        adk0.m_KeyInContract.m_ID = id_;
        adk1.m_KeyInContract.m_ID = id_;
        _POD_(adk0.m_KeyInContract.m_pkArtist).SetZero();
        _POD_(adk1.m_KeyInContract.m_pkArtist).SetObject(0xff);

        Env::Key_T<Gallery::Events::AddArtworkLabel::Key> alk0, alk1;
        _POD_(alk0.m_Prefix.m_Cid) = cid;
        _POD_(alk1.m_Prefix.m_Cid) = cid;
        alk0.m_KeyInContract.m_ID = id_;
        alk1.m_KeyInContract.m_ID = id_;
        _POD_(alk0.m_KeyInContract.m_pkArtist).SetZero();
        _POD_(alk1.m_KeyInContract.m_pkArtist).SetObject(0xff);

        uint32_t nDataCount = 0;
        uint32_t nLabelCount = 0;

        Env::LogReader adr(adk0, adk1);
        for ( ; ; nDataCount++)
        {
            uint32_t nData = 0, nKey = sizeof(adk0);
            if (!adr.MoveNext(&adk0, nKey, nullptr, nData, 0))
                break;

            vData.Prepare(vData.m_Count + nData);
            adr.MoveNext(&adk0, nKey, vData.m_p + vData.m_Count, nData, 1);
            vData.m_Count += nData;
        }

        Env::LogReader alr(alk0, alk1);
        for ( ; ; nLabelCount++)
        {
            uint32_t nData = 0, nKey = sizeof(alk0);
            if (!alr.MoveNext(&alk0, nKey, nullptr, nData, 0))
                break;

            vLabel.Prepare(vLabel.m_Count + nData);
            alr.MoveNext(&alk0, nKey, vLabel.m_p + vLabel.m_Count, nData, 1);
            vLabel.m_Count += nData;
        }

        if (!nDataCount)
            return false;

        return true;
    }
};
#pragma pack (pop)

bool artist_label_exists(const ContractID& cid, const std::string_view& label, bool& artist_exists)
{
    artist_exists = false;

    Env::Key_T<Gallery::Artist::SecondStageKey> k0, k1;
    _POD_(k0.m_Prefix.m_Cid) = cid;
    _POD_(k1.m_Prefix.m_Cid) = cid;

    _POD_(k0.m_KeyInContract.id).SetZero();
    _POD_(k1.m_KeyInContract.id).SetObject(0xff);

    k0.m_KeyInContract.h_updated = 0;
    k1.m_KeyInContract.h_updated = static_cast<Height>(-1);

    Env::VarReader r(k0, k1);
    MyArtist a;

    KeyMaterial::Owner km;
    km.SetCid(cid);
    PubKey my_key;
    km.Get(my_key);

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

bool PrintCollections(const ContractID& cid, uint32_t id, Height h0, bool bFindAll, uint32_t print_artworks, uint32_t count)
{
    Gallery::Collection::SecondStageKey ssck;
    if (!bFindAll) {
        Env::Key_T<Gallery::Collection::FirstStageKey> k2;
        k2.m_Prefix.m_Cid = cid;
        k2.m_KeyInContract.id = id;
        Env::VarReader::Read_T(k2, ssck);
    }

    Env::Key_T<Gallery::Collection::SecondStageKey> k0, k1;
    _POD_(k0.m_Prefix.m_Cid) = cid;
    _POD_(k1.m_Prefix.m_Cid) = cid;

    if (bFindAll) {
        k0.m_KeyInContract.id = 0;
        k1.m_KeyInContract.id = static_cast<uint32_t>(-1);
    } else {
        k0.m_KeyInContract.id = id;
        k1.m_KeyInContract.id = id;
    }
    k0.m_KeyInContract.h_updated = bFindAll ? Utils::FromBE(h0) : ssck.h_updated;
    k1.m_KeyInContract.h_updated = bFindAll ? static_cast<Height>(-1) : ssck.h_updated;

    Env::VarReader r(k0, k1);
    MyCollection c;
    int cur_cnt = 0;
    if (bFindAll) {
        Env::DocArray gr0("collections");

        while (true) {
            if (!c.ReadNext(r, k0))
                break;

            if (count && cur_cnt++ >= count)
                break;

            Env::DocGroup gr1("");
            Env::DocAddNum32("id", k0.m_KeyInContract.id);
            Env::DocAddNum32("updated", Utils::FromBE(k0.m_KeyInContract.h_updated));

            if (print_artworks) {
                Env::Key_T<Gallery::CollectionArtworkKey> cak0, cak1;
                _POD_(cak0.m_Prefix.m_Cid) = cid;
                _POD_(cak1.m_Prefix.m_Cid) = cid;

                cak0.m_KeyInContract.collection_id = k0.m_KeyInContract.id;
                cak1.m_KeyInContract.collection_id = k0.m_KeyInContract.id;

                cak0.m_KeyInContract.artwork_id = 0;
                cak1.m_KeyInContract.artwork_id = static_cast<Gallery::Artwork::Id>(-1);

                Env::VarReader rca(cak0, cak1);
                {
                    Env::DocArray gr2("artworks");
                    while (true)
                    {
                        bool exists;
                        if (!rca.MoveNext_T(cak0, exists))
                            break;
                        Env::DocAddNum32("", Utils::FromBE(cak0.m_KeyInContract.artwork_id));
                    }
                }
            }
            c.Print();
        }
    } else {
        if (!c.ReadNext(r, k0))
            return false;
        Env::DocGroup gr1("");
        Env::DocAddNum32("id", k0.m_KeyInContract.id);
        Env::DocAddNum32("updated", Utils::FromBE(k0.m_KeyInContract.h_updated));

        if (print_artworks) {
            Env::Key_T<Gallery::CollectionArtworkKey> cak0, cak1;
            _POD_(cak0.m_Prefix.m_Cid) = cid;
            _POD_(cak1.m_Prefix.m_Cid) = cid;

            cak0.m_KeyInContract.collection_id = k0.m_KeyInContract.id;
            cak1.m_KeyInContract.collection_id = k0.m_KeyInContract.id;

            cak0.m_KeyInContract.artwork_id = 0;
            cak1.m_KeyInContract.artwork_id = static_cast<Gallery::Artwork::Id>(-1);

            Env::VarReader rca(cak0, cak1);
            {
                Env::DocArray gr2("artworks");
                while (true)
                {
                    bool exists;
                    if (!rca.MoveNext_T(cak0, exists))
                        break;
                    Env::DocAddNum32("", Utils::FromBE(cak0.m_KeyInContract.artwork_id));
                }
            }
        }
        c.Print();
    }

    return true;
}

ON_METHOD(manager, view_artists_stats)
{
    Gallery::State s;
    Env::Key_T<uint8_t> k;
    k.m_KeyInContract = 0;
    k.m_Prefix.m_Cid = cid;
    Env::VarReader::Read_T(k, s);
    Env::DocAddNum32("total", s.artists_stats.total);
}

ON_METHOD(manager, view_collections_stats)
{
    Gallery::State s;
    Env::Key_T<uint8_t> k;
    k.m_KeyInContract = 0;
    k.m_Prefix.m_Cid = cid;
    Env::VarReader::Read_T(k, s);
    Env::DocAddNum32("total", s.collections_stats.total);
}

ON_METHOD(manager, view_artworks_stats)
{
    Gallery::State s;
    Env::Key_T<uint8_t> k;
    k.m_KeyInContract = 0;
    k.m_Prefix.m_Cid = cid;
    Env::VarReader::Read_T(k, s);
    Env::DocAddNum32("total", s.artworks_stats.total);
}

ON_METHOD(user, view_artists_stats)
{
    Gallery::State s;
    Env::Key_T<uint8_t> k;
    k.m_KeyInContract = 0;
    k.m_Prefix.m_Cid = cid;
    Env::VarReader::Read_T(k, s);
    Env::DocAddNum32("total", s.artists_stats.approved);
}

ON_METHOD(user, view_collections_stats)
{
    Gallery::State s;
    Env::Key_T<uint8_t> k;
    k.m_KeyInContract = 0;
    k.m_Prefix.m_Cid = cid;
    Env::VarReader::Read_T(k, s);
    Env::DocAddNum32("total", s.collections_stats.approved);
}

ON_METHOD(user, view_artworks_stats)
{
    Gallery::State s;
    Env::Key_T<uint8_t> k;
    k.m_KeyInContract = 0;
    k.m_Prefix.m_Cid = cid;
    Env::VarReader::Read_T(k, s);
    Env::DocAddNum32("total", s.artworks_stats.approved);
}

ON_METHOD(user, view_artists)
{
    const size_t MAX_IDS = 128;
    char buf[MAX_IDS * sizeof(PubKey)];
    int buf_len = Env::DocGetText("ids", buf, sizeof(buf));
    if (buf_len)
        buf[buf_len - 1] = ';';

    MyArtist a;
    if (count && h0)
        a.Print(cid, h0, count);
    else if (count && idx0)
        a.Print(cid, idx0, count);
    else if (buf_len)
        a.Print(cid, buf);
    else
        a.Print(cid);
}

ON_METHOD(user, view_collections)
{
    const size_t MAX_IDS = 128;
    char buf[MAX_IDS * sizeof(Gallery::Collection::Id)];
    int buf_len = Env::DocGetText("ids", buf, sizeof(buf));
    if (buf_len)
        buf[buf_len - 1] = ';';

    if (count) {
        if (h0) {
            PrintCollections(cid, 0, h0, true, print_artworks, count);
        } else {
            Env::Key_T<Gallery::Collection::FirstStageKey> k0, k1;
            _POD_(k0.m_Prefix.m_Cid) = cid;
            _POD_(k1.m_Prefix.m_Cid) = cid;

            k0.m_KeyInContract.id = 0;
            k1.m_KeyInContract.id = static_cast<Gallery::Collection::Id>(-1);

            Env::VarReader r(k0, k1);
            Gallery::Collection::SecondStageKey ssck;
            Env::DocArray gr0("collections");
            int cur_idx = 0, cur_cnt = 0;
            while (cur_cnt < count) {
                if (!r.MoveNext_T(k0, ssck))
                    break;

                if (!_POD_(artist_id).IsZero()) {
                    Env::Key_T<Gallery::ArtistCollectionKey> ack;
                    ack.m_Prefix.m_Cid = cid;
                    ack.m_KeyInContract.collection_id = k0.m_KeyInContract.id;
                    ack.m_KeyInContract.pkArtist = artist_id;
                    bool exists;
                    Env::VarReader::Read_T(ack, exists);
                    if (!exists) continue;
                }
                if (cur_idx >= idx0) {
                    PrintCollections(cid, k0.m_KeyInContract.id, 0, false, print_artworks, 0);
                    ++cur_cnt;
                }
                ++cur_idx;
            }
        }
    } else if (buf_len) {
        std::string_view ids(buf);
        int cur_pos = 0;
        int next_pos;
        Env::DocArray gr0("collections");
        while ((next_pos = ids.find(';', cur_pos)) != ids.npos) {
            uint32_t id = 0;

            for (int i = cur_pos; i < next_pos; ++i) {
                if (ids[i] >= '0' && ids[i] <= '9') {
                    id = id * 10 + ids[i] - '0';
                }
            }
            cur_pos = next_pos + 1;
            PrintCollections(cid, id, 0, false, print_artworks, 0);
        }
    } else {
        PrintCollections(cid, 0, h0, true, print_artworks, 0);
    }
}

/*
ON_METHOD(manager, manage_artist)
{
    Gallery::Method::ManageArtist args;

    args.m_pkArtist = pkArtist;

    args.req = bEnable ?
        Gallery::Method::ManageArtist::RequestType::ENABLE :
        Gallery::Method::ManageArtist::RequestType::DISABLE;

    args.m_LabelLen = Gallery::Artist::s_LabelMaxLen + 1;
    args.role = Gallery::Role::MANAGER;

    KeyMaterial::MyAdminKey kid;
    Env::GenerateKernel(&cid, args.s_iMethod, &args, sizeof(args), nullptr, 0, &kid, 1, "Gallery manage artist", 120000);
}
*/

ON_METHOD(manager, view_artists)
{
    const size_t MAX_IDS = 128;
    char buf[MAX_IDS * sizeof(PubKey)];
    int buf_len = Env::DocGetText("ids", buf, sizeof(buf));
    if (buf_len)
        buf[buf_len - 1] = ';';

    MyArtist a;
    if (count && h0)
        a.Print(cid, h0, count);
    else if (count && idx0)
        a.Print(cid, idx0, count);
    else if (buf_len)
        a.Print(cid, buf);
    else
        a.Print(cid);
}

ON_METHOD(manager, view_collections)
{
    const size_t MAX_IDS = 128;
    char buf[MAX_IDS * sizeof(Gallery::Collection::Id)];
    int buf_len = Env::DocGetText("ids", buf, sizeof(buf));
    if (buf_len)
        buf[buf_len - 1] = ';';

    if (count) {
        if (h0) {
            PrintCollections(cid, 0, h0, true, print_artworks, count);
        } else {
            Env::Key_T<Gallery::Collection::FirstStageKey> k0, k1;
            _POD_(k0.m_Prefix.m_Cid) = cid;
            _POD_(k1.m_Prefix.m_Cid) = cid;

            k0.m_KeyInContract.id = 0;
            k1.m_KeyInContract.id = static_cast<Gallery::Collection::Id>(-1);

            Env::VarReader r(k0, k1);
            Gallery::Collection::SecondStageKey ssck;
            Env::DocArray gr0("collections");
            int cur_idx = 0, cur_cnt = 0;
            while (cur_cnt < count) {
                if (!r.MoveNext_T(k0, ssck))
                    break;

                if (!_POD_(artist_id).IsZero()) {
                    Env::Key_T<Gallery::ArtistCollectionKey> ack;
                    ack.m_Prefix.m_Cid = cid;
                    ack.m_KeyInContract.collection_id = k0.m_KeyInContract.id;
                    ack.m_KeyInContract.pkArtist = artist_id;
                    bool exists;
                    Env::VarReader::Read_T(ack, exists);
                    if (!exists) continue;
                }
                if (cur_idx >= idx0) {
                    PrintCollections(cid, k0.m_KeyInContract.id, 0, false, print_artworks, 0);
                    ++cur_cnt;
                }
                ++cur_idx;
            }
        }
    } else if (buf_len) {
        std::string_view ids(buf);
        int cur_pos = 0;
        int next_pos;
        Env::DocArray gr0("collections");
        while ((next_pos = ids.find(';', cur_pos)) != ids.npos) {
            uint32_t id = 0;

            for (int i = cur_pos; i < next_pos; ++i) {
                if (ids[i] >= '0' && ids[i] <= '9') {
                    id = id * 10 + ids[i] - '0';
                }
            }
            cur_pos = next_pos + 1;
            PrintCollections(cid, id, 0, false, print_artworks, 0);
        }
    } else {
        PrintCollections(cid, 0, h0, true, print_artworks, 0);
    }
}

ON_METHOD(artist, set_artwork)
{
    KeyMaterial::Owner km;
    km.SetCid(cid);
    PubKey pkArtist;
    km.Get(pkArtist);

    struct {
        Gallery::Method::AddExhibit args;
        char m_szLabelData[Gallery::Artwork::s_TotalMaxLen + 2];
    } d;

    d.args.m_pkArtist = pkArtist;
    d.args.req = Gallery::Method::AddExhibit::RequestType::SET;
    d.args.role = Gallery::Role::ARTIST;

    uint32_t nArgSize = sizeof(d.args);

    uint32_t nLabelSize = Env::DocGetText("label", d.m_szLabelData, Gallery::Artwork::s_LabelMaxLen + 1); // including 0-term

    if (nLabelSize > Gallery::Artwork::s_LabelMaxLen + 1) // plus \0
    {
        OnError("label is too long");
        return;
    }

    d.args.label_len = (nLabelSize ? nLabelSize - 1 : 0);

    nArgSize += d.args.label_len;

    auto nDataSize = Env::DocGetBlob("data", nullptr, 0);
    if (!nDataSize)
    {
        OnError("data not specified");
        return;
    }

    if (Env::DocGetBlob("data", d.m_szLabelData + d.args.label_len, nDataSize) != nDataSize)
    {
        OnError("data can't be parsed");
        return;
    }

    if (nDataSize > Gallery::Artist::s_DataMaxLen + 1)
    {
        OnError("data too long");
        return;
    }

    d.args.data_len = nDataSize;
    d.args.collection_id = collection_id;

    if (!collection_id) {
        OnError("collection_id must be specified");
        return;
    }

    nArgSize += d.args.data_len;

    SigRequest sig;
    sig.m_pID = &km;
    sig.m_nID = sizeof(km);

    // estimate charge, it may be non-negligible.
    // - Patent verification: LoadVar + SaveVar, 25K units
    // - Artist verification: LoadVar, 5K units
    // - Global state: LoadVar + SaveVar, 25K units
    // - Artwork creation: SaveVar, 20K units
    // - AddSig by admin, 10K units
    // - event for the data. This is 100 units per byte, plus 5K units per call, which is repeated for each 8K of data
    // - add some extra for other stuff

    uint32_t nCharge =
        ManagerUpgadable2::get_ChargeInvoke() +
        Env::Cost::LoadVar_For(sizeof(Gallery::State)) +
        Env::Cost::SaveVar_For(sizeof(Gallery::State)) +
        Env::Cost::LoadVar_For(sizeof(Gallery::Artist)) +
        Env::Cost::LoadVar_For(sizeof(Gallery::Artist::SecondStageKey)) +
        Env::Cost::SaveVar_For(sizeof(Gallery::Artwork)) +
        Env::Cost::SaveVar_For(sizeof(Height)) +
        Env::Cost::AddSig +
        Env::Cost::Cycle * 200;

    const uint32_t nSizeEvt = 0x2000;
    uint32_t nFullCycles = nDataSize / nSizeEvt;

    nCharge += (Env::Cost::Log_For(nSizeEvt) + Env::Cost::Cycle * 50) * nFullCycles;

    nDataSize %= nSizeEvt;
    if (nDataSize)
        nCharge += Env::Cost::Log_For(nDataSize) + Env::Cost::Cycle * 50;

    Env::GenerateKernel(&cid, d.args.s_iMethod, &d, nArgSize, nullptr, 0, &sig, 1, "Upload masterpiece", nCharge + 2500000);
}

ON_METHOD(manager, admin_delete)
{
    auto id_ = Utils::FromBE(id);

    Gallery::Method::AdminDelete args;
    args.m_ID = id_;

    KeyMaterial::MyAdminKey kid;
    Env::GenerateKernel(&cid, args.s_iMethod, &args, sizeof(args), nullptr, 0, &kid, 1, "Delete artwork", 0);
}

ON_METHOD(manager, add_rewards)
{
    StatePlus s;
    if (!s.Init(cid))
        return;

    Gallery::Method::AddVoteRewards args;
    args.m_Amount = s.m_Config.m_VoteReward.m_Amount * num;

    if (!args.m_Amount)
    {
        OnError("no rewards");
        return;
    }

    FundsChange fc;
    fc.m_Aid = s.m_Config.m_VoteReward.m_Aid;
    fc.m_Amount = args.m_Amount;
    fc.m_Consume = 1;

    Env::GenerateKernel(&cid, args.s_iMethod, &args, sizeof(args), &fc, 1, nullptr, 0, "Add voting rewards", 0);
}

ON_METHOD(manager, my_admin_key)
{
    PubKey pk;
    KeyMaterial::MyAdminKey().get_Pk(pk);
    Env::DocAddBlob_T("admin_key", pk);
}

ON_METHOD(manager, explicit_upgrade)
{
    ManagerUpgadable2::MultiSigRitual::Perform_ExplicitUpgrade(cid);
}

struct BalanceWalker
{
    Env::VarReaderEx<true> m_Reader;
    Env::Key_T<Gallery::Payout::Key> m_Key;
    Gallery::Payout m_Data;

    // TODO: add tree (map) class
    Utils::Vector<Gallery::AmountWithAsset> m_Totals;

    void Enum(const ContractID& cid)
    {
        _POD_(m_Key.m_Prefix.m_Cid) = cid;
        _POD_(m_Key.m_KeyInContract).SetZero();

        Env::Key_T<Gallery::Payout::Key> k1;
        _POD_(k1.m_Prefix.m_Cid) = cid;
        _POD_(k1.m_KeyInContract).SetObject(0xff);

        m_Reader.Enum_T(m_Key, k1);
    }

    bool MoveNext()
    {
        return m_Reader.MoveNext_T(m_Key, m_Data);
    }

    void Print() const
    {
        Env::DocAddNum("aid", m_Key.m_KeyInContract.m_Aid);
        Env::DocAddNum("amount", m_Data.m_Amount);
    }

    void AddToTotals()
    {
        auto aid = m_Key.m_KeyInContract.m_Aid;

        uint32_t iIdx = 0;
        for (; ; iIdx++)
        {
            if (iIdx == m_Totals.m_Count)
            {
                auto& x = m_Totals.emplace_back();
                x.m_Aid = aid;
                x.m_Amount = m_Data.m_Amount;
                break;
            }

            if (m_Totals.m_p[iIdx].m_Aid == aid)
            {
                m_Totals.m_p[iIdx].m_Amount += m_Data.m_Amount; // don't care if overflows
                break;
            }
        }
    }

    void PrintTotals()
    {
        Env::DocArray gr0("totals");

        for (uint32_t i = 0; i < m_Totals.m_Count; i++)
        {
            Env::DocGroup gr1("");

            auto& x = m_Totals.m_p[i];
            Env::DocAddNum("aid", x.m_Aid);
            Env::DocAddNum("amount", x.m_Amount);
        }
    }
};

ON_METHOD(manager, view_balance)
{
    BalanceWalker wlk;

    {
        Env::DocArray gr0("items");
        for (wlk.Enum(cid); wlk.MoveNext(); )
        {
            Env::DocGroup gr1("");
            wlk.Print();
            wlk.AddToTotals();
        }
    }
    wlk.PrintTotals();
}

ON_METHOD(artist, set_artist)
{
    struct {
        Gallery::Method::ManageArtist args;
        char m_szLabelData[Gallery::Artist::s_TotalMaxLen + 2];
    } d;

    KeyMaterial::Owner km;
    km.SetCid(cid);
    PubKey pkArtist;
    km.Get(pkArtist);

    d.args.m_pkArtist = pkArtist;
    d.args.req = Gallery::Method::ManageArtist::RequestType::SET;
    d.args.role = Gallery::Role::ARTIST;

    uint32_t nArgSize = sizeof(d.args);

    uint32_t nLabelSize = Env::DocGetText("label", d.m_szLabelData, Gallery::Artist::s_LabelMaxLen + 1); // including 0-term

    if (nLabelSize > Gallery::Artist::s_LabelMaxLen + 1) // plus \0
    {
        OnError("label is too long");
        return;
    }

    d.args.m_LabelLen = (nLabelSize ? nLabelSize - 1 : 0);

    bool artist_exists = false;
    if (artist_label_exists(cid, d.m_szLabelData, artist_exists)) {
        OnError("label already exists");
        return;
    }

    nArgSize += d.args.m_LabelLen;

    uint32_t nDataSize = Env::DocGetText("data", d.m_szLabelData + d.args.m_LabelLen, Gallery::Artist::s_DataMaxLen + 1); // including 0-term

    if (nDataSize > Gallery::Artist::s_DataMaxLen + 1)
    {
        OnError("data too long");
        return;
    }

    d.args.m_DataLen = (nDataSize ? nDataSize - 1 : 0);
    nArgSize += d.args.m_DataLen;

    SigRequest sig;
    sig.m_pID = &km;
    sig.m_nID = sizeof(km);

    const char* comment = artist_exists ? "Updating artist's info" : "Becoming an artist";

    Env::GenerateKernel(&cid, d.args.s_iMethod, &d.args, nArgSize, nullptr, 0, &sig, 1, comment, 2500000);
}

ON_METHOD(artist, set_collection)
{
    struct {
        Gallery::Method::ManageCollection args;
        char m_szLabelData[Gallery::Collection::s_TotalMaxLen + 2];
    } d;

    KeyMaterial::Owner km;
    km.SetCid(cid);
    PubKey pkArtist;
    km.Get(pkArtist);

    d.args.m_pkArtist = pkArtist;
    d.args.req = Gallery::Method::ManageCollection::RequestType::SET;
    d.args.role = Gallery::Role::ARTIST;
    d.args.collection_id = collection_id;

    uint32_t nArgSize = sizeof(d.args);

    uint32_t nLabelSize = Env::DocGetText("label", d.m_szLabelData, Gallery::Collection::s_LabelMaxLen + 1); // including 0-term

    if (nLabelSize > Gallery::Collection::s_LabelMaxLen + 1) // plus \0
    {
        OnError("label is too long");
        return;
    }

    d.args.m_LabelLen = (nLabelSize ? nLabelSize - 1 : 0);

    nArgSize += d.args.m_LabelLen;

    uint32_t nDataSize = Env::DocGetText("data", d.m_szLabelData + d.args.m_LabelLen, Gallery::Collection::s_DataMaxLen + 1); // including 0-term

    if (nDataSize > Gallery::Collection::s_DataMaxLen + 1)
    {
        OnError("data too long");
        return;
    }

    d.args.m_DataLen = (nDataSize ? nDataSize - 1 : 0);
    nArgSize += d.args.m_DataLen;

    SigRequest sig;
    sig.m_pID = &km;
    sig.m_nID = sizeof(km);

    Env::GenerateKernel(&cid, d.args.s_iMethod, &d.args, nArgSize, nullptr, 0, &sig, 1, "Set collection", 2500000);
}

ON_METHOD(user, download)
{
    auto id_ = Utils::FromBE(id);
    
    Env::Key_T<Gallery::Events::AddArtworkData::Key> adk0, adk1;
    _POD_(adk0.m_Prefix.m_Cid) = cid;
    _POD_(adk1.m_Prefix.m_Cid) = cid;
    adk0.m_KeyInContract.m_ID = id_;
    adk1.m_KeyInContract.m_ID = id_;
    _POD_(adk0.m_KeyInContract.m_pkArtist).SetZero();
    _POD_(adk1.m_KeyInContract.m_pkArtist).SetObject(0xff);

    Env::Key_T<Gallery::Events::AddArtworkLabel::Key> alk0, alk1;
    _POD_(alk0.m_Prefix.m_Cid) = cid;
    _POD_(alk1.m_Prefix.m_Cid) = cid;
    alk0.m_KeyInContract.m_ID = id_;
    alk1.m_KeyInContract.m_ID = id_;
    _POD_(alk0.m_KeyInContract.m_pkArtist).SetZero();
    _POD_(alk1.m_KeyInContract.m_pkArtist).SetObject(0xff);

    uint32_t nDataCount = 0;
    Utils::Vector<uint8_t> vData;

    uint32_t nLabelCount = 0;
    Utils::Vector<char> vLabel;

    Env::LogReader adr(adk0, adk1);
    for ( ; ; nDataCount++)
    {
        uint32_t nData = 0, nKey = sizeof(adk0);
        if (!adr.MoveNext(&adk0, nKey, nullptr, nData, 0))
            break;

        vData.Prepare(vData.m_Count + nData);
        adr.MoveNext(&adk0, nKey, vData.m_p + vData.m_Count, nData, 1);
        vData.m_Count += nData;
    }

    Env::LogReader alr(alk0, alk1);
    for ( ; ; nLabelCount++)
    {
        uint32_t nData = 0, nKey = sizeof(alk0);
        if (!alr.MoveNext(&alk0, nKey, nullptr, nData, 0))
            break;

        vLabel.Prepare(vLabel.m_Count + nData);
        alr.MoveNext(&alk0, nKey, vLabel.m_p + vLabel.m_Count, nData, 1);
        vLabel.m_Count += nData;
    }

    if (nDataCount)
    {
        Env::DocAddNum("h", alr.m_Pos.m_Height);
        Env::DocAddBlob_T("artist", alk0.m_KeyInContract.m_pkArtist);
        Env::DocAddText("label", vLabel.m_p);
        Env::DocAddBlob("data", vData.m_p, vData.m_Count);
    }
    else
        OnError("not found");
}

ON_METHOD(artist, view)
{
    KeyMaterial::Owner km;
    km.SetCid(cid);
    PubKey pk;
    km.Get(pk);

    MyArtist a;
    a.Print(cid, pk);
}

ON_METHOD(artist, get_id)
{
    KeyMaterial::Owner km;
    km.SetCid(cid);
    PubKey pk;
    km.Get(pk);

    Env::DocAddBlob_T("id", pk);
}

bool PrintArtworks(const ContractID& cid, uint32_t id, Height h0, bool bFindAll, uint32_t count)
{
    Gallery::Artwork::SecondStageKey ssak;
    if (!bFindAll) {
        Env::Key_T<Gallery::Artwork::FirstStageKey> k2;
        k2.m_Prefix.m_Cid = cid;
        k2.m_KeyInContract.id = Utils::FromBE(id);
        Env::VarReader::Read_T(k2, ssak);
    }

    Env::Key_T<Gallery::Artwork::SecondStageKey> k0, k1;
    _POD_(k0.m_Prefix.m_Cid) = cid;
    _POD_(k1.m_Prefix.m_Cid) = cid;

    if (bFindAll) {
        k0.m_KeyInContract.id = 0;
        k1.m_KeyInContract.id = static_cast<Gallery::Artwork::Id>(-1);
    } else {
        k0.m_KeyInContract.id = Utils::FromBE(id);
        k1.m_KeyInContract.id = Utils::FromBE(id);
    }
    k0.m_KeyInContract.h_updated = bFindAll ? Utils::FromBE(h0) : ssak.h_updated;
    k1.m_KeyInContract.h_updated = bFindAll ? static_cast<Height>(-1) : ssak.h_updated;

    Env::VarReader r(k0, k1);
    MyArtwork a;
    int cur_cnt = 0;
    if (bFindAll) {
        Env::DocArray gr0("artworks");

        while (true) {
            if (!a.ReadNext(r, k0))
                break;

            if (count && cur_cnt++ >= count)
                break;

            Env::DocGroup gr1("");
            Env::DocAddNum32("id", Utils::FromBE(k0.m_KeyInContract.id));
            Env::DocAddNum32("updated", Utils::FromBE(k0.m_KeyInContract.h_updated));

            ImpressionWalker iwlk;
            iwlk.Enum(cid, Utils::FromBE(k0.m_KeyInContract.id));

            a.Print(iwlk);
        }
    } else {
        if (!a.ReadNext(r, k0))
            return false;
        Env::DocGroup gr1("");
        Env::DocAddNum32("id", Utils::FromBE(k0.m_KeyInContract.id));
        Env::DocAddNum32("updated", Utils::FromBE(k0.m_KeyInContract.h_updated));

        ImpressionWalker iwlk;
        iwlk.Enum(cid, Utils::FromBE(k0.m_KeyInContract.id));

        a.Print(iwlk);
    }

    return true;
}

void PrintItem(const Gallery::Artwork& m, Gallery::Artwork::Id id, ImpressionWalker& iwlk)
{
    const ContractID& cid = iwlk.m_Key.m_Prefix.m_Cid;
    OwnerInfo oi;

    if (m.m_Aid)
        Env::DocAddBlob_T("checkout.aid", m.m_Aid);

    Env::DocAddBlob_T("pk_author", m.m_pkAuthor);

    if (!_POD_(m.m_pkOwner).IsZero())
    {
        Env::DocAddBlob_T("pk_owner", m.m_pkOwner);
        Env::DocAddNum("owned", (uint32_t) !!oi.DeduceOwner(cid, id, m));

        if (m.m_Price.m_Amount)
        {
            Env::DocAddNum("price.aid", m.m_Price.m_Aid);
            Env::DocAddNum("price.amount", m.m_Price.m_Amount);
        }
    }

    uint32_t nImpressions = 0;
    uint32_t nMyImpression = 0;
    bool bMyImpressionSet = false;
    bool bMyImpressionKey = false;
    PubKey pkMyImpression;

    auto idNorm = Utils::FromBE(id);

    for ( ; iwlk.m_Valid; iwlk.Move())
    {
        auto idWlk = Utils::FromBE(iwlk.m_Key.m_KeyInContract.m_ID.m_ArtworkID);
        if (idWlk < idNorm)
            continue;
        if (idWlk > idNorm)
            break;

        if (!bMyImpressionSet)
        {
            if (!bMyImpressionKey)
            {
                bMyImpressionKey = true;
                oi.m_km.SetCid(cid);
                oi.m_km.m_ID = id | KeyMaterial::g_MskImpression;
                oi.m_km.Get(pkMyImpression);
            }

            if (_POD_(pkMyImpression) == iwlk.m_Key.m_KeyInContract.m_ID.m_pkUser)
            {
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

/*ON_METHOD(user, view_item)
{
    auto id_ = Utils::FromBE(id);

    Gallery::Artwork m;
    if (!ReadItem(cid, id_, m))
        return;

    ImpressionWalker iwlk;
    iwlk.Enum(cid, id_);

    PrintItem(m, id_, iwlk);

    Env::DocArray gr0("sales");


    Env::Key_T<Gallery::Events::Sell::Key> key;
    _POD_(key.m_Prefix.m_Cid) = cid;
    key.m_KeyInContract.m_ID = id_;

    for (Env::LogReader r(key, key); ; )
    {
        Gallery::Events::Sell evt;
        if (!r.MoveNext_T(key, evt))
            break;

        Env::DocGroup gr("");
        Env::DocAddNum("height", r.m_Pos.m_Height);
        Env::DocAddNum("amount", evt.m_Price.m_Amount);
        Env::DocAddNum("aid", evt.m_Price.m_Aid);
    }

}
*/

ON_METHOD(user, view_artworks)
{
    const size_t MAX_IDS = 128;
    char buf[MAX_IDS * sizeof(Gallery::Artwork::Id)];
    int buf_len = Env::DocGetText("ids", buf, sizeof(buf));
    if (buf_len)
        buf[buf_len - 1] = ';';

    if (count) {
        if (h0) {
            PrintArtworks(cid, 0, h0, true, count);
        } else {
            Env::Key_T<Gallery::Artwork::FirstStageKey> k0, k1;
            _POD_(k0.m_Prefix.m_Cid) = cid;
            _POD_(k1.m_Prefix.m_Cid) = cid;

            k0.m_KeyInContract.id = Utils::FromBE(idx0);
            k1.m_KeyInContract.id = static_cast<Gallery::Artwork::Id>(-1);

            Env::VarReader r(k0, k1);
            Gallery::Artwork::SecondStageKey ssak;
            Env::DocArray gr0("artworks");
            int cur_cnt = 0;
            while (cur_cnt < count) {
                if (!r.MoveNext_T(k0, ssak))
                    break;

                if (!_POD_(artist_id).IsZero()) {
                    Env::Key_T<Gallery::Artwork::SecondStageKey> ssak1;
                    ssak1.m_Prefix.m_Cid = cid;
                    ssak1.m_KeyInContract = ssak;
                    Gallery::Artwork a;
                    Env::VarReader::Read_T(ssak1, a);
                    if (_POD_(a.m_pkAuthor) != artist_id) continue;
                }

                if (collection_id) {
                    Env::Key_T<Gallery::CollectionArtworkKey> cak;
                    cak.m_Prefix.m_Cid = cid;
                    cak.m_KeyInContract.collection_id = collection_id;
                    cak.m_KeyInContract.artwork_id = k0.m_KeyInContract.id;
                    bool exists;
                    Env::VarReader::Read_T(cak, exists);
                    if (!exists) continue;
                }

                PrintArtworks(cid, Utils::FromBE(k0.m_KeyInContract.id), 0, false, 0);
                ++cur_cnt;
            }
        }
    } else if (buf_len) {
        std::string_view ids(buf);
        int cur_pos = 0;
        int next_pos;
        Env::DocArray gr0("artworks");
        while ((next_pos = ids.find(';', cur_pos)) != ids.npos) {
            uint32_t id = 0;

            for (int i = cur_pos; i < next_pos; ++i) {
                if (ids[i] >= '0' && ids[i] <= '9') {
                    id = id * 10 + ids[i] - '0';
                }
            }
            cur_pos = next_pos + 1;
            PrintArtworks(cid, id, 0, false, 0);
        }
    } else {
        PrintArtworks(cid, 0, h0, true, 0);
    }
}

ON_METHOD(manager, view_artworks)
{
    const size_t MAX_IDS = 128;
    char buf[MAX_IDS * sizeof(Gallery::Artwork::Id)];
    int buf_len = Env::DocGetText("ids", buf, sizeof(buf));
    if (buf_len)
        buf[buf_len - 1] = ';';

    if (count) {
        if (h0) {
            PrintArtworks(cid, 0, h0, true, count);
        } else {
            Env::Key_T<Gallery::Artwork::FirstStageKey> k0, k1;
            _POD_(k0.m_Prefix.m_Cid) = cid;
            _POD_(k1.m_Prefix.m_Cid) = cid;

            k0.m_KeyInContract.id = Utils::FromBE(idx0);
            k1.m_KeyInContract.id = static_cast<Gallery::Artwork::Id>(-1);

            Env::VarReader r(k0, k1);
            Gallery::Artwork::SecondStageKey ssak;
            Env::DocArray gr0("artworks");
            int cur_cnt = 0;
            while (cur_cnt < count) {
                if (!r.MoveNext_T(k0, ssak))
                    break;

                if (!_POD_(artist_id).IsZero()) {
                    Env::Key_T<Gallery::Artwork::SecondStageKey> ssak1;
                    ssak1.m_Prefix.m_Cid = cid;
                    ssak1.m_KeyInContract = ssak;
                    Gallery::Artwork a;
                    Env::VarReader::Read_T(ssak1, a);
                    if (_POD_(a.m_pkAuthor) != artist_id) continue;
                }

                if (collection_id) {
                    Env::Key_T<Gallery::CollectionArtworkKey> cak;
                    cak.m_Prefix.m_Cid = cid;
                    cak.m_KeyInContract.collection_id = collection_id;
                    cak.m_KeyInContract.artwork_id = k0.m_KeyInContract.id;
                    bool exists;
                    Env::VarReader::Read_T(cak, exists);
                    if (!exists) continue;
                }

                PrintArtworks(cid, Utils::FromBE(k0.m_KeyInContract.id), 0, false, 0);
                ++cur_cnt;
            }
        }
    } else if (buf_len) {
        std::string_view ids(buf);
        int cur_pos = 0;
        int next_pos;
        Env::DocArray gr0("artworks");
        while ((next_pos = ids.find(';', cur_pos)) != ids.npos) {
            uint32_t id = 0;

            for (int i = cur_pos; i < next_pos; ++i) {
                if (ids[i] >= '0' && ids[i] <= '9') {
                    id = id * 10 + ids[i] - '0';
                }
            }
            cur_pos = next_pos + 1;
            PrintArtworks(cid, id, 0, false, 0);
        }
    } else {
        PrintArtworks(cid, 0, h0, true, 0);
    }
}

ON_METHOD(user, set_price)
{
    auto id_ = Utils::FromBE(id);

    Gallery::Artwork m;
    OwnerInfo oi;
    if (!oi.ReadOwnedItem(cid, id_, m))
        return;

    Gallery::Method::SetPrice args;
    args.m_ID = id_;
    args.m_Price.m_Amount = amount;
    args.m_Price.m_Aid = aid;

    SigRequest sig;
    sig.m_pID = &oi.m_km;
    sig.m_nID = sizeof(oi.m_km);

    const char* comment = args.m_Price.m_Amount ? "Set item price" : "Remove from sale";

    Env::GenerateKernel(&cid, args.s_iMethod, &args, sizeof(args), nullptr, 0, &sig, 1, comment, 2000000);
}

ON_METHOD(user, transfer)
{
    auto id_ = Utils::FromBE(id);

    Gallery::Artwork m;
    OwnerInfo oi;
    if (!oi.ReadOwnedItem(cid, id_, m))
        return;

    Gallery::Method::Transfer args;
    args.m_ID = id_;
    _POD_(args.m_pkNewOwner) = pkNewOwner;

    SigRequest sig;
    sig.m_pID = &oi.m_km;
    sig.m_nID = sizeof(oi.m_km);

    Env::GenerateKernel(&cid, args.s_iMethod, &args, sizeof(args), nullptr, 0, &sig, 1, "Transfer item", 0);
}

ON_METHOD(user, buy)
{
    auto id_ = Utils::FromBE(id);

    Gallery::Artwork m;
    if (!ReadItem(cid, id_, m))
        return;

    if (!m.m_Price.m_Amount)
    {
        OnError("not for sale");
        return;
    }

    Gallery::Method::Buy args;
    args.m_ID = id_;
    args.m_HasAid = !!m.m_Aid;
    args.m_PayMax = m.m_Price.m_Amount;

    KeyMaterial::Owner km;
    km.SetCid(cid);
    km.m_ID = id_;
    km.Get(args.m_pkUser);

    FundsChange fc;
    fc.m_Consume = 1;
    fc.m_Amount = m.m_Price.m_Amount;
    fc.m_Aid = m.m_Price.m_Aid;

    Env::GenerateKernel(&cid, args.s_iMethod, &args, sizeof(args), &fc, 1, nullptr, 0, "Buy item", 0);
}

struct BalanceWalkerOwner
    :public BalanceWalker
{
    OwnerInfo m_Oi;

    void Enum(const ContractID& cid)
    {
        BalanceWalker::Enum(cid);
    }

    bool MoveNext()
    {
        while (true)
        {
            if (!BalanceWalker::MoveNext())
                return false;

            if (m_Oi.DeduceOwner(m_Key.m_Prefix.m_Cid, m_Key.m_KeyInContract.m_ID, m_Key.m_KeyInContract.m_pkUser))
                break;
        }

        return true;
    }
};

ON_METHOD(user, view_balance)
{
    BalanceWalkerOwner wlk;

    {
        Env::DocArray gr0("items");
        for (wlk.Enum(cid); wlk.MoveNext(); )
        {
            Env::DocGroup gr1("");
            wlk.Print();
            wlk.AddToTotals();
        }
    }
    wlk.PrintTotals();
}

ON_METHOD(user, withdraw)
{
    BalanceWalkerOwner wlk;
    uint32_t nCount = 0;
    for (wlk.Enum(cid); wlk.MoveNext(); )
    {
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

ON_METHOD(user, vote)
{
    StatePlus s;
    if (!s.Init(cid))
        return;

    auto id_ = Utils::FromBE(id);

    Gallery::Method::Vote args;
    args.m_Impression.m_Value = val;
    args.m_ID.m_ArtworkID = id_;

    KeyMaterial::Owner km;
    km.SetCid(cid);
    km.m_ID = id_ | KeyMaterial::g_MskImpression;
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

    Env::GenerateKernel(&cid, args.s_iMethod, &args, sizeof(args), &fc, 1, &sig, 1, "Vote", 0);
}

#undef ON_METHOD
#undef THE_FIELD

BEAM_EXPORT void Method_1()
{
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
