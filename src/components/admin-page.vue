<template>
  <promptModal ref="prompt"/>
  <div class="admin-container">
    <div class="cards">
      <div class="card">
        <div class="card-title">Like rewards pool</div>
        <div class="card-value">
          <amount :amount="balance_reward_raw" :aid="reward_aid" :size="'24px'"/>
        </div>
        <div class="card-note">{{ reward_note }}</div>
        <btn text="add rewards" color="green" height="33px" @click="onAddRewards">
          <img src="~assets/create.svg"/>
        </btn>
      </div>

      <div class="card">
        <div class="card-title">Moderators</div>
        <div class="card-value plain">{{ moderators.length }}</div>
        <btn text="add moderator" color="blue" height="33px" @click="onAddModerator">
          <img src="~assets/add-user.svg"/>
        </btn>
      </div>
    </div>

    <div class="section-title">Moderators</div>

    <div v-if="moderators.length" class="table">
      <div class="table-head">
        <div class="col-id">Artist ID</div>
        <div class="col-name">Name</div>
        <div class="col-status">Status</div>
        <div class="col-action">Action</div>
      </div>
      <div class="table-body">
        <div v-for="(moderator) in (moderators || [])" :key="moderator.id" class="row">
          <div class="col-id mono">{{ shortId(moderator.id) }}</div>
          <div class="col-name">{{ moderator.name }}</div>
          <div class="col-status">
            <span class="badge" :class="moderator.status === 'approved' ? 'ok' : 'off'">
              {{ moderator.status }}
            </span>
          </div>
          <div class="col-action">
            <btn :text="moderator.status === 'approved' ? 'disable' : 'enable'"
                 :color="moderator.status === 'approved' ? 'transparent' : 'green'"
                 :text_color="moderator.status === 'approved' ? 'magenta' : undefined"
                 height="31px"
                 padding="0px 12px"
                 @click="onAction(moderator.id, moderator.status)"
            />
          </div>
        </div>
      </div>
    </div>

    <div v-else class="empty">
      <img src="~assets/empty-gallery.svg"/>
      <div class="text">There are no moderators at the moment</div>
    </div>
  </div>
</template>

<style lang="stylus" scoped>
  .admin-container {
    display: flex
    flex-direction: column
    box-sizing: border-box
    min-height: 0
    overflow-y: auto
    padding-top: 20px

    & > .cards {
      display: flex
      flex-direction: row
      flex-wrap: wrap
      margin: 0 -10px

      & > .card {
        display: flex
        flex-direction: column
        align-items: flex-start
        box-sizing: border-box
        min-width: 240px
        margin: 0 10px
        padding: 20px
        border-radius: 10px
        background-color: rgba(255, 255, 255, 0.05)

        & > .card-title {
          opacity: 0.5
          font-size: 12px
          text-transform: uppercase
          letter-spacing: 1.5px
          color: #fff
        }

        & > .card-value {
          margin: 12px 0 10px 0

          &.plain {
            font-size: 24px
            font-weight: bold
            color: #fff
          }
        }

        & > .card-note {
          max-width: 260px
          margin-bottom: 16px
          font-size: 12px
          line-height: 1.5
          color: rgba(255, 255, 255, 0.5)
        }
      }
    }

    & > .section-title {
      padding: 30px 0 20px 0
      color: rgba(255, 255, 255, 0.5)
      font-size: 14px
      text-transform: uppercase
      font-weight: bold
      letter-spacing: 3.1px
      user-select: none
    }

    & > .table {
      display: flex
      flex-direction: column

      .col-id {
        width: 200px
      }

      .col-name {
        flex: 1
      }

      .col-status {
        width: 140px
      }

      .col-action {
        width: 140px
        display: flex
        justify-content: flex-end
      }

      & > .table-head {
        display: flex
        align-items: center
        padding: 0 15px 12px 15px
        border-bottom: 1px solid rgba(255, 255, 255, 0.15)
        opacity: 0.5
        font-size: 12px
        text-transform: uppercase
        letter-spacing: 1.5px
        color: #fff
      }

      & > .table-body {
        display: flex
        flex-direction: column

        & > .row {
          display: flex
          align-items: center
          padding: 12px 15px
          border-bottom: 1px solid rgba(255, 255, 255, 0.05)
          font-size: 14px
          color: #fff

          &:hover {
            background-color: rgba(255, 255, 255, 0.03)
          }

          & .mono {
            font-family: 'SFProDisplay', monospace
            opacity: 0.7
          }

          & .badge {
            padding: 3px 10px
            border-radius: 999px
            font-size: 12px
            text-transform: uppercase
            letter-spacing: 0.5px

            &.ok {
              color: #00f6d2
              background-color: rgba(0, 246, 210, 0.12)
            }

            &.off {
              color: rgba(255, 255, 255, 0.6)
              background-color: rgba(255, 255, 255, 0.08)
            }
          }
        }
      }
    }

    & > .empty {
      margin: 60px auto 0 auto
      display: flex
      align-items: center
      flex-direction: column
      user-select: none

      & > .text {
        margin-top: 30px
        opacity: 0.5
        font-size: 14px
        color: #fff
      }
    }
  }
</style>

<script>
import utils from 'utils/utils'
import btn from 'controls/button'
import amount from 'controls/amount'
import promptModal from 'controls/prompt-modal'
import artistsStore from 'stores/artists'
import assetsStore from 'stores/assets'
import {common} from 'utils/consts'
import {computed} from 'vue'

export default {
  components: {
    btn,
    amount,
    promptModal
  },

  computed: {
    balance_reward () {
      return utils.formatAmountFixed(this.$state.balance_reward, 4)
    },
    balance_reward_raw () {
      return Number(this.$state.balance_reward || 0)
    },

    reward_aid () {
      return Number(this.$state.reward_aid || 0)
    },

    reward_unit () {
      return assetsStore.get(this.reward_aid).unit_name
    },

    reward_amount_raw () {
      return Number(this.$state.reward_amount || 0)
    },

    // The pool pays out on a first like, so the useful number is how many
    // more likes it still covers.
    reward_note () {
      if (!this.reward_amount_raw) {
        return 'Pays users the first time they like an NFT.'
      }

      let each = utils.formatAmountFixed(this.reward_amount_raw, 8)
      let left = Math.floor(this.balance_reward_raw / this.reward_amount_raw)

      return `Funds ${utils.groupThousands(String(left))} more likes at ${each} ${this.reward_unit} each.`
    },
    moderators () {
      let result = []
      for (let moderator of this.$state.moderators) {
        let author = artistsStore.loadArtist(moderator.id)
        moderator.name = computed(() => {
          if (author.loading) return 'N/A'
          if (author.error) return 'ERROR'
          return author.label
        })
        result.push(moderator)
      }
      return result
    }
  },

  methods: {
    shortId(id) {
      if (!id || id.length <= 16) return id
      return `${id.substring(0, 8)}...${id.slice(-8)}`
    },

    async onAddRewards() {
      let entered = await this.$refs.prompt.open({
        title: 'Add rewards',
        message: 'Tops up the pool that pays users the first time they like an NFT.',
        mode: 'amount',
        aid: this.reward_aid,
        ok_text: 'add rewards'
      })

      if (entered === undefined) {
        return
      }

      // every Beam asset uses 8 decimals, same as BEAM
      await this.$store.addRewards(Math.round(parseFloat(entered) * common.GROTHS_IN_BEAM))
    },
    async onAction(id, status) {
      await this.$store.setModerator(id, status === 'approved' ? 0 : 1)
    },
    async onAddModerator() {
      let id = await this.$refs.prompt.open({
        title: 'Add moderator',
        message: 'Paste the artist key of the person you want to make a moderator.',
        max_length: 66,
        tip: 'Valid key is 66 chars long',
        allowed: /^[a-zA-Z0-9]{1,66}$/,
        validate: (value) => /^[a-zA-Z0-9]{66}$/.test(value),
        ok_text: 'add moderator'
      })

      if (id === undefined) {
        return
      }

      await this.$store.setModerator(id, 1)
    }
  }
}
</script>
