#include "Shaders/common.h"
#include "Shaders/Math.h"
#include "contract.h"

#include <algorithm>

using namespace gallery;

struct ContractState : public State {
    ContractState() {
        Env::LoadVar_T(static_cast<uint8_t>(kKey), *this);
    }

    explicit ContractState(bool) {
        // no auto-load
    }

    void Save() {
        Env::SaveVar_T(static_cast<uint8_t>(kKey), *this);
    }

    void AddSigAdmin() {
        Env::AddSig(config.admin_id);
    }
};

void PayoutMove(const Payout::Key& key, Amount val, bool add) {
    if (!val)
        return;

    Payout po;
    if (Env::LoadVar_T(key, po)) {
        if (add) {
            Strict::Add(po.amount, val);
        } else {
            Strict::Sub(po.amount, val);
            if (!po.amount) {
                Env::DelVar_T(key);
                return;
            }
        }
    } else {
        Env::Halt_if(!add);
        po.amount = val;
    }
    Env::SaveVar_T(key, po);
}

BEAM_EXPORT void Ctor(const method::Init& r) {
    if (Env::get_CallDepth() > 1) {
        ContractState s(false);
        _POD_(s).SetZero();
        _POD_(s.config) = r.config;
        s.Save();
    }
}

BEAM_EXPORT void Dtor(void*) {
    // ignore
}

BEAM_EXPORT void Method_2(void*) {
    // called on upgrade
}

BEAM_EXPORT void Method_3(const method::SetFeeBase& r) {
    ContractState s;
    s.fee_base = r.amount;
    s.Save();
    s.AddSigAdmin();
}

BEAM_EXPORT void Method_4(const method::SetModerator& r) {
    Env::Halt_if(_POD_(r.id).IsZero());

    Height cur_height = Env::get_Height();
    ContractState s;
    Moderator m;
    if (!GalleryObject::Load(m, r.id)) {
        m.registered = cur_height;
        m.updated = cur_height;
        s.total_moderators++;
        s.Save();
    }
    Index<Tag::kHeightModeratorIdx, Height, Moderator>::Update(
        m.updated, cur_height, r.id);
    m.approved = r.approved;
    m.updated = cur_height;
    GalleryObject::Save(m, r.id);
    s.AddSigAdmin();
}

BEAM_EXPORT void Method_5(const method::SetArtist& r) {
    Env::Halt_if(r.label_len > Artist::kLabelMaxLen);
    Env::Halt_if(r.data_len > Artist::kDataMaxLen);
    Env::Halt_if(!r.label_len);
    Env::Halt_if(_POD_(r.artist_id).IsZero());

    Height cur_height = Env::get_Height();
    struct ArtistPlus : public Artist {
        char data[kDataMaxLen];
    } a;

    if (!GalleryObject::Load(a, r.artist_id, sizeof(a))) {
        ContractState s;
        _POD_(a).SetZero();
        a.registered = cur_height;
        a.updated = cur_height;
        s.total_artists++;
        s.Save();

        Events::AddArtistLabel::Key alk{r.artist_id};
        auto label_ptr = reinterpret_cast<const char*>(&r + 1);
        Env::EmitLog(&alk, sizeof(alk), label_ptr, r.label_len,
                     KeyTag::Internal);
        Artist::LabelKey lk{
            GetLabelHash(std::string_view(label_ptr, r.label_len))};
        // if label already exists
        Env::Halt_if(Env::SaveVar_T(lk, r.artist_id));
    }

    auto data_ptr = reinterpret_cast<const uint8_t*>(&r + 1) + r.label_len;
    Env::Memcpy(a.data, data_ptr, r.data_len);
    a.data_len = r.data_len;
    a.status = Status::kPending;
    Index<Tag::kHeightArtistIdx, Height, Artist>::Update(a.updated, cur_height,
                                                         r.artist_id);
    a.updated = cur_height;
    GalleryObject::Save(a, r.artist_id, sizeof(Artist) + a.data_len);
    Env::AddSig(r.artist_id);
}

BEAM_EXPORT void Method_6(const method::SetArtistStatus& r) {
    Env::Halt_if(r.status == Status::kNone);
    Env::Halt_if(!r.ids_num);

    Height cur_height = Env::get_Height();
    struct ArtistPlus : public Artist {
        char data[kDataMaxLen];
    } a;

    Moderator m;
    ContractState s;
    if (!GalleryObject::Load(m, r.signer) || !m.approved)
        s.AddSigAdmin();
    else
        Env::AddSig(r.signer);

    for (uint32_t i = 0; i < r.ids_num; ++i) {
        Env::Halt_if(!GalleryObject::Load(a, r.ids[i], sizeof(a)));
        a.status = r.status;
        a.approved_cnt += (r.status == Status::kApproved);
        Index<Tag::kHeightArtistIdx, Height, Artist>::Update(
            a.updated, cur_height, r.ids[i]);
        a.updated = cur_height;
        GalleryObject::Save(a, r.ids[i], sizeof(Artist) + a.data_len);
    }
}

BEAM_EXPORT void Method_7(const method::SetCollectionStatus& r) {
    Env::Halt_if(r.status == Status::kNone);
    Env::Halt_if(!r.ids_num);

    Height cur_height = Env::get_Height();
    struct CollectionPlus : public Collection {
        char label_and_data[kTotalMaxLen];
    } c;

    Moderator m;
    ContractState s;
    if (!GalleryObject::Load(m, r.signer) || !m.approved)
        s.AddSigAdmin();
    else
        Env::AddSig(r.signer);

    for (uint32_t i = 0; i < r.ids_num; ++i) {
        Env::Halt_if(!GalleryObject::Load(c, r.ids[i], sizeof(c)));
        c.status = r.status;
        Index<Tag::kHeightCollectionIdx, Height, Collection>::Update(
            c.updated, cur_height, r.ids[i]);
        c.updated = cur_height;
        GalleryObject::Save(c, r.ids[i],
                            sizeof(Collection) + c.label_len + c.data_len);
    }
}

BEAM_EXPORT void Method_8(const method::SetCollection& r) {
    Env::Halt_if(r.label_len > Collection::kLabelMaxLen);
    Env::Halt_if(r.data_len > Collection::kDataMaxLen);
    Env::Halt_if(!r.label_len);
    Env::Halt_if(_POD_(r.artist_id).IsZero());

    Height cur_height = Env::get_Height();
    struct CollectionPlus : public Collection {
        char label_and_data[kTotalMaxLen];
    } c;

    Collection::Id c_id = r.collection_id;

    if (!GalleryObject::Load(c, c_id, sizeof(c))) {
        Env::Halt_if(c_id);

        ContractState s;
        c_id = ++s.total_collections;
        _POD_(c).SetZero();
        c.author = r.artist_id;
        c.updated = cur_height;
        Index<Tag::kArtistCollectionIdx, Artist::Id, Collection>::Save(c.author,
                                                                       c_id);

        struct ArtistPlus : public Artist {
            char data[kDataMaxLen];
        } a;

        GalleryObject::Load(a, r.artist_id, sizeof(a));
        ++a.collections_num;
        Index<Tag::kHeightArtistIdx, Height, Artist>::Update(
            a.updated, cur_height, r.artist_id);
        a.updated = cur_height;
        Env::Halt_if(cur_height - a.last_created_object <= s.rate_limit);
        a.last_created_object = cur_height;
        GalleryObject::Save(a, r.artist_id, sizeof(Artist) + a.data_len);
        s.Save();
    }

    Env::Halt_if(_POD_(c.author) != r.artist_id);

    Collection::LabelKey lk{r.artist_id, GetLabelHash(std::string_view(
                                             c.label_and_data, c.label_len))};
    Env::DelVar_T(lk);

    auto label_ptr = reinterpret_cast<const char*>(&r + 1);
    lk.label_hash = GetLabelHash(std::string_view(label_ptr, r.label_len));
    // if already exists
    Env::Halt_if(Env::SaveVar_T(lk, c_id));

    Env::Memcpy(c.label_and_data, &r + 1, r.label_len + r.data_len);
    c.label_len = r.label_len;
    c.data_len = r.data_len;
    c.status = Status::kPending;
    Index<Tag::kHeightCollectionIdx, Height, Collection>::Update(
        c.updated, cur_height, c_id);
    c.updated = cur_height;
    GalleryObject::Save(c, c_id, sizeof(Collection) + c.label_len + c.data_len);
    Env::AddSig(r.artist_id);
}

BEAM_EXPORT void Method_9(const method::SetNft& r) {
    Env::Halt_if(r.label_len > Nft::kLabelMaxLen);
    Env::Halt_if(r.data_len > Nft::kDataMaxLen);
    Env::Halt_if(!r.label_len);
    Env::Halt_if(!r.data_len);
    Env::Halt_if(_POD_(r.artist_id).IsZero());

    Height cur_height = Env::get_Height();
    Nft m;
    ContractState s;

    m.id = ++s.total_nfts;
    m.owner = r.artist_id;
    m.author = r.artist_id;
    m.collection_id = r.collection_id;
    m.status = Status::kPending;
    m.likes_number = 0;
    _POD_(m.price) = r.price;
    // assert: artist owns collection
    Env::Halt_if(
        !Index<Tag::kArtistCollectionIdx, Artist::Id, Collection>::Load(
            r.artist_id, r.collection_id));
    Index<Tag::kCollectionNftIdx, Collection::Id, Nft>::Save(r.collection_id,
                                                             m.id);

    struct CollectionPlus : public Collection {
        char label_and_data[kTotalMaxLen];
    } c;

    // assert: collection exists
    Env::Halt_if(!GalleryObject::Load(c, r.collection_id, sizeof(c)));
    c.nfts_num++;
    Env::Halt_if(c.nfts_num > c.kMaxNfts);
    Index<Tag::kHeightCollectionIdx, Height, Collection>::Update(
        c.updated, cur_height, r.collection_id);
    c.updated = cur_height;
    GalleryObject::Save(c, r.collection_id,
                        sizeof(Collection) + c.label_len + c.data_len);

    // verify artist
    struct ArtistPlus : public Artist {
        char data[kDataMaxLen];
    } a;

    // assert: artist exists
    Env::Halt_if(!GalleryObject::Load(a, r.artist_id, sizeof(a)));
    ++a.nfts_num;
    Index<Tag::kHeightArtistIdx, Height, Artist>::Update(a.updated, cur_height,
                                                         r.artist_id);
    Env::Halt_if(cur_height - a.last_created_object <= s.rate_limit);
    a.last_created_object = cur_height;
    a.updated = cur_height;
    GalleryObject::Save(a, r.artist_id, sizeof(Artist) + a.data_len);

    auto data_ptr = reinterpret_cast<const uint8_t*>(&r + 1) + r.label_len;
    Events::AddNftData::Key adk;
    adk.nft_id = m.id;
    adk.artist_id = m.owner;
    Env::EmitLog(&adk, sizeof(adk), data_ptr, r.data_len, KeyTag::Internal);

    auto label_ptr = reinterpret_cast<const uint8_t*>(&r + 1);
    Events::AddNftLabel::Key alk;
    alk.nft_id = m.id;
    alk.artist_id = m.owner;
    Env::EmitLog(&alk, sizeof(alk), label_ptr, r.label_len, KeyTag::Internal);

    Index<Tag::kHeightNftIdx, Height, Nft>::Update(m.updated, cur_height, m.id);
    m.updated = cur_height;
    GalleryObject::Save(m, m.id);
    s.Save();
    Env::AddSig(r.artist_id);
}

BEAM_EXPORT void Method_10(const method::SetNftStatus& r) {
    Env::Halt_if(r.status == Status::kNone);
    Env::Halt_if(!r.ids_num);

    Height cur_height = Env::get_Height();
    Nft n;
    Moderator m;
    ContractState s;
    if (!GalleryObject::Load(m, r.signer) || !m.approved)
        s.AddSigAdmin();
    else
        Env::AddSig(r.signer);

    struct CollectionPlus : public Collection {
        char label_and_data[kTotalMaxLen];
    } c;

    for (uint32_t i = 0; i < r.ids_num; ++i) {
        Env::Halt_if(!GalleryObject::Load(n, r.ids[i]));
        Env::Halt_if(!GalleryObject::Load(c, n.collection_id, sizeof(c)));
        if (r.status == Status::kApproved && n.status != Status::kApproved)
            ++c.approved_nfts_num;
        else if (r.status != Status::kApproved && n.status == Status::kApproved)
            --c.approved_nfts_num;
        Index<Tag::kHeightCollectionIdx, Height, Collection>::Update(
            c.updated, cur_height, n.collection_id);
        c.updated = cur_height;
        GalleryObject::Save(c, n.collection_id,
                            sizeof(Collection) + c.label_len + c.data_len);
        n.status = r.status;
        Index<Tag::kHeightNftIdx, Height, Nft>::Update(n.updated, cur_height,
                                                       n.id);
        n.updated = cur_height;
        GalleryObject::Save(n, n.id);
    }
}

BEAM_EXPORT void Method_11(const method::SetPrice& r) {
    Nft m;
    Env::Halt_if(!GalleryObject::Load(m, r.nft_id));
    _POD_(m.price) = r.price;
    Height cur_height = Env::get_Height();
    Index<Tag::kHeightNftIdx, Height, Nft>::Update(m.updated, cur_height,
                                                   r.nft_id);
    m.updated = cur_height;
    GalleryObject::Save(m, r.nft_id);
    Env::AddSig(m.owner);  // would fail if no current owner (i.e. checked out)
}

BEAM_EXPORT void Method_12(const method::Buy& r) {
    Height cur_height = Env::get_Height();
    Nft m;
    Env::Halt_if(!GalleryObject::Load(m, r.nft_id));
    Env::Halt_if(!m.price.amount ||  // not for sale!
                 (r.has_aid != (!!m.aid)));

    struct CollectionPlus : public Collection {
        char label_and_data[kTotalMaxLen];
    } c;

    GalleryObject::Load(c, m.collection_id, sizeof(c));
    ++c.total_sold;
    c.total_sold_price += m.price.amount;
    if (!c.max_sold.nft_id || m.price.amount > c.max_sold.price.amount) {
        c.max_sold.price = m.price;
        c.max_sold.nft_id = r.nft_id;
    }
    if (!c.min_sold.nft_id || m.price.amount < c.min_sold.price.amount) {
        c.min_sold.price = m.price;
        c.min_sold.nft_id = r.nft_id;
    }
    Index<Tag::kHeightCollectionIdx, Height, Collection>::Update(
        c.updated, cur_height, m.collection_id);
    c.updated = cur_height;
    GalleryObject::Save(c, m.collection_id,
                        sizeof(Collection) + c.label_len + c.data_len);

    struct ArtistPlus : public Artist {
        char data[kDataMaxLen];
    } a;

    if (_POD_(m.author) == m.owner) {
        GalleryObject::Load(a, m.author, sizeof(a));
        ++a.total_sold;
        a.total_sold_price += m.price.amount;
        Index<Tag::kHeightArtistIdx, Height, Artist>::Update(
            a.updated, cur_height, m.author);
        a.updated = cur_height;
        GalleryObject::Save(a, m.author, sizeof(Artist) + a.data_len);
    }

    Env::FundsLock(m.price.aid, m.price.amount);

    Events::Sell::Key esk;
    esk.nft_id = r.nft_id;
    Events::Sell es;
    _POD_(es.price) = m.price;
    es.has_aid = r.has_aid;
    Env::EmitLog_T(esk, es);

    Payout::Key pok;
    pok.nft_id = r.nft_id;
    pok.aid = m.price.aid;
    _POD_(pok.user) = m.owner;
    PayoutMove(pok, m.price.amount, true);

    _POD_(m.owner) = r.user;
    _POD_(m.price).SetZero();  // not for sale until new owner sets the price
    Index<Tag::kHeightNftIdx, Height, Nft>::Update(m.updated, cur_height,
                                                   r.nft_id);
    m.updated = cur_height;
    GalleryObject::Save(m, r.nft_id);
    Env::AddSig(r.user);
}

BEAM_EXPORT void Method_13(const method::Withdraw& r) {
    PayoutMove(r.key, r.value, false);
    Env::FundsUnlock(r.key.aid, r.value);
    Env::AddSig(r.key.user);
}

BEAM_EXPORT void Method_14(const method::CheckPrepare& r) {
    /*
    Nft m;
    Env::Halt_if(!GalleryObject::Load(m, r.nft_id));
    Env::AddSig(m.owner);

    if (m.aid) {
        // destroy it
        Env::Halt_if(!Env::AssetDestroy(m.aid));
        m.aid = 0;
    } else {
        // 1st call. Don't checkout, only prepare
        static const char szMeta[] =
            "STD:SCH_VER=1;N=Gallery Nft;SN=Gall;UN=GALL;NTHUN=unique";
        m.aid = Env::AssetCreate(szMeta, sizeof(szMeta) - 1);
    }

    Height cur_height = Env::get_Height();

    Index<Tag::kHeightNftIdx, Height, Nft>::Update(m.updated, cur_height,
                                                   r.nft_id);

    m.updated = cur_height;
    GalleryObject::Save(m, r.nft_id);
    */
}

BEAM_EXPORT void Method_15(const method::CheckOut& r) {
    /*
    Nft m;
    Env::Halt_if(!GalleryObject::Load(m, r.nft_id) || !m.aid);
    Env::AddSig(m.owner);

    Env::Halt_if(!Env::AssetEmit(m.aid, 1, 1));
    Env::FundsUnlock(m.aid, 1);

    _POD_(m.owner).SetZero();
    _POD_(m.price).SetZero();

    Height cur_height = Env::get_Height();

    Index<Tag::kHeightNftIdx, Height, Nft>::Update(m.updated, cur_height,
                                                   r.nft_id);

    m.updated = cur_height;
    GalleryObject::Save(m, r.nft_id);
    */
}

BEAM_EXPORT void Method_16(const method::CheckIn& r) {
    /*
    Nft m;
    Env::Halt_if(!GalleryObject::Load(m, r.nft_id) || !_POD_(m.owner).IsZero());

    Env::FundsLock(m.aid, 1);
    Env::Halt_if(!Env::AssetEmit(m.aid, 1, 0));

    _POD_(m.owner) = r.user;

    Height cur_height = Env::get_Height();

    Index<Tag::kHeightNftIdx, Height, Nft>::Update(m.updated, cur_height,
                                                   r.nft_id);

    m.updated = cur_height;
    GalleryObject::Save(m, r.nft_id);

    // Env::AddSig(r.user);
    */
}

BEAM_EXPORT void Method_17(const method::Vote& r) {
    Nft m;
    Env::Halt_if(!GalleryObject::Load(m, r.nft_id));

    Like::Key like_key;
    like_key.nft_id = r.nft_id;
    like_key.artist_id = r.artist_id;

    Like like;
    if (!Env::LoadVar_T(like_key, like)) {
        like.value = 0;
        ContractState s;
        Strict::Sub(s.vote_balance, s.config.vote_reward.amount);
        s.Save();
        Env::FundsUnlock(s.config.vote_reward.aid, s.config.vote_reward.amount);
    }
    Env::Halt_if(like.value == r.value);  // not changed
    bool nft_updated = false;
    if (like.value && !r.value) {
        m.likes_number--;
        nft_updated = true;
    } else if (!like.value && r.value) {
        m.likes_number++;
        nft_updated = true;
    }
    like.value = r.value;
    Env::SaveVar_T(like_key, like);
    if (nft_updated) {
        Height cur_height = Env::get_Height();
        Index<Tag::kHeightNftIdx, Height, Nft>::Update(m.updated, cur_height,
                                                       r.nft_id);
        m.updated = cur_height;
        GalleryObject::Save(m, r.nft_id);
    }
    Env::AddSig(r.artist_id);
}

BEAM_EXPORT void Method_18(const method::AddVoteRewards& r) {
    ContractState s;
    Strict::Add(s.vote_balance, r.amount);
    s.Save();
    Env::FundsLock(s.config.vote_reward.aid, r.amount);
}

BEAM_EXPORT void Method_19(const method::Transfer& r) {
    Env::Halt_if(_POD_(r.new_owner).IsZero());

    Height cur_height = Env::get_Height();
    Nft m;
    Env::Halt_if(!GalleryObject::Load(m, r.nft_id));
    Env::AddSig(m.owner);

    Events::Sell::Key esk;
    esk.nft_id = r.nft_id;
    Events::Sell es;
    _POD_(es.price).SetZero();
    es.has_aid = 0;
    Env::EmitLog_T(esk, es);

    struct CollectionPlus : public Collection {
        char label_and_data[kTotalMaxLen];
    } c;

    GalleryObject::Load(c, m.collection_id, sizeof(c));
    ++c.total_sold;
    Index<Tag::kHeightCollectionIdx, Height, Collection>::Update(
        c.updated, cur_height, m.collection_id);
    c.updated = cur_height;
    GalleryObject::Save(c, m.collection_id,
                        sizeof(Collection) + c.label_len + c.data_len);

    _POD_(m.owner) = r.new_owner;
    Index<Tag::kHeightNftIdx, Height, Nft>::Update(m.updated, cur_height,
                                                   r.nft_id);
    m.updated = cur_height;
    GalleryObject::Save(m, r.nft_id);
}

BEAM_EXPORT void Method_20(const method::SetRateLimit& r) {
    ContractState s;
    s.rate_limit = r.amount;
    s.Save();
    s.AddSigAdmin();
}

BEAM_EXPORT void Method_21(const method::MigrateArtist& r) {
    Env::Halt_if(r.label_len > Artist::kLabelMaxLen);
    Env::Halt_if(r.data_len > Artist::kDataMaxLen);
    Env::Halt_if(!r.label_len);
    Env::Halt_if(_POD_(r.artist_id).IsZero());

    Moderator m;
    ContractState s;
    if (!GalleryObject::Load(m, r.signer) || !m.approved)
        s.AddSigAdmin();
    else
        Env::AddSig(r.signer);

    Height cur_height = Env::get_Height();
    struct ArtistPlus : public Artist {
        char data[kDataMaxLen];
    } a;

    if (!GalleryObject::Load(a, r.artist_id, sizeof(a))) {
        _POD_(a).SetZero();
        a.registered = cur_height;
        a.updated = cur_height;
        s.total_artists++;
        s.Save();

        Events::AddArtistLabel::Key alk{r.artist_id};
        auto label_ptr = reinterpret_cast<const char*>(&r + 1);
        Env::EmitLog(&alk, sizeof(alk), label_ptr, r.label_len,
                     KeyTag::Internal);
        Artist::LabelKey lk{
            GetLabelHash(std::string_view(label_ptr, r.label_len))};
        // if label already exists
        Env::Halt_if(Env::SaveVar_T(lk, r.artist_id));
    }

    auto data_ptr = reinterpret_cast<const uint8_t*>(&r + 1) + r.label_len;
    Env::Memcpy(a.data, data_ptr, r.data_len);
    a.data_len = r.data_len;
    a.status = Status::kApproved;
    a.approved_cnt = 1;
    Index<Tag::kHeightArtistIdx, Height, Artist>::Update(a.updated, cur_height,
                                                         r.artist_id);
    a.updated = cur_height;
    GalleryObject::Save(a, r.artist_id, sizeof(Artist) + a.data_len);
}

BEAM_EXPORT void Method_22(const method::MigrateCollection& r) {
    Env::Halt_if(r.label_len > Collection::kLabelMaxLen);
    Env::Halt_if(r.data_len > Collection::kDataMaxLen);
    Env::Halt_if(!r.label_len);
    Env::Halt_if(_POD_(r.artist_id).IsZero());

    Moderator m;
    ContractState s;
    if (!GalleryObject::Load(m, r.signer) || !m.approved)
        s.AddSigAdmin();
    else
        Env::AddSig(r.signer);

    Height cur_height = Env::get_Height();
    struct CollectionPlus : public Collection {
        char label_and_data[kTotalMaxLen];
    } c;

    Collection::Id c_id = 0;

    if (!GalleryObject::Load(c, c_id, sizeof(c))) {
        Env::Halt_if(c_id);

        c_id = ++s.total_collections;
        _POD_(c).SetZero();
        c.author = r.artist_id;
        c.updated = cur_height;
        Index<Tag::kArtistCollectionIdx, Artist::Id, Collection>::Save(c.author,
                                                                       c_id);

        struct ArtistPlus : public Artist {
            char data[kDataMaxLen];
        } a;

        GalleryObject::Load(a, r.artist_id, sizeof(a));
        ++a.collections_num;
        Index<Tag::kHeightArtistIdx, Height, Artist>::Update(
            a.updated, cur_height, r.artist_id);
        a.updated = cur_height;
        GalleryObject::Save(a, r.artist_id, sizeof(Artist) + a.data_len);
        s.Save();
    }

    Collection::LabelKey lk{r.artist_id, GetLabelHash(std::string_view(
                                             c.label_and_data, c.label_len))};
    Env::DelVar_T(lk);

    auto label_ptr = reinterpret_cast<const char*>(&r + 1);
    lk.label_hash = GetLabelHash(std::string_view(label_ptr, r.label_len));
    // if already exists
    Env::Halt_if(Env::SaveVar_T(lk, c_id));

    Env::Memcpy(c.label_and_data, &r + 1, r.label_len + r.data_len);
    c.label_len = r.label_len;
    c.data_len = r.data_len;
    c.status = Status::kApproved;
    Index<Tag::kHeightCollectionIdx, Height, Collection>::Update(
        c.updated, cur_height, c_id);
    c.updated = cur_height;
    GalleryObject::Save(c, c_id, sizeof(Collection) + c.label_len + c.data_len);
}

BEAM_EXPORT void Method_23(const method::MigrateNft& r) {
    Env::Halt_if(r.label_len > Nft::kLabelMaxLen);
    Env::Halt_if(r.data_len > Nft::kDataMaxLen);
    Env::Halt_if(!r.label_len);
    Env::Halt_if(!r.data_len);
    Env::Halt_if(_POD_(r.artist_id).IsZero());
    Env::Halt_if(_POD_(r.owner_id).IsZero());

    Height cur_height = Env::get_Height();
    Nft m;
    Moderator moder;
    ContractState s;
    if (!GalleryObject::Load(moder, r.signer) || !moder.approved)
        s.AddSigAdmin();
    else
        Env::AddSig(r.signer);

    m.id = r.nft_id;
    s.total_nfts = r.nft_id;
    m.owner = r.owner_id;
    m.author = r.artist_id;
    m.collection_id = r.collection_id;
    m.status = Status::kApproved;
    m.likes_number = 0;
    _POD_(m.price) = r.price;
    // assert: artist owns collection
    Env::Halt_if(
        !Index<Tag::kArtistCollectionIdx, Artist::Id, Collection>::Load(
            r.artist_id, r.collection_id));
    Index<Tag::kCollectionNftIdx, Collection::Id, Nft>::Save(r.collection_id,
                                                             m.id);

    struct CollectionPlus : public Collection {
        char label_and_data[kTotalMaxLen];
    } c;

    // assert: collection exists
    Env::Halt_if(!GalleryObject::Load(c, r.collection_id, sizeof(c)));
    c.nfts_num++;
    c.approved_nfts_num++;
    Env::Halt_if(c.nfts_num > c.kMaxNfts);
    Index<Tag::kHeightCollectionIdx, Height, Collection>::Update(
        c.updated, cur_height, r.collection_id);
    c.updated = cur_height;
    GalleryObject::Save(c, r.collection_id,
                        sizeof(Collection) + c.label_len + c.data_len);

    // verify artist
    struct ArtistPlus : public Artist {
        char data[kDataMaxLen];
    } a;

    // assert: artist exists
    Env::Halt_if(!GalleryObject::Load(a, r.artist_id, sizeof(a)));
    ++a.nfts_num;
    Index<Tag::kHeightArtistIdx, Height, Artist>::Update(a.updated, cur_height,
                                                         r.artist_id);
    a.updated = cur_height;
    GalleryObject::Save(a, r.artist_id, sizeof(Artist) + a.data_len);

    auto data_ptr = reinterpret_cast<const uint8_t*>(&r + 1) + r.label_len;
    Events::AddNftData::Key adk;
    adk.nft_id = m.id;
    adk.artist_id = m.author;
    Env::EmitLog(&adk, sizeof(adk), data_ptr, r.data_len, KeyTag::Internal);

    auto label_ptr = reinterpret_cast<const uint8_t*>(&r + 1);
    Events::AddNftLabel::Key alk;
    alk.nft_id = m.id;
    alk.artist_id = m.author;
    Env::EmitLog(&alk, sizeof(alk), label_ptr, r.label_len, KeyTag::Internal);

    Index<Tag::kHeightNftIdx, Height, Nft>::Update(m.updated, cur_height, m.id);
    m.updated = cur_height;
    GalleryObject::Save(m, m.id);
    s.Save();
}

BEAM_EXPORT void Method_24(const method::MigrateSales& r) {
    Height cur_height = Env::get_Height();
    Nft m;
    Env::Halt_if(!GalleryObject::Load(m, r.nft_id));
    Env::Halt_if(!r.sales_len);

    struct CollectionPlus : public Collection {
        char label_and_data[kTotalMaxLen];
    } c;

    GalleryObject::Load(c, m.collection_id, sizeof(c));
    for (uint32_t i = 0; i < r.sales_len; ++i) {
        ++c.total_sold;
        c.total_sold_price += r.prices[i];
        if (!c.max_sold.nft_id || r.prices[i] > c.max_sold.price.amount) {
            c.max_sold.price.amount = r.prices[i];
            c.max_sold.price.aid = 0;
            c.max_sold.nft_id = r.nft_id;
        }
        if (!c.min_sold.nft_id || r.prices[i] < c.min_sold.price.amount) {
            c.min_sold.price.amount = r.prices[i];
            c.min_sold.price.aid = 0;
            c.min_sold.nft_id = r.nft_id;
        }
        Events::Sell::Key esk;
        esk.nft_id = r.nft_id;
        Events::Sell es;
        es.price.amount = r.prices[i];
        es.price.aid = 0;
        es.has_aid = 0;
        Env::EmitLog_T(esk, es);
    }
    Index<Tag::kHeightCollectionIdx, Height, Collection>::Update(
        c.updated, cur_height, m.collection_id);
    c.updated = cur_height;
    GalleryObject::Save(c, m.collection_id,
                        sizeof(Collection) + c.label_len + c.data_len);

    struct ArtistPlus : public Artist {
        char data[kDataMaxLen];
    } a;

    GalleryObject::Load(a, m.author, sizeof(a));
    ++a.total_sold;
    a.total_sold_price += r.prices[0];
    Index<Tag::kHeightArtistIdx, Height, Artist>::Update(a.updated, cur_height,
                                                         m.author);
    a.updated = cur_height;
    GalleryObject::Save(a, m.author, sizeof(Artist) + a.data_len);

    Moderator moder;
    ContractState s;
    if (!GalleryObject::Load(moder, r.signer) || !moder.approved)
        s.AddSigAdmin();
    else
        Env::AddSig(r.signer);
}
