<template>
  <publicKeyModal ref="keyModal"/>
  <div class="header">
    <backBtn @click="$router.go(-1)"/>
    <h1 class="title">MY PAGE</h1>
    <div class="options">
      <btn :hover="false">
        <img src="~assets/icon-heart.svg">
      </btn>
      <btn :hover="false">
        <img src="~assets/icon-eye-crossed.svg">
      </btn>
      <btn :hover="false" @click="onShowKeyClick">
        <img src="~assets/icon-key.svg">
      </btn>
    </div>
  </div>
</template>

<style scoped lang="stylus">
.header {
  display: flex
  flex-direction: row
  align-items: center
  justify-content: space-between
  justify-items: center

  & > * {
    flex-grow: 1
    flex-basis: 0
  }

  & > .title {
    font-size: 14px
    letter-spacing: 3.11px
    text-align: center
  }

  & > .options {
    height: 36px
    display: flex
    justify-content: flex-end

    & > button {
      width: 36px
      border-radius: 10px
      padding: 5px 12px
      margin-left: 20px
    }
  }
}
</style>

<script>
import backBtn from './back-btn.vue'
import btn from './button.vue'
import publicKeyModal from './public-key-dialog.vue'

export default {
  components: {
    backBtn,
    btn,
    publicKeyModal
  },
  methods: {
    onShowKeyClick () {
      this.$refs.keyModal.open()
    },
  },
}
</script>
