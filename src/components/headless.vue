<template>
  <div class="balance-container">
    <div class="user-opt">
      <div class="key" @click="onConnect">
        <span class="connect">Connect to wallet</span>
      </div>
    </div>
  </div>
</template>

<style lang="stylus" scoped>
.balance-container {
    display: flex
    flex-direction: row
    justify-content: space-between
    margin-bottom: 10px
    
    .user-opt {
        display: flex
        flex-direction: column
        margin-left: auto

        .key {
            padding: 14px
            border-radius: 10px
            background-color: rgba(255, 255, 255, .1)
            display: flex
            align-items: center
            cursor: pointer
        }
        
        .connect {
            font-size: 16px
            color: #00f6d2
        }
    }
}
</style>

<script>
export default {
  methods: {
    async onConnect () {
      this.$store.switchToHeaded()  
    }
  }
}
</script>