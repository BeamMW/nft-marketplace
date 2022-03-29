#include "Shaders/common.h"
#include "Shaders/app_common_impl.h"
#include "contract.h"
#include "Shaders/upgradable2/contract.h"
#include "Shaders/upgradable2/app_common_impl.h"

#include <string_view>

#define Gallery_manager_view(macro)
#define Gallery_manager_view_params(macro) macro(ContractID, cid)
#define Gallery_manager_view_artists(macro) \
    macro(ContractID, cid) \
    macro(Height, h0) \

//#define Gallery_manager_view_artist(macro) \
    macro(ContractID, cid) \
    macro(PubKey, pkArtist)

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
    macro(Gallery::Masterpiece::ID, id)

#define GalleryRole_manager(macro) \
    macro(manager, view) \
    macro(manager, view_params) \
    macro(manager, view_artists) \
    macro(manager, view_balance) \
    macro(manager, add_rewards) \
    macro(manager, my_admin_key) \
    macro(manager, explicit_upgrade) \
    macro(manager, admin_delete) \
    //macro(manager, view_artist) \
    //macro(manager, manage_artist) \

#define Gallery_artist_view(macro) macro(ContractID, cid)
#define Gallery_artist_get_key(macro) macro(ContractID, cid)

#define Gallery_artist_set_artist(macro) \
    macro(ContractID, cid) \

#define Gallery_artist_upload(macro) \
    macro(ContractID, cid) \

#define GalleryRole_artist(macro) \
    macro(artist, view) \
    macro(artist, get_key) \
    macro(artist, upload) \
    macro(artist, set_artist) \

#define Gallery_user_view_item(macro) \
    macro(ContractID, cid) \
    macro(Gallery::Masterpiece::ID, id)

#define Gallery_user_view_items(macro) \
    macro(ContractID, cid) \
    macro(Height, h0) \
    macro(Amount, count)

#define Gallery_user_download(macro) \
    macro(ContractID, cid) \
    macro(Gallery::Masterpiece::ID, id)

#define Gallery_user_set_price(macro) \
    macro(ContractID, cid) \
    macro(Gallery::Masterpiece::ID, id) \
    macro(Amount, amount) \
    macro(AssetID, aid)

#define Gallery_user_transfer(macro) \
    macro(ContractID, cid) \
    macro(Gallery::Masterpiece::ID, id) \
    macro(PubKey, pkNewOwner)

#define Gallery_user_buy(macro) \
    macro(ContractID, cid) \
    macro(Gallery::Masterpiece::ID, id)

#define Gallery_user_view_balance(macro) macro(ContractID, cid)

#define Gallery_user_withdraw(macro) \
    macro(ContractID, cid) \
    macro(uint32_t, nMaxCount) \

#define Gallery_user_vote(macro) \
    macro(ContractID, cid) \
    macro(Gallery::Masterpiece::ID, id) \
    macro(uint32_t, val) \

#define GalleryRole_user(macro) \
    macro(user, view_item) \
    macro(user, view_items) \
    macro(user, download) \
    macro(user, set_price) \
    macro(user, transfer) \
    macro(user, buy) \
    macro(user, view_balance) \
    macro(user, withdraw) \
    macro(user, vote) \

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
        Gallery::Masterpiece::ID m_ID;
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

    const Gallery::Masterpiece::ID g_MskImpression = Utils::FromBE((static_cast<Gallery::Masterpiece::ID>(-1) >> 1) + 1); // hi bit
}

ON_METHOD(manager, view)
{
    static const ShaderID s_pSid[] = {
        Gallery::s_SID_0,
        Gallery::s_SID_1,
        Gallery::s_SID_2,
        Gallery::s_SID_3,
        Gallery::s_SID_4,
        Gallery::s_SID_5,
        Gallery::s_SID_6,
        Gallery::s_SID_7,
        Gallery::s_SID_8,
        Gallery::s_SID_9,
        Gallery::s_SID_10,
        Gallery::s_SID_11,
        Gallery::s_SID_12,
        Gallery::s_SID_13,
        Gallery::s_SID_14,
        Gallery::s_SID_15,
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
    Env::DocAddNum("Exhibits", s.m_Exhibits);
    Env::DocAddNum("voteReward.aid", s.m_Config.m_VoteReward.m_Aid);
    Env::DocAddNum("voteReward.amount", s.m_Config.m_VoteReward.m_Amount);
    Env::DocAddNum("voteReward_balance", s.m_VoteBalance);
}

#pragma pack (push, 0)
struct MyArtist
    :public Gallery::Artist
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
        Env::DocAddNum("hReg", m_hRegistered);
        Env::DocAddNum32("isApproved", is_approved);
    }

    std::string_view GetData()
    {
        return std::string_view(m_szLabelData + label_len, data_len);
    }

    std::string_view GetLabel()
    {
        return std::string_view(m_szLabelData, label_len);
    }

    bool ReadNext(Env::VarReader& r, Env::Key_T<Gallery::Artist::SecondStageKey>& key)
    {
        while (true)
        {
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

bool artist_label_exists(const ContractID& cid, const std::string_view& label)
{
    Env::Key_T<Gallery::Artist::SecondStageKey> k0, k1;
    _POD_(k0.m_Prefix.m_Cid) = cid;
    _POD_(k1.m_Prefix.m_Cid) = cid;

    _POD_(k0.m_KeyInContract.m_pkUser).SetZero();
    _POD_(k1.m_KeyInContract.m_pkUser).SetObject(0xff);

    k0.m_KeyInContract.h_last_updated = 0;
    k1.m_KeyInContract.h_last_updated = static_cast<Height>(-1);

    Env::VarReader r(k0, k1);
    MyArtist a;
    while (true) {
        if (!a.ReadNext(r, k0))
            break;

        if (a.GetLabel() == label)
            return true;
    }
    return false;
}

bool PrintArtists(const ContractID& cid, const PubKey& pkArtist, Height h0, bool bFindAll, bool bMustFind)
{
    Env::Key_T<Gallery::Artist::SecondStageKey> k0, k1;
    _POD_(k0.m_Prefix.m_Cid) = cid;
    _POD_(k1.m_Prefix.m_Cid) = cid;

    if (bFindAll) {
        _POD_(k0.m_KeyInContract.m_pkUser).SetZero();
        _POD_(k1.m_KeyInContract.m_pkUser).SetObject(0xff);
    } else {
        _POD_(k0.m_KeyInContract.m_pkUser) = pkArtist;
        _POD_(k1.m_KeyInContract.m_pkUser) = pkArtist;
    }
    k0.m_KeyInContract.h_last_updated = bFindAll ? Utils::FromBE(h0) : 0;
    k1.m_KeyInContract.h_last_updated = static_cast<Height>(-1);

    Env::VarReader r(k0, k1);
    MyArtist a;
    if (bFindAll) {
        Env::DocArray gr0("artists");

        while (true) {
            if (!a.ReadNext(r, k0))
                break;

            Env::DocGroup gr1("");
            Env::DocAddBlob_T("key", k0.m_KeyInContract.m_pkUser);
            Env::DocAddNum32("last_updated", Utils::FromBE(k0.m_KeyInContract.h_last_updated));
            a.Print();
        }
    } else {
        if (!a.ReadNext(r, k0)) {
            if (bMustFind)
                OnError("not found");
            return false;
        }
        Env::DocGroup gr1("");
        Env::DocAddBlob_T("key", k0.m_KeyInContract.m_pkUser);
        Env::DocAddNum32("last_updated", Utils::FromBE(k0.m_KeyInContract.h_last_updated));
        a.Print();
    }

    return true;
}

ON_METHOD(manager, view_artists)
{
    const size_t MAX_IDS = 128;
    char buf[MAX_IDS * sizeof(PubKey)];
    int buf_len = Env::DocGetText("ids", buf, sizeof(buf));
    buf[buf_len - 1] = ';';

    if (!h0 && !buf_len) {
        PrintArtists(cid, PubKey{}, h0, true, false);
    } else if (buf_len) {
        std::string_view ids(buf);
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
            PrintArtists(cid, blob.pk, 0, false, false);
        }
    }
}

/*ON_METHOD(manager, view_artist)
{
    PrintArtists(cid, pkArtist, 0, false, true);
}

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

ON_METHOD(artist, upload)
{
    KeyMaterial::Owner km;
    km.SetCid(cid);
    PubKey pkArtist;
    km.Get(pkArtist);

    auto nDataLen = Env::DocGetBlob("data", nullptr, 0);
    if (!nDataLen)
    {
        OnError("data not specified");
        return;
    }

    uint32_t nSizeArgs = sizeof(Gallery::Method::AddExhibit) + nDataLen;
    auto* pArgs = (Gallery::Method::AddExhibit*) Env::Heap_Alloc(nSizeArgs);

    _POD_(pArgs->m_pkArtist) = pkArtist;
    pArgs->m_Size = nDataLen;

    if (Env::DocGetBlob("data", pArgs + 1, nDataLen) != nDataLen)
    {
        OnError("data can't be parsed");
        return;
    }

    SigRequest sig;
    sig.m_pID = &km;
    sig.m_nID = sizeof(km);

    // estimate charge, it may be non-negligible.
    // - Patent verification: LoadVar + SaveVar, 25K units
    // - Artist verification: LoadVar, 5K units
    // - Global state: LoadVar + SaveVar, 25K units
    // - Masterpiece creation: SaveVar, 20K units
    // - AddSig by admin, 10K units
    // - event for the data. This is 100 units per byte, plus 5K units per call, which is repeated for each 8K of data
    // - add some extra for other stuff

    uint32_t nCharge =
        ManagerUpgadable2::get_ChargeInvoke() +
        Env::Cost::LoadVar_For(sizeof(Gallery::State)) +
        Env::Cost::SaveVar_For(sizeof(Gallery::State)) +
        Env::Cost::LoadVar_For(sizeof(Gallery::Artist)) +
        Env::Cost::LoadVar_For(sizeof(Gallery::Artist::SecondStageKey)) +
        Env::Cost::SaveVar_For(sizeof(Gallery::Masterpiece)) +
        Env::Cost::SaveVar_For(sizeof(Height)) +
        Env::Cost::AddSig +
        Env::Cost::Cycle * 200;

    const uint32_t nSizeEvt = 0x2000;
    uint32_t nFullCycles = nDataLen / nSizeEvt;

    nCharge += (Env::Cost::Log_For(nSizeEvt) + Env::Cost::Cycle * 50) * nFullCycles;

    nDataLen %= nSizeEvt;
    if (nDataLen)
        nCharge += Env::Cost::Log_For(nDataLen) + Env::Cost::Cycle * 50;

    Env::GenerateKernel(&cid, pArgs->s_iMethod, pArgs, nSizeArgs, nullptr, 0, &sig, 1, "Gallery upload masterpiece", nCharge);

    Env::Heap_Free(pArgs);
}

ON_METHOD(manager, admin_delete)
{
    auto id_ = Utils::FromBE(id);

    Gallery::Method::AdminDelete args;
    args.m_ID = id_;

    KeyMaterial::MyAdminKey kid;
    Env::GenerateKernel(&cid, args.s_iMethod, &args, sizeof(args), nullptr, 0, &kid, 1, "Gallery delete masterpiece", 0);

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

    Env::GenerateKernel(&cid, args.s_iMethod, &args, sizeof(args), &fc, 1, nullptr, 0, "Gallery add voting rewards", 0);
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

    d.args.m_LabelLen = (nLabelSize ? nLabelSize : 0);

    if (artist_label_exists(cid, d.m_szLabelData)) {
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

    Env::GenerateKernel(&cid, d.args.s_iMethod, &d.args, nArgSize, nullptr, 0, &sig, 1, "Set artist", 122964);
}

ON_METHOD(user, download)
{
    auto id_ = Utils::FromBE(id);

    Env::Key_T<Gallery::Events::Add::Key> k0, k1;
    _POD_(k0.m_Prefix.m_Cid) = cid;
    _POD_(k1.m_Prefix.m_Cid) = cid;
    k0.m_KeyInContract.m_ID = id_;
    k1.m_KeyInContract.m_ID = id_;
    _POD_(k0.m_KeyInContract.m_pkArtist).SetZero();
    _POD_(k1.m_KeyInContract.m_pkArtist).SetObject(0xff);

    uint32_t nCount = 0;
    Utils::Vector<uint8_t> vData;

    Env::LogReader r(k0, k1);
    for ( ; ; nCount++)
    {
        uint32_t nData = 0, nKey = sizeof(k0);
        if (!r.MoveNext(&k0, nKey, nullptr, nData, 0))
            break;

        vData.Prepare(vData.m_Count + nData);
        r.MoveNext(&k0, nKey, vData.m_p + vData.m_Count, nData, 1);
        vData.m_Count += nData;
    }

    if (nCount)
    {
        Env::DocAddNum("h", r.m_Pos.m_Height);
        Env::DocAddBlob_T("artist", k0.m_KeyInContract.m_pkArtist);
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

    if (PrintArtists(cid, pk, 0, false, false))
        return;

    // try workaround
    km.SetCid();
    km.Get(pk);
    PrintArtists(cid, pk, 0, false, true);

}

ON_METHOD(artist, get_key)
{
    KeyMaterial::Owner km;
    km.SetCid(cid);
    PubKey pk;
    km.Get(pk);

    Env::DocAddBlob_T("key", pk);
}

bool ReadItem(const ContractID& cid, Gallery::Masterpiece::ID id, Gallery::Masterpiece& m)
{
    Env::Key_T<Gallery::Masterpiece::FirstStageKey> fskey;
    Env::Key_T<Gallery::Masterpiece::SecondStageKey> sskey;
    _POD_(fskey.m_Prefix.m_Cid) = cid;
    _POD_(sskey.m_Prefix.m_Cid) = cid;
    fskey.m_KeyInContract.m_ID = id;
    sskey.m_KeyInContract.m_ID = id;

    if (!Env::VarReader::Read_T(fskey, sskey.m_KeyInContract.h_last_updated)) {
        OnError("not found");
        return false;
    }

    Env::VarReader::Read_T(sskey, m);
    return true;
}

struct OwnerInfo
{
    KeyMaterial::Owner m_km;

    bool DeduceOwner(const ContractID& cid, Gallery::Masterpiece::ID id, const Gallery::Masterpiece& m)
    {
        return DeduceOwner(cid, id, m.m_pkOwner);
    }

    bool DeduceOwnerRaw(Gallery::Masterpiece::ID id, const PubKey& pkOwner)
    {
        PubKey pk;

        m_km.m_ID = id;
        m_km.Get(pk);
        return (_POD_(pk) == pkOwner);
    }

    bool DeduceOwner(const ContractID& cid, Gallery::Masterpiece::ID id, const PubKey& pkOwner)
    {
        m_km.SetCid(cid);
        if (DeduceOwnerRaw(id, pkOwner) || // owner
            DeduceOwnerRaw(0, pkOwner)) // artist
            return true;

        m_km.SetCid();
        return DeduceOwnerRaw(0, pkOwner); // artist, older key gen
    }

    bool ReadOwnedItem(const ContractID& cid, Gallery::Masterpiece::ID id, Gallery::Masterpiece& m)
    {
        if (!ReadItem(cid, id, m))
            return false;

        if (DeduceOwner(cid, id, m))
            return true;

        OnError("not owned");
        return false;
    }

};

struct ImpressionWalker
{
    Env::VarReaderEx<true> m_Reader;
    Env::Key_T<Gallery::Impression::Key> m_Key;
    Gallery::Impression m_Value;
    bool m_Valid = false;

    void Enum(const ContractID& cid, Gallery::Masterpiece::ID id) {
        Enum(cid, id, id);
    }

    void Enum(const ContractID& cid, Gallery::Masterpiece::ID id0, Gallery::Masterpiece::ID id1)
    {
        _POD_(m_Key.m_Prefix.m_Cid) = cid;
        m_Key.m_KeyInContract.m_ID.m_MasterpieceID = id0;
        _POD_(m_Key.m_KeyInContract.m_ID.m_pkUser).SetZero();

        Env::Key_T<Gallery::Impression::Key> k1;
        _POD_(k1.m_Prefix.m_Cid) = cid;
        k1.m_KeyInContract.m_ID.m_MasterpieceID = id1;
        _POD_(k1.m_KeyInContract.m_ID.m_pkUser).SetObject(0xff);

        m_Reader.Enum_T(m_Key, k1);
        Move();
    }

    void Move()
    {
        m_Valid = m_Reader.MoveNext_T(m_Key, m_Value);
    }
};

void PrintItem(const Gallery::Masterpiece& m, Gallery::Masterpiece::ID id, ImpressionWalker& iwlk)
{
    const ContractID& cid = iwlk.m_Key.m_Prefix.m_Cid;
    OwnerInfo oi;

    if (m.m_Aid)
        Env::DocAddBlob_T("checkout.aid", m.m_Aid);

    Env::DocAddBlob_T("pk_author", m.m_pkAuthor);

    if (!_POD_(m.m_pkOwner).IsZero())
    {
        Env::DocAddBlob_T("pk", m.m_pkOwner);
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
        auto idWlk = Utils::FromBE(iwlk.m_Key.m_KeyInContract.m_ID.m_MasterpieceID);
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

ON_METHOD(user, view_item)
{
    auto id_ = Utils::FromBE(id);

    Gallery::Masterpiece m;
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

ON_METHOD(user, view_items)
{
    Env::Key_T<Gallery::Masterpiece::SecondStageKey> k0, k1;
    _POD_(k0.m_Prefix.m_Cid) = cid;
    _POD_(k1.m_Prefix.m_Cid) = cid;
    k0.m_KeyInContract.m_ID = 0;
    k0.m_KeyInContract.h_last_updated = Utils::FromBE(h0);
    k1.m_KeyInContract.m_ID = static_cast<Gallery::Masterpiece::ID>(-1);
    k1.m_KeyInContract.h_last_updated = Utils::FromBE(Env::get_Height());

    Env::DocArray gr0("items");

    int i = 1;
    auto prev_key = k0;
    for (Env::VarReader r(k0, k1); ; ++i)
    {
        Gallery::Masterpiece m;
        if (!r.MoveNext_T(k0, m))
            break;

        if (count && i > count) {
            // to finish current block
            if (k0.m_KeyInContract.h_last_updated != prev_key.m_KeyInContract.h_last_updated)
                break;
        }
        ImpressionWalker iwlk;
        iwlk.Enum(cid, k0.m_KeyInContract.m_ID);

        Env::DocGroup gr1("");
        Env::DocAddNum("id", Utils::FromBE(k0.m_KeyInContract.m_ID));
        Env::DocAddNum("height", Utils::FromBE(k0.m_KeyInContract.h_last_updated));
        PrintItem(m, k0.m_KeyInContract.m_ID, iwlk);
        prev_key = k0;
    }
}

ON_METHOD(user, set_price)
{
    auto id_ = Utils::FromBE(id);

    Gallery::Masterpiece m;
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

    // TODO: replace hardcoded charge
    Env::GenerateKernel(&cid, args.s_iMethod, &args, sizeof(args), nullptr, 0, &sig, 1, "Gallery set item price", 138184);
}

ON_METHOD(user, transfer)
{
    auto id_ = Utils::FromBE(id);

    Gallery::Masterpiece m;
    OwnerInfo oi;
    if (!oi.ReadOwnedItem(cid, id_, m))
        return;

    Gallery::Method::Transfer args;
    args.m_ID = id_;
    _POD_(args.m_pkNewOwner) = pkNewOwner;

    SigRequest sig;
    sig.m_pID = &oi.m_km;
    sig.m_nID = sizeof(oi.m_km);

    Env::GenerateKernel(&cid, args.s_iMethod, &args, sizeof(args), nullptr, 0, &sig, 1, "Gallery transfer item", 0);
}

ON_METHOD(user, buy)
{
    auto id_ = Utils::FromBE(id);

    Gallery::Masterpiece m;
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

    Env::GenerateKernel(&cid, args.s_iMethod, &args, sizeof(args), &fc, 1, nullptr, 0, "Gallery buy item", 0);
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

        Env::GenerateKernel(&cid, args.s_iMethod, &args, sizeof(args), &fc, 1, &sig, 1, nCount ? "" : "Gallery withdraw", 0);

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
    args.m_ID.m_MasterpieceID = id_;

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

    Env::GenerateKernel(&cid, args.s_iMethod, &args, sizeof(args), &fc, 1, &sig, 1, "Gallery vote", 0);
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

