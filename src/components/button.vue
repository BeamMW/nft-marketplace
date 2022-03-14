<template>
  <div class="button" :class="type">
    <img />
    <span class="text">{{ title }}</span>
  </div>
</template>

<script>

export default {
  props: {
    type: {
      type: String,
      required: true,
    },
    title: {
      type: String,
      required: true,
    },
    icon: {}
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

  &.text {
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
