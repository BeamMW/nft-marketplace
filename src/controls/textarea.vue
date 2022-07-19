<template>
  <div class="textarea-container">
    <label v-if="label" class="label">{{ label }}</label>
    <textarea v-bind="$attrs" class="input"
              :value="modelValue"
              :placeholder="placeholder"
              :maxlength="max_length"
              :readonly="readonly"
              :style="input_style"
              @input="$emit('update:modelValue', $event.target.value)"
              @keydown="onKeyDown"
    >
    </textarea>
    <charslen v-if="max_length" :readonly="readonly" :max_length="max_length" :value="modelValue.length"/>
  </div>
</template>

<style scoped lang="stylus">
.textarea-container {
  width: 100%
  box-sizing: border-box

  .label {
    display: block
    margin-bottom: 10px
    color: rgba(255,255,255,0.6)
    font-family: 'SFProDisplay', sans-serif
    font-size: 14px
  }

  .input {
    box-sizing: border-box
    font-family: 'SFProDisplay', sans-serif
    background-color: rgba(255, 255, 255, 0.08)
    border: none
    outline-width: 0
    font-size: 14px
    color: white
    padding: 0
    border-radius: 10px
    padding: 10px 12px 
    width: 100%
    resize: none

    &:read-only {
      background-color: rgba(255, 255, 255, 0.03)
      color: rgba(255, 255, 255, 0.3)
    }

    &::placeholder {
      font-size: 14px
      color: rgba(255, 255, 255, 0.3)
    }

    &:not(:read-only):focus {
      background-color: rgba(255, 255, 255, 0.12)
      outline-width: 0
    }
  }

  .info {
    display: flex
    justify-content: space-between
    font-family: 'SFProDisplay', sans-serif
    opacity: 0.7
    font-style: italic
    font-weight: 400
    font-size: 12px

    &.readonly {
      color: rgba(255, 255, 255, 0.35)
    }
  }
}
</style>

<script>
import charslen from 'controls/charslen'
import validators from 'utils/validators'

export default {
  components: {
    charslen
  },

  props: {
    label: {
      type: String,
      default: '',
      required: false
    },
    readonly: {
      type: Boolean,
      default: false,
      required: false
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
    valid:{
      type: Boolean,
      required: false
    },
    max_length:{
      type: Number,
      default: 150,
      required: false
    },
    height: {
      type: String,
      default: '79px'
    },
    allowed: {
      type: Object,
      default: validators.text_allowed(),
      required: false
    }
  },
  
  emits: [
    'update:modelValue'
  ],

  computed: {
    input_style () {
      return {
        'min-height': this.height
      }
    }
  },

  methods: {
    onKeyDown(ev) {
      // TODO: merge 'allowed', 'valid' & 'max_chars' to one regex
      if (this.allowed && !this.allowed.test(ev.key)) {
        ev.preventDefault()
      }
    }
  }
}
</script>
