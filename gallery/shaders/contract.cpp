#include "Shaders/common.h"
#include "Shaders/Math.h"
#include "contract.h"

#include <algorithm>

struct MyState : public Gallery::State {
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

BEAM_EXPORT void Ctor(const Gallery::Method::Init& r) {
    if (Env::get_CallDepth() > 1) {
        MyState s(false);
        s.total_artworks = 0;
        s.total_artists = 0;
        s.total_collections = 0;
        s.total_moderators = 0;
        s.fee_base = 0;

        s.m_VoteBalance = 0;
        _POD_(s.m_Config) = r.m_Config;

        s.Save();
    }
}

BEAM_EXPORT void Dtor(void*) {
    // ignore
}

void PayoutMove(const Gallery::Payout::Key& key, Amount val, bool bAdd) {
    if (!val) return;

    Gallery::Payout po;
    if (Env::LoadVar_T(key, po)) {
        if (bAdd) {
            Strict::Add(po.m_Amount, val);
        } else {
            Strict::Sub(po.m_Amount, val);

            if (!po.m_Amount) {
                Env::DelVar_T(key);
                return;
            }
        }
    } else {
        Env::Halt_if(!bAdd);
        po.m_Amount = val;
    }

    Env::SaveVar_T(key, po);
}

BEAM_EXPORT void Method_2(void*) {
    // called on upgrade
}

BEAM_EXPORT void Method_3(const Gallery::Method::SetFeeBase& r) {
    MyState s;
    s.fee_base = r.amount;
    s.Save();
    s.AddSigAdmin();
}

BEAM_EXPORT void Method_4(const Gallery::Method::SetModerator& r) {
    Gallery::Moderator m;
    MyState s;
    Height cur_height = Env::get_Height();

    if (!m.Load(r.id)) {
        m.registered = cur_height;
        m.updated = cur_height;
        s.total_moderators++;
    }

    m.approved = r.approved;

    Gallery::Index<Gallery::Tag::kHeightModeratorIdx, Height, Gallery::Moderator>
        ::Update(m.updated, cur_height, r.id);

    m.updated = cur_height;
    m.Save(r.id);
    s.Save();
    s.AddSigAdmin();
}

BEAM_EXPORT void Method_6(const Gallery::Method::SetArtistStatus& r) {
    Height cur_height = Env::get_Height();

    struct ArtistPlus : public Gallery::Artist {
        char m_szLabelData[s_TotalMaxLen];
    } a;

    Gallery::Moderator m;
    MyState s;
    if (!m.Load(r.signer) || !m.approved)
        s.AddSigAdmin();
    else
        Env::AddSig(r.signer);

    for (int i = 0; i < r.ids_num; ++i) {
        Env::Halt_if(!a.Load(r.ids[i], sizeof(a)));
        a.status = r.status;

        Gallery::Index<Gallery::Tag::kHeightArtistIdx, Height, Gallery::Artist>
            ::Update(a.updated, cur_height, r.ids[i]);

        a.updated = cur_height;
        a.Save(r.ids[i], sizeof(Gallery::Artist) + a.label_len + a.data_len);
    }
}

BEAM_EXPORT void Method_5(const Gallery::Method::SetArtist& r) {
    Height cur_height = Env::get_Height();

    struct ArtistPlus : public Gallery::Artist {
        char m_szLabelData[s_TotalMaxLen];
    } a;

    if (!a.Load(r.m_pkArtist, sizeof(a))) {
        MyState s;
        // if artist doesn't exist, then label is required
        Env::Halt_if(!r.m_LabelLen); 
        a.m_hRegistered = cur_height;
        a.updated = cur_height;
        a.collections_num = 0;
        a.artworks_num = 0;
        s.total_artists++;
        s.Save();
    }

    Env::Memcpy(a.m_szLabelData, &r + 1, r.m_LabelLen + r.m_DataLen);
    a.label_len = r.m_LabelLen;
    a.data_len = r.m_DataLen;
    a.status = Gallery::Status::kPending;

    Gallery::Index<Gallery::Tag::kHeightArtistIdx, Height, Gallery::Artist>
        ::Update(a.updated, cur_height, r.m_pkArtist);

    a.updated = cur_height;
    a.Save(r.m_pkArtist, sizeof(Gallery::Artist) + a.label_len + a.data_len);
    Env::AddSig(r.m_pkArtist);
}

BEAM_EXPORT void Method_7(const Gallery::Method::SetCollectionStatus& r) {
    Height cur_height = Env::get_Height();

    struct CollectionPlus : public Gallery::Collection {
        char m_szLabelData[s_TotalMaxLen];
    } c;

    Gallery::Moderator m;
    MyState s;
    if (!m.Load(r.signer) || !m.approved)
        s.AddSigAdmin();
    else
        Env::AddSig(r.signer);

    for (int i = 0; i < r.ids_num; ++i) {
        Env::Halt_if(!c.Load(r.ids[i], sizeof(c)));

        c.status = r.status;

        Gallery::Index<Gallery::Tag::kHeightCollectionIdx, Height, Gallery::Collection>
            ::Update(c.updated, cur_height, r.ids[i]);

        c.updated = cur_height;
        c.Save(r.ids[i], sizeof(Gallery::Collection) + c.label_len + c.data_len);
    }
}

BEAM_EXPORT void Method_8(const Gallery::Method::SetCollection& r) {
    Height cur_height = Env::get_Height();

    struct CollectionPlus : public Gallery::Collection {
        char m_szLabelData[s_TotalMaxLen];
    } c;

    Gallery::Collection::Id c_id = r.collection_id;

    if (!c.Load(c_id, sizeof(c))) {
        // if collection doesn't exist, then label is required
        Env::Halt_if(!r.m_LabelLen); 

        MyState s;
        c_id = ++s.total_collections;
        c.m_pkAuthor = r.m_pkArtist;
        c.updated = cur_height;
        c.total_sold = 0;
        c.total_sold_price = 0;
        c.max_sold.artwork_id = 0;
        c.max_sold.price.m_Amount = 0;
        c.max_sold.price.m_Aid = 0;
        c.min_sold.artwork_id = 0;
        c.min_sold.price.m_Amount = 0;
        c.min_sold.price.m_Aid = 0;

        Gallery::Index<Gallery::Tag::kArtistCollectionIdx,
            Gallery::Artist::Id, Gallery::Collection>
                ::Save(c.m_pkAuthor, c_id);

        struct ArtistPlus : public Gallery::Artist {
            char m_szLabelData[s_TotalMaxLen];
        } a;

        a.Load(r.m_pkArtist, sizeof(a));
        ++a.collections_num;

        Gallery::Index<Gallery::Tag::kHeightArtistIdx, Height, Gallery::Artist>
            ::Update(a.updated, cur_height, r.m_pkArtist);

        a.updated = cur_height;
        a.Save(r.m_pkArtist, sizeof(Gallery::Artist) + a.label_len + a.data_len);
        s.Save();
    }

    Env::Memcpy(c.m_szLabelData, &r + 1, r.m_LabelLen + r.m_DataLen);
    c.label_len = r.m_LabelLen;
    c.data_len = r.m_DataLen;
    c.status = Gallery::Status::kPending;

    Gallery::Index<Gallery::Tag::kHeightCollectionIdx, Height, Gallery::Collection>
        ::Update(c.updated, cur_height, c_id);

    c.updated = cur_height;
    c.Save(c_id, sizeof(Gallery::Collection) + c.label_len + c.data_len);
    Env::AddSig(r.m_pkArtist);
}

BEAM_EXPORT void Method_9(const Gallery::Method::SetArtwork& r) {
    Height cur_height = Env::get_Height();

    Gallery::Artwork m;

    MyState s;
    m.id = ++s.total_artworks;
    m.m_pkOwner = r.m_pkArtist;
    m.m_pkAuthor = r.m_pkArtist;
    m.collection_id = r.collection_id;
    m.status = Gallery::Status::kPending;
    _POD_(m.m_Price) = r.m_Price;

    // assert: artist owns collection
    Env::Halt_if(!Gallery::Index<Gallery::Tag::kArtistCollectionIdx,
        Gallery::Artist::Id, Gallery::Collection>
            ::Load(r.m_pkArtist, r.collection_id));

    Gallery::Index<Gallery::Tag::kCollectionArtworkIdx,
        Gallery::Collection::Id, Gallery::Artwork>
            ::Save(r.collection_id, m.id);
        
    struct CollectionPlus : public Gallery::Collection {
        char m_szLabelData[s_TotalMaxLen];
    } c;

    // assert: collection exists
    Env::Halt_if(!c.Load(r.collection_id, sizeof(c)));
    c.artworks_num++;
    Env::Halt_if(c.artworks_num > c.s_MaxArtworks);

    Gallery::Index<Gallery::Tag::kHeightCollectionIdx, Height, Gallery::Collection>
        ::Update(c.updated, cur_height, r.collection_id);
    c.updated = cur_height;
    c.Save(r.collection_id, sizeof(Gallery::Collection) + c.label_len + c.data_len);

    // verify artist
    struct ArtistPlus : public Gallery::Artist {
        char m_szLabelData[s_TotalMaxLen];
    } a;

    // assert: artist exists
    Env::Halt_if(!a.Load(r.m_pkArtist, sizeof(a)));
    ++a.artworks_num;

    Gallery::Index<Gallery::Tag::kHeightArtistIdx, Height, Gallery::Artist>
        ::Update(a.updated, cur_height, r.m_pkArtist);

    a.updated = cur_height;
    a.Save(r.m_pkArtist, sizeof(Gallery::Artist) + a.label_len + a.data_len);

    auto pData = reinterpret_cast<const uint8_t*>(&r + 1) + r.label_len;
    uint32_t nData = r.data_len;

    auto pLabel = reinterpret_cast<const uint8_t*>(&r + 1);
    uint32_t nLabel = r.label_len;

    Gallery::Events::AddArtworkData::Key adk;
    adk.m_ID = m.id;
    adk.m_pkArtist = m.m_pkOwner;

    uint32_t nMaxEventSize = 0x100000;

    while (true) {
        Env::EmitLog(&adk, sizeof(adk), pData, std::min(nData, nMaxEventSize), KeyTag::Internal);
        if (nData <= nMaxEventSize)
            break;

        nData -= nMaxEventSize;
        pData += nMaxEventSize;
    }

    Gallery::Events::AddArtworkLabel::Key alk;
    alk.m_ID = m.id;
    alk.m_pkArtist = m.m_pkOwner;
    Env::EmitLog(&alk, sizeof(alk), pLabel, nLabel, KeyTag::Internal);

    Gallery::Index<Gallery::Tag::kHeightArtworkIdx, Height, Gallery::Artwork>
        ::Update(m.updated, cur_height, m.id);
    m.updated = cur_height;
    m.Save(m.id);
    s.Save();
    Env::AddSig(r.m_pkArtist);
}

BEAM_EXPORT void Method_10(const Gallery::Method::SetArtworkStatus& r) {
    Height cur_height = Env::get_Height();

    Gallery::Moderator m;
    MyState s;
    if (!m.Load(r.signer) || !m.approved)
        s.AddSigAdmin();
    else
        Env::AddSig(r.signer);

    Gallery::Artwork a;
    for (int i = 0; i < r.ids_num; ++i) {
        Env::Halt_if(!a.Load(r.ids[i]));
        a.status = r.status;
        Gallery::Index<Gallery::Tag::kHeightArtworkIdx, Height, Gallery::Artwork>
            ::Update(a.updated, cur_height, a.id);
        a.updated = cur_height;
        a.Save(a.id);
    }
}

BEAM_EXPORT void Method_11(const Gallery::Method::SetPrice& r) {
    Gallery::Artwork m;
    Env::Halt_if(!m.Load(r.m_ID));

    _POD_(m.m_Price) = r.m_Price;

    Height cur_height = Env::get_Height();
    Gallery::Index<Gallery::Tag::kHeightArtworkIdx, Height, Gallery::Artwork>
        ::Update(m.updated, cur_height, r.m_ID);
    m.updated = cur_height;
    m.Save(r.m_ID);

    Env::AddSig(m.m_pkOwner); // would fail if no current owner (i.e. checked out)
}

BEAM_EXPORT void Method_12(const Gallery::Method::Buy& r) {
    Height cur_height = Env::get_Height();

    Gallery::Artwork m;
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

    Gallery::Index<Gallery::Tag::kHeightArtworkIdx, Height, Gallery::Artwork>
        ::Update(m.updated, cur_height, r.m_ID);
    m.updated = cur_height;
    m.Save(r.m_ID);

    struct CollectionPlus : public Gallery::Collection {
        char m_szLabelData[s_TotalMaxLen];
    } c;

    c.Load(m.collection_id, sizeof(c));
    ++c.total_sold;
    c.total_sold_price += m.m_Price.m_Amount;
    if (m.m_Price.m_Amount > c.max_sold.price.m_Amount) {
        c.max_sold.price = m.m_Price;
        c.max_sold.artwork_id = r.m_ID;
    }
    if (!c.min_sold.artwork_id || m.m_Price.m_Amount < c.min_sold.price.m_Amount) {
        c.min_sold.price = m.m_Price;
        c.min_sold.artwork_id = r.m_ID;
    }
    Gallery::Index<Gallery::Tag::kHeightCollectionIdx, Height, Gallery::Collection>
        ::Update(c.updated, cur_height, m.collection_id);
    c.updated = cur_height;
    c.Save(m.collection_id, sizeof(Gallery::Collection) + c.label_len + c.data_len);
    //Env::AddSig(r.m_pkUser);
}

BEAM_EXPORT void Method_13(const Gallery::Method::Withdraw& r) {
    PayoutMove(r.m_Key, r.m_Value, false);
    Env::FundsUnlock(r.m_Key.m_Aid, r.m_Value);
    Env::AddSig(r.m_Key.m_pkUser);
}

BEAM_EXPORT void Method_14(const Gallery::Method::CheckPrepare& r) {
    Gallery::Artwork m;
    Env::Halt_if(!m.Load(r.m_ID));
    Env::AddSig(m.m_pkOwner);

    if (m.m_Aid) {
        // destroy it
        Env::Halt_if(!Env::AssetDestroy(m.m_Aid));
        m.m_Aid = 0;
    } else {
        // 1st call. Don't checkout, only prepare
        static const char szMeta[] = "STD:SCH_VER=1;N=Gallery Artwork;SN=Gall;UN=GALL;NTHUN=unique";
        m.m_Aid = Env::AssetCreate(szMeta, sizeof(szMeta) - 1);
    }

    Height cur_height = Env::get_Height();

    Gallery::Index<Gallery::Tag::kHeightArtworkIdx, Height, Gallery::Artwork>
        ::Update(m.updated, cur_height, r.m_ID);

    m.updated = cur_height;
    m.Save(r.m_ID);
}

BEAM_EXPORT void Method_15(const Gallery::Method::CheckOut& r) {
    Gallery::Artwork m;
    Env::Halt_if(!m.Load(r.m_ID) || !m.m_Aid);
    Env::AddSig(m.m_pkOwner);

    Env::Halt_if(!Env::AssetEmit(m.m_Aid, 1, 1));
    Env::FundsUnlock(m.m_Aid, 1);

    _POD_(m.m_pkOwner).SetZero();
    _POD_(m.m_Price).SetZero();

    Height cur_height = Env::get_Height();

    Gallery::Index<Gallery::Tag::kHeightArtworkIdx, Height, Gallery::Artwork>
        ::Update(m.updated, cur_height, r.m_ID);

    m.updated = cur_height;
    m.Save(r.m_ID);
}

BEAM_EXPORT void Method_16(const Gallery::Method::CheckIn& r) {
    Gallery::Artwork m;
    Env::Halt_if(!m.Load(r.m_ID) || !_POD_(m.m_pkOwner).IsZero());

    Env::FundsLock(m.m_Aid, 1);
    Env::Halt_if(!Env::AssetEmit(m.m_Aid, 1, 0));

    _POD_(m.m_pkOwner) = r.m_pkUser;

    Height cur_height = Env::get_Height();

    Gallery::Index<Gallery::Tag::kHeightArtworkIdx, Height, Gallery::Artwork>
        ::Update(m.updated, cur_height, r.m_ID);

    m.updated = cur_height;
    m.Save(r.m_ID);

    //Env::AddSig(r.m_pkUser);
}

BEAM_EXPORT void Method_17(const Gallery::Method::Vote& r) {
    Gallery::Artwork m;

    Env::Halt_if(!m.Load(r.m_ID.m_ArtworkID));
    //Env::Halt_if(m.status != Gallery::Status::kApproved);
    
    Gallery::Impression::Key impk;
    _POD_(impk.m_ID) = r.m_ID;

    Gallery::Impression imp;
    if (!Env::LoadVar_T(impk, imp)) {
        imp.m_Value = 0;

        MyState s;
        Strict::Sub(s.m_VoteBalance, s.m_Config.m_VoteReward.m_Amount);
        s.Save();

        Env::FundsUnlock(s.m_Config.m_VoteReward.m_Aid, s.m_Config.m_VoteReward.m_Amount);
    }

    Env::Halt_if(imp.m_Value == r.m_Impression.m_Value); // not changed

    imp.m_Value = r.m_Impression.m_Value;
    Env::SaveVar_T(impk, imp);

    Env::AddSig(impk.m_ID.m_pkUser);
}

BEAM_EXPORT void Method_18(const Gallery::Method::AddVoteRewards& r) {
    MyState s;
    Strict::Add(s.m_VoteBalance, r.m_Amount);
    s.Save();

    Env::FundsLock(s.m_Config.m_VoteReward.m_Aid, r.m_Amount);
}

BEAM_EXPORT void Method_19(const Gallery::Method::Transfer& r) {
    Gallery::Artwork m;
    Env::Halt_if(!m.Load(r.m_ID));

    Env::AddSig(m.m_pkOwner);
    _POD_(m.m_pkOwner) = r.m_pkNewOwner;

    Height cur_height = Env::get_Height();

    Gallery::Index<Gallery::Tag::kHeightArtworkIdx, Height, Gallery::Artwork>
        ::Update(m.updated, cur_height, r.m_ID);

    m.updated = cur_height;
    m.Save(r.m_ID);
}
