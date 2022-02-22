#include "Shaders/common.h"
#include "Shaders/Math.h"
#include "contract.h"

struct MyState
    :public Gallery::State
{
    MyState() {
        Env::LoadVar_T((uint8_t) s_Key, *this);
    }

    MyState(bool) {
        // no auto-load
    }

    void Save() {
        Env::SaveVar_T((uint8_t) s_Key, *this);
    }

    void AddSigAdmin() {
        Env::AddSig(m_Config.m_pkAdmin);
    }
};

bool LoadMasterpiece(const Gallery::Masterpiece::FirstStageKey& fsmk, Gallery::Masterpiece::SecondStageKey& ssmk, Gallery::Masterpiece& m) {
    if (!Env::LoadVar_T(fsmk, ssmk.h_last_updated)) {
        return false;
    }
    return Env::LoadVar_T(ssmk, m);
}

void RewriteMasterpiece(const Gallery::Masterpiece::FirstStageKey& fsmk, Gallery::Masterpiece::SecondStageKey& ssmk, const Gallery::Masterpiece& m) {
    Env::DelVar_T(fsmk);
    Env::DelVar_T(ssmk);
    ssmk.h_last_updated = Utils::FromBE(Env::get_Height());
    Env::SaveVar_T(fsmk, ssmk.h_last_updated);
    Env::SaveVar_T(ssmk, m);
}

BEAM_EXPORT void Ctor(const Gallery::Method::Init& r)
{
    if (Env::get_CallDepth() > 1)
    {
        MyState s(false);
        s.m_Exhibits = 0;
        s.m_VoteBalance = 0;
        _POD_(s.m_Config) = r.m_Config;

        s.Save();
    }
}

BEAM_EXPORT void Dtor(void*)
{
    // ignore
}

void PayoutMove(const Gallery::Payout::Key& key, Amount val, bool bAdd)
{
    if (!val)
        return;

    Gallery::Payout po;
    if (Env::LoadVar_T(key, po))
    {
        if (bAdd)
            Strict::Add(po.m_Amount, val);
        else
        {
            Strict::Sub(po.m_Amount, val);

            if (!po.m_Amount)
            {
                Env::DelVar_T(key);
                return;
            }
        }
    }
    else
    {
        Env::Halt_if(!bAdd);
        po.m_Amount = val;
    }

    Env::SaveVar_T(key, po);
}

BEAM_EXPORT void Method_2(void*)
{
    // called on upgrade
}

BEAM_EXPORT void Method_10(const Gallery::Method::ManageArtist& r)
{
    using ArtistReqType = Gallery::Method::ManageArtist::RequestType;

    Gallery::Artist::FirstStageKey fsak;
    _POD_(fsak.m_pkUser) = r.m_pkArtist;

    struct ArtistPlus : public Gallery::Artist {
        char m_szLabel[s_LabelMaxLen];
    } a;

    Gallery::Artist::SecondStageKey ssak;

    // call from user:become_artist
    if (r.m_LabelLen <= Gallery::Artist::s_LabelMaxLen)
    {
        Env::Halt_if(r.req != ArtistReqType::CREATE);
        a.is_approved = false;
        a.m_hRegistered = Env::get_Height();
        Env::Memcpy(a.m_szLabel, &r + 1, r.m_LabelLen);

        Gallery::Artist::SecondStageKey ssak;
        _POD_(ssak.m_pkUser) = r.m_pkArtist;
        ssak.h_last_updated = Utils::FromBE(a.m_hRegistered);

        Env::Halt_if(Env::SaveVar_T(fsak, ssak));
        Env::SaveVar(&ssak, sizeof(ssak), &a, sizeof(Gallery::Artist) + r.m_LabelLen, KeyTag::Internal); // will fail if already exists
        Env::AddSig(r.m_pkArtist);
    } else { // call from manager:manage_artist
        Env::Halt_if(!Env::LoadVar_T(fsak, ssak));
        switch (r.req) {
        case ArtistReqType::APPROVE: {
            uint32_t artist_size = Env::LoadVar(&ssak, sizeof(ssak), &a, sizeof(a), KeyTag::Internal);
            Env::DelVar_T(ssak);

            a.is_approved = true;
            ssak.h_last_updated = Utils::FromBE(Env::get_Height());

            Env::SaveVar_T(fsak, ssak);
            Env::SaveVar(&ssak, sizeof(ssak), &a, artist_size, KeyTag::Internal); // will fail if already exists
            break;
        }
        case ArtistReqType::DELETE:
            Env::Halt_if(!Env::DelVar_T(ssak));
            Env::Halt_if(!Env::DelVar_T(fsak));
            break;
        default:
            Env::Halt();
        }

        MyState s;
        s.AddSigAdmin();
    }
}

BEAM_EXPORT void Method_3(const Gallery::Method::AddExhibit& r)
{
    MyState s;

    Gallery::Masterpiece::FirstStageKey fsmk;
    fsmk.m_ID = Utils::FromBE(++s.m_Exhibits);

    Gallery::Masterpiece::SecondStageKey ssmk;
    ssmk.m_ID = fsmk.m_ID;
    s.Save();

    Gallery::Masterpiece m;
    _POD_(m).SetZero();
    _POD_(m.m_pkOwner) = r.m_pkArtist;

    auto pData = reinterpret_cast<const uint8_t*>(&r + 1);
    uint32_t nData = r.m_Size;

    {
        // verify artist
        Gallery::Artist::FirstStageKey fsak;
        _POD_(fsak.m_pkUser) = r.m_pkArtist;

        Gallery::Artist::SecondStageKey ssak;
        Env::Halt_if(!Env::LoadVar_T(fsak, ssak));

        struct ArtistPlus : public Gallery::Artist {
            char m_szLabel[s_LabelMaxLen];
        } a;

        Env::LoadVar(&ssak, sizeof(ssak), &a, sizeof(a), KeyTag::Internal);
        Env::Halt_if(!a.is_approved);
    }

    ssmk.h_last_updated = Utils::FromBE(Env::get_Height());
    Env::SaveVar_T(ssmk, m);
    Env::SaveVar_T(fsmk, ssmk.h_last_updated);

    Env::AddSig(r.m_pkArtist);

    Gallery::Events::Add::Key eak;
    eak.m_ID = ssmk.m_ID;
    _POD_(eak.m_pkArtist) = m.m_pkOwner;

    uint32_t nMaxEventSize = 0x2000; // TODO: max event size is increased to 1MB from HF4

    while (true)
    {
        Env::EmitLog(&eak, sizeof(eak), pData, std::min(nData, nMaxEventSize), KeyTag::Internal);
        if (nData <= nMaxEventSize)
            break;

        nData -= nMaxEventSize;
        pData += nMaxEventSize;
    }
}

BEAM_EXPORT void Method_4(const Gallery::Method::SetPrice& r)
{
    Gallery::Masterpiece::FirstStageKey fsmk;
    Gallery::Masterpiece::SecondStageKey ssmk;
    fsmk.m_ID = r.m_ID;
    ssmk.m_ID = r.m_ID;

    Gallery::Masterpiece m;
    Env::Halt_if(!LoadMasterpiece(fsmk, ssmk, m));

    _POD_(m.m_Price) = r.m_Price;

    RewriteMasterpiece(fsmk, ssmk, m);

    Env::AddSig(m.m_pkOwner); // would fail if no current owner (i.e. checked out)
}

BEAM_EXPORT void Method_5(const Gallery::Method::Buy& r)
{
    Gallery::Masterpiece::FirstStageKey fsmk;
    Gallery::Masterpiece::SecondStageKey ssmk;
    fsmk.m_ID = r.m_ID;
    ssmk.m_ID = r.m_ID;

    Gallery::Masterpiece m;
    Env::Halt_if(!LoadMasterpiece(fsmk, ssmk, m));

    Env::Halt_if(
        !m.m_Price.m_Amount || // not for sale!
        (r.m_PayMax < m.m_Price.m_Amount) || // too expensive
        (r.m_HasAid != (!!m.m_Aid))
    );

    Env::FundsLock(m.m_Price.m_Aid, r.m_PayMax);

    Gallery::Events::Sell::Key esk;
    esk.m_ID = r.m_ID;
    Gallery::Events::Sell es;
    _POD_(es.m_Price) = m.m_Price;
    es.m_HasAid = r.m_HasAid;
    Env::EmitLog_T(esk, es);

    Gallery::Payout::Key pok;
    pok.m_ID = r.m_ID;
    pok.m_Aid = m.m_Price.m_Aid;

    _POD_(pok.m_pkUser) = m.m_pkOwner;
    PayoutMove(pok, m.m_Price.m_Amount, true);

    _POD_(pok.m_pkUser) = r.m_pkUser;
    PayoutMove(pok, r.m_PayMax - m.m_Price.m_Amount, true);

    _POD_(m.m_pkOwner) = r.m_pkUser;
    _POD_(m.m_Price).SetZero(); // not for sale until new owner sets the price

    RewriteMasterpiece(fsmk, ssmk, m);

    //Env::AddSig(r.m_pkUser);
}

BEAM_EXPORT void Method_6(const Gallery::Method::Withdraw& r)
{
    PayoutMove(r.m_Key, r.m_Value, false);
    Env::FundsUnlock(r.m_Key.m_Aid, r.m_Value);
    Env::AddSig(r.m_Key.m_pkUser);
}

BEAM_EXPORT void Method_7(const Gallery::Method::CheckPrepare& r)
{
    Gallery::Masterpiece::FirstStageKey fsmk;
    Gallery::Masterpiece::SecondStageKey ssmk;
    fsmk.m_ID = r.m_ID;
    ssmk.m_ID = r.m_ID;

    Gallery::Masterpiece m;
    Env::Halt_if(!LoadMasterpiece(fsmk, ssmk, m));
    Env::AddSig(m.m_pkOwner);

    if (m.m_Aid)
    {
        // destroy it
        Env::Halt_if(!Env::AssetDestroy(m.m_Aid));
        m.m_Aid = 0;
    }
    else
    {
        // 1st call. Don't checkout, only prepare
        static const char szMeta[] = "STD:SCH_VER=1;N=Gallery Masterpiece;SN=Gall;UN=GALL;NTHUN=unique";
        m.m_Aid = Env::AssetCreate(szMeta, sizeof(szMeta) - 1);
    }

    RewriteMasterpiece(fsmk, ssmk, m);
}

BEAM_EXPORT void Method_8(const Gallery::Method::CheckOut& r)
{
    Gallery::Masterpiece::FirstStageKey fsmk;
    Gallery::Masterpiece::SecondStageKey ssmk;
    fsmk.m_ID = r.m_ID;
    ssmk.m_ID = r.m_ID;

    Gallery::Masterpiece m;
    Env::Halt_if(!LoadMasterpiece(fsmk, ssmk, m) || !m.m_Aid);
    Env::AddSig(m.m_pkOwner);

    Env::Halt_if(!Env::AssetEmit(m.m_Aid, 1, 1));
    Env::FundsUnlock(m.m_Aid, 1);

    _POD_(m.m_pkOwner).SetZero();
    _POD_(m.m_Price).SetZero();

    RewriteMasterpiece(fsmk, ssmk, m);
}

BEAM_EXPORT void Method_9(const Gallery::Method::CheckIn& r)
{
    Gallery::Masterpiece::FirstStageKey fsmk;
    Gallery::Masterpiece::SecondStageKey ssmk;
    fsmk.m_ID = r.m_ID;
    ssmk.m_ID = r.m_ID;

    Gallery::Masterpiece m;
    Env::Halt_if(!LoadMasterpiece(fsmk, ssmk, m) || !_POD_(m.m_pkOwner).IsZero());

    Env::FundsLock(m.m_Aid, 1);
    Env::Halt_if(!Env::AssetEmit(m.m_Aid, 1, 0));

    _POD_(m.m_pkOwner) = r.m_pkUser;
    RewriteMasterpiece(fsmk, ssmk, m);

    //Env::AddSig(r.m_pkUser);
}

BEAM_EXPORT void Method_11(const Gallery::Method::Vote& r)
{
    Gallery::Impression::Key impk;
    _POD_(impk.m_ID) = r.m_ID;

    Gallery::Impression imp;
    if (!Env::LoadVar_T(impk, imp))
    {
        imp.m_Value = 0;

        MyState s;
        Strict::Sub(s.m_VoteBalance, s.m_Config.m_VoteReward.m_Amount);
        s.Save();

        Env::Halt_if(Utils::FromBE(impk.m_ID.m_MasterpieceID) > s.m_Exhibits);

        Env::FundsUnlock(s.m_Config.m_VoteReward.m_Aid, s.m_Config.m_VoteReward.m_Amount);
    }

    Env::Halt_if(imp.m_Value == r.m_Impression.m_Value); // not changed

    imp.m_Value = r.m_Impression.m_Value;
    Env::SaveVar_T(impk, imp);

    Env::AddSig(impk.m_ID.m_pkUser);
}

BEAM_EXPORT void Method_12(const Gallery::Method::AddVoteRewards& r)
{
    MyState s;
    Strict::Add(s.m_VoteBalance, r.m_Amount);
    s.Save();

    Env::FundsLock(s.m_Config.m_VoteReward.m_Aid, r.m_Amount);
}

BEAM_EXPORT void Method_13(const Gallery::Method::AdminDelete& r)
{
    // ensure the masterpiece doesn't have aid
    Gallery::Masterpiece::FirstStageKey fsmk;
    Gallery::Masterpiece::SecondStageKey ssmk;
    fsmk.m_ID = r.m_ID;
    ssmk.m_ID = r.m_ID;

    Gallery::Masterpiece m;
    Env::Halt_if(!LoadMasterpiece(fsmk, ssmk, m));

    Env::Halt_if(m.m_Aid);

    Env::DelVar_T(ssmk);
    Env::DelVar_T(fsmk);

    MyState s;
    s.AddSigAdmin();
}

BEAM_EXPORT void Method_14(const Gallery::Method::Transfer& r)
{
    Gallery::Masterpiece::FirstStageKey fsmk;
    Gallery::Masterpiece::SecondStageKey ssmk;
    fsmk.m_ID = r.m_ID;
    ssmk.m_ID = r.m_ID;

    Gallery::Masterpiece m;
    Env::Halt_if(!LoadMasterpiece(fsmk, ssmk, m));

    Env::AddSig(m.m_pkOwner);
    _POD_(m.m_pkOwner) = r.m_pkNewOwner;

    RewriteMasterpiece(fsmk, ssmk, m);
}

