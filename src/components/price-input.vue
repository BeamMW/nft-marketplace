<template>
  <div class="input">
    <input id="sell-input" 
           :value="modelValue"
           type="text" 
           :style="{'--color': color}"
           class="elem"
           :placeholder="placeholder"
           @keydown="onKey"
           @paste="onPaste"
           @input="$emit('update:modelValue', $event.target.value)"
    />
    <span class="text">BEAM</span>
  </div>
</template>

<style scoped lang="stylus">
.input {
  margin-top: 20px
  height: 45px
  width: 100%
  border-radius: 10px
  background-color: rgba(255, 255, 255, 0.05)
  display: flex
  flex-direction: row
  align-items: center

  .elem {
    background: rgba(255, 255, 255, 0)
    border: none
    font-size: 24px
    resize: none
    color: var(--color)
    height: 100%
    padding: 0
    border-radius: 10px
    padding: 8px
    max-width: 292px
      
    &:focus {
      outline-width: 0
      color: var(--color)
    }

    &::placeholder {
      color: var(--color)
    }
  }

  .text {
    margin-right: 12px
    font-size: 16px
    margin-left: auto
    color: var(--color)
  }
}
</style>

<script>

export default {
  props: {
    color: {
      type: String,
      default: '#fff'
    },
    placeholder: {
      type: String,
      default: '',
      required: false
    },
    // eslint-disable-next-line vue/prop-name-casing
    modelValue: {
      type: String,
      default: '',
      required: true
    },
  },

  emits: [
    'trigger-key', 
    'trigger-paste', 'update:modelValue', 
  ],

  methods: {    
    onKey(ev) {
      this.$emit('trigger-key', ev)
    },
    onPaste(ev) {
      this.$emit('trigger-paste', ev)
    }
  }
}
</script>