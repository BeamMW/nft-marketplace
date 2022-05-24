<template>
  <div>
    <div class="input-container">
      <img src="/assets/search.svg" alt="icon" @click="onShowInput"/>
      <input v-show="show"
             :value="modelValue"
             :maxlength="max_length"
             :placeholder="placeholder"
             class="input"
             @input="$emit('update:modelValue', $event.target.value)"
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
      padding: 8px 11px 10px 10px
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
      padding: 6px 46px 9px 20px
      width: 100%
      overflow: hidden
      text-overflow: ellipsis
      white-space: nowrap

      &::placeholder {
        font-size: 12px
        color: rgba(255, 255, 255, 0.3)
      }
  
      &:focus-within {
        background-color: rgba(255, 255, 255, 0.12)
      }
    }
  }
</style>

<script>

export default {
  props: {
    // eslint-disable-next-line vue/prop-name-casing
    modelValue: {
      type: String,
      default: '',
      required: true
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
    'update:modelValue'
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
