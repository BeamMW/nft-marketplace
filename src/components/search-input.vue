<template>
  <div>
    <div class="input-container">
      <img src="../assets/icon-search.svg" alt="icon" @click="onShowInput"/>
      <input v-show="show"
             :value="search"
             :maxlength="max_length"
             :placeholder="placeholder"
             class="input"
             @input="$emit('update:search', $event.target.value)"
      />
    </div>
  </div>
</template>

<style scoped lang="stylus">
  .input-container {
    display: flex
    position: relative
    width: 279px
    height: 32px

    & > img {
      position: absolute
      right: 0
      padding: 10px 11px 9px 10px
      cursor: pointer
    }

    .input {
      box-sizing: border-box
      font-family: 'SFProDisplay', sans-serif
      background-color: transparent
      border: 1px solid rgba(255,255,255,0.08)
      outline-width: 0
      font-size: 14px
      color: white
      border-radius: 999px
      padding: 9px 46px 9px 20px
      width: 100%
      overflow: hidden
      text-overflow: ellipsis
      white-space: nowrap

      &::placeholder {
        font-size: 12px
        color: rgba(255, 255, 255, 0.3)
      }
    }
  }
</style>

<script>

export default {
  props: {
    search: {
      type: String,
      default: '',
      required: true
    },
    img: {
      type: String,
      default:'',
      required: false,
    },
    max_length:{
      type: Number,
      default: 10,
      required: true
    },
    placeholder: {
      type: String,
      default: '',
      required: true
    },
  },
  emits: [
    'update:search'
  ],
  data () {
    return {
      show: false
    }
  },
  methods: {
    onShowInput() {
      this.show = !this.show
    }
  }

}
</script>
