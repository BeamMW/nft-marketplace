#include "Shaders/common.h"
#include "Shaders/Math.h"
#include "contract.h"

#include <algorithm>

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

BEAM_EXPORT void Ctor(const Gallery::Method::Init& r)
{
    if (Env::get_CallDepth() > 1)
    {
        MyState s(false);
        s.artists_stats.total = 0;
        s.artworks_stats.total = 0;
        s.artworks_stats.free_id = 0;
        s.collections_stats.total = 0;
        s.collections_stats.free_id = 0;

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

    struct ArtistPlus : public Gallery::Artist {
        char m_szLabelData[s_TotalMaxLen];
    } a;

    switch (r.req) {
    case ArtistReqType::SET: {
        bool artist_exists = a.Exists(r.m_pkArtist);

        if (artist_exists) {
            a.TakeOut(r.m_pkArtist, sizeof(a));

            uint32_t old_data_len = a.data_len;
            uint32_t old_label_len = a.label_len;
            uint8_t old_data[old_data_len];
            if (r.m_LabelLen) {
                Env::Memcpy(old_data, a.m_szLabelData + old_label_len, old_data_len);
                a.label_len = r.m_LabelLen;
                Env::Memcpy(a.m_szLabelData, &r + 1, r.m_LabelLen);
            }

            if (r.m_DataLen) {
                a.data_len = r.m_DataLen;
                Env::Memcpy(a.m_szLabelData + a.label_len, reinterpret_cast<const uint8_t *>(&r + 1) + r.m_LabelLen, a.data_len);
            } else {
                Env::Memcpy(a.m_szLabelData + a.label_len, old_data, old_data_len);
            }
        } else {
            // if artist doesn't exist, then label is required
            Env::Halt_if(!r.m_LabelLen); 

            // will be uncommented in future (with moderation adding)
            //a.is_approved = false;
            a.is_approved = true;

            a.m_hRegistered = Env::get_Height();
            a.label_len = r.m_LabelLen;
            a.data_len = r.m_DataLen;
            Env::Memcpy(a.m_szLabelData, &r + 1, r.m_LabelLen + r.m_DataLen);

            // create default collection
            struct CollectionPlus : public Gallery::Collection {
                char m_szLabelData[s_TotalMaxLen];
            } c;

            MyState s;
            c.id = ++s.collections_stats.free_id;
            s.collections_stats.total++;
            s.artists_stats.total++;
            s.Save();

            c.is_default = true;

            // will be uncommented in future (with moderation adding)
            //c.is_approved = false;
            c.is_approved = true;

            c.label_len = a.label_len;
            c.data_len = 0;
            _POD_(c.m_pkAuthor) = r.m_pkArtist;
            Env::Memcpy(c.m_szLabelData, &r + 1, r.m_LabelLen);

            Gallery::ArtistCollectionKey ack;
            _POD_(ack.pkArtist) = c.m_pkAuthor;
            ack.collection_id = c.id;
            Env::SaveVar_T(ack, true);

            c.Save(c.id, Utils::FromBE(Env::get_Height()), sizeof(Gallery::Collection) + c.label_len + c.data_len);
        }

        a.Save(r.m_pkArtist, Utils::FromBE(Env::get_Height()), sizeof(Gallery::Artist) + a.label_len + a.data_len);

        Env::AddSig(r.m_pkArtist);
        break;
    }
    case ArtistReqType::ENABLE:
    case ArtistReqType::DISABLE: {
                                     /*
        Env::Halt_if(!Env::LoadVar_T(fsak, ssak));
        uint32_t artist_size = Env::LoadVar(&ssak, sizeof(ssak), &a, sizeof(a), KeyTag::Internal);
        Env::Halt_if(!artist_size);
        Env::DelVar_T(ssak);

        a.is_approved = (r.req == ArtistReqType::ENABLE);
        ssak.h_updated = Utils::FromBE(Env::get_Height());

        Env::SaveVar_T(fsak, ssak);
        Env::SaveVar(&ssak, sizeof(ssak), &a, artist_size, KeyTag::Internal); // will fail if already exists
        */
        MyState s;
        s.AddSigAdmin();
        break;
    }
    default:
        Env::Halt();
    }
}

BEAM_EXPORT void Method_15(const Gallery::Method::ManageCollection& r)
{
    using CollectionReqType = Gallery::Method::ManageCollection::RequestType;

    struct CollectionPlus : public Gallery::Collection {
        char m_szLabelData[s_TotalMaxLen];
    } c;

    switch (r.req) {
    case CollectionReqType::SET: {
        bool collection_exists = r.collection_id > 0 && c.Exists(r.collection_id);

        if (collection_exists) {
            c.TakeOut(r.collection_id);

            uint32_t old_data_len = c.data_len;
            uint32_t old_label_len = c.label_len;
            uint8_t old_data[old_data_len];
            if (r.m_LabelLen) {
                Env::Memcpy(old_data, c.m_szLabelData + old_label_len, old_data_len);
                c.label_len = r.m_LabelLen;
                Env::Memcpy(c.m_szLabelData, &r + 1, r.m_LabelLen);
            }

            if (r.m_DataLen) {
                c.data_len = r.m_DataLen;
                Env::Memcpy(c.m_szLabelData + c.label_len, reinterpret_cast<const uint8_t *>(&r + 1) + r.m_LabelLen, c.data_len);
            } else {
                Env::Memcpy(c.m_szLabelData + c.label_len, old_data, old_data_len);
            }
        } else {
            // if collection doesn't exist, then label is required
            Env::Halt_if(!r.m_LabelLen); 

            MyState s;
            c.id = ++s.collections_stats.free_id;
            s.collections_stats.total++;
            s.Save();

            // will be uncommented in future (with moderation adding)
            //c.is_approved = false;
            c.is_approved = true;

            c.is_default = false;

            c.label_len = r.m_LabelLen;
            c.data_len = r.m_DataLen;
            _POD_(c.m_pkAuthor) = r.m_pkArtist;
            Env::Memcpy(c.m_szLabelData, &r + 1, r.m_LabelLen + r.m_DataLen);

            Gallery::ArtistCollectionKey ack;
            _POD_(ack.pkArtist) = c.m_pkAuthor;
            ack.collection_id = c.id;
            Env::SaveVar_T(ack, true);
        }
        c.Save(c.id, Utils::FromBE(Env::get_Height()), sizeof(Gallery::Collection) + c.label_len + c.data_len);

        Env::AddSig(r.m_pkArtist);
        break;
    }
    case CollectionReqType::ENABLE:
    case CollectionReqType::DISABLE: {
                                     /*
        Env::Halt_if(!Env::LoadVar_T(fsak, ssak));
        uint32_t artist_size = Env::LoadVar(&ssak, sizeof(ssak), &a, sizeof(a), KeyTag::Internal);
        Env::Halt_if(!artist_size);
        Env::DelVar_T(ssak);

        a.is_approved = (r.req == ArtistReqType::ENABLE);
        ssak.h_updated = Utils::FromBE(Env::get_Height());

        Env::SaveVar_T(fsak, ssak);
        Env::SaveVar(&ssak, sizeof(ssak), &a, artist_size, KeyTag::Internal); // will fail if already exists
        MyState s;
        s.AddSigAdmin();
        */
        break;
    }
    default:
        Env::Halt();
    }
}

BEAM_EXPORT void Method_3(const Gallery::Method::AddExhibit& r)
{
    MyState s;
    Gallery::Masterpiece m;

    Gallery::Masterpiece::Id m_id = Utils::FromBE(++s.artworks_stats.free_id);
    s.artworks_stats.total++;
    
    s.Save();

    _POD_(m.m_pkOwner) = r.m_pkArtist;
    _POD_(m.m_pkAuthor) = r.m_pkArtist;
    m.collection_id = r.collection_id;

    Gallery::ArtistCollectionKey ack;
    ack.collection_id = r.collection_id;
    _POD_(ack.pkArtist) = r.m_pkArtist;
    bool exists;
    Env::Halt_if(!Env::LoadVar_T(ack, exists));
    
    Gallery::CollectionArtworkKey cak;
    cak.collection_id = r.collection_id;
    cak.artwork_id = m_id;
    Env::SaveVar_T(cak, true);

    auto pData = reinterpret_cast<const uint8_t*>(&r + 1) + r.label_len;
    uint32_t nData = r.data_len;

    auto pLabel = reinterpret_cast<const uint8_t*>(&r + 1);
    uint32_t nLabel = r.label_len;

    {
        // verify artist
        struct ArtistPlus : public Gallery::Artist {
            char m_szLabelData[s_TotalMaxLen];
        } a;

        Env::Halt_if(!a.Load(r.m_pkArtist));
        Env::Halt_if(!a.is_approved);
    }

    m.Save(m_id);

    Env::AddSig(r.m_pkArtist);

    Gallery::Events::AddArtworkData::Key adk;
    adk.m_ID = m_id;
    _POD_(adk.m_pkArtist) = m.m_pkOwner;

    uint32_t nMaxEventSize = 0x2000; // TODO: max event size is increased to 1MB from HF4

    while (true)
    {
        Env::EmitLog(&adk, sizeof(adk), pData, std::min(nData, nMaxEventSize), KeyTag::Internal);
        if (nData <= nMaxEventSize)
            break;

        nData -= nMaxEventSize;
        pData += nMaxEventSize;
    }

    Gallery::Events::AddArtworkLabel::Key alk;
    alk.m_ID = m_id;
    _POD_(alk.m_pkArtist) = m.m_pkOwner;

    Env::EmitLog(&alk, sizeof(alk), pLabel, nLabel, KeyTag::Internal);
}

BEAM_EXPORT void Method_4(const Gallery::Method::SetPrice& r)
{
    Gallery::Masterpiece m;
    Env::Halt_if(!m.Load(r.m_ID));

    _POD_(m.m_Price) = r.m_Price;

    m.Save(r.m_ID);

    Env::AddSig(m.m_pkOwner); // would fail if no current owner (i.e. checked out)
}

BEAM_EXPORT void Method_5(const Gallery::Method::Buy& r)
{
    Gallery::Masterpiece m;
    Env::Halt_if(!m.Load(r.m_ID));

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

    m.Save(r.m_ID);
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
    Gallery::Masterpiece m;
    Env::Halt_if(!m.Load(r.m_ID));
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

    m.Save(r.m_ID);
}

BEAM_EXPORT void Method_8(const Gallery::Method::CheckOut& r)
{
    Gallery::Masterpiece m;
    Env::Halt_if(!m.Load(r.m_ID) || !m.m_Aid);
    Env::AddSig(m.m_pkOwner);

    Env::Halt_if(!Env::AssetEmit(m.m_Aid, 1, 1));
    Env::FundsUnlock(m.m_Aid, 1);

    _POD_(m.m_pkOwner).SetZero();
    _POD_(m.m_Price).SetZero();

    m.Save(r.m_ID);
}

BEAM_EXPORT void Method_9(const Gallery::Method::CheckIn& r)
{
    Gallery::Masterpiece m;
    Env::Halt_if(!m.Load(r.m_ID) || !_POD_(m.m_pkOwner).IsZero());

    Env::FundsLock(m.m_Aid, 1);
    Env::Halt_if(!Env::AssetEmit(m.m_Aid, 1, 0));

    _POD_(m.m_pkOwner) = r.m_pkUser;
    m.Save(r.m_ID);

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

        Env::Halt_if(Utils::FromBE(impk.m_ID.m_MasterpieceID) > s.artworks_stats.free_id);

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
    Gallery::Masterpiece m;
    Env::Halt_if(!m.Load(r.m_ID));

    Env::Halt_if(m.m_Aid);

    m.Delete(r.m_ID);

    MyState s;
    s.AddSigAdmin();
}

BEAM_EXPORT void Method_14(const Gallery::Method::Transfer& r)
{
    Gallery::Masterpiece m;
    Env::Halt_if(!m.Load(r.m_ID));

    Env::AddSig(m.m_pkOwner);
    _POD_(m.m_pkOwner) = r.m_pkNewOwner;

    m.Save(r.m_ID);
}
