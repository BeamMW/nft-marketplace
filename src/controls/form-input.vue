<template>
  <div class="input-container">
    <label v-if="label" class="label" :class="{'err': !valid}">
      {{ label }}
    </label>
    <div class="input" :class="{'err': !valid, 'readonly': readonly}">
      <img v-if="img" :src="`/assets/${img}.svg`" :class="{'error': !valid}" alt="icon"/>
      <input :value="modelValue"
             :placeholder="placeholder"
             :style="style"
             :maxlength="max_length"
             :class="{'err': !valid}"
             :readonly="readonly"
             @input="$emit('update:modelValue', $event.target.value)"
             @keydown="onKeyDown"
      />
      <slot></slot>
    </div>
    <charslen v-if="max_length && counter" 
              :readonly="readonly" 
              :max_length="max_length" 
              :value="modelValue.length"
              :class="{'chars-err': !valid}"
              :tip="tip"
              style="margin-top:3px;"
    />
  </div>
</template>

<style scoped lang="stylus">
.input-container {
  box-sizing: border-box

  .label {
    display: block
    margin-bottom:10px
    color: rgba(255, 255, 255, 0.6)
    font-family: 'SFProDisplay', sans-serif
    font-size: 14px

    &.err {
      color: rgba(255, 98, 92, 0.7)
    }
  }

  .input {
    display: flex
    background-color: rgba(255, 255, 255, 0.08)
    border: none
    border-radius: 10px
    align-items: center

    & > img {
      position: absolute
      padding: 15px 15px
    }

    &.readonly {
      background-color: rgba(255, 255, 255, 0.03)
    }

    &.err {
      background-color: rgba(255, 98, 92, 0.07)
      &:focus-within {
        background-color: rgba(255, 98, 92, 0.12)
      }
    }

    &:not(.err):not(.readonly):focus-within {
      background-color: rgba(255, 255, 255, 0.12)
    }

    & > input {
      font-family: 'SFProDisplay', sans-serif
      background-color: transparent
      border: none
      border-radius: 10px
      outline-width: 0
      font-size: 14px
      color: rgba(255, 255, 255, 0.8)
      padding: 10px 8px
      width: 100%

      &:read-only {
        color: rgba(255, 255, 255, 0.3)
      }

      &.err {
        color: rgba(255, 98, 92, 1)
      }

      &:not(.err)::placeholder {
        font-size: 14px
        color: rgba(255, 255, 255, 0.3)
      }

      &.err::placeholder {
        font-size: 14px
        color: rgba(255, 98, 92, 0.4)
      }
    }
  }

  .chars-err {
    color: rgba(255, 98, 92, 0.7)
  }
}
</style>

<script>
import charslen from 'controls/charslen'

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
    valid: {
      type: Boolean,
      default: true,
      required: false
    },
    img: {
      type: String,
      default:'',
      required: false,
    },
    max_length:{
      type: Number,
      default: undefined,
      required: false
    },
    counter: {
      type: Boolean,
      default: true,
      required: false
    },
    tip: {
      type: String,
      default: undefined,
      required: false
    },
    allowed: {
      type: Object,
      default: undefined,
      required: false
    }
  },

  emits: [
    'update:modelValue'
  ],

  computed: {
    style() {
      return {
        'padding-left': this.img.length ? '38px' : '15px'
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
