<template>
  <div class="button" :class="type">
    <img 
      v-if="type === 'sell' || type === 'buy' || type === 'change'" 
      src="~assets/icon-button.svg" 
    />
    <img 
      v-if="type === 'close' || type === 'cancel'"
      src="~assets/icon-cancel.svg"
    />
    <img 
      v-if="type === 'copy'"
      src="~assets/icon-copy-blue.svg"
    />
    <img 
      v-if="type === 'proceed'"
      src="~assets/icon-receive-proceed.svg"
    />
    <span class="text">{{ text }}</span>
  </div>
</template>

<script>
export default {
  props: {
    type: {
      type: String,
      required: true,
    },
    text: {
      type: String,
      required: true,
    },
  }
}
</script>

<style scoped lang="stylus">
.button {
  padding: 11px 25px
  border-radius: 50px
  display: flex
  align-items: center
  color: #032e49
  font-size: 14px
  font-weight: bold
  cursor: pointer
  margin-left: auto

  & .text {
    margin-left: 5px
    line-height: 1
  }
}

.buy {
  background-color: #da68f5
}

.sell, .change {
  background-color: #0bccf7
}

.cancel, .close {
  background-color: rgba(255, 255, 255, 0.1)
  color: #fff
}

.proceed {
  margin-left: 30px
  background-color: #0bccf7
}

.copy {
  margin-left: 30px
  background-color: #00f6d2
}

.button:not(.disabled):hover {
  box-shadow: 0 0 8px white
}
</style>
