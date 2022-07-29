<template>
  <div class="admin-container">
    <div class="rewards-row">
      <span>Rewards left: {{ balance_reward }}</span>&nbsp;&nbsp;&nbsp;
      <input type="button" value="Add Rewards" @click="onAddRewards"/>
    </div>

    <div v-if="moderators.length" class="moderators-list">
      Moderators list
      <table> 
        <thead><th>ID</th><th>Name</th><th>Status</th><th>Action</th></thead>
        <tbody>
          <tr v-for="(moderator) in (moderators || [])" :key="moderator.id" class="moderator">
            <td>{{ moderator.id.substring(0, 5) }}...{{ moderator.id.slice(-5) }}</td>
            <td>{{ moderator.name }}</td>
            <td>{{ moderator.status }}</td>
            <td class="link" @click="onAction(moderator.id, moderator.status)">
              {{ moderator.status === "approved" ? 'DISABLE' : 'ENABLE' }}
            </td>
          </tr>
        </tbody>
      </table>
    </div>
    <input type="button" value="Add Moderator" @click="onAddModerator"/>
  </div>
</template>

<style lang="stylus" scoped>
  .admin-container {
    .rewards-row {
      display: flex
      margin-bottom: 20px
    }
    .moderators-list {
      border-top: 1px solid rgba(225, 255, 255, 0.6)
      padding-top: 10px
      
      & table {
        margin-top: 10px
        border-collapse: collapse
        border: 1px solid rgba(225, 255, 255, 0.6) !important
        margin-bottom: 15px

        & thead {
          & th {
            padding-bottom: 5px
            border-bottom: 1px solid rgba(225, 255, 255, 0.6) !important
          }
        }

        & td {
          padding: 6px 10px
        }

        & tr:last-child {
          & td {
            padding-bottom: 9px
          }
        }
      }
      
      .link {
        color: #00f6d2
        cursor: pointer
      }
    }
  }
</style>

<script>
import utils from 'utils/utils'
import artistsStore from 'stores/artists'
import {common} from 'utils/consts'
import {computed} from 'vue'

export default {
  computed: {
    balance_reward () {
      return utils.formatAmountFixed(this.$state.balance_reward, 4)
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
    async onAddRewards() {
      let amount = prompt('Enter amount in REWARD TOKEN (full coins)')
      if (amount == null) {
        return
      }
      await this.$store.addRewards(parseInt(amount) * common.GROTHS_IN_BEAM)
    },
    async onAction(id, status) {
      await this.$store.setModerator(id, status === 'approved' ? 0 : 1)
    },
    async onAddModerator() {
      let id = prompt('Enter moderator\'s artist ID')
      if (id != null) {
        await this.$store.setModerator(id, 1)
      }
    }
  }
}
</script>
