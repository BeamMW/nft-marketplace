#include "Shaders/common.h"
#include "Shaders/Math.h"
#include "contract.h"

#include <algorithm>

struct MyState : public gallery::State {
    MyState() {
        Env::LoadVar_T((uint8_t) kKey, *this);
    }

    MyState(bool) {
        // no auto-load
    }

    void Save() {
        Env::SaveVar_T((uint8_t) kKey, *this);
    }

    void AddSigAdmin() {
        Env::AddSig(config.admin_id);
    }
};

BEAM_EXPORT void Ctor(const gallery::method::Init& r) {
    if (Env::get_CallDepth() > 1) {
        MyState s(false);
        s.total_nfts = 0;
        s.total_artists = 0;
        s.total_collections = 0;
        s.total_moderators = 0;
        s.fee_base = 0;

        s.vote_balance = 0;
        _POD_(s.config) = r.config;

        s.Save();
    }
}

BEAM_EXPORT void Dtor(void*) {
    // ignore
}

void PayoutMove(const gallery::Payout::Key& key, Amount val, bool bAdd) {
    if (!val) return;

    gallery::Payout po;
    if (Env::LoadVar_T(key, po)) {
        if (bAdd) {
            Strict::Add(po.amount, val);
        } else {
            Strict::Sub(po.amount, val);

            if (!po.amount) {
                Env::DelVar_T(key);
                return;
            }
        }
    } else {
        Env::Halt_if(!bAdd);
        po.amount = val;
    }

    Env::SaveVar_T(key, po);
}

BEAM_EXPORT void Method_2(void*) {
    // called on upgrade
}

BEAM_EXPORT void Method_3(const gallery::method::SetFeeBase& r) {
    MyState s;
    s.fee_base = r.amount;
    s.Save();
    s.AddSigAdmin();
}

BEAM_EXPORT void Method_4(const gallery::method::SetModerator& r) {
    Height cur_height = Env::get_Height();

    MyState s;
    gallery::Moderator m;
    if (!m.Load(r.id)) {
        m.registered = cur_height;
        m.updated = cur_height;
        s.total_moderators++;
    }

    gallery::Index<gallery::Tag::kHeightModeratorIdx, Height, gallery::Moderator>
        ::Update(m.updated, cur_height, r.id);

    m.approved = r.approved;
    m.updated = cur_height;
    m.Save(r.id);
    s.Save();
    s.AddSigAdmin();
}

BEAM_EXPORT void Method_6(const gallery::method::SetArtistStatus& r) {
    Height cur_height = Env::get_Height();

    struct ArtistPlus : public gallery::Artist {
        char m_szLabelData[kTotalMaxLen];
    } a;

    gallery::Moderator m;
    MyState s;
    if (!m.Load(r.signer) || !m.approved)
        s.AddSigAdmin();
    else
        Env::AddSig(r.signer);

    for (int i = 0; i < r.ids_num; ++i) {
        Env::Halt_if(!a.Load(r.ids[i], sizeof(a)));
        a.status = r.status;

        gallery::Index<gallery::Tag::kHeightArtistIdx, Height, gallery::Artist>
            ::Update(a.updated, cur_height, r.ids[i]);

        a.updated = cur_height;
        a.Save(r.ids[i], sizeof(gallery::Artist) + a.data_len);
    }
}

BEAM_EXPORT void Method_5(const gallery::method::SetArtist& r) {
    Height cur_height = Env::get_Height();

    struct ArtistPlus : public gallery::Artist {
        char data[kDataMaxLen];
    } a;

    if (!a.Load(r.aritst_id, sizeof(a))) {
        MyState s;
        // if artist doesn't exist, then label is required
        Env::Halt_if(!r.label_len); 
        a.registered = cur_height;
        a.updated = cur_height;
        a.collections_num = 0;
        a.nfts_num = 0;
        s.total_artists++;
        s.Save();

        gallery::Events::AddArtistLabel::Key alk;
        alk.id = r.aritst_id;
        auto label_ptr = reinterpret_cast<const char*>(&r + 1);
        Env::EmitLog(&alk, sizeof(alk), label_ptr, r.label_len, KeyTag::Internal);
        gallery::Artist::LabelKey lk;
        lk.label_hash = gallery::GetLabelHash(std::string_view(label_ptr, r.label_len));
        Env::SaveVar_T(lk, r.aritst_id);
    }

    auto data_ptr = reinterpret_cast<const uint8_t*>(&r + 1) + r.label_len;
    Env::Memcpy(a.data, data_ptr, r.data_len);
    a.data_len = r.data_len;
    a.status = gallery::Status::kPending;

    gallery::Index<gallery::Tag::kHeightArtistIdx, Height, gallery::Artist>
        ::Update(a.updated, cur_height, r.aritst_id);

    a.updated = cur_height;
    a.Save(r.aritst_id, sizeof(gallery::Artist) + a.data_len);
    Env::AddSig(r.aritst_id);
}

BEAM_EXPORT void Method_7(const gallery::method::SetCollectionStatus& r) {
    Height cur_height = Env::get_Height();

    struct CollectionPlus : public gallery::Collection {
        char m_szLabelData[kTotalMaxLen];
    } c;

    gallery::Moderator m;
    MyState s;
    if (!m.Load(r.signer) || !m.approved)
        s.AddSigAdmin();
    else
        Env::AddSig(r.signer);

    for (int i = 0; i < r.ids_num; ++i) {
        Env::Halt_if(!c.Load(r.ids[i], sizeof(c)));

        c.status = r.status;

        gallery::Index<gallery::Tag::kHeightCollectionIdx, Height, gallery::Collection>
            ::Update(c.updated, cur_height, r.ids[i]);

        c.updated = cur_height;
        c.Save(r.ids[i], sizeof(gallery::Collection) + c.label_len + c.data_len);
    }
}

BEAM_EXPORT void Method_8(const gallery::method::SetCollection& r) {
    Height cur_height = Env::get_Height();

    struct CollectionPlus : public gallery::Collection {
        char m_szLabelData[kTotalMaxLen];
    } c;

    gallery::Collection::Id c_id = r.collection_id;

    if (!c.Load(c_id, sizeof(c))) {
        // if collection doesn't exist, then label is required
        Env::Halt_if(!r.label_len); 

        MyState s;
        c_id = ++s.total_collections;
        c.author = r.aritst_id;
        c.updated = cur_height;
        c.total_sold = 0;
        c.total_sold_price = 0;
        c.max_sold.nft_id = 0;
        c.max_sold.price.amount = 0;
        c.max_sold.price.aid = 0;
        c.min_sold.nft_id = 0;
        c.min_sold.price.amount = 0;
        c.min_sold.price.aid = 0;

        gallery::Index<gallery::Tag::kArtistCollectionIdx,
            gallery::Artist::Id, gallery::Collection>
                ::Save(c.author, c_id);

        struct ArtistPlus : public gallery::Artist {
            char m_szLabelData[kTotalMaxLen];
        } a;

        a.Load(r.aritst_id, sizeof(a));
        ++a.collections_num;

        gallery::Index<gallery::Tag::kHeightArtistIdx, Height, gallery::Artist>
            ::Update(a.updated, cur_height, r.aritst_id);

        a.updated = cur_height;
        a.Save(r.aritst_id, sizeof(gallery::Artist) + a.data_len);
        s.Save();
    }

    gallery::Collection::LabelKey lk;
    lk.artist_id = r.aritst_id;
    lk.label_hash = gallery::GetLabelHash(std::string_view(c.m_szLabelData, c.label_len));
    Env::DelVar_T(lk);

    auto label_ptr = reinterpret_cast<const char*>(&r + 1);
    lk.label_hash = gallery::GetLabelHash(std::string_view(label_ptr, r.label_len));
    Env::SaveVar_T(lk, c_id);

    Env::Memcpy(c.m_szLabelData, &r + 1, r.label_len + r.data_len);
    c.label_len = r.label_len;
    c.data_len = r.data_len;
    c.status = gallery::Status::kPending;

    gallery::Index<gallery::Tag::kHeightCollectionIdx, Height, gallery::Collection>
        ::Update(c.updated, cur_height, c_id);

    c.updated = cur_height;
    c.Save(c_id, sizeof(gallery::Collection) + c.label_len + c.data_len);
    Env::AddSig(r.aritst_id);
}

BEAM_EXPORT void Method_9(const gallery::method::SetNft& r) {
    Height cur_height = Env::get_Height();

    gallery::Nft m;

    MyState s;
    m.id = ++s.total_nfts;
    m.owner = r.aritst_id;
    m.author = r.aritst_id;
    m.collection_id = r.collection_id;
    m.status = gallery::Status::kPending;
    _POD_(m.price) = r.price;

    // assert: artist owns collection
    Env::Halt_if(!gallery::Index<gallery::Tag::kArtistCollectionIdx,
        gallery::Artist::Id, gallery::Collection>
            ::Load(r.aritst_id, r.collection_id));

    gallery::Index<gallery::Tag::kCollectionNftIdx,
        gallery::Collection::Id, gallery::Nft>
            ::Save(r.collection_id, m.id);
        
    struct CollectionPlus : public gallery::Collection {
        char m_szLabelData[kTotalMaxLen];
    } c;

    // assert: collection exists
    Env::Halt_if(!c.Load(r.collection_id, sizeof(c)));
    c.nfts_num++;
    Env::Halt_if(c.nfts_num > c.kMaxNfts);

    gallery::Index<gallery::Tag::kHeightCollectionIdx, Height, gallery::Collection>
        ::Update(c.updated, cur_height, r.collection_id);
    c.updated = cur_height;
    c.Save(r.collection_id, sizeof(gallery::Collection) + c.label_len + c.data_len);

    // verify artist
    struct ArtistPlus : public gallery::Artist {
        char m_szLabelData[kTotalMaxLen];
    } a;

    // assert: artist exists
    Env::Halt_if(!a.Load(r.aritst_id, sizeof(a)));
    ++a.nfts_num;

    gallery::Index<gallery::Tag::kHeightArtistIdx, Height, gallery::Artist>
        ::Update(a.updated, cur_height, r.aritst_id);

    a.updated = cur_height;
    a.Save(r.aritst_id, sizeof(gallery::Artist) + a.data_len);

    auto pData = reinterpret_cast<const uint8_t*>(&r + 1) + r.label_len;
    uint32_t nData = r.data_len;

    auto pLabel = reinterpret_cast<const uint8_t*>(&r + 1);
    uint32_t nLabel = r.label_len;

    gallery::Events::AddNftData::Key adk;
    adk.nft_id = m.id;
    adk.aritst_id = m.owner;

    uint32_t nMaxEventSize = 0x100000;

    while (true) {
        Env::EmitLog(&adk, sizeof(adk), pData, std::min(nData, nMaxEventSize), KeyTag::Internal);
        if (nData <= nMaxEventSize)
            break;

        nData -= nMaxEventSize;
        pData += nMaxEventSize;
    }

    gallery::Events::AddNftLabel::Key alk;
    alk.nft_id = m.id;
    alk.aritst_id = m.owner;
    Env::EmitLog(&alk, sizeof(alk), pLabel, nLabel, KeyTag::Internal);

    gallery::Index<gallery::Tag::kHeightNftIdx, Height, gallery::Nft>
        ::Update(m.updated, cur_height, m.id);
    m.updated = cur_height;
    m.Save(m.id);
    s.Save();
    Env::AddSig(r.aritst_id);
}

BEAM_EXPORT void Method_10(const gallery::method::SetNftStatus& r) {
    Height cur_height = Env::get_Height();

    gallery::Moderator m;
    MyState s;
    if (!m.Load(r.signer) || !m.approved)
        s.AddSigAdmin();
    else
        Env::AddSig(r.signer);

    gallery::Nft a;
    for (int i = 0; i < r.ids_num; ++i) {
        Env::Halt_if(!a.Load(r.ids[i]));
        a.status = r.status;
        gallery::Index<gallery::Tag::kHeightNftIdx, Height, gallery::Nft>
            ::Update(a.updated, cur_height, a.id);
        a.updated = cur_height;
        a.Save(a.id);
    }
}

BEAM_EXPORT void Method_11(const gallery::method::SetPrice& r) {
    gallery::Nft m;
    Env::Halt_if(!m.Load(r.nft_id));

    _POD_(m.price) = r.price;

    Height cur_height = Env::get_Height();
    gallery::Index<gallery::Tag::kHeightNftIdx, Height, gallery::Nft>
        ::Update(m.updated, cur_height, r.nft_id);
    m.updated = cur_height;
    m.Save(r.nft_id);

    Env::AddSig(m.owner); // would fail if no current owner (i.e. checked out)
}

BEAM_EXPORT void Method_12(const gallery::method::Buy& r) {
    Height cur_height = Env::get_Height();

    gallery::Nft m;
    Env::Halt_if(!m.Load(r.nft_id));

    Env::Halt_if(
        !m.price.amount || // not for sale!
        (r.pay_max < m.price.amount) || // too expensive
        (r.has_aid != (!!m.aid))
    );

    struct CollectionPlus : public gallery::Collection {
        char m_szLabelData[kTotalMaxLen];
    } c;

    c.Load(m.collection_id, sizeof(c));
    ++c.total_sold;
    c.total_sold_price += m.price.amount;
    if (m.price.amount > c.max_sold.price.amount) {
        c.max_sold.price = m.price;
        c.max_sold.nft_id = r.nft_id;
    }
    if (!c.min_sold.nft_id || m.price.amount < c.min_sold.price.amount) {
        c.min_sold.price = m.price;
        c.min_sold.nft_id = r.nft_id;
    }
    gallery::Index<gallery::Tag::kHeightCollectionIdx, Height, gallery::Collection>
        ::Update(c.updated, cur_height, m.collection_id);
    c.updated = cur_height;
    c.Save(m.collection_id, sizeof(gallery::Collection) + c.label_len + c.data_len);

    Env::FundsLock(m.price.aid, r.pay_max);

    gallery::Events::Sell::Key esk;
    esk.nft_id = r.nft_id;
    gallery::Events::Sell es;
    _POD_(es.price) = m.price;
    es.has_aid = r.has_aid;
    Env::EmitLog_T(esk, es);

    gallery::Payout::Key pok;
    pok.nft_id = r.nft_id;
    pok.aid = m.price.aid;

    _POD_(pok.user) = m.owner;
    PayoutMove(pok, m.price.amount, true);

    _POD_(pok.user) = r.user;
    PayoutMove(pok, r.pay_max - m.price.amount, true);

    _POD_(m.owner) = r.user;
    _POD_(m.price).SetZero(); // not for sale until new owner sets the price

    gallery::Index<gallery::Tag::kHeightNftIdx, Height, gallery::Nft>
        ::Update(m.updated, cur_height, r.nft_id);
    m.updated = cur_height;
    m.Save(r.nft_id);

    //Env::AddSig(r.user);
}

BEAM_EXPORT void Method_13(const gallery::method::Withdraw& r) {
    PayoutMove(r.key, r.value, false);
    Env::FundsUnlock(r.key.aid, r.value);
    Env::AddSig(r.key.user);
}

BEAM_EXPORT void Method_14(const gallery::method::CheckPrepare& r) {
    gallery::Nft m;
    Env::Halt_if(!m.Load(r.nft_id));
    Env::AddSig(m.owner);

    if (m.aid) {
        // destroy it
        Env::Halt_if(!Env::AssetDestroy(m.aid));
        m.aid = 0;
    } else {
        // 1st call. Don't checkout, only prepare
        static const char szMeta[] = "STD:SCH_VER=1;N=Gallery Nft;SN=Gall;UN=GALL;NTHUN=unique";
        m.aid = Env::AssetCreate(szMeta, sizeof(szMeta) - 1);
    }

    Height cur_height = Env::get_Height();

    gallery::Index<gallery::Tag::kHeightNftIdx, Height, gallery::Nft>
        ::Update(m.updated, cur_height, r.nft_id);

    m.updated = cur_height;
    m.Save(r.nft_id);
}

BEAM_EXPORT void Method_15(const gallery::method::CheckOut& r) {
    gallery::Nft m;
    Env::Halt_if(!m.Load(r.nft_id) || !m.aid);
    Env::AddSig(m.owner);

    Env::Halt_if(!Env::AssetEmit(m.aid, 1, 1));
    Env::FundsUnlock(m.aid, 1);

    _POD_(m.owner).SetZero();
    _POD_(m.price).SetZero();

    Height cur_height = Env::get_Height();

    gallery::Index<gallery::Tag::kHeightNftIdx, Height, gallery::Nft>
        ::Update(m.updated, cur_height, r.nft_id);

    m.updated = cur_height;
    m.Save(r.nft_id);
}

BEAM_EXPORT void Method_16(const gallery::method::CheckIn& r) {
    gallery::Nft m;
    Env::Halt_if(!m.Load(r.nft_id) || !_POD_(m.owner).IsZero());

    Env::FundsLock(m.aid, 1);
    Env::Halt_if(!Env::AssetEmit(m.aid, 1, 0));

    _POD_(m.owner) = r.user;

    Height cur_height = Env::get_Height();

    gallery::Index<gallery::Tag::kHeightNftIdx, Height, gallery::Nft>
        ::Update(m.updated, cur_height, r.nft_id);

    m.updated = cur_height;
    m.Save(r.nft_id);

    //Env::AddSig(r.user);
}

BEAM_EXPORT void Method_17(const gallery::method::Vote& r) {
    gallery::Nft m;

    Env::Halt_if(!m.Load(r.nft_id));
    
    gallery::Like::Key like_key;
    like_key.nft_id = r.nft_id;
    like_key.artist_id = r.artist_id;

    gallery::Like like;
    if (!Env::LoadVar_T(like_key, like)) {
        like.value = 0;

        MyState s;
        Strict::Sub(s.vote_balance, s.config.vote_reward.amount);
        s.Save();

        Env::FundsUnlock(s.config.vote_reward.aid, s.config.vote_reward.amount);
    }

    Env::Halt_if(like.value == r.value); // not changed

    like.value = r.value;
    Env::SaveVar_T(like_key, like);

    Env::AddSig(r.artist_id);
}

BEAM_EXPORT void Method_18(const gallery::method::AddVoteRewards& r) {
    MyState s;
    Strict::Add(s.vote_balance, r.amount);
    s.Save();

    Env::FundsLock(s.config.vote_reward.aid, r.amount);
}

BEAM_EXPORT void Method_19(const gallery::method::Transfer& r) {
    gallery::Nft m;
    Env::Halt_if(!m.Load(r.nft_id));

    Env::AddSig(m.owner);
    _POD_(m.owner) = r.new_owner;

    Height cur_height = Env::get_Height();

    gallery::Index<gallery::Tag::kHeightNftIdx, Height, gallery::Nft>
        ::Update(m.updated, cur_height, r.nft_id);

    m.updated = cur_height;
    m.Save(r.nft_id);
}
