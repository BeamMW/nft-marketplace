<template>
  <div class="admin-page">
    <pageTitle :title="page_title">
      <btnWallet/>
      <btnKey/>
      <btnProfile/>
    </pageTitle>
  </div>
</template>

<script>
import tabsctrl from './tabs'
import pageTitle from './page-title'
import btnKey from './btn-key'
import btnWallet from './btn-wallet'
import btnProfile from './btn-profile'

export default {
  components: {
    tabsctrl,
    pageTitle,
    btnKey,
    btnWallet,
    btnProfile
  },

  computed: {
    page_title() {
      return this.$state.is_admin ? 'admin panel' : 'moderator panel'
    }
  }
}
</script>
